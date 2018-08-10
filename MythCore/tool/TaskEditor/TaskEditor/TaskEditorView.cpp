
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
	ON_NOTIFY(GVN_BEGINLABELEDIT, ID_COND_DATA_GRID, OnCondStartEdit)
	ON_NOTIFY(GVN_BEGINLABELEDIT, ID_MAIN_DATA_GRID, OnMainStartEdit)
	ON_NOTIFY(GVN_COMBODROPDOWN, ID_COND_DATA_GRID, OnCondComboDropDown)
	ON_NOTIFY(GVN_ENDLABELEDIT, ID_MAIN_DATA_GRID, OnMainEndEdit)
	ON_NOTIFY(GVN_TEXT_CHANGE, ID_MAIN_DATA_GRID, OnMainChange)
	ON_NOTIFY(GVN_TEXT_CHANGE, ID_COND_DATA_GRID, OnMainChange)
	ON_NOTIFY(GVN_TEXT_CHANGE, ID_DIAG_DATA_GRID, OnMainChange)
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
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
	
	if (NULL != mDiagGrid)
	{
		delete mDiagGrid;
		mDiagGrid = NULL;
	}

	GRID_DATA_LIST::iterator it = mGridDataList.begin();
	for (; it != mGridDataList.end(); ++it)
	{
		delete *it;
	}
	mGridDataList.clear();
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

/// 初始化主节点
void CTaskEditorView::InitialMainNode()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (NULL == pMainFrame)
	{
		return;
	}
	int nNum = (pMainFrame->mTaskTemplate.mMainNodeList.size() + 1) / 2;

	CRect rectGridWnd;
	GetClientRect(rectGridWnd);

	mMainGrid = new CGridCtrl(nNum, 4);
	mMainGrid->Create(rectGridWnd, this, ID_MAIN_DATA_GRID);
	mMainGrid->ModifyStyle(0, WS_TABSTOP);
	char acBuffer[4096] = { 0 };

	TASK_NODE_LIST& rNodeList = pMainFrame->mTaskTemplate.mMainNodeList;
	unsigned int nCount = 0;
	for (int i = 0; i < mMainGrid->GetRowCount(); ++i)
	{
		for (int j = 0; j < mMainGrid->GetColumnCount();)
		{
			mMainGrid->SetItemText(i,j, rNodeList[nCount]->mName.c_str());
			mMainGrid->SetItemState(i, j, mMainGrid->GetItemState(i, j) | GVIS_READONLY);
			CGridCellBase* pCellBase = mMainGrid->GetCell(i, j);
			CGridData* pNewData = NewGridData();
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

				mMainGrid->SetItemHintText(i, j, rOptionList[0]->mDes.c_str());
				CStringArray tStringOption;
				for (unsigned int nOptionNum = 0; nOptionNum < rOptionList.size(); ++nOptionNum)
				{
					tStringOption.Add(rOptionList[nOptionNum]->mDes.c_str());
				}

				CGridCellCombo *pCell = (CGridCellCombo*)mMainGrid->GetCell(i, j);
				pCell->SetOptions(tStringOption);
				pCell->SetStyle(CBS_DROPDOWN); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE
			}

			pCellBase = mMainGrid->GetCell(i, j);
			if (NULL != pCellBase)
			{
				pCellBase->SetData((LPARAM)pNewData);
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

/// 初始化条件节点
void CTaskEditorView::InitialCondNode()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (NULL == pMainFrame)
	{
		return;
	}

	TASK_NODE_LIST& rNodeList = pMainFrame->mTaskTemplate.mCondNodeList;
	InitCondDiagNode(mCondGrid, ID_COND_DATA_GRID, rNodeList, MAX_COND_PARAM_NUM + 1);
}

/// 初始化对话节点
void CTaskEditorView::InitialDiagNode()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (NULL == pMainFrame)
	{
		return;
	}
	TASK_NODE_LIST& rNodeList = pMainFrame->mTaskTemplate.mDiagNodeList;
	InitCondDiagNode(mDiagGrid, ID_DIAG_DATA_GRID, rNodeList, MAX_DIAG_PARAM_NUM + 1);
}

