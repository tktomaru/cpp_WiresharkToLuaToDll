#include "pch.h"

//####################################################################################
//# Lua�̃��C�u������錾
//####################################################################################
/*
<< �{�T���v���Ŏg�p����Lua�̊T�v >>
�o�[�W����          Lua 5.1.2
���                Windows x86 Static Library and Includes
                    (Visual C++ 6 Compatible)
�t�@�C������        lua5_1_2_Win32_vc6_lib.zip
�_�E�����[�hURL     http://luabinaries.luaforge.net/download.html

<< �_�E�����[�h�����t�@�C���̔z�u >>
SampleLuaEx         <-Lua�g��DLL�̃v���W�F�N�g�t�H���_
�� include          <-�_�E�����[�h����Lua���C�u�����̃C���N���[�h�p�w�b�_�t�H���_
�� lua5.1.lib       <-�_�E�����[�h����Lua���C�u������lib�t�@�C��
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
--  <<SocketDebugger�����P�[�W>>
--
--  ���W���[�����̎擾
--------------------------------------------------------------------------------------
<<�T�v>>
�{���W���[�������ɁASocketDebugger�͑Ή�DLL�ł��邱�Ƃ𔻒f���܂��B
�{���W���[���� count ��0���珇���A1,2...�ƌĂяo���A
���W���[�����̂��擾���悤�Ƃ��܂��B

�߂�l0�A�y�у��W���[������Ԃ����Ƃɂ��A�Ăяo���郂�W���[������F�����܂��B
�߂�l1��Ԃ����ꍇ�́A�Ăяo���郂�W���[���͑S�Ď擾�����Ɣ��f���܂��B

�擾�ł������W���[���́ASocketDebugger���ŗ��p�\�ƂȂ�܂��B

<<����>>
lpName                ���W���[�����̂��i�[����A�h���X
                      (�����Ƀ��W���[�����̂��i�[����)
bufferCount           �o�b�t�@lpName�̗��p�\�o�C�g��
count                 �֐����̎��o���ԍ�
                      SocketDebugger��0����1,2...�ƒl��ς���
					  �߂�l��0�ȊO�ƂȂ�܂ŌJ��Ԃ��Ăяo���܂��B

<<�߂�l>>
                      0:����I��  �֐��L��(lpName�̖��̗L��)
                      1:�֐�����
                      2:�i�[�T�C�Y�s��

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

	ary.CreateOneDim( VT_UI1, 0 );							// ��z��̍쐬
	lpBuffer = (BYTE*)::GlobalAlloc( GPTR, bufferSize );	// �����ޯ̧�̍쐬

	// �z������
	for( DWORD ix=1 ;  ; ix++ ){
		// �l������o��
		lua_rawgeti(L, no, ix);
		// NULL����
		if( lua_isnil(L, -1) ){
			lua_pop(L, 1);
			break;
		}
		// ���l�ȊO����
		if( 1 != lua_isnumber(L,-1) ){
			lua_pop(L, 1);
			break;
		}
		// �ޯ̧�ɐςݍ���
		lpBuffer[bufferCount] = (BYTE)lua_tonumber(L,-1);
		lua_pop(L, 1);
		bufferCount++;
		// �ޯ̧���s����Δ{�Ɋg������
		if( bufferCount >= bufferSize ){
			BYTE* lpNewAddr = (BYTE*)::GlobalAlloc( GPTR, bufferSize*2 );
			CopyMemory( lpNewAddr, lpBuffer, bufferSize );
			::GlobalFree( lpBuffer );
			lpBuffer = lpNewAddr;
			bufferSize *= 2;
		}
	}
	// ��޼ު�Ăɒǉ�����
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
