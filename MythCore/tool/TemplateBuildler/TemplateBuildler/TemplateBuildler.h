// TemplateBuildler.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTemplateBuildlerApp:
// �йش����ʵ�֣������ TemplateBuildler.cpp
//

class CTemplateBuildlerApp : public CWinApp
{
public:
	CTemplateBuildlerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTemplateBuildlerApp theApp;