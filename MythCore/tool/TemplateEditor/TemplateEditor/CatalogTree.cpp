// CatalogTree.cpp : 实现文件
//

#include "stdafx.h"
#include "TemplateEditor.h"
#include "CatalogTree.h"
#include "CommonDefine.h"
#include "TemplateManager.h"
#include "MainFrm.h"
#include "TemplateEditorView.h"
#include "CreateTemlateDialog.h"
#include "CreateTableDialog.h"
#include "TemplateCloneDialog.h"
#include "ChangeTemplateNameDialog.h"
#include "ChangeTableInfo.h"

// CCatalogTree 对话框

IMPLEMENT_DYNAMIC(CCatalogTree, CDialogBar)

CCatalogTree* CCatalogTree::m_pInst = NULL;
CCatalogTree::CCatalogTree(CWnd* pParent /*=NULL*/)
{
	if (NULL != m_pInst)
	{
		return;
	}
	m_bReadTempDataing = FALSE;
	m_pInst = this;
}

CCatalogTree::~CCatalogTree()
{
}

void CCatalogTree::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CATALOGTREE, m_CatalogTree);
}


BEGIN_MESSAGE_MAP(CCatalogTree, CDialogBar)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog )
	ON_NOTIFY(NM_DBLCLK, IDC_CATALOGTREE, &CCatalogTree::OnNMDblclkCatalogtree)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_CATALOGTREE, &CCatalogTree::OnTvnItemexpandingCatalogtree)
	ON_NOTIFY(NM_RCLICK, IDC_CATALOGTREE, &CCatalogTree::OnNMRclickCatalogtree)
	ON_COMMAND(ID_CREATE_TEMPLATE, &CCatalogTree::OnCreateTemplate)
	ON_COMMAND(ID_CLONE_TEMPLATE, &CCatalogTree::OnCloneTemplate)
	ON_COMMAND(ID_CREATETEMPLATETABLE, &CCatalogTree::OnCreateTemplateTable)
	ON_COMMAND(ID_OPENWITHEXCEL, &CCatalogTree::OnOpenWithExcel)
	ON_COMMAND(ID_CHANGETEMPLATENAME, &CCatalogTree::OnChangeTemplateName)
	ON_COMMAND(ID_DELETETEMPLATE, &CCatalogTree::OnDeleteTemplate)
	ON_COMMAND(ID_CHANGETABLEINFO, &CCatalogTree::OnChangeTableInfo)
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CCatalogTree 消息处理程序