/// 初始化条件节点
void CTaskEditorView::InitCondDiagNode(CGridCtrl*& pGridCtrl, int nGridCtrlID, TASK_NODE_LIST& rNodeList, int nColumnNum)
{
	int nNum = rNodeList.size();

	CRect rectDiagWnd;
	GetClientRect(rectDiagWnd);

	pGridCtrl = new CGridCtrl(nNum, nColumnNum);
	pGridCtrl->Create(rectDiagWnd, this, nGridCtrlID);
	pGridCtrl->ModifyStyle(0, WS_TABSTOP);
	for (int i = 0; i < nNum; ++i)
	{
		CGridData* pGridData = NewGridData();
		if (NULL == pGridData)
		{
			break;
		}
		pGridData->mDataType = emDataType_MainNode;
		pGridData->mData = rNodeList[i];
		for (int j = 0; j < nColumnNum; ++j)
		{
			pGridCtrl->SetItemState(i, j, pGridCtrl->GetItemState(i, 0) | GVIS_READONLY);
			CGridCellBase* pCellBase = pGridCtrl->GetCell(i, j);
			if (NULL != pCellBase)
			{
				pCellBase->SetData((LPARAM)pGridData);
			}
		}
		pGridCtrl->SetItemText(i, 0, rNodeList[i]->mName.c_str());

		pGridCtrl->SetItemText(i, 1, _T("增加"));
		pGridCtrl->SetCellType(i, 1, RUNTIME_CLASS(CGridCellButton));


		pGridCtrl->SetItemText(i, 2, _T("删除"));
		pGridCtrl->SetCellType(i, 2, RUNTIME_CLASS(CGridCellButton));
	}

	pGridCtrl->Invalidate();
	pGridCtrl->ShowWindow(SW_SHOW);
}

CGridData* CTaskEditorView::NewGridData()
{
	CGridData* pNewData = new CGridData;
	mGridDataList.push_back(pNewData);
	return pNewData;
}

void CTaskEditorView::DeleteGridData(CGridData* pGridData)
{
	if (NULL == pGridData)
	{
		return;
	}

	GRID_DATA_LIST::iterator it = mGridDataList.begin();
	for (; it != mGridDataList.end(); ++ it)
	{
		if (*it == pGridData)
		{
			delete *it;
			mGridDataList.erase(it);
			break;
		}
	}
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

	CondDiagGridButtonDown(mCondGrid, nRow, nColumn, nOldSelect, MAX_COND_PARAM_NUM, true);
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
	OutputDebugString(pGrideCellBase->GetText());
	if (pGrideCellBase->IsKindOf(RUNTIME_CLASS(CGridCellCombo)))
	{
		CGridData* pGridData = (CGridData*)pGrideCellBase->GetData();
		if (NULL == pGridData)
		{
			return;
		}
		if (emDataType_DataNode !=  pGridData->mDataType)
		{
			return;
		}
		CTaskMainNode* pMainNode = (CTaskMainNode*)pGridData->mData;
		for (unsigned int i = 0; i < pMainNode->mOptionList.size(); ++i)
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

void CTaskEditorView::OnCondComboDropDown(NMHDR* pNMHDR, LRESULT* pResult)
{

}

void CTaskEditorView::OnCondStartEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
	int nRow = pItem->iRow;
	int nColumn = pItem->iColumn;
	MainCondStartEdit(mCondGrid, nRow, nColumn);
}

void CTaskEditorView::OnMainStartEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
	int nRow = pItem->iRow;
	int nColumn = pItem->iColumn;
	MainCondStartEdit(mMainGrid, nRow, nColumn);
}

void CTaskEditorView::MainCondStartEdit(CGridCtrl* pGridCtrl, int nRow, int nColumn)
{
	CGridCellBase* pGrideCellBase = pGridCtrl->GetCell(nRow, nColumn);
	if (NULL == pGrideCellBase)
	{
		return;
	}

	CGridData* pGridData = (CGridData*)pGrideCellBase->GetData();
	if (NULL == pGridData)
	{
		return;
	}


	CTaskMainNode* pMainNode = (CTaskMainNode*)pGridData->mData;
	if (NULL == pMainNode)
	{
		return;
	}
	if (pMainNode->mConfigName != _T(""))
	{
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		if (NULL != pMainFrame)
		{
			pMainFrame->ShowOptionView(m_hWnd, pMainNode->mConfigName.c_str(), pGridCtrl, nRow, nColumn);
		}
	}
}

