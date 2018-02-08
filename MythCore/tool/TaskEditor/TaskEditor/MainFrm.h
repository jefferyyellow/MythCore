
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "FileView.h"
#include "TaskTemplate.h"
#include "OptionView.h"
#include "commondefine.h"

/// ����һ���Ƚϵ��۵����飬IDR_MAINFRAME�ж����˺ܶ��ݼ���CGridCtrl��OnKeyDown��Ҳ������
/// Ctrl+X, Ctrl+C,Ctrl+V����IDR_MAINFRAME��ֻҪ��һ����ݼ�ʱ��CGridCtrl��OnKeyDown�о��ղ���
/// Ctrl+X, Ctrl+C,Ctrl+V����Ϣ�����ǰ�IDR_MAINFRAME��գ�CGridCtrl�����յ�
/// Ctrl+X, Ctrl+C,Ctrl+V����Ϣ�����ֻ�ܰ�IDR_MAINFRAME��գ������Զ�����һ��
/// IDR_CUSTOMSIZE�Ŀ�ݼ���û�ҵ�ԭ������ô��ܴ����ܴﵽͬ����Ŀ��

class CMainFrame : public CMDIFrameWndEx
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
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	/// �ļ����������ļ��ڵ�
	void	AddFileItem(tinyxml2::XMLDocument& tDocument, CString strFileName);
	/// ��ʾѡ�����
	void	ShowOptionView(HWND hWnd, CString strConfigName, CGridCtrl* pGridCtrl, int nRowNum, int nColumnNum);
	/// �����ļ����ڵ�
	void	UpdateFileViewItem(CString& strTaskID, CString& strTaskType, CString& strTaskName);
	/// �������ļ���õ�����ȼ���Npc Id
	int		GetTaskLevelAndNpcId(tinyxml2::XMLDocument& tDocument, int& rNpcId);
	/// �����������õ���������
	void	ProcessTaskLanguage(tinyxml2::XMLDocument& tDocument, XMLElement* pLangRootElem);
	/// ��������ȼ�����
	void	SaveTaskLevelConfig(const char* pXmlFilePath, vector<CString>& rVecString);
	/// ����NPC��������
	void	SaveNpcTaskConfig(const char* pXmlFilePath, map<int, CString>& rNpcTask);
protected:  // �ؼ���Ƕ���Ա
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CFileView         m_wndFileView;
	COptionView		  m_wndOptionView;
public:
	/// ����ģ��
	CTaskTemplate		mTaskTemplate;
	/// ����༭������
	CTaskEditorConfig	mTaskEditorConfig;
// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	// �����������������ļ�
	afx_msg void OnTaskConfig();
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	/// �����ļ���xml
	tinyxml2::XMLDocument mLangDocument;
	/// ��ݼ������ΪʲôҪ�Զ��壬�뿴�ļ�ͷ�ľ��ᣩ
	HACCEL m_hAccel;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


