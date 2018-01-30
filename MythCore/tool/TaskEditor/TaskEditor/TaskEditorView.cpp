
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
	ON_NOTIFY(NM_CLICK, ID_DIAG_DATA_GRID, OnDiagGridClickDown)
	ON_NOTIFY(GVN_COMBOSELCHANGE, ID_COND_DATA_GRID, OnComboSelChange)
	ON_NOTIFY(GVN_BEGINLABELEDIT, ID_COND_DATA_GRID, OnStartEdit)
END_MESSAGE_MAP()

// CTaskEditorView 构造/析构

CTaskEditorView::CTaskEditorView()
{
	// TODO:  在此处添加构造代码
	mMainGrid = NULL;
	mCondGrid = NULL;
	mDiagGrid = NULL;
	mCondSelectRow = -1;
	mDiagSelectRow = -1;
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

	//CRect rectGridWnd;
	//pMainFrame->GetClientRect(rectGridWnd);

	CRect rectGridWnd;
	GetClientRect(rectGridWnd);

	mMainGrid = new CGridCtrl(nNum, 4);
	int nHeight = mMainGrid->GetRowHeight(0) * (nNum) + 10;
	rectGridWnd.bottom = rectGridWnd.top + nHeight;
	mMainGrid->Create(rectGridWnd, this, ID_MAIN_DATA_GRID);
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
				pCell->SetStyle(CBS_DROPDOWNLIST); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE
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

		pGridData->mDataType = emDataType_MainNode;
		pGridData->mData = rNodeList[i];
		for (int j = 0; j < MAX_COND_PARAM_NUM + 1; ++ j)
		{
			mCondGrid->SetItemState(i, j, mMainGrid->GetItemState(i, 0) | GVIS_READONLY);
			CGridCellBase* pCellBase = mCondGrid->GetCell(i, j);
			if (NULL != pCellBase)
			{
				pCellBase->SetData((LPARAM)pGridData);
			}
		}

		mCondGrid->SetItemText(i, 1, _T("增加"));
		mCondGrid->SetCellType(i, 1, RUNTIME_CLASS(CGridCellButton));


		mCondGrid->SetItemText(i, 2, _T("删除"));
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
		CGridData* pGridData = new CGridData;
		if (NULL == pGridData)
		{
			break;
		}
		pGridData->mDataType = emDataType_MainNode;
		pGridData->mData = rNodeList[i];
		for (int j = 0; j < 3; ++j)
		{
			mDiagGrid->SetItemState(i, j, mMainGrid->GetItemState(i, 0) | GVIS_READONLY);
			CGridCellBase* pCellBase = mDiagGrid->GetCell(i, j);
			if (NULL != pCellBase)
			{
				pCellBase->SetData((LPARAM)pGridData);
			}
		}
		mDiagGrid->SetItemText(i, 0, rNodeList[i]->mName.c_str());

		mDiagGrid->SetItemText(i, 1, _T("增加"));
		mDiagGrid->SetCellType(i, 1, RUNTIME_CLASS(CGridCellButton));


		mDiagGrid->SetItemText(i, 2, _T("删除"));
		mDiagGrid->SetCellType(i, 2, RUNTIME_CLASS(CGridCellButton));
	}

	mDiagGrid->Invalidate();
	mDiagGrid->ShowWindow(SW_SHOW);
}

BOOL CTaskEditorView::DestroyWindow()
{
	// TODO:  在此添加专用代码和/或调用基类
	return CView::DestroyWindow();
}

