// SSHServerManager.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSSHServerManagerApp:
// �йش����ʵ�֣������ SSHServerManager.cpp
//

class CSSHServerManagerApp : public CWinApp
{
public:
	CSSHServerManagerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSSHServerManagerApp theApp;