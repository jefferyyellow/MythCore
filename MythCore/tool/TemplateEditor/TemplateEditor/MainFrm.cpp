// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "TemplateEditor.h"

#include "MainFrm.h"
#include "TemplateEditorView.h"
#include "SearchTemplateDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND( ID_GENERATEBIN, &CMainFrame::OnGenerateBin )
	ON_COMMAND( ID_EDIT_COPY, &CMainFrame::OnClone )
	ON_COMMAND( ID_NEW_TEMPLATE, &CMainFrame::OnNewTemplate )
	ON_COMMAND( ID_IDTOTEMPLATE, &CMainFrame::IDToTemplate)
	ON_WM_SIZE()
	ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR, 
	ID_INDICATOR_DESCRIBE,
	
	ID_SEPARATOR,           // ״̬��ָʾ��
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	mbConsole = false;
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	// TODO: �������Ҫ��������ͣ������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	CRect rect;
	this->GetWindowRect( &rect );
	ScreenToClient( rect );
	m_CatalogTree.Create( this,IDD_CATALOGTREE, CBRS_LEFT | WS_THICKFRAME, IDD_CATALOGTREE );
	m_CatalogTree.ShowWindow( SW_SHOW );

	m_CatalogTree.SetBarStyle(m_CatalogTree.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY  | CBRS_HIDE_INPLACE);
	m_CatalogTree.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	DockControlBar(&m_CatalogTree);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}


// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::OnGenerateBin()
{

	if(IDOK != MessageBox(_T("�Ƿ�����ˢģ��?"), _T("��ʾ"), MB_OKCANCEL))
	{
		return;
	}

	if (CCatalogTree::GetSingletonPtr()->GetReadTempDataing())
	{
		AfxMessageBox(_T("���ڽ��к�̨�����ݶ�ȡ�����Ժ����ԣ�����"));
		return;
	}

	if (CTemplateWrite::GetSingletonPtr()->GetWritingPB())
	{
		return ;
	}
	// ������������PB�ļ�
	CTemplateWrite::GetSingletonPtr()->SetWritingPB(TRUE);

	CTemplateManager::GetSingletonPtr()->WriteAllDirtyData();
	HANDLE ThreadHandle = ::CreateThread( NULL, 0, CMainFrame::GenerateBinThread, NULL, 0, NULL );

}


void CMainFrame::OnClone()
{
	HTREEITEM pSelItem	= m_CatalogTree.GetCatalogTree().GetSelectedItem();
	if(NULL == pSelItem)
	{
		return;
	}

	CTreeItemData* pData = (CTreeItemData*)m_CatalogTree.GetCatalogTree().GetItemData(pSelItem);
	if (NULL == pData)
	{
		return;
	}

	if(pData->GetItemType() != TEMP_ITEM_DATA)
	{
		return;
	}

	m_CatalogTree.OnCloneTemplate();
}

void CMainFrame::OnNewTemplate()
{
	HTREEITEM pSelItem	= m_CatalogTree.GetCatalogTree().GetSelectedItem();
	if(NULL == pSelItem)
	{
		return;
	}

	CTreeItemData* pData = (CTreeItemData*)m_CatalogTree.GetCatalogTree().GetItemData(pSelItem);
	if (NULL == pData)
	{
		return;
	}
	if(pData->GetItemType() != TEMP_ITEM_TABLE)
	{
		return;
	}
	m_CatalogTree.OnCreateTemplate();
}

void CMainFrame::IDToTemplate()
{
	CSearchTemplateDialog dialog;
	dialog.DoModal();
}

// ���ɶ������ļ��ĺ�̨�߳�
DWORD WINAPI CMainFrame::GenerateBinThread(LPVOID param)
{

	CTemplateWrite::GetSingletonPtr()->SetProcessText(_T("��ʼ����ģ������ת��"));
	// ��ʼ����PB�ļ�
	BOOL bResult = CTemplateWrite::GetSingletonPtr()->WriteToPB(CTemplateManager::GetSingletonPtr());
	CTemplateWrite::GetSingletonPtr()->SetProcess(0);
	CTemplateWrite::GetSingletonPtr()->SetProcessText(_T("ģ������ת�����"));

	// ��������PB�ļ��Ѿ�����
	CTemplateWrite::GetSingletonPtr()->SetWritingPB(FALSE);

	CTemplateEditorApp* pApp = (CTemplateEditorApp*)AfxGetApp();
	CMainFrame *pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	if (pMain->mbConsole)
	{
		pMain->SendMessage(WM_CLOSE);
	}
	else
	{
		if(bResult)
		{
			AfxMessageBox(_T("ģ������������ɣ�����"));
		}
		else
		{
			AfxMessageBox(_T("ģ����������ʧ�ܣ�����"));
		}
	}
	return 0;
}


void CMainFrame::SetProcess(int nProgress)
{
	m_wndStatusBar.OnProgress(nProgress);
}

HWND CMainFrame::GetStatusHwnd()
{
	return m_wndStatusBar.m_hWnd;
}
void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: �ڴ˴������Ϣ����������
	
	ReLocate();
}

