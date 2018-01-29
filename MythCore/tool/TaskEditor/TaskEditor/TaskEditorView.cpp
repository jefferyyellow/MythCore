
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
	ON_NOTIFY(NM_CLICK, ID_COND_DATA_GRID, OnCondGridClickDown)
//	ON_NOTIFY(CBN_SELCHANGE, ID_COND_DATA_GRID, OnCondGridSelChanged)
	ON_NOTIFY(GVN_ENDLABELEDIT, ID_COND_DATA_GRID, OnCondEndEdit)
	ON_NOTIFY(NM_CLICK, ID_DIAG_DATA_GRID, OnDiagGridClickDown)
END_MESSAGE_MAP()

// CTaskEditorView ����/����

CTaskEditorView::CTaskEditorView()
{
	// TODO:  �ڴ˴���ӹ������

}

CTaskEditorView::~CTaskEditorView()
{
	if (NULL != mMainGrid)
	{
		delete mMainGrid;
		mMainGrid = NULL;
	}

	if (NULL != mCondGrid)
	{
		delete mCondGrid;
		mCondGrid = NULL;
	}
	
	if (NULL == mDiagGrid)
	{
		delete mDiagGrid;
		mDiagGrid = NULL;
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

	// TODO:  �ڴ����ר�ô����/����û���
	int captionHeight = ::GetSystemMetrics(SM_CYCAPTION);
	int cxframe = GetSystemMetrics(SM_CXFRAME);
	int cyframe = GetSystemMetrics(SM_CYFRAME);

	InitialMainNode();
	InitialCondNode();
	InitialDiagNode();

	GetDocument()->OpenDocument();
}

void CTaskEditorView::InitialMainNode()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (NULL == pMainFrame)
	{
		return;
	}
	int nNum = (pMainFrame->mTaskTemplate.mTextNodeList.size() + 2) / 2;

	CRect rectGridWnd;
	pMainFrame->GetClientRect(rectGridWnd);

	mMainGrid = new CGridCtrl(nNum, 4);
	int nHeight = mMainGrid->GetRowHeight(0) * (nNum) + 10;
	rectGridWnd.bottom = rectGridWnd.top + nHeight;
	mMainGrid->Create(rectGridWnd, this, ID_DATA_GRID);
	int nWidth = (rectGridWnd.Width() - 200 - 10) / 2;
	mMainGrid->SetColumnWidth(0, 100);
	mMainGrid->SetColumnWidth(1, nWidth);
	mMainGrid->SetColumnWidth(2, 100);
	mMainGrid->SetColumnWidth(3, nWidth);

	char acBuffer[4096] = { 0 };

	TASK_NODE_LIST& rNodeList = pMainFrame->mTaskTemplate.mTextNodeList;
	int nCount = 0;
	for (int i = 0; i < mMainGrid->GetRowCount(); ++i)
	{
		for (int j = 0; j < mMainGrid->GetColumnCount();)
		{
			mMainGrid->SetItemText(i,j, rNodeList[nCount]->mName.c_str());
			mMainGrid->SetItemState(i, j, mMainGrid->GetItemState(i, j) | GVIS_READONLY);
			CGridCellBase* pCellBase = mMainGrid->GetCell(i, j);
			CGridData* pNewData = new CGridData;
			if (NULL != pCellBase && NULL != pNewData)
			{
				pNewData->mDataType = emDataType_MainNode;
				pNewData->mData = rNodeList[nCount];
				pCellBase->SetData((LPARAM)pNewData);
			}

			++j;
			OPTION_LIST& rOptionList = rNodeList[nCount]->mOptionList;
			if (rOptionList.size() > 0)
			{
				if (!mMainGrid->SetCellType(i, j, RUNTIME_CLASS(CGridCellCombo)))
				{
					continue;
				}

				mMainGrid->SetItemText(i, j, rOptionList[0]->mDes.c_str());
				CStringArray tStringOption;
				for (int nOptionNum = 0; nOptionNum < rOptionList.size(); ++nOptionNum)
				{
					tStringOption.Add(rOptionList[nOptionNum]->mDes.c_str());
				}

				CGridCellCombo *pCell = (CGridCellCombo*)mMainGrid->GetCell(i, j);
				pCell->SetOptions(tStringOption);
				pCell->SetStyle(CBS_DROPDOWN); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE
			}

			++j;

			++nCount;
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
	mMainGrid->Invalidate();
	mMainGrid->ShowWindow(SW_SHOW);
}

void CTaskEditorView::InitialCondNode()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (NULL == pMainFrame)
	{
		return;
	}
	TASK_NODE_LIST& rNodeList = pMainFrame->mTaskTemplate.mCondNodeList;
	int nNum = rNodeList.size();

	CRect rectGridWnd;
	pMainFrame->GetClientRect(rectGridWnd);
	rectGridWnd.top += mMainGrid->GetRowHeight(0) * (mMainGrid->GetRowCount()) + 10;
	rectGridWnd.right = rectGridWnd.Width() / 2 - 4;
	int nWidth = rectGridWnd.Width() / 5 - 1;
	mCondGrid = new CGridCtrl(nNum, 5);
	mCondGrid->Create(rectGridWnd, this, ID_COND_DATA_GRID);
	for (int i = 0; i < 5; ++ i)
	{
		mCondGrid->SetColumnWidth(i, nWidth);
	}

	for (int i = 0; i < nNum; ++ i)
	{
		mCondGrid->SetItemText(i, 0, rNodeList[i]->mName.c_str());
		CGridData* pGridData = new CGridData;
		if (NULL == pGridData)
		{
			break;
		}

		pGridData->mDataType = emDataType_CondMainNode;
		pGridData->mData = rNodeList[i];
		for (int j = 0; j < 5; ++ j)
		{
			mCondGrid->SetItemState(i, j, mMainGrid->GetItemState(i, 0) | GVIS_READONLY);
			CGridCellBase* pCellBase = mCondGrid->GetCell(i, j);
			if (NULL != pCellBase)
			{
				pCellBase->SetData((LPARAM)pGridData);
			}
		}

		mCondGrid->SetItemText(i, 1, _T("����"));
		mCondGrid->SetCellType(i, 1, RUNTIME_CLASS(CGridCellButton));


		mCondGrid->SetItemText(i, 2, _T("ɾ��"));
		mCondGrid->SetCellType(i, 2, RUNTIME_CLASS(CGridCellButton));

	}

	mCondGrid->Invalidate();
	mCondGrid->ShowWindow(SW_SHOW);
}

void CTaskEditorView::InitialDiagNode()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (NULL == pMainFrame)
	{
		return;
	}
	TASK_NODE_LIST& rNodeList = pMainFrame->mTaskTemplate.mDiagNodeList;
	int nNum = rNodeList.size();

	CRect rectDiagWnd;
	pMainFrame->GetClientRect(rectDiagWnd);
	//rectDiagWnd.top += m_pGrid->GetRowHeight(0) * m_pGrid->GetRowCount();
	rectDiagWnd.top += mMainGrid->GetRowHeight(0) * (mMainGrid->GetRowCount()) + 10;

	CRect rectCond;
	mCondGrid->GetClientRect(rectCond);
	rectDiagWnd.left += rectCond.Width() + 10;

	mDiagGrid = new CGridCtrl(nNum, 3);
	mDiagGrid->Create(rectDiagWnd, this, ID_DIAG_DATA_GRID);
	mDiagGrid->SetColumnWidth(0, 100);
	mDiagGrid->SetColumnWidth(1, rectDiagWnd.Width() - 200 - 10);
	mDiagGrid->SetColumnWidth(2, 100);

	for (int i = 0; i < nNum; ++i)
	{
		mDiagGrid->SetItemText(i, 0, rNodeList[i]->mName.c_str());
		for (int j = 0; j < 3; ++j)
		{
			mDiagGrid->SetItemState(i, j, mMainGrid->GetItemState(i, 0) | GVIS_READONLY);
		}
	}

	mDiagGrid->Invalidate();
	mDiagGrid->ShowWindow(SW_SHOW);
}