LONG CCatalogTree::OnInitDialog ( UINT wParam, LONG lParam)
{
	BOOL bRet = ( BOOL )HandleInitDialog( wParam, lParam );
	if (!UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init.\n");
	}
	// 创建目录树
	BuildCatalogTree(_T(".\\Template"), NULL);
	// 根据目录树建立模板数据
	CTemplateManager::GetSingletonPtr()->BuildTempFromTreeItem(NULL, _T(""), &m_CatalogTree, m_CatalogTree.GetRootItem());



	//设置树显示模式
	mTreeImages.Create( 16, 16, ILC_COLOR32, 2, 5 );
	// 加载树节点显示图片
	mDirBitmap.LoadBitmap( IDB_BITMAP_DIR );
	mFileBitmap.LoadBitmap( IDB_BITMAP_FILE );
	mDirTBitmap.LoadBitmap( IDB_BITMAP_DIRT );
	mFileCOBitmap.LoadBitmap( IDB_BITMAP_FILECO );
	mTreeImages.Add( &mDirBitmap, RGB( 0, 0, 0 ) );
	mTreeImages.Add( &mFileBitmap, RGB( 0, 0, 0 ) );
	mTreeImages.Add( &mDirTBitmap, RGB( 0, 0, 0 ) );
	mTreeImages.Add( &mFileCOBitmap, RGB( 0, 0, 0 ) );
	// 设置树的图片列表
	CTreeCtrl* pTree = (CTreeCtrl*) GetDlgItem( IDC_CATALOGTREE );
	pTree->SetImageList( &mTreeImages, LVSIL_NORMAL );


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// --------------------------------------------------------------------------------------------------------------------
// 根据文件搜索创建目录树
BOOL CCatalogTree::BuildCatalogTree(TCHAR* pServerPath, HTREEITEM ParentItem)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError;

	hFind = FindFirstFile(pServerPath, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return FALSE;
	} 
	else 
	{
		BuildTreeItem( pServerPath, FindFileData.cFileName, ParentItem, (FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY );
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			BuildTreeItem( pServerPath, FindFileData.cFileName, ParentItem, (FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY );
		}

		dwError = GetLastError();
		FindClose(hFind);
		// 如果错误不是已经没有其它文件了,就表示出现了其它的错误
		if (dwError != ERROR_NO_MORE_FILES) 
		{
			return FALSE;
		}

	}
	return TRUE;
}

// --------------------------------------------------------------------------------------------------------------------
// 创建目录树结点
BOOL CCatalogTree::BuildTreeItem(TCHAR* pServerPath, TCHAR* pFileName, HTREEITEM vParent, bool bIsDir )
{
	CString svrpath(pServerPath);
	// 如果是当前路径,过滤掉
	if (pFileName[0] == '.')
		return TRUE;
	svrpath.Replace( L"*", pFileName );

	if (bIsDir)
	{
		HTREEITEM tItem = m_CatalogTree.InsertItem( (LPCTSTR)pFileName, 0, 0, vParent );
		m_CatalogTree.SetItemData( tItem, TEMP_ITEM_DIR );
		svrpath += "\\*";
		int tBuild	= BuildCatalogTree( svrpath.GetBuffer(), tItem );

		m_CatalogTree.SetItemImage( tItem, 0, 0 );
	}
	else
	{
		HTREEITEM tItem = NULL;
		CString tChildName (pFileName);
		//如果是原始模版
		if ( tChildName.Find( L"#" ) >= 0 )
		{
			m_CatalogTree.SetItemData( vParent, TEMP_ITEM_ORIGIN );
		}
		// 如果是模板表数据
		else if ( tChildName.Right(3) == L"txt" )
		{
			tItem = m_CatalogTree.InsertItem( (LPCTSTR)tChildName.GetBuffer(), 1, 1, vParent );
			m_CatalogTree.SetItemData( tItem, TEMP_ITEM_TABLE );
			m_CatalogTree.SetItemImage( tItem, 1, 1 );
		}
	}
	return TRUE;
}

// --------------------------------------------------------------------------------------------------------------------
// 通过树数据结点得到对应的模板，原始模板文件的路径
BOOL CCatalogTree::GetTemplatePath(HTREEITEM pTreeItem, CString& strPath)
{
	CString strParent;
	HTREEITEM pParentItem = m_CatalogTree.GetParentItem(pTreeItem);
	while (pParentItem != NULL)
	{
		strParent = m_CatalogTree.GetItemText(pParentItem);
		strPath = _T("\\") + strPath;
		strPath = strParent + strPath;
		pParentItem = m_CatalogTree.GetParentItem(pParentItem);
	}
	return TRUE;
}

// --------------------------------------------------------------------------------------------------------------------
// 双击树节点
void CCatalogTree::OnNMDblclkCatalogtree(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM pItem		= m_CatalogTree.GetSelectedItem( );
	if ( pItem == NULL )
		return;

	// TODO: 在此添加控件通知处理程序代码

	CTreeItemData* pItemData = (CTreeItemData*)m_CatalogTree.GetItemData(pItem);
	if (NULL == pItemData)
	{
		return;
	}
	// 只有是模板数据才响应
	if (pItemData->GetItemType() == TEMP_ITEM_DATA)
	{
		CTemplateFieldData* pTemplateFieldData = (CTemplateFieldData*)(pItemData->GetData());
		if (NULL == pTemplateFieldData)
		{
			return;
		}
		CTemplateInfo* pTemplateInfo = (CTemplateInfo*)(pTemplateFieldData->GetParent());
		if (NULL == pTemplateInfo)
		{
			return;
		}

		// 当前的数据还没初始化完成
		if (pTemplateFieldData->m_emTempLoadState != emLoadState_Inited)
		{
			AfxMessageBox(_T("当前的数据还没初始化完成，请稍后再试！！！"));
			return;
		}

		CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
		if(NULL == pMainFrame)
		{
			return;
		}
		pMainFrame->UpdateDataToView(pTemplateFieldData);
	}

	//pEditorView->Update(templateinfo, 0);

	*pResult = 0;
}

// --------------------------------------------------------------------------------------------------------------------
// 展开树节点
void CCatalogTree::OnTvnItemexpandingCatalogtree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	HTREEITEM   pItem   =   (pNMTreeView-> itemNew).hItem;
	CTreeItemData* pItemData = (CTreeItemData*)m_CatalogTree.GetItemData(pItem);
	if (NULL == pItemData)
	{
		return;
	}

	// 如果树节点是模板表
	if (pItemData->GetItemType() == TEMP_ITEM_TABLE)
	{
		CTemplateInfo* pTemplateInfo = (CTemplateInfo*)pItemData->GetData();
		// 如果模板表已经初始化完成了就不用再从文件里读数据了
		if (NULL == pTemplateInfo || emLoadState_Inited == pTemplateInfo->m_emTempLoadState)
		{
			return;
		}

		// 如果正在刷模板就不能再读数据了，因为有可能那边也在读了
		if(CTemplateWrite::GetSingletonPtr()->GetWritingPB())
		{
			AfxMessageBox(_T("正在生成模板中，有可能导致数据读取冲突！！！"));
			*pResult = 1;
			return;
		}
		if (m_bReadTempDataing)
		{
			AfxMessageBox(_T("不要过于快速打开不同的表节点，请稍后再试！！！"));
			*pResult = 1;
			return;
		}

		m_bReadTempDataing = TRUE;
		// 创建后台线程读模板表数据
		::CreateThread( NULL, 0, CCatalogTree::ReadTemplateDataThreadFunc, pItem, 0, NULL );
	}
	*pResult = 0;
}

