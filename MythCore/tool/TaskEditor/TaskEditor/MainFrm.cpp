
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "TaskEditor.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_WM_HOTKEY()
	ON_COMMAND(ID_TASK_CONFIG, &CMainFrame::OnTaskConfig)
	ON_COMMAND(ID_INTERNATION, &CMainFrame::OnTaskInternation)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO:  �ڴ���ӳ�Ա��ʼ������
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
	//UnregisterHotKey(this->GetSafeHwnd(), 1001);
	//UnregisterHotKey(this->GetSafeHwnd(), 1002);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // ����������ʽ...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // ����Ϊ FALSE �Ὣ�رհ�ť������ѡ�������Ҳ�
	mdiTabParams.m_bTabIcons = FALSE;    // ����Ϊ TRUE ���� MDI ѡ��������ĵ�ͼ��
	mdiTabParams.m_bAutoColor = TRUE;    // ����Ϊ FALSE ������ MDI ѡ����Զ���ɫ
	mdiTabParams.m_bDocumentMenu = TRUE; // ��ѡ�������ұ�Ե�����ĵ��˵�
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("δ�ܴ����˵���\n");
		return -1;      // δ�ܴ���
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// ��ֹ�˵����ڼ���ʱ��ý���
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// �����û�����Ĺ���������: 
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO:  �������ϣ���������Ͳ˵�����ͣ������ɾ��������
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// ���ز˵���ͼ��(�����κα�׼��������): 
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// ����ͣ������
	if (!CreateDockingWindows())
	{
		TRACE0("δ�ܴ���ͣ������\n");
		return -1;
	}

	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);
	CDockablePane* pTabbedBar = NULL;
	//m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);
	//m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndOutput);
	//m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndProperties);

	m_wndOptionView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOptionView);
	m_wndOptionView.AttachToTabWnd(&m_wndFileView, DM_SHOW, FALSE, &pTabbedBar);

	// ���ڳ־�ֵ�����Ӿ�����������ʽ
	OnApplicationLook(theApp.m_nAppLook);

	// ������ǿ�Ĵ��ڹ���Ի���
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// ���ù�������ͣ�����ڲ˵��滻
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// ���ÿ���(��ס Alt �϶�)�������Զ���
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// �����û�����Ĺ�����ͼ��
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// ���ò˵����Ի�(���ʹ�õ�����)
	// TODO:  �������Լ��Ļ������ȷ��ÿ�������˵�������һ���������
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	//lstBasicCommands.AddTail(ID_EDIT_CUT);
	//lstBasicCommands.AddTail(ID_EDIT_PASTE);
	//lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);
	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	// ���ĵ�����Ӧ�ó��������ڴ��ڱ������ϵ�˳����н�������
	// ���Ľ��������Ŀ����ԣ���Ϊ��ʾ���ĵ�����������ͼ��
	ModifyStyle(0, FWS_PREFIXTITLE);
	mTaskTemplate.loadTaskTemplate("TaskTemplate.xml");
	mTaskTemplate.LoadItemNameFile("TempName.xml");
	mTaskTemplate.LoadItemNameFile("MapName.xml");
	mTaskEditorConfig.LoadTaskEditorConfig("TaskEditorConfig.xml");
	m_wndOptionView.FillTempOptionView();
	m_hAccel = LoadAccelerators(AfxGetInstanceHandle (), MAKEINTRESOURCE(IDR_CUSTOMSIZE));;   
	int nError = GetLastError();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// ��������ͼ
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);
	//if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("δ�ܴ���������ͼ������\n");
	//	return FALSE; // δ�ܴ���
	//}

	// �����ļ���ͼ
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ������ļ���ͼ������\n");
		return FALSE; // δ�ܴ���
	}

	//// �����������
	//CString strOutputWnd;
	//bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	//ASSERT(bNameValid);
	//if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("δ�ܴ����������\n");
	//	return FALSE; // δ�ܴ���
	//}

	//// �������Դ���
	//CString strPropertiesWnd;
	//bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	//ASSERT(bNameValid);
	//if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("δ�ܴ��������ԡ�����\n");
	//	return FALSE; // δ�ܴ���
	//}


	CString strTempSelectWnd;
	bNameValid = strTempSelectWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOptionView.Create(strTempSelectWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ����������\n");
		return FALSE; // δ�ܴ���
	}
	
	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	//HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndClassView.SetIcon(hClassViewIcon, FALSE);

	//HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	//HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOptionView.SetIcon(hPropertiesBarIcon, FALSE);

	UpdateMDITabbedBarsIcons();
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* ɨ��˵�*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	//m_wndOutput.UpdateFonts();
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// ���ཫִ�������Ĺ���

	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// Ϊ�����û������������Զ��尴ť
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}
	m_wndOptionView.ShowPane(FALSE, FALSE, FALSE);
	m_wndFileView.FillFileView();
	//RegisterHotKey(m_hWnd, 1001, MOD_CONTROL, 's');
	//RegisterHotKey(m_hWnd, 1002, MOD_CONTROL, 'S');
	return TRUE;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
	//m_wndOutput.UpdateFonts();
}

