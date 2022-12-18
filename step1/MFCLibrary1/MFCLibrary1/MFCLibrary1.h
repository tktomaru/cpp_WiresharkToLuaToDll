// MFCLibrary1.h : MFCLibrary1 DLL のメイン ヘッダー ファイル
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CMFCLibrary1App
// このクラスの実装に関しては MFCLibrary1.cpp をご覧ください
//

class CMFCLibrary1App : public CWinApp
{
public:
	CMFCLibrary1App();

// オーバーライド
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