// --------------------------------------------------------------------------------------------------------------------
// 建立模板数据树节点
BOOL CCatalogTree::BuildTemplateItem(CTemplateInfo* pNewTemplate, HTREEITEM pParentItem)
{
	if (NULL == pNewTemplate)
	{
		return FALSE;
	}

	TCHAR acDest[MAX_PATH] = {0};

	string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
	setlocale(LC_ALL, "chs"); 


	vector<CTemplateFieldData*>& TemplateFieldData = pNewTemplate->m_vecTemplateFieldData;
	HTREEITEM pItem = NULL;
	// 遍历模板表里的所有模板数据
	for (size_t i = 0; i < TemplateFieldData.size() && NULL != TemplateFieldData[i]; i++)
	{

		// 没有对应过滤器的直接挂在表节点下
		mbstowcs(acDest,TemplateFieldData[i]->m_strTemplateName.c_str(),TemplateFieldData[i]->m_strTemplateName.size() + 1);
		_snwprintf(acDest, sizeof(acDest), _T("%s(%d)"), acDest, TemplateFieldData[i]->m_nTemplateID);
		pItem = m_CatalogTree.InsertItem(acDest, 1, 1, pParentItem);
		// 创建树节点数据
		CTreeItemData* pData = new CTreeItemData;
		if (pData == NULL)
		{
			// 报错吧
			return FALSE;
		}
		pData->SetData(TemplateFieldData[i]);
		pData->SetItemType(TEMP_ITEM_DATA);


		m_CatalogTree.SetItemData( pItem, (DWORD_PTR)(pData) );
		m_CatalogTree.SetItemImage( pItem, 1, 1 );

	}
	setlocale(LC_ALL, curLocale.c_str());

	return TRUE;
}

