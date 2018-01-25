
// TaskEditorView.cpp : CTaskEditorView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "TaskEditor.h"
#endif

#include "TaskEditorDoc.h"
#include "TaskEditorView.h"
#include "GridCellButton.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTaskEditorView

IMPLEMENT_DYNCREATE(CTaskEditorView, CView)

BEGIN_MESSAGE_MAP(CTaskEditorView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTaskEditorView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CTaskEditorView 构造/析构

CTaskEditorView::CTaskEditorView()
{
	// TODO:  在此处添加构造代码

}

CTaskEditorView::~CTaskEditorView()
{
	if (NULL != m_pGrid)
	{
		delete m_pGrid;
		m_pGrid = NULL;
	}
}

BOOL CTaskEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CTaskEditorView 绘制

void CTaskEditorView::OnDraw(CDC* /*pDC*/)
{
	CTaskEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  在此处为本机数据添加绘制代码
}


// CTaskEditorView 打印


void CTaskEditorView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTaskEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CTaskEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加额外的打印前进行的初始化过程
}

void CTaskEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加打印后进行的清理过程
}

void CTaskEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTaskEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTaskEditorView 诊断

#ifdef _DEBUG
void CTaskEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CTaskEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTaskEditorDoc* CTaskEditorView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTaskEditorDoc)));
	return (CTaskEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CTaskEditorView 消息处理程序


void CTaskEditorView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (NULL == pMainFrame)
	{
		return;
	}

	int nNum = (pMainFrame->mTaskTemplate.mNodeList.size() + 1) / 2;

	// TODO:  在此添加专用代码和/或调用基类
	int captionHeight = ::GetSystemMetrics(SM_CYCAPTION);
	int cxframe = GetSystemMetrics(SM_CXFRAME);
	int cyframe = GetSystemMetrics(SM_CYFRAME);
	
	CRect rectGridWnd;
	GetWindowRect(&rectGridWnd);
	ScreenToClient(&rectGridWnd);

	//rectGridWnd.OffsetRect(cxframe, cyframe - captionHeight);
	m_pGrid = new CGridCtrl(nNum, 4);
	m_pGrid->Create(rectGridWnd, this, ID_DATA_GRID);
	int nWidth = (rectGridWnd.Width() - 200 - 6) / 2;
	m_pGrid->SetColumnWidth(0, 100);
	m_pGrid->SetColumnWidth(1, nWidth);
	m_pGrid->SetColumnWidth(2, 100);
	m_pGrid->SetColumnWidth(3, nWidth);
	//m_pGrid->SetFixedRowCount(1);
	//m_pGrid->SetFixedColumnCount(1);

	int nCount = 0;
	for (int i = 0; i < m_pGrid->GetRowCount(); i++)
	{
		for (int j = 0; j < m_pGrid->GetColumnCount(); j++)
		{
			GV_ITEM Item;
			Item.mask = GVIF_TEXT;
			Item.row = i;
			Item.col = j;
			wstring strName = pMainFrame->mTaskTemplate.mNodeList[nCount].mName;
			Item.strText.Format(strName.c_str(), 2);
			m_pGrid->SetItem(&Item);
			++ nCount;
			if (nCount >= pMainFrame->mTaskTemplate.mNodeList.size())
			{
				break;
			}
		}

		if (nCount >= pMainFrame->mTaskTemplate.mNodeList.size())
		{
			break;
		}
	}
	m_pGrid->SetCellType(1, 0, RUNTIME_CLASS(CGridCellButton));
	m_pGrid->SetItemState(1, 1, m_pGrid->GetItemState(1, 1) | GVIS_READONLY);
	m_pGrid->SetItemState(1, 0, m_pGrid->GetItemState(1, 0) | GVIS_READONLY);

	m_pGrid->Invalidate();
	m_pGrid->ShowWindow(SW_SHOW);
}


BOOL CTaskEditorView::DestroyWindow()
{
	// TODO:  在此添加专用代码和/或调用基类
	return CView::DestroyWindow();
}
