
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
// ���ڵ���������Ҷ�ӽڵ㣩����������ID����
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

	// ȡ������ID
	strItem1 = strItem1.Left(nPos1);
	strItem2 = strItem2.Left(nPos2);

	// ������IDת���������Ƚ�
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
	AdjustLayout();

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

/// ����ļ����ڵ�
void CFileView::FillFileView()
{
	HTREEITEM hRoot = m_wndFileView.InsertItem(_T("�����ļ�"), 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	WIN32_FIND_DATA ffd;	
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;	

	// ע��FindFirstFile��Ҫ�õ�*��ͨ���
	hFind = FindFirstFile(_T("Tasks\\*"), &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return ;
	}

	do
	{
		// ֻ������ļ����ļ�
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			CString strFilePath = CString("Tasks\\") + ffd.cFileName;
			CString strFileName = ffd.cFileName;
			// ȡ���ļ�����������IDһ��
			strFileName = strFileName.Left(strFileName.Find('.'));
			CString strName;
			CString strType;
			// �Բ��������ֿ�ͷ���ļ���ֱ�ӹ��˵�
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
			// �õ���������ͺ����֣����������ID�����Թ����������ڵ�ĸ���Ԫ����
			GetTaskTypeAndName(tDocument, strType, strName);
			// ͨ���������ֵõ���Ӧ�����ͽڵ㣬û�еĻ����½�һ��
			HTREEITEM tTypeItem = GetRootChildItem(strType);
			if (NULL == tTypeItem)
			{
				tTypeItem = m_wndFileView.InsertItem(strType, 1, 1, hRoot, TVI_SORT);
				m_wndFileView.SetItemData(tTypeItem, (DWORD)tTypeItem);
			}

			strName = strName + _T("(") + strFileName + _T(")");
			
			HTREEITEM hItem = m_wndFileView.InsertItem(strName, 1, 1, tTypeItem, TVI_SORT);
			// �������ڵ�����ݣ����ڽڵ������������Ĳ����������õĽڵ��Data
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
	// չ�����ڵ�
	m_wndFileView.Expand(hRoot, TVE_EXPAND);
	// ��Ҷ�ӽڵ�����
	FileViewSort(hRoot);
}

/// չ�����еĽڵ�
void CFileView::ExtendAllItem(HTREEITEM hItem)
{
	m_wndFileView.Expand(hItem, TVE_EXPAND);
	HTREEITEM hChildItem = m_wndFileView.GetChildItem(hItem);
	for (; NULL != hChildItem; hChildItem = m_wndFileView.GetNextSiblingItem(hChildItem))
	{
		ExtendAllItem(hChildItem);
	}
}

/// ��������ID��Ӧ���������ͺ���������
void CFileView::UpdateTreeItem(CString& strTaskID, CString& strTaskType, CString& strTaskName)
{
	HTREEITEM hRoot = m_wndFileView.GetRootItem();
	HTREEITEM tTaskItem = FindTreeItem(hRoot, strTaskID);
	HTREEITEM tParentItem = m_wndFileView.GetParentItem(tTaskItem);

	CString strOldTaskName = m_wndFileView.GetItemText(tTaskItem);
	strOldTaskName = strOldTaskName.Left(strOldTaskName.Find('('));
	CString strOldTypeName = m_wndFileView.GetItemText(tParentItem);
	// û�б仯��ֱ�ӷ���
	if (strOldTaskName == strTaskName && strOldTypeName == strTaskType)
	{
		return;
	}

	// ���Ͳ���Ȳ���Ҫ������ɾ��
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

/// ͨ�����������ڵ���ӽڵ�
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

/// ͨ�������������ڵ���ӽڵ�
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

/// �����ļ��ڵ�
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
/// �ļ��ڵ���Ҷ�ӽڵ�����
void CFileView::FileViewSort(HTREEITEM hParentItem)
{
	HTREEITEM hChildItem = m_wndFileView.GetChildItem(hParentItem);
	if (NULL == hChildItem)
	{
		return;
	}
	// ��Ҷ�ӽڵ�����
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

/// ��ѡ����ļ�
void CFileView::OnFileOpen()
{
	// TODO:  �ڴ˴���������������
	OpenSelectFile();
}

/// ����ѡ��ڵ���ı�
void CFileView::OnEditCopy()
{
	// TODO:  �ڴ˴���������������
	HTREEITEM hTreeItem = m_wndFileView.GetSelectedItem();
	if (NULL == hTreeItem)
	{
		return;
	}
	mCopyItemString = m_wndFileView.GetItemText(hTreeItem);
	mCopyItemString = mCopyItemString.Right(mCopyItemString.GetLength() - mCopyItemString.Find(_T('(')) - 1);
	mCopyItemString = mCopyItemString.Left(mCopyItemString.Find(_T(')')));
}

/// ճ��ѡ����ļ�
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
	// TODO:  �ڴ˴���������������
	int nResult = AfxMessageBox(_T("Ϊ�˱�����ɾ��Ŀǰ��֧��ɾ��"), MB_OKCANCEL);
	//if (nResult == IDOK)
	//{
	//	
	//}
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

/// ��ѡ�е��ļ�
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

/// ��xml����õ��ļ������ͺ�����
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