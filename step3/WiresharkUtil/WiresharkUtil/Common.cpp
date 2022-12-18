#include "pch.h"
#include <string>
#include <vector>
/*
######################################################################################
--------------------------------------------------------------------------------------
--  Lua�z��������₷�����邽�߂̊g���֐��Q
--------------------------------------------------------------------------------------
<<�T�v>>
�z��̃A�h���X�ƃT�C�Y���w�肵�āALua�X�^�b�N�ɐςݏグ��B

void lua_pushtableEx( lua_State *L, void* lpBuffer, DWORD count )

<<����>>
L               Lua�|�C���^
lpBuffer        �z��擪�A�h���X
count           �z��̌�
<<�߂�l>>
����

======================================================================================
<<�T�v>>
COleSafeArray���w�肵�āA�z���Lua�X�^�b�N�ɐςݏグ��B

void lua_pushtableEx( lua_State *L, COleSafeArray& ary )

<<����>>
L               Lua�|�C���^
ary             �z��I�u�W�F�N�g
<<�߂�l>>
����

======================================================================================
<<�T�v>>
Lua�X�^�b�N�̔ԍ����w�肵�āA�z��f�[�^��COleSafeArray�I�u�W�F�N�g�Ɏ��o���B

COleSafeArray lua_totableEx( lua_State *L, int no )

<<����>>
L               Lua�|�C���^
no              �擾����X�^�b�N�ԍ�
<<�߂�l>>
���o�����e�[�u�����i�[���ꂽCOleSafeArray�I�u�W�F�N�g

--------------------------------------------------------------------------------------
######################################################################################
*/
void lua_pushtableEx(lua_State* L, void* lpBuffer, DWORD count)
{
	lua_createtable(L, 0, count);
	for (DWORD ix = 0; ix < count; ix++) {
		lua_pushnumber(L, ((LPBYTE)lpBuffer)[ix]);
		lua_rawseti(L, -2, ix + 1);
	}
}
void lua_pushtableEx(lua_State* L, COleSafeArray& ary)
{
	BYTE* lpBuffer = NULL;
	ary.AccessData((void**)& lpBuffer);
	lua_pushtableEx(L, lpBuffer, ary.GetOneDimSize());
	ary.UnaccessData();
}
void lua_pushString(lua_State* L, std::string ary)
{
	BYTE* lpBuffer = NULL;
	const char* c = ary.c_str();
	lua_pushstring(L, c);
}
void lua_pushVectorString(lua_State* L, std::vector<std::string> ary)
{
	BYTE* lpBuffer = NULL;
	int sum=0;

	lua_newtable(L);

	for (DWORD ivec = 0; ivec < ary.size(); ivec++) {
		const char* c = ary[ivec].c_str();
		lua_pushnumber(L, ivec);
		lua_pushstring(L, c);
		// lua_setfield(L, -2, ivec + 1);
		lua_settable(L,-3);
	}
}
COleSafeArray lua_totableEx(lua_State* L, int no)
{
	DWORD			bufferCount = 0;
	DWORD			bufferSize = 1024;
	BYTE* lpBuffer = NULL;
	COleSafeArray	ary;

	ary.CreateOneDim(VT_UI1, 0);							// ��z��̍쐬
	lpBuffer = (BYTE*)::GlobalAlloc(GPTR, bufferSize);	// �����ޯ̧�̍쐬

	// �z������
	for (DWORD ix = 1; ; ix++) {
		// �l������o��
		lua_rawgeti(L, no, ix);
		// NULL����
		if (lua_isnil(L, -1)) {
			lua_pop(L, 1);
			break;
		}
		// ���l�ȊO����
		if (1 != lua_isnumber(L, -1)) {
			lua_pop(L, 1);
			break;
		}
		// �ޯ̧�ɐςݍ���
		lpBuffer[bufferCount] = (BYTE)lua_tonumber(L, -1);
		lua_pop(L, 1);
		bufferCount++;
		// �ޯ̧���s����Δ{�Ɋg������
		if (bufferCount >= bufferSize) {
			BYTE* lpNewAddr = (BYTE*)::GlobalAlloc(GPTR, bufferSize * 2);
			CopyMemory(lpNewAddr, lpBuffer, bufferSize);
			::GlobalFree(lpBuffer);
			lpBuffer = lpNewAddr;
			bufferSize *= 2;
		}
	}
	// ��޼ު�Ăɒǉ�����
	if (bufferCount > 0)
		ary.CreateOneDim(VT_UI1, bufferCount, lpBuffer);
	::GlobalFree(lpBuffer);
	lpBuffer = NULL;
	return ary;
}
