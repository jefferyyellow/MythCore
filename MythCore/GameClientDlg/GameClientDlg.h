
// GameClientDlg.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGameClientDlgApp: 
// �йش����ʵ�֣������ GameClientDlg.cpp
//

class CGameClientDlgApp : public CWinApp
{
public:
	CGameClientDlgApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGameClientDlgApp theApp;