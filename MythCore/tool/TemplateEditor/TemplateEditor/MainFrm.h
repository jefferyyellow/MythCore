// MainFrm.h : CMainFrame 类的接口
//


#pragma once
#include "CatalogTree.h"
#include "ProgStatusBar.h"
class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CCatalogTree&	GetCatalogTreeDialog(){return m_CatalogTree;}

protected:  // 控件条嵌入成员
	CProgStatusBar 	m_wndStatusBar;
	CToolBar		m_wndToolBar;
	CCatalogTree	m_CatalogTree;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGenerateBin();
	afx_msg void OnClone();
	afx_msg void OnNewTemplate();
	afx_msg void IDToTemplate();
	DECLARE_MESSAGE_MAP()

private:
	static DWORD WINAPI GenerateBinThread(LPVOID param);  // 生成二进制文件的后台线程
public:
	void	SetProcess(int nProgress);
	HWND	GetStatusHwnd();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void	ReLocate();
	
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);

	//查找当前点击模版是否已打开
	CTemplateEditorView* FindChildWindow(CTemplateFieldData* pFieldData);
	void UpdateDataToView(CTemplateFieldData* pTemplateFieldData);

public:
	void	Bi();
	bool	mbConsole;
};


