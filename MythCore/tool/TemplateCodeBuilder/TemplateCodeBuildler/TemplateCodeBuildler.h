
// TemplateCodeBuildler.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTemplateCodeBuildlerApp: 
// �йش����ʵ�֣������ TemplateCodeBuildler.cpp
//

class CTemplateCodeBuildlerApp : public CWinApp
{
public:
	CTemplateCodeBuildlerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTemplateCodeBuildlerApp theApp;