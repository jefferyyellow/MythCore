
#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "TaskEditor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
{
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_NOTIFY(NM_DBLCLK, 4, &OnDblClkFileView)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar ��Ϣ�������

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// ������ͼ: 
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("δ�ܴ����ļ���ͼ\n");
		return -1;      // δ�ܴ���
	}

	// ������ͼͼ��: 
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* ������*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��: 
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// ����һЩ��̬����ͼ����(�˴�ֻ������������룬�����Ǹ��ӵ�����)
	FillFileView();
	AdjustLayout();

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileView::FillFileView()
{
	HTREEITEM hRoot = m_wndFileView.InsertItem(_T("�����ļ�"), 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	WIN32_FIND_DATA ffd;	
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;	

	hFind = FindFirstFile(_T("Tasks\\Task_*"), &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return ;
	}

	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			m_wndFileView.InsertItem(ffd.cFileName, 1, 1, hRoot);
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
		return;
	}
	FindClose(hFind);
	m_wndFileView.Expand(hRoot, TVE_EXPAND);
}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// ѡ���ѵ�������: 
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

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnProperties()
{
	AfxMessageBox(_T("����...."));

}

void CFileView::OnFileOpen()
{
	// TODO:  �ڴ˴���������������
	OpenSelectFile();
	//pEditorView = (CTemplateEditorView*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive()->GetActiveView();
	//pMainFrame->MDIGetActive()->GetActiveDocument()->SetTitle(s2ws(pTemplateFieldData->m_strTemplateName).c_str());

}

void CFileView::OnFileOpenWith()
{
	// TODO:  �ڴ˴���������������
}

void CFileView::OnDummyCompile()
{
	// TODO:  �ڴ˴���������������
}

void CFileView::OnEditCut()
{
	// TODO:  �ڴ˴���������������
}

void CFileView::OnEditCopy()
{
	// TODO:  �ڴ˴���������������
}

void CFileView::OnEditClear()
{
	// TODO:  �ڴ˴���������������
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* ����*/);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("�޷�����λͼ:  %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}

void CFileView::OnDblClkFileView(NMHDR *pNMHDR, LRESULT *pResult)
{
	OpenSelectFile();
}

void CFileView::OpenSelectFile()
{
	HTREEITEM pItem = m_wndFileView.GetSelectedItem();
	if (pItem == NULL)
	{
		return;
	}
	CString strTaskFileName = m_wndFileView.GetItemText(pItem);
	CString strFilePath = CString("Tasks\\") + strTaskFileName;

	AfxGetApp()->OpenDocumentFile(strFilePath);
}