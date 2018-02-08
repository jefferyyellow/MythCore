
#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "TaskEditor.h"
#include "TaskEditorDoc.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView
// 树节点排序函数（叶子节点），按照任务ID排序
int CALLBACK TreeCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CViewTree*       pViewTree = (CViewTree*)lParamSort;
	CString         strItem1 = pViewTree->GetItemText((HTREEITEM)lParam1);
	CString         strItem2 = pViewTree->GetItemText((HTREEITEM)lParam2);

	int nPos1 = strItem1.Find(_T('('));
	int nPos2 = strItem2.Find(_T('('));

	if (nPos1 < 0)
	{
		return -1;
	}

	if (nPos2 < 0)
	{
		return 1;
	}

	strItem1 = strItem1.Right(strItem1.GetLength() - nPos1 - 1);
	strItem2 = strItem2.Right(strItem2.GetLength() - nPos2 - 1);

	nPos1 = strItem1.Find(_T(')'));
	nPos2 = strItem2.Find(_T(')'));

	if (nPos1 < 0)
	{
		return -1;
	}

	if (nPos2 < 0)
	{
		return 1;
	}

	// 取出任务ID
	strItem1 = strItem1.Left(nPos1);
	strItem2 = strItem2.Left(nPos2);

	// 将任务ID转换成整数比较
	int nItem1 = _ttoi(strItem1);
	int nItem2 = _ttoi(strItem2);
	if (nItem1 > nItem2)
	{
		return 1;
	}
	else if (nItem1 < nItem2)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

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
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_NOTIFY(NM_DBLCLK, 4, &OnDblClkFileView)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar 消息处理程序

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图: 
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("未能创建文件视图\n");
		return -1;      // 未能创建
	}

	// 加载视图图像: 
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

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

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

/// 填充文件树节点
void CFileView::FillFileView()
{
	HTREEITEM hRoot = m_wndFileView.InsertItem(_T("任务文件"), 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	WIN32_FIND_DATA ffd;	
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;	

	// 注意FindFirstFile需要用到*的通配符
	hFind = FindFirstFile(_T("Tasks\\*"), &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return ;
	}

	do
	{
		// 只处理非文件夹文件
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			CString strFilePath = CString("Tasks\\") + ffd.cFileName;
			CString strFileName = ffd.cFileName;
			// 取出文件名，和任务ID一样
			strFileName = strFileName.Left(strFileName.Find('.'));
			CString strName;
			CString strType;
			// 对不是以数字开头的文件，直接过滤掉
			if (ffd.cFileName[0] < _T('0') || ffd.cFileName[0] > _T('9'))
			{
				continue;
			}
			tinyxml2::XMLDocument tDocument;
			char acBuffer[4096] = { 0 };
			UnicodeToAnsi(strFilePath, acBuffer, sizeof(acBuffer));
			if (tinyxml2::XML_SUCCESS != tDocument.LoadFile(acBuffer))
			{
				continue;
			}
			// 得到任务的类型和名字，加上上面的ID，可以构成任务树节点的各个元素了
			GetTaskTypeAndName(tDocument, strType, strName);
			// 通过类型名字得到对应的类型节点，没有的话就新建一个
			HTREEITEM tTypeItem = GetRootChildItem(strType);
			if (NULL == tTypeItem)
			{
				tTypeItem = m_wndFileView.InsertItem(strType, 1, 1, hRoot, TVI_SORT);
				m_wndFileView.SetItemData(tTypeItem, (DWORD)tTypeItem);
			}

			strName = strName + _T("(") + strFileName + _T(")");
			
			HTREEITEM hItem = m_wndFileView.InsertItem(strName, 1, 1, tTypeItem, TVI_SORT);
			// 设置树节点的数据，用于节点排序，排序函数的参数就是设置的节点的Data
			m_wndFileView.SetItemData(hItem, (DWORD)hItem);

		}
	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
		FindClose(hFind);
		return;
	}
	FindClose(hFind);
	//ExtendAllItem(hRoot);
	// 展开根节点
	m_wndFileView.Expand(hRoot, TVE_EXPAND);
	// 对叶子节点排序
	FileViewSort(hRoot);
}

/// 展开所有的节点
void CFileView::ExtendAllItem(HTREEITEM hItem)
{
	m_wndFileView.Expand(hItem, TVE_EXPAND);
	HTREEITEM hChildItem = m_wndFileView.GetChildItem(hItem);
	for (; NULL != hChildItem; hChildItem = m_wndFileView.GetNextSiblingItem(hChildItem))
	{
		ExtendAllItem(hChildItem);
	}
}