void CTaskEditorView::OnMainEndEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
	int nRow = pItem->iRow;
	int nColumn = pItem->iColumn;
	CGridCellBase* pGrideCellBase = mMainGrid->GetCell(nRow, nColumn);
	if (NULL == pGrideCellBase)
	{
		return;
	}

	CGridData* pGridData = (CGridData*)pGrideCellBase->GetData();
	if (NULL == pGridData)
	{
		return;
	}


	CTaskMainNode* pMainNode = (CTaskMainNode*)pGridData->mData;
	if (NULL == pMainNode)
	{
		return;
	}
	if (pMainNode->mDesName == _T("TaskId"))
	{
		CString strTitile = CString(pGrideCellBase->GetText()) + _T(".xml");
		GetDocument()->SetTitle(strTitile);
	}
	
}

void CTaskEditorView::OnMainChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	GetDocument()->SetModifiedFlag(TRUE);
}

// 增加一行
void CTaskEditorView::AddCondRow(CGridCtrl* pGridCtrl, int nRowNum, CTaskMainNode* pMainNode, wstring& strCondType, CStringArray& strParaValue, int nParamNum)
{
	if (NULL == pMainNode)
	{
		return;
	}

	OPTION_LIST& rOptionList = pMainNode->mOptionList;
	CGridData* pGridData = NewGridData();
	if (NULL == pGridData)
	{
		return;
	}
	int nOptionIndex = 0;
	CGridCellCombo* pCellCombo = AddComboBox(pGridCtrl, pMainNode, nRowNum, 0, strCondType, nOptionIndex, CBS_DROPDOWNLIST);
	if (NULL == pCellCombo)
	{
		return;
	}

	pGridData->mDataType = emDataType_DataNode;
	pGridData->mData = pMainNode;
	pCellCombo->SetData((LPARAM)pGridData);
	SetCondParam(pGridCtrl, nRowNum, rOptionList[nOptionIndex]->mNodeList, strParaValue, nParamNum);
}

/// 增加一个下拉框的控件
CGridCellCombo* CTaskEditorView::AddComboBox(CGridCtrl* pGridCtrl, CTaskMainNode* pMainNode, int nRowNum, int nColumnNum, wstring& strDefaultValue, int& rOptionIndex, int nStyle)
{
	rOptionIndex = 0;
	OPTION_LIST& rOptionList = pMainNode->mOptionList;
	if (rOptionList.size() > 0)
	{
		bool bFindOptionIndex = false;
		wstring strCellText;
		if (strDefaultValue != _T(""))
		{
			if (pMainNode->mConfigName != _T(""))
			{
				CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if (NULL != pMainFrame)
				{
					int nNum = _wtoi(strDefaultValue.c_str());
					strCellText = pMainFrame->mTaskTemplate.FindOptionName(pMainNode->mConfigName, nNum);
					if (_T("") != strCellText)
					{
						bFindOptionIndex = true;
					}
				}
			}
			else
			{
				for (unsigned int nOptionNum = 0; nOptionNum < rOptionList.size(); ++nOptionNum)
				{
					int tOffset = rOptionList[nOptionNum]->mDes.find(',');
					wstring strValue = rOptionList[nOptionNum]->mDes.substr(0, tOffset);
					if (strValue == strDefaultValue)
					{
						rOptionIndex = nOptionNum;
						bFindOptionIndex = true;
						strCellText = rOptionList[rOptionIndex]->mDes.c_str();
						break;
					}
				}
			}
		}


		if (!pGridCtrl->SetCellType(nRowNum, nColumnNum, RUNTIME_CLASS(CGridCellCombo)))
		{
			return NULL;
		}
		CStringArray tStringOption;
		for (unsigned int nOptionNum = 0; nOptionNum < rOptionList.size(); ++nOptionNum)
		{
			tStringOption.Add(rOptionList[nOptionNum]->mDes.c_str());
		}

		CGridCellCombo *pCell = (CGridCellCombo*)pGridCtrl->GetCell(nRowNum, nColumnNum);
		if (bFindOptionIndex)
		{
			pGridCtrl->SetItemText(nRowNum, nColumnNum, strCellText.c_str());
		}
		else
		{
			pCell->SetHintText(rOptionList[0]->mDes.c_str());
		}
		pCell->SetOptions(tStringOption);
		pCell->SetStyle(nStyle); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE
		return pCell;
	}

	return NULL;
}

