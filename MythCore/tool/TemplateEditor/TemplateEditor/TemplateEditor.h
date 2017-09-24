// TemplateEditor.h : TemplateEditor 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// CTemplateEditorApp:
// 有关此类的实现，请参阅 TemplateEditor.cpp
//
#include "TemplateManager.h"
#include "TemplateWrite.h"
class CTemplateEditorApp : public CWinApp
{
public:
	CTemplateEditorApp();


// 重写
public:
	virtual BOOL InitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

private:
	CTemplateManager	m_TemplateManager;
	CTemplateWrite	m_TemplateWrite;

public:
	virtual int ExitInstance();
};

extern CTemplateEditorApp theApp;