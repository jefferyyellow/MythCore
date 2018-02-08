
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "FileView.h"
#include "TaskTemplate.h"
#include "OptionView.h"
#include "commondefine.h"

/// 碰到一个比较蛋疼的事情，IDR_MAINFRAME中定义了很多快捷键，CGridCtrl的OnKeyDown中也处理了
/// Ctrl+X, Ctrl+C,Ctrl+V，当IDR_MAINFRAME中只要有一个快捷键时，CGridCtrl的OnKeyDown中就收不到
/// Ctrl+X, Ctrl+C,Ctrl+V的消息，但是把IDR_MAINFRAME清空，CGridCtrl就能收到
/// Ctrl+X, Ctrl+C,Ctrl+V的消息，最后只能把IDR_MAINFRAME清空，重新自定义了一个
/// IDR_CUSTOMSIZE的快捷键，没找到原因，先这么规避处理，能达到同样的目的

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
	/// 文件树上增加文件节点
	void	AddFileItem(tinyxml2::XMLDocument& tDocument, CString strFileName);
	/// 显示选项面板
	void	ShowOptionView(HWND hWnd, CString strConfigName, CGridCtrl* pGridCtrl, int nRowNum, int nColumnNum);
	/// 更新文件树节点
	void	UpdateFileViewItem(CString& strTaskID, CString& strTaskType, CString& strTaskName);
	/// 从配置文件里得到任务等级和Npc Id
	int		GetTaskLevelAndNpcId(tinyxml2::XMLDocument& tDocument, int& rNpcId);
	/// 处理任务配置的语言生成
	void	ProcessTaskLanguage(tinyxml2::XMLDocument& tDocument, XMLElement* pLangRootElem);
	/// 保存任务等级配置
	void	SaveTaskLevelConfig(const char* pXmlFilePath, vector<CString>& rVecString);
	/// 保存NPC任务配置
	void	SaveNpcTaskConfig(const char* pXmlFilePath, map<int, CString>& rNpcTask);
protected:  // 控件条嵌入成员
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CFileView         m_wndFileView;
	COptionView		  m_wndOptionView;
public:
	/// 任务模板
	CTaskTemplate		mTaskTemplate;
	/// 任务编辑器配置
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
	// 生成任务最终配置文件
	afx_msg void OnTaskConfig();
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	/// 语言文件的xml
	tinyxml2::XMLDocument mLangDocument;
	/// 快捷键句柄（为什么要自定义，请看文件头的纠结）
	HACCEL m_hAccel;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