// --------------------------------------------------------------------------------------------------------------------
// 右键点击
void CCatalogTree::OnNMRclickCatalogtree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 如果正在刷模板，不让他们使用右键
	if(CTemplateWrite::GetSingletonPtr()->GetWritingPB())
	{
		AfxMessageBox(_T("正在生成模板中，不允许修改数据！"));
		return;
	}
	// TODO: 在此添加控件通知处理程序代码
	CMenu tMenu;
	CPoint tPoint;
	::GetCursorPos( &tPoint );

	m_CatalogTree.ScreenToClient( &tPoint );
	unsigned int nFlags = 0;  
	HTREEITEM pSelItem = m_CatalogTree.HitTest( tPoint, &nFlags ); //tpTree->GetSelectedItem( );
	CTreeItemData* pData = (CTreeItemData*)m_CatalogTree.GetItemData(pSelItem);
	if (NULL == pData)
	{
		return;
	}
	
	// 根据不同的树节点类型加载不同的右键菜单
	switch (pData->GetItemType())
	{
		// 过滤器
	case TEMP_ITEM_FILTER:
		{
			tMenu.LoadMenu(IDR_MENUFILTER);
			break;
		}
		// 原始模板
	case TEMP_ITEM_ORIGIN:
		{
			tMenu.LoadMenu(IDR_MENUORIGIN);
			break;
		}
		// 模板表
	case TEMP_ITEM_TABLE:
		{
			tMenu.LoadMenu(IDR_MENUTEMPTABLE);
			break;
		}
		// 模板数据
	case TEMP_ITEM_DATA:
		{
			tMenu.LoadMenu(IDR_MENUTEMPDATA);
			break;
		}
	default:
		{
			return;
		}
	}


	int tPosX = -1;
	int tPosY = -1;
	m_CatalogTree.GetItemImage( pSelItem, tPosX, tPosY );
	if (NULL != pSelItem)
	{
		::GetCursorPos( &tPoint );
		tMenu.GetSubMenu( 0 )->TrackPopupMenu( TPM_LEFTALIGN, tPoint.x, tPoint.y, this );
	}

	//设置右键选中结点
	if( nFlags & TVHT_ONITEM )
	{

		m_CatalogTree.SetFocus( );
		m_CatalogTree.SelectItem( pSelItem );
	}
	*pResult = 0;
}

// 创建模板
void CCatalogTree::OnCreateTemplate()
{
	if(CTemplateWrite::GetSingletonPtr()->GetWritingPB())
	{
		AfxMessageBox(_T("正在生成模板中，不允许修改数据！"));
		return;
	}
	// TODO: 在此添加命令处理程序代码
	CCreateTemlateDialog CreateTemlateDialog;
	CreateTemlateDialog.DoModal();
}

// 克隆模板
void CCatalogTree::OnCloneTemplate()
{
	if(CTemplateWrite::GetSingletonPtr()->GetWritingPB())
	{
		AfxMessageBox(_T("正在生成模板中，不允许修改数据！"));
		return;
	}
	// TODO: 在此添加命令处理程序代码
	CTemplateCloneDialog CloneDialog;
	CloneDialog.DoModal();
}

// 创建模板表
void CCatalogTree::OnCreateTemplateTable()
{
	if(CTemplateWrite::GetSingletonPtr()->GetWritingPB())
	{
		AfxMessageBox(_T("正在生成模板中，不允许修改数据！"));
		return;
	}
	CCreateTableDialog CreateTeamplateDialog;
	CreateTeamplateDialog.DoModal();
}

// 用Excel打开模板表
void CCatalogTree::OnOpenWithExcel()
{
	if(CTemplateWrite::GetSingletonPtr()->GetWritingPB())
	{
		AfxMessageBox(_T("正在生成模板中，不允许修改数据！"));
		return;
	}
	HTREEITEM pSelectItem = m_CatalogTree.GetSelectedItem();
	if (NULL == pSelectItem)
	{
		return;
	}

	HTREEITEM pParentItem = CCatalogTree::GetSingletonPtr()->m_CatalogTree.GetParentItem(pSelectItem);
	if(NULL == pParentItem)
	{
		return;
	}

	CTreeItemData* pData = (CTreeItemData*)m_CatalogTree.GetItemData(pSelectItem);
	if (NULL == pData || pData->GetItemType() != TEMP_ITEM_TABLE)
	{
		return;
	}
	CTemplateInfo* pTempInfo = (CTemplateInfo*)(pData->GetData());
	if (NULL == pTempInfo)
	{
		return;
	}

	// 得到模板表路径
	TCHAR acFilePath[MAX_PATH] = {0};
	// 如果文件名中有空格就得用引号引起来，所以干脆都加引号
	CString strFilePath = CString("\"") + pTempInfo->m_strPathFile + CString(_T("\""));
	wcsncpy(acFilePath, strFilePath, MAX_PATH);

	// 设置启动参数并启动
	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = GetSafeHwnd();
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = _T("excel.exe");		
	ShExecInfo.lpParameters = acFilePath;	
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL;	
	ShellExecuteEx(&ShExecInfo);
}