void CMainFrame::OnTaskConfig()
{
	mLangDocument.Clear();
	XMLDeclaration* pDeclaration = mLangDocument.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"");
	mLangDocument.LinkEndChild(pDeclaration);

	XMLElement* pLangRootElem = mLangDocument.NewElement("LangList");
	mLangDocument.LinkEndChild(pLangRootElem);



	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;

	vector<CString> vecLevelString;
	map<int, CString> hashNpcTaskId;


	int nMaxPhase = (mTaskEditorConfig.mMaxLevel + mTaskEditorConfig.mLevelPhase - 1) / mTaskEditorConfig.mLevelPhase;
	for (int i = 0; i < nMaxPhase; ++ i)
	{
		vecLevelString.push_back(_T(""));
	}


	hFind = FindFirstFile(_T("Tasks\\*"), &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return;
	}

	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			CString strFilePath = CString("Tasks\\") + ffd.cFileName;
			CString strFileName = ffd.cFileName;
			strFileName = strFileName.Left(strFileName.Find('.'));
			if (ffd.cFileName[0] < _T('0') || ffd.cFileName[0] > _T('9'))
			{
				continue;
			}
			tinyxml2::XMLDocument tDocument;
			char acBuffer[4096] = { 0 };
			UnicodeToAnsi(strFilePath, acBuffer, sizeof(acBuffer));
			if (tinyxml2::XML_SUCCESS != tDocument.LoadFile(acBuffer))
			{
				continue;
			}

			int nNpcId = 0;
			int nLevel = GetTaskLevelAndNpcId(tDocument, nNpcId);
			int nPhase = (nLevel - 1) / mTaskEditorConfig.mLevelPhase;
			if (nPhase >= nMaxPhase)
			{
				AfxMessageBox(_T("����ȼ��Ѿ��������ȼ�!"), MB_ICONEXCLAMATION);
				break;
			}

			if (vecLevelString[nPhase] != _T(""))
			{
				vecLevelString[nPhase].Append(_T(","));
			}

			vecLevelString[nPhase].Append(strFileName);

			if (nNpcId > 0)
			{

				if (hashNpcTaskId[nNpcId] != _T(""))
				{
					hashNpcTaskId[nNpcId].Append(_T(","));
				}
				
				hashNpcTaskId[nNpcId].Append(strFileName);
			}

			ProcessTaskLanguage(tDocument, pLangRootElem);
			CString strTargetFileName = CString(_T("Targets\\")) + ffd.cFileName;
			UnicodeToAnsi(strTargetFileName, acBuffer, sizeof(acBuffer));
			tDocument.SaveFile(acBuffer);
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
		return;
	}
	FindClose(hFind);

	SaveTaskLevelConfig("Targets\\LevelTaskList.xml", vecLevelString);
	SaveNpcTaskConfig("Targets\\NpcTaskList.xml", hashNpcTaskId);
	mLangDocument.SaveFile("Targets\\TaskLang.xml");
	AfxMessageBox(_T("���������������!"));
}

int CMainFrame::GetTaskLevelAndNpcId(tinyxml2::XMLDocument& tDocument, int& rNpcId)
{
	XMLElement* pRootElem = tDocument.RootElement();
	if (NULL == pRootElem)
	{
		return -1;
	}

	XMLElement* pNpcIdElem = pRootElem->FirstChildElement("AcceptNpc");
	if (NULL != pNpcIdElem)
	{
		rNpcId = pNpcIdElem->IntAttribute("Value");
	}

	XMLElement* pAccpetCondElem = pRootElem->FirstChildElement("AccpetCond");
	if (NULL == pAccpetCondElem)
	{
		return -1;
	}

	int nLevel = 0;
	wchar_t wBuffer[4096] = { 0 };
	char acBuffer[4096] = { 0 };

	XMLElement* pCondElem = pAccpetCondElem->FirstChildElement("Cond");
	for (; NULL != pCondElem; pCondElem = pCondElem->NextSiblingElement("Cond"))
	{
		Utf8ToUnicode(pCondElem->Attribute("Type"), wBuffer, sizeof(wBuffer) / 2 - 1);
		int nCondType = _ttoi(wBuffer);
		if (nCondType == mTaskEditorConfig.mLevelCond)
		{
			UnicodeToAnsi(mTaskEditorConfig.mLevelParam.c_str(), acBuffer, sizeof(acBuffer));
			nLevel = pCondElem->IntAttribute(acBuffer);
			break;
		}
	}

	return nLevel;
}