void CTaskEditorView::OnCondGridClickDown(NMHDR* pNMHDR, LRESULT* pResult)
{
	CRect rectGridWnd;
	GetClientRect(rectGridWnd);

	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNMHDR;
	int nRow = pItem->iRow;
	int nColumn = pItem->iColumn;

	int nOldSelect = mCondSelectRow;
	mCondSelectRow = nRow;

	CGridCellBase* pGrideCellBase = mCondGrid->GetCell(nRow, nColumn);
	if (NULL == pGrideCellBase)
	{
		return;
	}

	// 点击的不是按钮
	if (!pGrideCellBase->IsKindOf(RUNTIME_CLASS(CGridCellButton)))
	{
		return;
	}


	if (wstring(pGrideCellBase->GetText()) == _T("增加"))
	{
		if (nRow == mCondGrid->GetRowCount() - 1)
		{
			mCondGrid->SetRowCount(mCondGrid->GetRowCount() + 1);
		}
		else
		{
			mCondGrid->InsertRow(_T(""), nRow + 1);
		}

		CGridData* pGridData = (CGridData*)pGrideCellBase->GetData();
		if (NULL != pGridData)
		{
			CStringArray strArray;
			wstring string;
			AddCondRow(mCondGrid, nRow + 1, (CTaskMainNode*)pGridData->mData, string, strArray, MAX_COND_PARAM_NUM);
		}
	}
	else if (wstring(pGrideCellBase->GetText()) == _T("删除"))
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

		if (emDataType_MainNode == pGridData->mDataType)
		{
			return;
		}

		mCondGrid->DeleteRow(nOldSelect);
	}

	mCondGrid->Invalidate();
}

void CTaskEditorView::OnComboSelChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
	int nRow = pItem->iRow;
	int nColumn = pItem->iColumn;
	CGridCellBase* pGrideCellBase = mCondGrid->GetCell(nRow, nColumn);
	if (NULL == pGrideCellBase)
	{
		return;
	}

	if (pGrideCellBase->IsKindOf(RUNTIME_CLASS(CGridCellCombo)))
	{
		CGridData* pGridData = (CGridData*)pGrideCellBase->GetData();
		if (NULL == pGridData)
		{
			return;
		}

		CTaskMainNode* pMainNode = (CTaskMainNode*)pGridData->mData;
		for (int i = 0; i < pMainNode->mOptionList.size(); ++i)
		{
			if (wstring(pGrideCellBase->GetText()) == pMainNode->mOptionList[i]->mDes)
			{
				CStringArray strArray;
				SetCondParam(mCondGrid, nRow, pMainNode->mOptionList[i]->mNodeList, strArray, MAX_COND_PARAM_NUM);
				break;
			}
		}
	}
	mCondGrid->Invalidate();

}

void CTaskEditorView::OnStartEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	//NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
	//int nRow = pItem->iRow;
	//int nColumn = pItem->iColumn;
	//CGridCellBase* pGrideCellBase = mCondGrid->GetCell(nRow, nColumn);
	//if (NULL == pGrideCellBase)
	//{
	//	return;
	//}
	//if (pGrideCellBase->IsKindOf(RUNTIME_CLASS(CGridCellCombo)))
	//{
	//	return;
	//}
	//
}

void CTaskEditorView::AddCondRow(CGridCtrl* pGridCtrl, int nRowNum, CTaskMainNode* pMainNode, wstring& strCondType, CStringArray& strParaValue, int nParamNum)
{
	if (NULL == pMainNode)
	{
		return;
	}

	OPTION_LIST& rOptionList = pMainNode->mOptionList;
	CGridData* pGridData = new CGridData;
	if (NULL == pGridData)
	{
		return;
	}
	int nOptionIndex = 0;
	CGridCellCombo* pCellCombo = AddComboBox(pGridCtrl, rOptionList, nRowNum, 0, strCondType, nOptionIndex);
	if (NULL == pCellCombo)
	{
		return;
	}

	pGridData->mDataType = emDataType_DataNode;
	pGridData->mData = pMainNode;
	pCellCombo->SetData((LPARAM)pGridData);
	SetCondParam(pGridCtrl, nRowNum, rOptionList[nOptionIndex]->mNodeList, strParaValue, nParamNum);
}

CGridCellCombo* CTaskEditorView::AddComboBox(CGridCtrl* pGridCtrl, OPTION_LIST& rOptionList, int nRowNum, int nColumnNum, wstring& strDefaultValue, int& rOptionIndex)
{
	rOptionIndex = 0;
	if (rOptionList.size() > 0)
	{
		for (int nOptionNum = 0; nOptionNum < rOptionList.size(); ++nOptionNum)
		{
			int tOffset = rOptionList[nOptionNum]->mDes.find(',');
			wstring strValue = rOptionList[nOptionNum]->mDes.substr(0, tOffset);
			if (strValue == strDefaultValue)
			{
				rOptionIndex = nOptionNum;
				break;
			}
		}

		if (!pGridCtrl->SetCellType(nRowNum, nColumnNum, RUNTIME_CLASS(CGridCellCombo)))
		{
			return NULL;
		}
		CStringArray tStringOption;
		for (int nOptionNum = 0; nOptionNum < rOptionList.size(); ++nOptionNum)
		{
			tStringOption.Add(rOptionList[nOptionNum]->mDes.c_str());
		}

		CGridCellCombo *pCell = (CGridCellCombo*)pGridCtrl->GetCell(nRowNum, nColumnNum);
		pCell->SetText(rOptionList[rOptionIndex]->mDes.c_str());
		pCell->SetOptions(tStringOption);
		pCell->SetStyle(CBS_DROPDOWNLIST); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE
		return pCell;
	}

	return NULL;
}