void CCatalogTree::OnChangeTemplateName()
{
	if(CTemplateWrite::GetSingletonPtr()->GetWritingPB())
	{
		AfxMessageBox(_T("正在生成模板中，不允许修改数据！"));
		return;
	}
	CChangeTemplateNameDialog dialog;
	dialog.DoModal();
}

void CCatalogTree::OnDeleteTemplate()
{
	if(CTemplateWrite::GetSingletonPtr()->GetWritingPB())
	{
		AfxMessageBox(_T("正在生成模板中，不允许修改数据！"));
		return;
	}

	if(IDOK != MessageBox(_T("是否真要删除该模板?"), _T("删除模板"), MB_OKCANCEL))
	{
		return;
	}

	HTREEITEM pSelItem	= m_CatalogTree.GetSelectedItem();
	if(NULL == pSelItem)
	{
		return;
	}

	CTreeItemData* pData = (CTreeItemData*)m_CatalogTree.GetItemData(pSelItem);
	if (NULL == pData)
	{
		return;
	}

	if(pData->GetItemType() != TEMP_ITEM_DATA)
	{
		return;
	}

	CTemplateFieldData* pFieldData = (CTemplateFieldData*)(pData->GetData());
	if (NULL == pFieldData)
	{
		return;
	}

	CTemplateInfo* pTemplateInfo = pFieldData->GetParent();
	if (NULL == pTemplateInfo)
	{
		return;
	}

	if(pTemplateInfo->m_emTempLoadState != emLoadState_Inited)
	{
		AfxMessageBox(_T("模板表数据尚未初始化完全,请稍后再试"));
		return;
	}

	// 删除数据
	pTemplateInfo->RemoveFieldData(pFieldData);
	// 删除树节点
	m_CatalogTree.DeleteItem(pSelItem);
	// 保存
	CTemplateManager::GetSingletonPtr()->WriteTemplateInfoToFile(pTemplateInfo);
}

void CCatalogTree::OnChangeTableInfo()
{
	if(CTemplateWrite::GetSingletonPtr()->GetWritingPB())
	{
		AfxMessageBox(_T("正在生成模板中，不允许修改数据！"));
		return;
	}
	CChangeTableInfo dialog;
	dialog.DoModal();
}

// 读模板数据的后台线程
DWORD WINAPI CCatalogTree::ReadTemplateDataThreadFunc(LPVOID param)
{
	HTREEITEM   pItem   =   HTREEITEM(param);

	CTreeItemData* pItemData = (CTreeItemData*)CCatalogTree::GetSingletonPtr()->m_CatalogTree.GetItemData(pItem);
	if (NULL == pItemData)
	{
		return -1;
	}
	
	// 如果数据不是是模板表直接返回
	if (pItemData->GetItemType() != TEMP_ITEM_TABLE)
	{
		return -1;
	}

	CString strFileName = CCatalogTree::GetSingletonPtr()->m_CatalogTree.GetItemText(pItem);
	CString strPath;
	CCatalogTree::GetSingletonPtr()->GetTemplatePath(pItem, strPath);


	// 读模板表并设置相关的状态
	CTemplateInfo* pTemplateInfo = (CTemplateInfo*)pItemData->GetData();
	TmTempLoadState emNewState = emLoadState_Initing;
	InterlockedExchange(((LONG*)&pTemplateInfo->m_emTempLoadState), emNewState);
	if (NULL != pTemplateInfo)
	{
		CTemplateManager::GetSingletonPtr()->ReadTemplate_CNext(pTemplateInfo, (strPath + strFileName).GetBuffer());
	}
	emNewState = emLoadState_Inited;
	InterlockedExchange(((LONG*)&pTemplateInfo->m_emTempLoadState), emNewState);

	CCatalogTree::GetSingletonPtr()->SetReadTempDataing(FALSE);
	return 0;
}

