#include "pch.h"
#include "Common.h"
#include "fileUtil.h"

/*
######################################################################################
--------------------------------------------------------------------------------------
--  <<Wiresharkリンケージ>>
--
--  モジュール名称取得
--------------------------------------------------------------------------------------
<<概要>>
本モジュールを元に、Wiresharkは対応DLLであることを判断します。
--------------------------------------------------------------------------------------
######################################################################################
*/

extern "C" __declspec(dllexport) int luaopen_TTaes_enc(lua_State* L);
extern "C" __declspec(dllexport) int luaopen_TTaes_dec(lua_State* L);
extern "C" __declspec(dllexport) int luaopen_getFileNames(lua_State* L);
extern "C" __declspec(dllexport) int luaopen_getIniFile(lua_State* L);
extern "C" __declspec(dllexport) int luaopen_getFile(lua_State* L);

extern "C" __declspec(dllexport) int luaopen_getFileNames(lua_State* L)
{
	return getFileNames(L);
}

extern "C" __declspec(dllexport) int luaopen_getIniFile(lua_State* L)
{
	return getIniFile(L);
}

extern "C" __declspec(dllexport) int luaopen_getFile(lua_State* L)
{
	return getFile(L);
}
static const luaL_Reg MFCLibrary[] = /* You could call this anything you want */
{
	{"my_aes_enc_function", luaopen_TTaes_enc },
	{"my_aes_dec_function", luaopen_TTaes_dec},
	{"my_getFileNames", luaopen_getFileNames},
	{"my_getIniFile", luaopen_getIniFile},
	{"my_getFile", luaopen_getFile},
	{NULL, NULL}
};

typedef int (*Sum)(int a, int b);
typedef int (*AES_DEC)(int* type, BYTE*key, BYTE* iv, BYTE* data);
typedef int (*AES_ENC)(int* type, BYTE* key, BYTE* iv, BYTE* data);

int TTaes(lua_State* L, int encdec) {

	int			iAnswer = 0;			// 戻り値
#if LUA_VERSION_NUM >= 502
	luaL_newlib(L, MFCLibrary);
#else
	luaL_register(L, "MFCLibrary1", MFCLibrary);
#endif
	int n = luaL_len(L, 1);

	if (n <= 7) {
		// 引数範囲外
		iAnswer = 1;
		goto ExitBlock;
}

	int type[7] = { 128, 128, 1, 1, 16, 16, 16 };
	int i;
	for (i = 1; i <= 7; ++i) {
		lua_rawgeti(L, 1, i);
		type[i - 1] = lua_tonumber(L, -1);
		lua_pop(L, 1);
	}

	BYTE* key = new BYTE[(size_t)type[4]];
	BYTE* iv = new BYTE[(size_t)type[5]];
	BYTE* data = new BYTE[(size_t)type[6]];

	if (type[4] < 16) {
		// 引数範囲外
		iAnswer = 1;
		goto ExitBlock;
	}
	if (type[5] < 16) {
		// 引数範囲外
		iAnswer = 1;
		goto ExitBlock;
	}
	if (type[6] < 16) {
		// 引数範囲外
		iAnswer = 1;
		goto ExitBlock;
	}
	if (n < 7 + type[4] + type[5] + type[6] - 1) {
		// 引数範囲外
		iAnswer = 1;
		goto ExitBlock;
	}

	for (; i <= 7 + type[4]; ++i) {
		lua_rawgeti(L, 1, i);
		key[i - 1 - 7] = lua_tonumber(L, -1);
		lua_pop(L, 1);
	}
	for (; i <= 7 + type[4] + type[5]; ++i) {
		lua_rawgeti(L, 1, i);
		iv[i - 1 - 7 - type[4]] = lua_tonumber(L, -1);
		lua_pop(L, 1);
	}
	for (; i <= 7 + type[4] + type[5] + type[6]; ++i) {
		lua_rawgeti(L, 1, i);
		data[i - 1 - 7 - type[4] - type[5]] = lua_tonumber(L, -1);
		lua_pop(L, 1);
	}

	/*
	  KeySize
	  BlockSize
	  CBC  1  ECB  2 OFB  3  CFB  4 CTS  5
	  ANSIX923 4 ISO10126 5 None 1 PKCS7 2 Zeros 3
	  key length
	  iv length
	  data length
	*/
	//byte type[7] =  { 128, 128, 1, 1, 16, 16, 16 };
	//byte key[16] =  { 0x07,0x01,0x02,0x03,0x04,0x05,0x06,0x02,0x07,0x01,0x02,0x03,0x04,0x05,0x06,0x02 };
	//byte iv[16] =   { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
	//byte data[16] = { 0xAD,0x78,0x0E,0x95,0xC3,0x70,0xA5,0x3C,0xC7,0xC0,0xE2,0xD2,0x79,0x84,0x8A,0x77 };

	auto hModule = LoadLibrary(L"DotNetToCplus.dll");
	if (hModule == NULL)
	{
		iAnswer = 2;
		goto ExitBlock;
	}
	
	if (encdec == 1) {
		auto aes_enc = reinterpret_cast<AES_DEC>(GetProcAddress(hModule, "AES_ENC"));
		if (aes_enc == NULL)
		{
			//std::puts("関数のアドレスの取得に失敗しました。");
			iAnswer = 3;
			goto ExitBlock;
		}
		aes_enc(&type[0], &key[0], &iv[0], &data[0]);
	}
	else {
		auto aes_dec = reinterpret_cast<AES_DEC>(GetProcAddress(hModule, "AES_DEC"));
		if (aes_dec == NULL)
		{
			//std::puts("関数のアドレスの取得に失敗しました。");
			iAnswer = 3;
			goto ExitBlock;
		}
		aes_dec(&type[0], &key[0], &iv[0], &data[0]);
	}

	lua_pushtableEx(L, data, type[6]);
	//lua_pushnumber(L, data[0]);

	// ﾊﾟﾗﾒｰﾀ数を返す
ExitBlock:
	if (iAnswer > 0) {
		// ｴﾗｰ発生時はﾊﾟﾗﾒｰﾀｾｯﾄ
		lua_pushnumber(L, iAnswer);
	}
	// ﾊﾟﾗﾒｰﾀ数を返す
	return 1;
}

extern "C" __declspec(dllexport) int luaopen_TTaes_dec(lua_State* L) 
{
	return TTaes(L, 0);
}

extern "C" __declspec(dllexport) int luaopen_TTaes_enc(lua_State* L)
{
	return TTaes(L, 1);
}