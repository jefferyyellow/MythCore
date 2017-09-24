// MainFrm.h : CMainFrame ��Ľӿ�
//


#pragma once
#include "CatalogTree.h"
#include "ProgStatusBar.h"
class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CCatalogTree&	GetCatalogTreeDialog(){return m_CatalogTree;}

protected:  // �ؼ���Ƕ���Ա
	CProgStatusBar 	m_wndStatusBar;
	CToolBar		m_wndToolBar;
	CCatalogTree	m_CatalogTree;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGenerateBin();
	afx_msg void OnClone();
	afx_msg void OnNewTemplate();
	afx_msg void IDToTemplate();
	DECLARE_MESSAGE_MAP()

private:
	static DWORD WINAPI GenerateBinThread(LPVOID param);  // ���ɶ������ļ��ĺ�̨�߳�
public:
	void	SetProcess(int nProgress);
	HWND	GetStatusHwnd();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void	ReLocate();
	
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);

	//���ҵ�ǰ���ģ���Ƿ��Ѵ�
	CTemplateEditorView* FindChildWindow(CTemplateFieldData* pFieldData);
	void UpdateDataToView(CTemplateFieldData* pTemplateFieldData);

public:
	void	Bi();
	bool	mbConsole;
};