// 树节点排序回调函数
int CALLBACK CCatalogTree::MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CTreeItemData* pData1 = (CTreeItemData*)lParam1;
	CTreeItemData* pData2 = (CTreeItemData*)lParam2;
	if (NULL == pData1 || NULL == pData2)
	{
		return TRUE;
	}
	
	if (pData1->GetItemType() != pData2->GetItemType())
	{
		return pData1->GetItemType() > pData2->GetItemType();
	}

	//if (lParam1 > ITEM_MAX_NUM && lParam2 > ITEM_MAX_NUM)
	//{
	//	CTemplateFieldData* pData1 = (CTemplateFieldData*)lParam1;
	//	CTemplateFieldData* pData2 = (CTemplateFieldData*)lParam2;
	//	return (pData1->m_nTemplateID > pData2->m_nTemplateID);
	//}
	//else
	//{
	//	return lParam1 > lParam2;
	//}

	return TRUE;
}

// 树结点排序
void CCatalogTree::SortTreeItem(HTREEITEM pParent)
{
	TVSORTCB tvs;

	// Sort the tree control's items using my
	// callback procedure.
	tvs.hParent = pParent;
	tvs.lpfnCompare = MyCompareProc;
	tvs.lParam = (LPARAM) (&m_CatalogTree);
	m_CatalogTree.SortChildrenCB(&tvs);
}

// 判断两个树节点是否在同一个模板表下面
BOOL CCatalogTree::IsSameDataTable(HTREEITEM pSource, HTREEITEM pDest)
{
	if (NULL == pSource || NULL == pDest)
	{
		return FALSE;
	}

	// 上溯到模板表
	HTREEITEM pSourceTable = NULL;
	for (; NULL != pSource; pSource = m_CatalogTree.GetParentItem(pSource))
	{
		CTreeItemData* pData = (CTreeItemData*)m_CatalogTree.GetItemData(pSource);
		if (NULL == pData)
		{
			return FALSE;
		}
		if (pData->GetItemType() == TEMP_ITEM_TABLE)
		{
			pSourceTable = pSource;
			break;
		}
	}

	// 上溯到模板表
	HTREEITEM pDestTable = NULL;
	for (; NULL != pDest; pDest = m_CatalogTree.GetParentItem(pDest))
	{
		CTreeItemData* pData = (CTreeItemData*)m_CatalogTree.GetItemData(pDest);
		if (NULL == pData)
		{
			return FALSE;
		}
		if (pData->GetItemType() == TEMP_ITEM_TABLE)
		{
			pDestTable = pDest;
			break;
		}
	}
	
	// 两个模板表是否相同
	return pDestTable == pSourceTable;
}

// 递归释放树结点动态分配的内存
void CCatalogTree::FreeTree(HTREEITEM pParent)
{
	if (NULL == pParent)
	{
		return;
	}

	if (m_CatalogTree.ItemHasChildren(pParent))
	{
		for (HTREEITEM pChildItem = m_CatalogTree.GetChildItem(pParent); 
			NULL != pChildItem; 
			pChildItem = m_CatalogTree.GetNextSiblingItem(pChildItem))
		{
			FreeTree(pChildItem);
		}
	}
	CTreeItemData* pData = (CTreeItemData*)m_CatalogTree.GetItemData(pParent);
	if (NULL != pData )
	{
		delete pData;
		m_CatalogTree.SetItemData(pParent, NULL);
	}
}
void CCatalogTree::OnDestroy()
{
	CDialogBar::OnDestroy();
	FreeTree(m_CatalogTree.GetRootItem());
	// TODO: 在此处添加消息处理程序代码
}

// 保存过滤器文件
void CCatalogTree::SaveFilterFile(HTREEITEM pTreeItem)
{
	HTREEITEM pTableItem = NULL;
	// 先得到模板表节点
	for (; NULL != pTreeItem; pTreeItem = m_CatalogTree.GetParentItem(pTreeItem))
	{
		CTreeItemData* pData = (CTreeItemData*)m_CatalogTree.GetItemData(pTreeItem);
		if (NULL == pData)
		{
			return ;
		}
		if (pData->GetItemType() == TEMP_ITEM_TABLE)
		{
			pTableItem = pTreeItem;
			break;
		}
	}

	// 得到路径
	CString strPath;
	GetTemplatePath(pTableItem, strPath);
	CString strFileName = m_CatalogTree.GetItemText(pTableItem);
	strFileName = strFileName.Left(strFileName.GetLength() - 4) + CString("_Filter.xml");
	strPath += strFileName;

}
void CCatalogTree::OnSize(UINT nType, int cx, int cy)
{
	CDialogBar::OnSize(nType, cx, cy);
}