/// 更新任务ID对应的任务类型和任务名字
void CFileView::UpdateTreeItem(CString& strTaskID, CString& strTaskType, CString& strTaskName)
{
	HTREEITEM hRoot = m_wndFileView.GetRootItem();
	HTREEITEM tTaskItem = FindTreeItem(hRoot, strTaskID);
	HTREEITEM tParentItem = m_wndFileView.GetParentItem(tTaskItem);

	CString strOldTaskName = m_wndFileView.GetItemText(tTaskItem);
	strOldTaskName = strOldTaskName.Left(strOldTaskName.Find('('));
	CString strOldTypeName = m_wndFileView.GetItemText(tParentItem);
	// 没有变化，直接返回
	if (strOldTaskName == strTaskName && strOldTypeName == strTaskType)
	{
		return;
	}

	// 类型不相等才需要将类型删除
	if (strOldTypeName != strTaskType)
	{
		m_wndFileView.DeleteItem(tTaskItem);
		if (!m_wndFileView.ItemHasChildren(tParentItem))
		{
			m_wndFileView.DeleteItem(tParentItem);
		}
		AddFileItem(strTaskType, strTaskName, strTaskID);
	}
	else
	{
		CString strName = strTaskName + _T("(") + strTaskID + _T(")");
		m_wndFileView.SetItemText(tTaskItem, strName);
	}
}

/// 通过名字找树节点的子节点
HTREEITEM CFileView::FindTreeItem(HTREEITEM hTreeItem, CString& strItemName)
{
	HTREEITEM hChildItem = m_wndFileView.GetChildItem(hTreeItem);
	for (; NULL != hChildItem; hChildItem = m_wndFileView.GetNextSiblingItem(hChildItem))
	{
		CString         strItemText = m_wndFileView.GetItemText(hChildItem);
		strItemText = strItemText.Right(strItemText.GetLength() - strItemText.Find(_T('(')) - 1);
		strItemText = strItemText.Left(strItemText.Find(_T(')')));

		if (strItemText == strItemName)
		{
			return hChildItem;
		}

		HTREEITEM tFindItem = FindTreeItem(hChildItem, strItemName);
		if (NULL != tFindItem)
		{
			return tFindItem;
		}
	}

	return NULL;
}

/// 通过名字搜索根节点的子节点
HTREEITEM CFileView::GetRootChildItem(CString strItemName)
{
	HTREEITEM hRoot = m_wndFileView.GetRootItem();
	HTREEITEM hTypeItem = m_wndFileView.GetChildItem(hRoot);
	for (; NULL != hTypeItem; hTypeItem = m_wndFileView.GetNextSiblingItem(hTypeItem))
	{
		CString strText = m_wndFileView.GetItemText(hTypeItem);
		if (strText == strItemName)
		{
			return hTypeItem;
		}
	}
	return NULL;
}

/// 增加文件节点
void CFileView::AddFileItem(tinyxml2::XMLDocument& tDocument, CString strFileName)
{
	CString strName;
	CString strType;
	GetTaskTypeAndName(tDocument, strType, strName);

	AddFileItem(strType, strName, strFileName);
}