void CTaskEditorView::SetCondParam(CGridCtrl* pGridCtrl, int nRowNum, TASK_NODE_LIST& rNodeList, CStringArray& strParaValue, int nParamNum)
{
	for (int i = 0; i < nParamNum; ++i)
	{
		pGridCtrl->SetCellType(nRowNum, i + 1, RUNTIME_CLASS(CGridCell));
		pGridCtrl->SetItemText(nRowNum, i + 1, _T(""));
		pGridCtrl->SetItemState(nRowNum, i + 1, pGridCtrl->GetItemState(nRowNum, i + 1) & ~GVIS_READONLY);

		wstring strValue;
		if (i < rNodeList.size())
		{
			if (i < strParaValue.GetSize())
			{
				strValue = strParaValue.GetAt(i);
			}

			OPTION_LIST& rParamOptionList = rNodeList[i]->mOptionList;
			if (rParamOptionList.size() > 0)
			{
				int nOptionIndex = 0;
				CGridCellCombo* pCellCombo = AddComboBox(pGridCtrl, rParamOptionList, nRowNum, i + 1, strValue, nOptionIndex);
				if (NULL == pCellCombo)
				{
					continue;
				}
			}
			else
			{
				if (strValue != _T(""))
				{
					pGridCtrl->SetItemText(nRowNum, i + 1, strValue.c_str());
				}
				else
				{
					pGridCtrl->SetItemText(nRowNum, i + 1, rNodeList[i]->mName.c_str());
				}
			}
		}
		else
		{
			pGridCtrl->SetItemText(nRowNum, i + 1, _T(""));
			pGridCtrl->SetItemState(nRowNum, i + 1, pGridCtrl->GetItemState(nRowNum, i + 1) | GVIS_READONLY);
		}
	}
}

void CTaskEditorView::OnDiagGridClickDown(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
	int nRow = pItem->iRow;
	int nColumn = pItem->iColumn;

	int nOldSelect = mDiagSelectRow;
	mDiagSelectRow = nRow;

	CGridCellBase* pGrideCellBase = mDiagGrid->GetCell(nRow, nColumn);
	if (NULL == pGrideCellBase)
	{
		return;
	}

	// 点击的不是按钮
	if (!pGrideCellBase->IsKindOf(RUNTIME_CLASS(CGridCellButton)))
	{
		return;
	}


	if (wstring(pGrideCellBase->GetText()) == _T("增加"))
	{
		if (nRow == mDiagGrid->GetRowCount() - 1)
		{
			mDiagGrid->SetRowCount(mDiagGrid->GetRowCount() + 1);
		}
		else
		{
			mDiagGrid->InsertRow(_T(""), nRow + 1);
		}

		CGridData* pGridData = (CGridData*)pGrideCellBase->GetData();
		if (NULL != pGridData)
		{
			CStringArray strArray;
			wstring string;
			AddCondRow(mDiagGrid, nRow + 1, (CTaskMainNode*)pGridData->mData, string, strArray, MAX_DIAG_PARAM_NUM);
		}
	}
	else if (wstring(pGrideCellBase->GetText()) == _T("删除"))
	{
		CGridCellBase* pOldSelectCellBase = mDiagGrid->GetCell(nOldSelect, 0);
		if (NULL == pOldSelectCellBase)
		{
			return;
		}
		CGridData* pGridData = (CGridData*)pOldSelectCellBase->GetData();
		if (NULL == pGridData)
		{
			return;
		}

		if (emDataType_MainNode == pGridData->mDataType)
		{
			return;
		}

		mDiagGrid->DeleteRow(nOldSelect);
	}

	mDiagGrid->Invalidate();
}