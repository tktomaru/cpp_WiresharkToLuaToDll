// WiresharkUtil.h : WiresharkUtil DLL のメイン ヘッダー ファイル
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CWiresharkUtilApp
// このクラスの実装に関しては WiresharkUtil.cpp をご覧ください
//

class CWiresharkUtilApp : public CWinApp
{
public:
	CWiresharkUtilApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