void CFileView::AddFileItem(CString strType, CString strName, CString strFileName)
{
	HTREEITEM hRoot = m_wndFileView.GetRootItem();
	HTREEITEM tTypeItem = GetRootChildItem(strType);
	if (NULL == tTypeItem)
	{
		tTypeItem = m_wndFileView.InsertItem(strType, 1, 1, hRoot, TVI_SORT);
		m_wndFileView.SetItemData(tTypeItem, (DWORD)tTypeItem);
	}

	strName = strName + _T("(") + strFileName + _T(")");
	HTREEITEM hItem = m_wndFileView.InsertItem(strName, 1, 1, tTypeItem, TVI_SORT);
	m_wndFileView.SetItemData(hItem, (DWORD)hItem);

	m_wndFileView.Expand(tTypeItem, TVE_EXPAND);
	FileViewSort(hRoot);
}
/// 文件节点树叶子节点排序
void CFileView::FileViewSort(HTREEITEM hParentItem)
{
	HTREEITEM hChildItem = m_wndFileView.GetChildItem(hParentItem);
	if (NULL == hChildItem)
	{
		return;
	}
	// 对叶子节点排序
	if (!m_wndFileView.ItemHasChildren(hChildItem))
	{
		TVSORTCB tSortCB;
		tSortCB.hParent = hParentItem;
		tSortCB.lpfnCompare = TreeCompareProc;
		tSortCB.lParam = (LPARAM)&m_wndFileView;
		m_wndFileView.SortChildrenCB(&tSortCB);
	}

	for (; NULL != hChildItem; hChildItem = m_wndFileView.GetNextSiblingItem(hChildItem))
	{
		FileViewSort(hChildItem);
	}
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

/// 打开选择的文件
void CFileView::OnFileOpen()
{
	// TODO:  在此处添加命令处理程序代码
	OpenSelectFile();
}

/// 拷贝选择节点的文本
void CFileView::OnEditCopy()
{
	// TODO:  在此处添加命令处理程序代码
	HTREEITEM hTreeItem = m_wndFileView.GetSelectedItem();
	if (NULL == hTreeItem)
	{
		return;
	}
	mCopyItemString = m_wndFileView.GetItemText(hTreeItem);
	mCopyItemString = mCopyItemString.Right(mCopyItemString.GetLength() - mCopyItemString.Find(_T('(')) - 1);
	mCopyItemString = mCopyItemString.Left(mCopyItemString.Find(_T(')')));
}

/// 粘贴选择的文件
void CFileView::OnEditPaste()
{
	CWinApp* pApp = AfxGetApp();
	POSITION curTemplatePos = pApp->GetFirstDocTemplatePosition();
	if (curTemplatePos != NULL)
	{
		CDocTemplate* curTemplate = pApp->GetNextDocTemplate(curTemplatePos);
		CTaskEditorDoc* pDoc = (CTaskEditorDoc*)curTemplate->OpenDocumentFile(NULL);
		if (NULL != pDoc)
		{
			pDoc->PasteNew(mCopyItemString);
		}
	}
}

void CFileView::OnEditClear()
{
	// TODO:  在此处添加命令处理程序代码
	int nResult = AfxMessageBox(_T("为了避免误删，目前不支持删除"), MB_OKCANCEL);
	//if (nResult == IDOK)
	//{
	//	
	//}
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

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
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* 锁定*/);

	m_FileViewImages.DeleteImageList();

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

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}

void CFileView::OnDblClkFileView(NMHDR *pNMHDR, LRESULT *pResult)
{
	OpenSelectFile();
}

/// 打开选中的文件
void CFileView::OpenSelectFile()
{
	HTREEITEM pItem = m_wndFileView.GetSelectedItem();
	if (pItem == NULL)
	{
		return;
	}
	if (m_wndFileView.ItemHasChildren(pItem))
	{
		return;
	}
	CString strTaskFileName = m_wndFileView.GetItemText(pItem);
	strTaskFileName = strTaskFileName.Right(strTaskFileName.GetLength() - strTaskFileName.Find('(') - 1);
	strTaskFileName = strTaskFileName.Left(strTaskFileName.Find(')'));
	CString strFilePath = CString("Tasks\\") + strTaskFileName + _T(".xml");

	AfxGetApp()->OpenDocumentFile(strFilePath);
}

/// 从xml里面得到文件的类型和名字
void CFileView::GetTaskTypeAndName(tinyxml2::XMLDocument& rDocument, CString& rStrType, CString& rStrName)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (NULL == pMainFrame)
	{
		return;
	}


	XMLElement* pRootElem = rDocument.RootElement();
	if (NULL == pRootElem)
	{
		return;
	}

	XMLElement* pTaskNameElem = pRootElem->FirstChildElement("TaskName");
	if (NULL == pTaskNameElem)
	{
		return;
	}

	XMLElement* pTaskTypeElem = pRootElem->FirstChildElement("TaskType");
	if (NULL == pTaskTypeElem)
	{
		return;
	}

	wchar_t wBuffer[4096] = { 0 };
	Utf8ToUnicode(pTaskNameElem->Attribute("Value"), wBuffer, sizeof(wBuffer) / 2 - 1);
	rStrName = wBuffer;

	CTaskMainNode* pMainNode = pMainFrame->mTaskTemplate.GetTaskMainNode(_T("TaskType"));
	if (NULL == pMainNode)
	{
		return;
	}
	Utf8ToUnicode(pTaskTypeElem->Attribute("Value"), wBuffer, sizeof(wBuffer) / 2 - 1);

	OPTION_LIST& rOptionList = pMainNode->mOptionList;
	for (unsigned int nOptionNum = 0; nOptionNum < rOptionList.size(); ++nOptionNum)
	{
		int tOffset = rOptionList[nOptionNum]->mDes.find(',');
		wstring strValue = rOptionList[nOptionNum]->mDes.substr(0, tOffset);
		if (strValue == wBuffer)
		{
			++ tOffset;
			rStrType = rOptionList[nOptionNum]->mDes.substr(tOffset, rOptionList[nOptionNum]->mDes.length() - tOffset).c_str();
			break;
		}
	}

}