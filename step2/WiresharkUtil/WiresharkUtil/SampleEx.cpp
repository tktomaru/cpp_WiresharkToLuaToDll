#include "pch.h"

//####################################################################################
//# Luaのライブラリを宣言
//####################################################################################
/*
<< 本サンプルで使用したLuaの概要 >>
バージョン          Lua 5.1.2
種類                Windows x86 Static Library and Includes
                    (Visual C++ 6 Compatible)
ファイル名称        lua5_1_2_Win32_vc6_lib.zip
ダウンロードURL     http://luabinaries.luaforge.net/download.html

<< ダウンロードしたファイルの配置 >>
SampleLuaEx         <-Lua拡張DLLのプロジェクトフォルダ
├ include          <-ダウンロードしたLuaライブラリのインクルード用ヘッダフォルダ
└ lua5.1.lib       <-ダウンロードしたLuaライブラリのlibファイル
*/
#include "include/lua.hpp"
#pragma comment(lib, "lua52.lib")
#pragma comment(linker, "/NODEFAULTLIB:LIBC.LIB")

//#pragma comment(linker, "./DotNetToCplusCipher.tlb")
//#import "DotNetToCplusCipher.tlb" raw_interfaces_only no_namespace
//#import "DotNetToCplusCipher.tlb" raw_interfaces_only
//using namespace DotNetToCplusCipher;
/*
######################################################################################
--------------------------------------------------------------------------------------
--  <<SocketDebuggerリンケージ>>
--
--  モジュール名称取得
--------------------------------------------------------------------------------------
<<概要>>
本モジュールを元に、SocketDebuggerは対応DLLであることを判断します。
本モジュールの count を0から順次、1,2...と呼び出し、
モジュール名称を取得しようとします。

戻り値0、及びモジュール名を返すことにより、呼び出せるモジュール名を認識します。
戻り値1を返した場合は、呼び出せるモジュールは全て取得したと判断します。

取得できたモジュールは、SocketDebugger内で利用可能となります。

<<引数>>
lpName                モジュール名称を格納するアドレス
                      (ここにモジュール名称を格納する)
bufferCount           バッファlpNameの利用可能バイト数
count                 関数名称取り出し番号
                      SocketDebuggerは0から1,2...と値を変えて
					  戻り値が0以外となるまで繰り返す呼び出します。

<<戻り値>>
                      0:正常終了  関数有り(lpNameの名称有効)
                      1:関数無し
                      2:格納サイズ不足

--------------------------------------------------------------------------------------
######################################################################################
*/

extern "C" __declspec(dllexport) int luaopen_TTaes(lua_State* L);

static const luaL_Reg MFCLibrary[] = /* You could call this anything you want */
{
	{"my_aes_function", luaopen_TTaes},
	{NULL, NULL}
};


/*
######################################################################################
--------------------------------------------------------------------------------------
--  Lua配列を扱いやすくするための拡張関数群
--------------------------------------------------------------------------------------
<<概要>>
配列のアドレスとサイズを指定して、Luaスタックに積み上げる。

void lua_pushtableEx( lua_State *L, void* lpBuffer, DWORD count )

<<引数>>
L               Luaポインタ
lpBuffer        配列先頭アドレス
count           配列の個数
<<戻り値>>
無し

======================================================================================
<<概要>>
COleSafeArrayを指定して、配列をLuaスタックに積み上げる。

void lua_pushtableEx( lua_State *L, COleSafeArray& ary )

<<引数>>
L               Luaポインタ
ary             配列オブジェクト
<<戻り値>>
無し

======================================================================================
<<概要>>
Luaスタックの番号を指定して、配列データをCOleSafeArrayオブジェクトに取り出す。

COleSafeArray lua_totableEx( lua_State *L, int no )

<<引数>>
L               Luaポインタ
no              取得するスタック番号
<<戻り値>>
取り出したテーブルが格納されたCOleSafeArrayオブジェクト

--------------------------------------------------------------------------------------
######################################################################################
*/
void lua_pushtableEx( lua_State *L, void* lpBuffer, DWORD count )
{
	lua_createtable(L,0,count);
	for( DWORD ix=0 ; ix<count ; ix++ ){
		lua_pushnumber(L,((LPBYTE)lpBuffer)[ix]);
		lua_rawseti(L,-2,ix+1);
	}
}
void lua_pushtableEx( lua_State *L, COleSafeArray& ary )
{
	BYTE *lpBuffer = NULL;
	ary.AccessData( (void**)&lpBuffer );
	lua_pushtableEx( L, lpBuffer, ary.GetOneDimSize() );
	ary.UnaccessData();
}
COleSafeArray lua_totableEx( lua_State *L, int no )
{
	DWORD			bufferCount = 0;
	DWORD			bufferSize = 1024;
	BYTE			*lpBuffer = NULL;
	COleSafeArray	ary;

	ary.CreateOneDim( VT_UI1, 0 );							// 空配列の作成
	lpBuffer = (BYTE*)::GlobalAlloc( GPTR, bufferSize );	// 初期ﾊﾞｯﾌｧの作成

	// 配列を取る
	for( DWORD ix=1 ;  ; ix++ ){
		// 値を一つ取り出す
		lua_rawgeti(L, no, ix);
		// NULLﾁｪｯｸ
		if( lua_isnil(L, -1) ){
			lua_pop(L, 1);
			break;
		}
		// 数値以外ﾁｪｯｸ
		if( 1 != lua_isnumber(L,-1) ){
			lua_pop(L, 1);
			break;
		}
		// ﾊﾞｯﾌｧに積み込み
		lpBuffer[bufferCount] = (BYTE)lua_tonumber(L,-1);
		lua_pop(L, 1);
		bufferCount++;
		// ﾊﾞｯﾌｧが尽きれば倍に拡張する
		if( bufferCount >= bufferSize ){
			BYTE* lpNewAddr = (BYTE*)::GlobalAlloc( GPTR, bufferSize*2 );
			CopyMemory( lpNewAddr, lpBuffer, bufferSize );
			::GlobalFree( lpBuffer );
			lpBuffer = lpNewAddr;
			bufferSize *= 2;
		}
	}
	// ｵﾌﾞｼﾞｪｸﾄに追加する
	if( bufferCount>0 )
		ary.CreateOneDim( VT_UI1, bufferCount, lpBuffer );
	::GlobalFree( lpBuffer );
	lpBuffer = NULL;
	return ary;
}

typedef int (*Sum)(int a, int b);

extern "C" __declspec(dllexport) int luaopen_TTaes(lua_State* L) 
{
#if LUA_VERSION_NUM >= 502
	luaL_newlib(L, MFCLibrary);
#else
	luaL_register(L, "MFCLibrary1", MFCLibrary);
#endif
	auto hModule = LoadLibrary(L"DotNetToCplus.dll");
	auto sum = reinterpret_cast<Sum>(GetProcAddress(hModule, "Sum"));

	lua_pushnumber(L, sum(1, 2));
	return 1;
}