void CMainFrame::ReLocate()
{
	CRect MainRect;
	GetClientRect(&MainRect);
	int nHeight = MainRect.bottom - MainRect.top;

	CRect StatusRect;
	m_wndStatusBar.GetWindowRect(&StatusRect);
	nHeight -= StatusRect.bottom - StatusRect.top;

	CRect ToolBarRect;
	m_wndToolBar.GetWindowRect(&ToolBarRect);
	nHeight -= ToolBarRect.bottom - ToolBarRect.top;

	CRect TreeDialogRect;
	m_CatalogTree.GetWindowRect(&TreeDialogRect);
	ScreenToClient(TreeDialogRect);
	m_CatalogTree.SetWindowPos( NULL, TreeDialogRect.left, TreeDialogRect.top, TreeDialogRect.Width( ), nHeight, SWP_NOZORDER | SWP_NOMOVE );


	CRect TreeRect;
	m_CatalogTree.GetWindowRect(&TreeRect);
	ScreenToClient(&TreeRect);
	CTreeCtrl *pTree = (CTreeCtrl*) m_CatalogTree.GetDlgItem( IDC_CATALOGTREE );
	if(NULL != pTree)
	{
		pTree->SetWindowPos( NULL, TreeRect.left, TreeRect.top, TreeRect.Width( ), nHeight, SWP_NOZORDER | SWP_NOMOVE );
	}


	//CRect tTempSheet;
	//CRect tTreeSheet;
	//CRect tTempTreeSheet;

	//GetWindowRect( &tTempTreeSheet );
	//m_CatalogTree.GetWindowRect( &tTempSheet );

	//tpTree->GetWindowRect( &tTreeSheet );
	//ScreenToClient( tTempTreeSheet );
	//ScreenToClient( tTempSheet );
	//ScreenToClient( tTreeSheet );
	//m_CatalogTree.SetWindowPos( NULL, tTempSheet.left, tTempSheet.top-5, tTempSheet.Width( ), tTempTreeSheet.Height( )-95 , SWP_NOZORDER | SWP_NOMOVE );
	//tpTree->SetWindowPos( NULL, tTreeSheet.left, tTreeSheet.top, tTreeSheet.Width( ), tTempTreeSheet.Height( ) - 120, SWP_NOZORDER | SWP_NOMOVE );

}
void CMainFrame::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CMDIFrameWnd::OnWindowPosChanged(lpwndpos);

	// TODO: �ڴ˴������Ϣ����������
	//ReLocate();
}

//���ҵ�ǰ���ģ���Ƿ��Ѵ�
CTemplateEditorView* CMainFrame::FindChildWindow(CTemplateFieldData* pFieldData)
{
	if(NULL == ((CMainFrame*)AfxGetMainWnd())->MDIGetActive())
	{
		return NULL;
	}
	CMDIChildWnd* pWnd = (CMDIChildWnd*) ((CMainFrame*)AfxGetMainWnd())->MDIGetActive();
	while( pWnd != NULL )
	{
		CTemplateEditorView* pView = (CTemplateEditorView*)(pWnd->GetActiveView());
		if(NULL != pView)
		{
			if(pView->GetCurrTempData() == pFieldData)
			{
				return pView;
			}
		}

		pWnd = (CMDIChildWnd*) pWnd->GetNextWindow();
	}

	return NULL;
}


void CMainFrame::UpdateDataToView(CTemplateFieldData* pTemplateFieldData)
{
	CTemplateEditorView* pEditorView = FindChildWindow(pTemplateFieldData);
	CMainFrame* pMainFrame = (CMainFrame*)((CMainFrame*)AfxGetMainWnd());
	if(NULL == pEditorView)
	{
		AfxGetApp()->m_pDocManager->OnFileNew();
		pEditorView = (CTemplateEditorView*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive()->GetActiveView();
		pMainFrame->MDIGetActive()->GetActiveDocument()->SetTitle(s2ws(pTemplateFieldData->m_strTemplateName).c_str());
	}		
	//LARGE_INTEGER  frequency,start,end;
	//QueryPerformanceFrequency(&frequency);
	//QueryPerformanceCounter(&start);
	// �����ݸ��µ���ͼ��
	pEditorView->Update(pTemplateFieldData);
	pMainFrame->MDIActivate(pEditorView->GetParentFrame());
	//QueryPerformanceCounter(&end);
	//double d   =   (double)(end.QuadPart   -   start.QuadPart)   /   (double)frequency.QuadPart   *   1000.0;
	//TRACE("\n********name: һ��������%fms********\n", d);
}

void CMainFrame::Bi()
{
	if (CTemplateWrite::GetSingletonPtr()->GetWritingPB())
	{
		return ;
	}
	// ������������PB�ļ�
	CTemplateWrite::GetSingletonPtr()->SetWritingPB(TRUE);

	HANDLE ThreadHandle = ::CreateThread( NULL, 0, CMainFrame::GenerateBinThread, NULL, 0, NULL );
}