BOOL CTaskEditorView::DestroyWindow()
{
	// TODO:  �ڴ����ר�ô����/����û���
	return CView::DestroyWindow();
}

void CTaskEditorView::OnCondGridClickDown(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNMHDR;
	int nRow = pItem->iRow;
	int nColumn = pItem->iColumn;

	CCellID cellid = mCondGrid->GetFocusCell();
	int nOldSelect = mCondSelectRow;
	mCondSelectRow = cellid.row;

	CGridCellBase* pGrideCellBase = mCondGrid->GetCell(nRow, nColumn);
	if (NULL == pGrideCellBase)
	{
		return;
	}

	// ����Ĳ��ǰ�ť
	if (!pGrideCellBase->IsKindOf(RUNTIME_CLASS(CGridCellButton)))
	{
		return;
	}




	if (wstring(pGrideCellBase->GetText()) == _T("����"))
	{
		if (nRow == mCondGrid->GetRowCount() - 1)
		{
			mCondGrid->SetRowCount(mCondGrid->GetRowCount() + 1);
			mCondGrid->SetItemText(mCondGrid->GetRowCount() - 1, 0, _T("�µ�һ��"));
		}
		else
		{
			mCondGrid->InsertRow(_T(""), nRow + 1);
		}

		CGridData* pGridData = (CGridData*)pGrideCellBase->GetData();
		if (NULL != pGridData)
		{
			AddCondRow(nRow + 1, (CTaskMainNode*)pGridData->mData);
		}
	}
	else if (wstring(pGrideCellBase->GetText()) == _T("ɾ��"))
	{
		CGridCellBase* pOldSelectCellBase = mCondGrid->GetCell(nOldSelect, 0);
		if (NULL == pOldSelectCellBase)
		{
			return;
		}
		CGridData* pGridData = (CGridData*)pOldSelectCellBase->GetData();
		if (NULL == pGridData)
		{
			return;
		}

		if (emDataType_CondMainNode == pGridData->mDataType)
		{
			return;
		}

		mCondGrid->DeleteRow(nOldSelect);
	}

	mCondGrid->Invalidate();
}