/// 设置对应的参数
void CTaskEditorView::SetCondParam(CGridCtrl* pGridCtrl, int nRowNum, TASK_NODE_LIST& rNodeList, CStringArray& strParaValue, int nParamNum)
{
	for (int i = 0; i < nParamNum; ++i)
	{
		pGridCtrl->SetCellType(nRowNum, i + 1, RUNTIME_CLASS(CGridCell));
		pGridCtrl->SetItemText(nRowNum, i + 1, _T(""));
		pGridCtrl->SetItemState(nRowNum, i + 1, pGridCtrl->GetItemState(nRowNum, i + 1) & ~GVIS_READONLY);
		CGridCellBase* pGrideCellBase = pGridCtrl->GetCell(nRowNum, i + 1);
		if (NULL == pGrideCellBase)
		{
			continue;
		}
		CGridData* pGridData = (CGridData*)pGrideCellBase->GetData();


		if (NULL == pGridData)
		{
			pGridData = NewGridData();
			if (NULL == pGridData)
			{
				continue;
			}
		}
		pGrideCellBase->SetData((LPARAM)pGridData);

		wstring strValue;
		if ((unsigned int)i < rNodeList.size())
		{
			if (i < strParaValue.GetSize())
			{
				strValue = strParaValue.GetAt(i);
			}

			pGridData->mData = rNodeList[i];
			pGridData->mDataType = emDataType_ParamNode;

			OPTION_LIST& rParamOptionList = rNodeList[i]->mOptionList;
			if (rParamOptionList.size() > 0 || rNodeList[i]->mConfigName != _T(""))
			{
				int nOptionIndex = 0;
				CGridCellCombo* pCellCombo = AddComboBox(pGridCtrl, rNodeList[i], nRowNum, i + 1, strValue, nOptionIndex, CBS_DROPDOWN);
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
					//pGridCtrl->SetItemText(nRowNum, i + 1, rNodeList[i]->mName.c_str());
					pGridCtrl->SetItemHintText(nRowNum, i + 1, rNodeList[i]->mName.c_str());
				}
			}
		}
		else
		{
			pGridData->mDataType = emDataType_ParamNode;
			pGridData->mData = NULL;

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

	CondDiagGridButtonDown(mDiagGrid, nRow, nColumn, nOldSelect, MAX_DIAG_PARAM_NUM, false);
}

/// 条件和对话的Grid控件的增加/删除按钮按下，增加一行或者删除一行
void CTaskEditorView::CondDiagGridButtonDown(CGridCtrl* pGridCtrl, int nRow, int nColumn, int nOldSelect, int nParamNum, bool Inverted)
{
	CGridCellBase* pGrideCellBase = pGridCtrl->GetCell(nRow, nColumn);
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
		if (Inverted)
		{
			if (nRow == pGridCtrl->GetRowCount() - 1)
			{
				pGridCtrl->SetRowCount(pGridCtrl->GetRowCount() + 1);
			}
			else
			{
				pGridCtrl->InsertRow(_T(""), nRow + 1);
			}

			CGridData* pGridData = (CGridData*)pGrideCellBase->GetData();
			if (NULL != pGridData)
			{
				CStringArray strArray;
				wstring string;
				AddCondRow(pGridCtrl, nRow + 1, (CTaskMainNode*)pGridData->mData, string, strArray, nParamNum);
			}
		}
		else
		{
			int i = nRow + 1;
			for (; i < pGridCtrl->GetRowCount(); ++ i)
			{
				CGridCellBase* pTempCellBase = pGridCtrl->GetCell(i, nColumn);
				if (NULL == pTempCellBase)
				{
					break;
				}
				CGridData* pGridData = (CGridData*)pTempCellBase->GetData();
				if (NULL == pGridData)
				{
					break;
				}

				if (emDataType_MainNode == pGridData->mDataType)
				{
					nRow = i - 1;
					break;
				}
			}
			if (i >= pGridCtrl->GetRowCount())
			{
				nRow = pGridCtrl->GetRowCount() - 1;
			}

			if (nRow == pGridCtrl->GetRowCount() - 1)
			{
				pGridCtrl->SetRowCount(pGridCtrl->GetRowCount() + 1);
			}
			else
			{
				pGridCtrl->InsertRow(_T(""), nRow + 1);
			}
			CGridData* pGridData = (CGridData*)pGrideCellBase->GetData();
			if (NULL != pGridData)
			{
				CStringArray strArray;
				wstring string;
				AddCondRow(pGridCtrl, nRow + 1, (CTaskMainNode*)pGridData->mData, string, strArray, nParamNum);
			}
		}
	}
	else if (wstring(pGrideCellBase->GetText()) == _T("删除"))
	{
		CGridCellBase* pOldSelectCellBase = pGridCtrl->GetCell(nOldSelect, 0);
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
		DeleteGridData(pGridData);
		for (int i = 0; i < nParamNum; ++ i)
		{
			CGridCellBase* pParamCellBase = pGridCtrl->GetCell(nOldSelect, i + 1);
			if (NULL == pParamCellBase)
			{
				continue;
			}
			CGridData* pParamGridData = (CGridData*)pParamCellBase->GetData();
			if (NULL == pParamGridData)
			{
				continue;
			}
			DeleteGridData(pParamGridData);
		}

		pGridCtrl->DeleteRow(nOldSelect);
	}

	pGridCtrl->Invalidate();
}

