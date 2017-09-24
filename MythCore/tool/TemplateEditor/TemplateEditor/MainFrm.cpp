// MainFrm.cpp : CMainFrame 类的实现
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
	
	ID_SEPARATOR,           // 状态行指示器
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
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
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	// TODO: 如果不需要工具栏可停靠，则删除这三行
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
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}


// CMainFrame 诊断

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


// CMainFrame 消息处理程序

void CMainFrame::OnGenerateBin()
{

	if(IDOK != MessageBox(_T("是否现在刷模板?"), _T("提示"), MB_OKCANCEL))
	{
		return;
	}

	if (CCatalogTree::GetSingletonPtr()->GetReadTempDataing())
	{
		AfxMessageBox(_T("正在进行后台的数据读取，请稍后再试！！！"));
		return;
	}

	if (CTemplateWrite::GetSingletonPtr()->GetWritingPB())
	{
		return ;
	}
	// 设置正在生成PB文件
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

// 生成二进制文件的后台线程
DWORD WINAPI CMainFrame::GenerateBinThread(LPVOID param)
{

	CTemplateWrite::GetSingletonPtr()->SetProcessText(_T("开始进行模板数据转换"));
	// 开始生成PB文件
	BOOL bResult = CTemplateWrite::GetSingletonPtr()->WriteToPB(CTemplateManager::GetSingletonPtr());
	CTemplateWrite::GetSingletonPtr()->SetProcess(0);
	CTemplateWrite::GetSingletonPtr()->SetProcessText(_T("模块数据转换完成"));

	// 设置生成PB文件已经结束
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
			AfxMessageBox(_T("模板数据生成完成！！！"));
		}
		else
		{
			AfxMessageBox(_T("模板数据生成失败！！！"));
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
	
	// TODO: 在此处添加消息处理程序代码
	
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

	// TODO: 在此处添加消息处理程序代码
	//ReLocate();
}

//查找当前点击模版是否已打开
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
	// 将数据更新到视图上
	pEditorView->Update(pTemplateFieldData);
	pMainFrame->MDIActivate(pEditorView->GetParentFrame());
	//QueryPerformanceCounter(&end);
	//double d   =   (double)(end.QuadPart   -   start.QuadPart)   /   (double)frequency.QuadPart   *   1000.0;
	//TRACE("\n********name: 一共运行了%fms********\n", d);
}

void CMainFrame::Bi()
{
	if (CTemplateWrite::GetSingletonPtr()->GetWritingPB())
	{
		return ;
	}
	// 设置正在生成PB文件
	CTemplateWrite::GetSingletonPtr()->SetWritingPB(TRUE);

	HANDLE ThreadHandle = ::CreateThread( NULL, 0, CMainFrame::GenerateBinThread, NULL, 0, NULL );
}