void CTaskEditorView::OnCondEndEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
	int nRow = pItem->iRow;
	int nColumn = pItem->iColumn;
	CGridCellBase* pGrideCellBase = mCondGrid->GetCell(nRow, nColumn);
	if (NULL == pGrideCellBase)
	{
		return;
	}

	if(pGrideCellBase->IsKindOf(RUNTIME_CLASS(CGridCellCombo)))
	{
		CGridData* pGridData = (CGridData*)pGrideCellBase->GetData();
		if (NULL == pGridData)
		{
			return;
		}

		CTaskMainNode* pMainNode = (CTaskMainNode*)pGridData->mData;
		for (int i = 0; i < pMainNode->mOptionList.size(); ++ i)
		{
			if (wstring(pGrideCellBase->GetText()) == pMainNode->mOptionList[i]->mDes)
			{
				SetCondParam(nRow, pMainNode->mOptionList[i]->mNodeList);
				break;
			}
		}
	}
	mCondGrid->Invalidate();
}

void CTaskEditorView::AddCondRow(int nRowNum, CTaskMainNode* pMainNode)
{
	OPTION_LIST& rOptionList = pMainNode->mOptionList;
	CGridData* pGridData = new CGridData;
	if (NULL == pGridData)
	{
		return;
	}
	if (rOptionList.size() > 0)
	{
		if (!mCondGrid->SetCellType(nRowNum, 0, RUNTIME_CLASS(CGridCellCombo)))
		{
			return;
		}
		CStringArray tStringOption;
		for (int nOptionNum = 0; nOptionNum < rOptionList.size(); ++nOptionNum)
		{
			tStringOption.Add(rOptionList[nOptionNum]->mDes.c_str());
		}

		CGridCellCombo *pCell = (CGridCellCombo*)mCondGrid->GetCell(nRowNum, 0);
		pCell->SetText(rOptionList[0]->mDes.c_str());
		pCell->SetOptions(tStringOption);
		pCell->SetStyle(CBS_DROPDOWN); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE
		
		pGridData->mDataType = emDataType_CondDataNode;
		pGridData->mData = pMainNode;
		pCell->SetData((LPARAM)pGridData);
		SetCondParam(nRowNum, rOptionList[0]->mNodeList);

	}
}

void CTaskEditorView::SetCondParam(int nRowNum, TASK_NODE_LIST& rNodeList)
{
	for (int i = 0; i < 4; ++i)
	{
		mCondGrid->SetCellType(nRowNum, i + 1, RUNTIME_CLASS(CGridCell));
		mCondGrid->SetItemText(nRowNum, i + 1, _T(""));
		mCondGrid->SetItemState(nRowNum, i + 1, mCondGrid->GetItemState(nRowNum, i + 1) & ~GVIS_READONLY);

		if (i < rNodeList.size())
		{
			OPTION_LIST& rParamOptionList = rNodeList[i]->mOptionList;
			if (rParamOptionList.size() > 0)
			{
				mCondGrid->SetItemText(nRowNum, i + 1, rParamOptionList[0]->mDes.c_str());
				if (!mCondGrid->SetCellType(nRowNum, i + 1, RUNTIME_CLASS(CGridCellCombo)))
				{
					return;
				}
				CStringArray tStringOption;
				for (int nOptionNum = 0; nOptionNum < rParamOptionList.size(); ++nOptionNum)
				{
					tStringOption.Add(rParamOptionList[nOptionNum]->mDes.c_str());
				}

				CGridCellCombo *pCell = (CGridCellCombo*)mCondGrid->GetCell(nRowNum, i + 1);
				pCell->SetText(rParamOptionList[0]->mDes.c_str());
				pCell->SetOptions(tStringOption);
				pCell->SetStyle(CBS_DROPDOWN); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE
			}
			else
			{
				mCondGrid->SetItemText(nRowNum, i + 1, rNodeList[i]->mName.c_str());
			}
		}
		else
		{
			mCondGrid->SetItemText(nRowNum, i + 1, _T(""));
			mCondGrid->SetItemState(nRowNum, i + 1, mCondGrid->GetItemState(nRowNum, i + 1) | GVIS_READONLY);
		}
	}
}

void CTaskEditorView::OnDiagGridClickDown(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
	int nRow = pItem->iRow;
	int nColumn = pItem->iColumn;

	CGridCellBase* pGrideCellBase = mDiagGrid->GetCell(nRow, nColumn);
	if (NULL == pGrideCellBase)
	{
		return;
	}

	if (NULL == pGrideCellBase->GetData())
	{
		return;
	}

	if (nRow == mDiagGrid->GetRowCount() - 1)
	{
		mDiagGrid->SetRowCount(mCondGrid->GetRowCount() + 1);
		mDiagGrid->SetItemText(mCondGrid->GetRowCount() - 1, 0, _T("�µ�һ��"));
	}
	else
	{
		mDiagGrid->InsertRow(_T("�µ�һ��"), nRow + 1);
	}
	mDiagGrid->Invalidate();
}