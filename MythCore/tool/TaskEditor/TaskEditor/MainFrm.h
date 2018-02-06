
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "FileView.h"
#include "ClassView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"
#include "TaskTemplate.h"
#include "OptionView.h"
#include "commondefine.h"
class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void	AddFileItem(tinyxml2::XMLDocument& tDocument, CString strFileName);
	void	ShowOptionView(CString strConfigName, CGridCtrl* pGridCtrl, int nRowNum, int nColumnNum);
	void	UpdateFileViewItem(CString& strTaskID, CString& strTaskType, CString& strTaskName);
	int		GetTaskLevelAndNpcId(tinyxml2::XMLDocument& tDocument, int& rNpcId);
	void	ProcessTaskLanguage(tinyxml2::XMLDocument& tDocument, XMLElement* pLangRootElem);
	void	SaveTaskLevelConfig(const char* pXmlFilePath, vector<CString>& rVecString);
	void	SaveNpcTaskConfig(const char* pXmlFilePath, map<int, CString>& rNpcTask);
protected:  // 控件条嵌入成员
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CFileView         m_wndFileView;
	COptionView		  m_wndOptionView;
	//CClassView        m_wndClassView;
	//COutputWnd        m_wndOutput;
	//CPropertiesWnd    m_wndProperties;
public:
	CTaskTemplate		mTaskTemplate;
	CTaskEditorConfig	mTaskEditorConfig;
// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnTaskConfig();
	afx_msg void OnTaskInternation();
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	tinyxml2::XMLDocument mLangDocument;
};


