// TemplateEditor.h : TemplateEditor Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CTemplateEditorApp:
// �йش����ʵ�֣������ TemplateEditor.cpp
//
#include "TemplateManager.h"
#include "TemplateWrite.h"
class CTemplateEditorApp : public CWinApp
{
public:
	CTemplateEditorApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

private:
	CTemplateManager	m_TemplateManager;
	CTemplateWrite	m_TemplateWrite;

public:
	virtual int ExitInstance();
};

extern CTemplateEditorApp theApp;