
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
#include "GridCellCombo.h"
#include "commondefine.h"
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

	int nNum = (pMainFrame->mTaskTemplate.mNodeList.size() + 2) / 3;

	// TODO:  �ڴ����ר�ô����/����û���
	int captionHeight = ::GetSystemMetrics(SM_CYCAPTION);
	int cxframe = GetSystemMetrics(SM_CXFRAME);
	int cyframe = GetSystemMetrics(SM_CYFRAME);
	
	CRect rectGridWnd;
	pMainFrame->GetClientRect(rectGridWnd);
	
	//rectGridWnd.right;
	//ScreenToClient(&rectGridWnd);

	//rectGridWnd.OffsetRect(cxframe, cyframe - captionHeight);
	m_pGrid = new CGridCtrl(nNum, 6);
	m_pGrid->Create(rectGridWnd, this, ID_DATA_GRID);
	int nWidth = (rectGridWnd.Width() - 300 - 6) / 3;
	m_pGrid->SetColumnWidth(0, 100);
	m_pGrid->SetColumnWidth(1, nWidth);
	m_pGrid->SetColumnWidth(2, 100);
	m_pGrid->SetColumnWidth(3, nWidth);
	m_pGrid->SetColumnWidth(4, 100);
	m_pGrid->SetColumnWidth(5, nWidth);

	char acBuffer[4096] = { 0 };

	TASK_NODE_LIST& rNodeList = pMainFrame->mTaskTemplate.mNodeList;
	int nCount = 0;
	for (int i = 0; i < m_pGrid->GetRowCount(); ++ i)
	{
		for (int j = 0; j < m_pGrid->GetColumnCount();)
		{
			GV_ITEM Item;
			Item.mask = GVIF_TEXT;
			Item.row = i;
			Item.col = j;
			wstring strName = rNodeList[nCount].mName;
			Item.strText.Format(strName.c_str(), 2);
			m_pGrid->SetItem(&Item);
			m_pGrid->SetItemState(i, j, m_pGrid->GetItemState(i, j) | GVIS_READONLY);

			++ j;
			OPTION_LIST& rOptionList = rNodeList[nCount].mOptionList;
			if (rOptionList.size())
			{
				if (!m_pGrid->SetCellType(i, j, RUNTIME_CLASS(CGridCellCombo)))
				{
					continue;
				}
				
				m_pGrid->SetItemText(i, j, rOptionList[0].mDes.c_str());
				CStringArray tStringOption;
				for (int nOptionNum = 0; nOptionNum < rOptionList.size(); ++ nOptionNum)
				{
					tStringOption.Add(rOptionList[nOptionNum].mDes.c_str());
				}

				CGridCellCombo *pCell = (CGridCellCombo*)m_pGrid->GetCell(i, j);
				pCell->SetOptions(tStringOption);
				pCell->SetStyle(CBS_DROPDOWN); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE
			}

			++ j;

			++ nCount;
			if (nCount >= rNodeList.size())
			{
				break;
			}
		}

		if (nCount >= rNodeList.size())
		{
			break;
		}
	}




	//m_pGrid->SetCellType(1, 0, RUNTIME_CLASS(CGridCellButton));
	m_pGrid->Invalidate();
	m_pGrid->ShowWindow(SW_SHOW);
}


BOOL CTaskEditorView::DestroyWindow()
{
	// TODO:  �ڴ����ר�ô����/����û���
	return CView::DestroyWindow();
}
