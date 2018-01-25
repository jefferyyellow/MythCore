
// TaskEditorView.cpp : CTaskEditorView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTaskEditorView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CTaskEditorView ����/����

CTaskEditorView::CTaskEditorView()
{
	// TODO:  �ڴ˴���ӹ������

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
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CTaskEditorView ����

void CTaskEditorView::OnDraw(CDC* /*pDC*/)
{
	CTaskEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CTaskEditorView ��ӡ


void CTaskEditorView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTaskEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CTaskEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CTaskEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӵ�ӡ����е��������
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


// CTaskEditorView ���

#ifdef _DEBUG
void CTaskEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CTaskEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTaskEditorDoc* CTaskEditorView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTaskEditorDoc)));
	return (CTaskEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CTaskEditorView ��Ϣ�������


void CTaskEditorView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (NULL == pMainFrame)
	{
		return;
	}

	int nNum = (pMainFrame->mTaskTemplate.mNodeList.size() + 1) / 2;

	// TODO:  �ڴ����ר�ô����/����û���
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
	// TODO:  �ڴ����ר�ô����/����û���
	return CView::DestroyWindow();
}