void CTaskEditorView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	TRACE("ONSize\n");
	CRect rectView;
	GetClientRect(rectView);
	if (rectView.left != rectView.right && rectView.Width() > 410 && NULL != mMainGrid)
	{
		// 处理主Grid控件的位置重新计算
		CRect tMainRect = rectView;
		tMainRect.bottom = tMainRect.top + mMainGrid->GetRowHeight(0) * (mMainGrid->GetRowCount())+6;
		int nWidth = (tMainRect.Width() - 240 - 6) / 2;
		mMainGrid->SetColumnWidth(0, 120);
		mMainGrid->SetColumnWidth(1, nWidth);
		mMainGrid->SetColumnWidth(2, 120);
		mMainGrid->SetColumnWidth(3, nWidth);
		mMainGrid->MoveWindow(tMainRect);


		// 处理条件Grid控件的位置重新计算
		CRect tCondRect = rectView;
		tCondRect.top = tMainRect.bottom;
		//tCondRect.bottom += mCondGrid->GetRowHeight(0) * (mCondGrid->GetRowCount()) + 6;
		tCondRect.right = tCondRect.Width() / 2;
		nWidth = (tCondRect.Width() - 6)/ (MAX_COND_PARAM_NUM + 1);
		for (int i = 0; i < MAX_COND_PARAM_NUM + 1; ++i)
		{
			mCondGrid->SetColumnWidth(i, nWidth);
		}
		mCondGrid->MoveWindow(tCondRect);

		// 处理对话Grid控件的位置重新计算
		CRect tDiagRect = rectView;
		tDiagRect.top += tMainRect.bottom;
		tDiagRect.left = tCondRect.right;
		mDiagGrid->SetColumnWidth(0, 120);
		mDiagGrid->SetColumnWidth(1, tDiagRect.Width() - 240 - 6);
		mDiagGrid->SetColumnWidth(2, 120);
		mDiagGrid->MoveWindow(tDiagRect);
	}
}

/// 通过主节点的名字，得到主节点对应的单元里的文本
CString CTaskEditorView::GetMainGridText(CString strName)
{
	for (int i = 0; i < mMainGrid->GetRowCount(); ++ i)
	{
		for (int j = 1; j < mMainGrid->GetColumnCount(); j += 2)
		{
			CGridCellBase* pGrideCellBase = mMainGrid->GetCell(i, j);
			if (NULL == pGrideCellBase)
			{
				continue;
			}

			CGridData* pGridData = (CGridData*)pGrideCellBase->GetData();
			if (NULL == pGridData)
			{
				continue;
			}


			CTaskMainNode* pMainNode = (CTaskMainNode*)pGridData->mData;
			if (NULL == pMainNode)
			{
				continue;
			}

			if (pMainNode->mDesName == strName.GetBuffer())
			{
				return pGrideCellBase->GetText();
			}
		}
	}

	return _T("");
}

void CTaskEditorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	// CGridCellCombo的可编辑状态下，不能用快捷键触发，特殊处理
	if (IsCTRLpressed() && (nChar == 's' || nChar == 'S'))
	{
		GetDocument()->DoFileSave();
		return;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
