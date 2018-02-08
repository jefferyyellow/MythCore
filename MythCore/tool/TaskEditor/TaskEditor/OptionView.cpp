
#include "stdafx.h"
#include "mainfrm.h"
#include "OptionView.h"
#include "Resource.h"
#include "TaskEditor.h"
#include "TaskEditorView.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CTempSelectView
COptionView::COptionView()
{
	mGridCtrl = NULL;
	mRowNum = 0;
	mColumnNum = 0;
}

COptionView::~COptionView()
{
}

BEGIN_MESSAGE_MAP(COptionView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_NOTIFY(NM_DBLCLK, ID_OPTION_VIEW_TREE, &OnDblClkFileView)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar 消息处理程序

int COptionView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图: 
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndOptionView.Create(dwViewStyle, rectDummy, this, ID_OPTION_VIEW_TREE))
	{
		TRACE0("未能创建文件视图\n");
		return -1;      // 未能创建
	}

	// 加载视图图像: 
	m_TempViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndOptionView.SetImageList(&m_TempViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* 已锁定*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由: 
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// 填入一些静态树视图数据(此处只需填入虚拟代码，而不是复杂的数据)
	AdjustLayout();

	return 0;
}

void COptionView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void COptionView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndOptionView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// 选择已单击的项: 
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void COptionView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndOptionView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void COptionView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rectTree;
	m_wndOptionView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void COptionView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndOptionView.SetFocus();
}

void COptionView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* 锁定*/);

	m_TempViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("无法加载位图:  %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_TempViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_TempViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndOptionView.SetImageList(&m_TempViewImages, TVSIL_NORMAL);
}

void COptionView::OnDblClkFileView(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM pItem = m_wndOptionView.GetSelectedItem();
	if (pItem == NULL)
	{
		return;
	}
	if (m_wndOptionView.ItemHasChildren(pItem))
	{
		return;
	}

	CView* pView = ((CFrameWnd*)(AfxGetApp()->m_pMainWnd))->GetActiveFrame()->GetActiveView();
	if (NULL != pView)
	{
		CTaskEditorView* pTaskEditorView = (CTaskEditorView*)pView;
		if (pTaskEditorView->m_hWnd == mHwnd &&
			(mGridCtrl == pTaskEditorView->mMainGrid ||
			mGridCtrl == pTaskEditorView->mCondGrid ||
			mGridCtrl == pTaskEditorView->mDiagGrid))
		{
			CString strTaskFileName = m_wndOptionView.GetItemText(pItem);
			mGridCtrl->SetItemText(mRowNum, mColumnNum, strTaskFileName);
			mGridCtrl->Invalidate();
		}
	}

	
	ShowPane(FALSE, FALSE, FALSE);
}

void COptionView::FillTempOptionView()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (NULL == pMainFrame)
	{
		return;
	}

	OPTION_NAME_LIST& rNameList = pMainFrame->mTaskTemplate.mOptionNameList;
	if (rNameList.size() <= 0)
	{
		return;
	}


	HTREEITEM hRoot = m_wndOptionView.InsertItem(_T("选择项"), 0, 0);
	m_wndOptionView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);


	for (int i = 0; i < rNameList.size(); ++ i)
	{
		HTREEITEM hItem = m_wndOptionView.InsertItem(rNameList[i]->mName.c_str(), 1, 1, hRoot);
		AddTempOptionItem(hItem, rNameList[i]);
	}

	m_wndOptionView.Expand(hRoot, TVE_EXPAND);
}

void COptionView::AddTempOptionItem(HTREEITEM hTreeItem, COptionNameItem* pTempNameItem)
{
	for (int i = 0; i < pTempNameItem->mChildList.size(); ++ i)
	{
		HTREEITEM hItem = m_wndOptionView.InsertItem(pTempNameItem->mChildList[i]->mName.c_str(), 1, 1, hTreeItem);
		AddTempOptionItem(hItem, pTempNameItem->mChildList[i]);
	}
}

HTREEITEM COptionView::GetRootChildItem(CString strItemName)
{
	HTREEITEM hRoot = m_wndOptionView.GetRootItem();
	HTREEITEM hTypeItem = m_wndOptionView.GetChildItem(hRoot);
	for (; NULL != hTypeItem; hTypeItem = m_wndOptionView.GetNextSiblingItem(hTypeItem))
	{
		CString strText = m_wndOptionView.GetItemText(hTypeItem);
		if (strText == strItemName)
		{
			return hTypeItem;
		}
	}
	return NULL;
}

void COptionView::ShowOptionView(HWND hWnd, CString& strConfigName, CGridCtrl* pGridCtrl, int nRowNum, int nColumnNum)
{
	HTREEITEM hRoot = m_wndOptionView.GetRootItem();
	HTREEITEM hChildItem = m_wndOptionView.GetChildItem(hRoot);
	for (; NULL != hChildItem; hChildItem = m_wndOptionView.GetNextSiblingItem(hChildItem))
	{
		m_wndOptionView.Expand(hChildItem, TVE_COLLAPSE );
	}

	ShowPane(TRUE, FALSE, TRUE);
	SetHwnd(hWnd);
	SetGridCtrl(pGridCtrl);
	SetRowNum(nRowNum);
	SetColumnNum(nColumnNum);

	HTREEITEM hTreeItem = GetRootChildItem(strConfigName);
	if (NULL != hTreeItem)
	{
		m_wndOptionView.Expand(hTreeItem, TVE_EXPAND);
	}
}