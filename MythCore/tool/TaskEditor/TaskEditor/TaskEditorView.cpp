
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
#include "GridCellCombo.h"
#include "commondefine.h"
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
	ON_NOTIFY(NM_CLICK, ID_COND_DATA_GRID, OnCondGridClickDown)
//	ON_NOTIFY(CBN_SELCHANGE, ID_COND_DATA_GRID, OnCondGridSelChanged)
	ON_NOTIFY(GVN_ENDLABELEDIT, ID_COND_DATA_GRID, OnCondEndEdit)
	ON_NOTIFY(NM_CLICK, ID_DIAG_DATA_GRID, OnDiagGridClickDown)
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

	if (NULL != m_pCondGrid)
	{
		delete m_pCondGrid;
		m_pCondGrid = NULL;
	}
	
	if (NULL == m_pDiagGrid)
	{
		delete m_pDiagGrid;
		m_pDiagGrid = NULL;
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

	// TODO:  在此添加专用代码和/或调用基类
	int captionHeight = ::GetSystemMetrics(SM_CYCAPTION);
	int cxframe = GetSystemMetrics(SM_CXFRAME);
	int cyframe = GetSystemMetrics(SM_CYFRAME);

	InitialMainNode();
	InitialCondNode();
	InitialDiagNode();
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

	m_pGrid = new CGridCtrl(nNum, 4);
	int nHeight = m_pGrid->GetRowHeight(0) * (nNum) + 10;
	rectGridWnd.bottom = rectGridWnd.top + nHeight;
	m_pGrid->Create(rectGridWnd, this, ID_DATA_GRID);
	int nWidth = (rectGridWnd.Width() - 200 - 10) / 2;
	m_pGrid->SetColumnWidth(0, 100);
	m_pGrid->SetColumnWidth(1, nWidth);
	m_pGrid->SetColumnWidth(2, 100);
	m_pGrid->SetColumnWidth(3, nWidth);

	char acBuffer[4096] = { 0 };

	TASK_NODE_LIST& rNodeList = pMainFrame->mTaskTemplate.mTextNodeList;
	int nCount = 0;
	for (int i = 0; i < m_pGrid->GetRowCount(); ++i)
	{
		for (int j = 0; j < m_pGrid->GetColumnCount();)
		{
			GV_ITEM Item;
			Item.mask = GVIF_TEXT;
			Item.row = i;
			Item.col = j;
			wstring strName = rNodeList[nCount]->mName;
			Item.strText.Format(strName.c_str(), 2);
			m_pGrid->SetItem(&Item);
			m_pGrid->SetItemState(i, j, m_pGrid->GetItemState(i, j) | GVIS_READONLY);

			++j;
			OPTION_LIST& rOptionList = rNodeList[nCount]->mOptionList;
			if (rOptionList.size() > 0)
			{
				if (!m_pGrid->SetCellType(i, j, RUNTIME_CLASS(CGridCellCombo)))
				{
					continue;
				}

				m_pGrid->SetItemText(i, j, rOptionList[0]->mDes.c_str());
				CStringArray tStringOption;
				for (int nOptionNum = 0; nOptionNum < rOptionList.size(); ++nOptionNum)
				{
					tStringOption.Add(rOptionList[nOptionNum]->mDes.c_str());
				}

				CGridCellCombo *pCell = (CGridCellCombo*)m_pGrid->GetCell(i, j);
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
	m_pGrid->Invalidate();
	m_pGrid->ShowWindow(SW_SHOW);
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
	rectGridWnd.top += m_pGrid->GetRowHeight(0) * (m_pGrid->GetRowCount()) + 10;
	rectGridWnd.right = rectGridWnd.Width() / 2 - 4;
	int nWidth = rectGridWnd.Width() / 5 - 1;
	m_pCondGrid = new CGridCtrl(nNum, 5);
	m_pCondGrid->Create(rectGridWnd, this, ID_COND_DATA_GRID);
	for (int i = 0; i < 5; ++ i)
	{
		m_pCondGrid->SetColumnWidth(i, nWidth);
	}

	for (int i = 0; i < nNum; ++ i)
	{
		m_pCondGrid->SetItemText(i, 0, rNodeList[i]->mName.c_str());
		for (int j = 0; j < 5; ++ j)
		{
			m_pCondGrid->SetItemState(i, j, m_pGrid->GetItemState(i, 0) | GVIS_READONLY);
			CGridCellBase* pCellBase = m_pCondGrid->GetCell(i, j);
			if (NULL != pCellBase)
			{
				pCellBase->SetData((LPARAM)rNodeList[i]);
			}
		}

		m_pCondGrid->SetItemText(i, 1, _T("增加"));
		m_pCondGrid->SetCellType(i, 1, RUNTIME_CLASS(CGridCellButton));


		m_pCondGrid->SetItemText(i, 2, _T("删除"));
		m_pCondGrid->SetCellType(i, 2, RUNTIME_CLASS(CGridCellButton));

	}

	m_pCondGrid->Invalidate();
	m_pCondGrid->ShowWindow(SW_SHOW);
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
	rectDiagWnd.top += m_pGrid->GetRowHeight(0) * (m_pGrid->GetRowCount()) + 10;

	CRect rectCond;
	m_pCondGrid->GetClientRect(rectCond);
	rectDiagWnd.left += rectCond.Width() + 10;

	m_pDiagGrid = new CGridCtrl(nNum, 3);
	m_pDiagGrid->Create(rectDiagWnd, this, ID_DIAG_DATA_GRID);
	m_pDiagGrid->SetColumnWidth(0, 100);
	m_pDiagGrid->SetColumnWidth(1, rectDiagWnd.Width() - 200 - 10);
	m_pDiagGrid->SetColumnWidth(2, 100);

	for (int i = 0; i < nNum; ++i)
	{
		m_pDiagGrid->SetItemText(i, 0, rNodeList[i]->mName.c_str());
		for (int j = 0; j < 3; ++j)
		{
			m_pDiagGrid->SetItemState(i, j, m_pGrid->GetItemState(i, 0) | GVIS_READONLY);
		}
	}

	m_pDiagGrid->Invalidate();
	m_pDiagGrid->ShowWindow(SW_SHOW);
}

BOOL CTaskEditorView::DestroyWindow()
{
	// TODO:  在此添加专用代码和/或调用基类
	return CView::DestroyWindow();
}

void CTaskEditorView::OnCondGridClickDown(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNMHDR;
	int nRow = pItem->iRow;
	int nColumn = pItem->iColumn;

	CGridCellBase* pGrideCellBase = m_pCondGrid->GetCell(nRow, nColumn);
	if (NULL == pGrideCellBase)
	{
		return;
	}

	CCellID cellid = m_pCondGrid->GetFocusCell();
	int nOldSelect = mCondSelectRow;
	mCondSelectRow = cellid.row;

	if (NULL == pGrideCellBase->GetData())
	{
		return;
	}

	if (wstring(pGrideCellBase->GetText()) == _T("增加"))
	{
		if (nRow == m_pCondGrid->GetRowCount() - 1)
		{
			m_pCondGrid->SetRowCount(m_pCondGrid->GetRowCount() + 1);
			m_pCondGrid->SetItemText(m_pCondGrid->GetRowCount() - 1, 0, _T("新的一行"));
		}
		else
		{
			m_pCondGrid->InsertRow(_T(""), nRow + 1);
		}
		AddCondRow(nRow + 1, (CTaskMainNode*)pGrideCellBase->GetData());
	}
	else if (wstring(pGrideCellBase->GetText()) == _T("删除"))
	{
		CGridCellBase* pGrideCellBase = m_pCondGrid->GetCell(nOldSelect, 0);
		if (NULL == pGrideCellBase)
		{
			return;
		}

		if (NULL != pGrideCellBase->GetData())
		{
			return;
		}
		m_pCondGrid->DeleteRow(nOldSelect);
	}

	m_pCondGrid->Invalidate();
}

void CTaskEditorView::OnCondEndEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
	int nRow = pItem->iRow;
	int nColumn = pItem->iColumn;
	CGridCellBase* pGrideCellBase = m_pCondGrid->GetCell(nRow, nColumn);
	if (NULL == pGrideCellBase)
	{
		return;
	}

	if(pGrideCellBase->IsKindOf(RUNTIME_CLASS(CGridCellCombo)))
	{
		CTaskMainNode* pMainNode = (CTaskMainNode*)pGrideCellBase->GetData();
		if (NULL == pMainNode)
		{
			return;
		}

		for (int i = 0; i < pMainNode->mOptionList.size(); ++ i)
		{
			if (wstring(pGrideCellBase->GetText()) == pMainNode->mOptionList[i]->mDes)
			{
				SetCondParam(nRow, pMainNode->mOptionList[i]->mNodeList);
				break;
			}
		}
	}
	m_pCondGrid->Invalidate();
}

void CTaskEditorView::AddCondRow(int nRowNum, CTaskMainNode* pMainNode)
{
	OPTION_LIST& rOptionList = pMainNode->mOptionList;

	if (rOptionList.size() > 0)
	{
		if (!m_pCondGrid->SetCellType(nRowNum, 0, RUNTIME_CLASS(CGridCellCombo)))
		{
			return;
		}
		CStringArray tStringOption;
		for (int nOptionNum = 0; nOptionNum < rOptionList.size(); ++nOptionNum)
		{
			tStringOption.Add(rOptionList[nOptionNum]->mDes.c_str());
		}

		CGridCellCombo *pCell = (CGridCellCombo*)m_pCondGrid->GetCell(nRowNum, 0);
		pCell->SetText(rOptionList[0]->mDes.c_str());
		pCell->SetOptions(tStringOption);
		pCell->SetStyle(CBS_DROPDOWN); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE
		pCell->SetData((LPARAM)pMainNode);
		SetCondParam(nRowNum, rOptionList[0]->mNodeList);

	}
}

void CTaskEditorView::SetCondParam(int nRowNum, TASK_NODE_LIST& rNodeList)
{
	for (int i = 0; i < 4; ++i)
	{
		m_pCondGrid->SetCellType(nRowNum, i + 1, RUNTIME_CLASS(CGridCell));
		m_pCondGrid->SetItemText(nRowNum, i + 1, _T(""));
		m_pCondGrid->SetItemState(nRowNum, i + 1, m_pCondGrid->GetItemState(nRowNum, i + 1) & ~GVIS_READONLY);

		if (i < rNodeList.size())
		{
			OPTION_LIST& rParamOptionList = rNodeList[i]->mOptionList;
			if (rParamOptionList.size() > 0)
			{
				m_pCondGrid->SetItemText(nRowNum, i + 1, rParamOptionList[0]->mDes.c_str());
				if (!m_pCondGrid->SetCellType(nRowNum, i + 1, RUNTIME_CLASS(CGridCellCombo)))
				{
					return;
				}
				CStringArray tStringOption;
				for (int nOptionNum = 0; nOptionNum < rParamOptionList.size(); ++nOptionNum)
				{
					tStringOption.Add(rParamOptionList[nOptionNum]->mDes.c_str());
				}

				CGridCellCombo *pCell = (CGridCellCombo*)m_pCondGrid->GetCell(nRowNum, i + 1);
				pCell->SetText(rParamOptionList[0]->mDes.c_str());
				pCell->SetOptions(tStringOption);
				pCell->SetStyle(CBS_DROPDOWN); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE
			}
			else
			{
				m_pCondGrid->SetItemText(nRowNum, i + 1, rNodeList[i]->mName.c_str());
			}
		}
		else
		{
			m_pCondGrid->SetItemText(nRowNum, i + 1, _T(""));
			m_pCondGrid->SetItemState(nRowNum, i + 1, m_pCondGrid->GetItemState(nRowNum, i + 1) | GVIS_READONLY);
		}
	}
}

void CTaskEditorView::OnDiagGridClickDown(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
	int nRow = pItem->iRow;
	int nColumn = pItem->iColumn;

	CGridCellBase* pGrideCellBase = m_pDiagGrid->GetCell(nRow, nColumn);
	if (NULL == pGrideCellBase)
	{
		return;
	}

	if (NULL == pGrideCellBase->GetData())
	{
		return;
	}

	if (nRow == m_pDiagGrid->GetRowCount() - 1)
	{
		m_pDiagGrid->SetRowCount(m_pCondGrid->GetRowCount() + 1);
		m_pDiagGrid->SetItemText(m_pCondGrid->GetRowCount() - 1, 0, _T("新的一行"));
	}
	else
	{
		m_pDiagGrid->InsertRow(_T("新的一行"), nRow + 1);
	}
	m_pDiagGrid->Invalidate();
}