void CMainFrame::ProcessTaskLanguage(tinyxml2::XMLDocument& tDocument, XMLElement* pLangRootElem)
{
	XMLElement* pRootElem = tDocument.RootElement();
	if (NULL == pRootElem)
	{
		return;
	}

	const char* pTaskId = NULL;
	XMLElement* pTaskIdElem = pRootElem->FirstChildElement("TaskId");
	if (NULL != pTaskIdElem)
	{
		pTaskId = pTaskIdElem->Attribute("Value");
	}
	if (NULL == pTaskId)
	{
		return;
	}
	char acBuffer[4096] = { 0 };

	int nLangCount = 0;
	for (int i = 0; i < mTaskTemplate.mMainLangList.size(); ++ i)
	{
		XMLElement* pMainElem = pRootElem->FirstChildElement(mTaskTemplate.mMainLangList[i].c_str());
		if (NULL != pMainElem)
		{

			const char* pLangValue = pMainElem->Attribute("Value");
			if (NULL != pLangValue)
			{
				_snprintf_s(acBuffer, sizeof(acBuffer), "tkl_%s_%d", pTaskId, nLangCount);
				pMainElem->SetAttribute("Value", acBuffer);

				XMLElement* pLangElem = mLangDocument.NewElement("Lang");
				pLangElem->SetAttribute("Id", acBuffer);
				pLangElem->SetAttribute("Value", pLangValue);
				pLangRootElem->LinkEndChild(pLangElem);
				++ nLangCount;
			}
		}
	}

	for (int i = 0; i < mTaskTemplate.mDiagLangList.size(); ++i)
	{
		XMLElement* pDiagElem = pRootElem->FirstChildElement(mTaskTemplate.mDiagLangList[i].c_str());
		if (NULL != pDiagElem)
		{
			XMLElement* pCondElem = pDiagElem->FirstChildElement("Cond");
			for (; NULL != pCondElem; pCondElem = pCondElem->NextSiblingElement("Cond"))
			{
				const char* pLangValue = pCondElem->Attribute("Para0");
				if (NULL != pLangValue)
				{
					_snprintf_s(acBuffer, sizeof(acBuffer), "tkl_%s_%d", pTaskId, nLangCount);
					pCondElem->SetAttribute("Para0", acBuffer);

					XMLElement* pLangElem = mLangDocument.NewElement("Lang");
					pLangElem->SetAttribute("Id", acBuffer);
					pLangElem->SetAttribute("Value", pLangValue);
					pLangRootElem->LinkEndChild(pLangElem);
					++nLangCount;
				}

			}
		}
	}
}

void CMainFrame::SaveTaskLevelConfig(const char* pXmlFilePath, vector<CString>& rVecString)
{
	if (NULL == pXmlFilePath)
	{
		return;
	}
	tinyxml2::XMLDocument tDocument;
	XMLDeclaration* pDeclaration = tDocument.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"");
	tDocument.LinkEndChild(pDeclaration);

	XMLElement* pRootElem = tDocument.NewElement("TaskList");
	tDocument.LinkEndChild(pRootElem);

	pRootElem->SetAttribute("level", mTaskEditorConfig.mLevelPhase);
	char acBuffer[4096] = { 0 };
	for (int i = 0; i < rVecString.size(); ++ i)
	{
		if (_T("") == rVecString[i])
		{
			break;
		}
		XMLElement* pTaskElem = tDocument.NewElement("Task");

		UnicodeToAnsi(rVecString[i].GetBuffer(), acBuffer, sizeof(acBuffer));
		pTaskElem->SetAttribute("id", acBuffer);
		pRootElem->LinkEndChild(pTaskElem);
	}

	tDocument.SaveFile(pXmlFilePath);
}

void CMainFrame::SaveNpcTaskConfig(const char* pXmlFilePath, map<int, CString>& rNpcTask)
{
	if (NULL == pXmlFilePath)
	{
		return;
	}

	tinyxml2::XMLDocument tDocument;
	XMLDeclaration* pDeclaration = tDocument.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"");
	tDocument.LinkEndChild(pDeclaration);

	XMLElement* pRootElem = tDocument.NewElement("TaskList");
	tDocument.LinkEndChild(pRootElem);

	char acBuffer[4096] = { 0 };
	map<int, CString>::iterator it = rNpcTask.begin();
	for (; it != rNpcTask.end(); ++ it)
	{
		XMLElement* pTaskElem = tDocument.NewElement("Task");
		pTaskElem->SetAttribute("npcID", it->first);

		UnicodeToAnsi(it->second, acBuffer, sizeof(acBuffer));
		pTaskElem->SetAttribute("taskID", acBuffer);
		pRootElem->LinkEndChild(pTaskElem);
	}

	tDocument.SaveFile(pXmlFilePath);
}

void CMainFrame::OnTaskInternation()
{

}

void CMainFrame::AddFileItem(tinyxml2::XMLDocument& tDocument, CString strFileName)
{
	m_wndFileView.AddFileItem(tDocument, strFileName);
}

void CMainFrame::ShowOptionView(HWND hWnd, CString strConfigName, CGridCtrl* pGridCtrl, int nRowNum, int nColumnNum)
{
	m_wndOptionView.ShowOptionView(hWnd, strConfigName, pGridCtrl, nRowNum, nColumnNum);
}

void CMainFrame::UpdateFileViewItem(CString& strTaskID, CString& strTaskType, CString& strTaskName)
{
	m_wndFileView.UpdateTreeItem(strTaskID, strTaskType, strTaskName);
}

void CMainFrame::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CMDIFrameWndEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	TranslateAccelerator (m_hWnd, m_hAccel, pMsg);  
	return CMDIFrameWndEx::PreTranslateMessage(pMsg);
}
