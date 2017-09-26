// CatalogTree.cpp : ʵ���ļ�
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

// CCatalogTree �Ի���

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


// CCatalogTree ��Ϣ�������

LONG CCatalogTree::OnInitDialog ( UINT wParam, LONG lParam)
{
	BOOL bRet = ( BOOL )HandleInitDialog( wParam, lParam );
	if (!UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init.\n");
	}
	// ����Ŀ¼��
	BuildCatalogTree(_T(".\\Template"), NULL);
	// ����Ŀ¼������ģ������
	CTemplateManager::GetSingletonPtr()->BuildTempFromTreeItem(NULL, _T(""), &m_CatalogTree, m_CatalogTree.GetRootItem());



	//��������ʾģʽ
	mTreeImages.Create( 16, 16, ILC_COLOR32, 2, 5 );
	// �������ڵ���ʾͼƬ
	mDirBitmap.LoadBitmap( IDB_BITMAP_DIR );
	mFileBitmap.LoadBitmap( IDB_BITMAP_FILE );
	mDirTBitmap.LoadBitmap( IDB_BITMAP_DIRT );
	mFileCOBitmap.LoadBitmap( IDB_BITMAP_FILECO );
	mTreeImages.Add( &mDirBitmap, RGB( 0, 0, 0 ) );
	mTreeImages.Add( &mFileBitmap, RGB( 0, 0, 0 ) );
	mTreeImages.Add( &mDirTBitmap, RGB( 0, 0, 0 ) );
	mTreeImages.Add( &mFileCOBitmap, RGB( 0, 0, 0 ) );
	// ��������ͼƬ�б�
	CTreeCtrl* pTree = (CTreeCtrl*) GetDlgItem( IDC_CATALOGTREE );
	pTree->SetImageList( &mTreeImages, LVSIL_NORMAL );


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

// --------------------------------------------------------------------------------------------------------------------
// �����ļ���������Ŀ¼��
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
		// ����������Ѿ�û�������ļ���,�ͱ�ʾ�����������Ĵ���
		if (dwError != ERROR_NO_MORE_FILES) 
		{
			return FALSE;
		}

	}
	return TRUE;
}

// --------------------------------------------------------------------------------------------------------------------
// ����Ŀ¼�����
BOOL CCatalogTree::BuildTreeItem(TCHAR* pServerPath, TCHAR* pFileName, HTREEITEM vParent, bool bIsDir )
{
	CString svrpath(pServerPath);
	// ����ǵ�ǰ·��,���˵�
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
		//�����ԭʼģ��
		if ( tChildName.Find( L"#" ) >= 0 )
		{
			m_CatalogTree.SetItemData( vParent, TEMP_ITEM_ORIGIN );
		}
		// �����ģ�������
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
// ͨ�������ݽ��õ���Ӧ��ģ�壬ԭʼģ���ļ���·��
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
// ˫�����ڵ�
void CCatalogTree::OnNMDblclkCatalogtree(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM pItem		= m_CatalogTree.GetSelectedItem( );
	if ( pItem == NULL )
		return;

	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CTreeItemData* pItemData = (CTreeItemData*)m_CatalogTree.GetItemData(pItem);
	if (NULL == pItemData)
	{
		return;
	}
	// ֻ����ģ�����ݲ���Ӧ
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

		// ��ǰ�����ݻ�û��ʼ�����
		if (pTemplateFieldData->m_emTempLoadState != emLoadState_Inited)
		{
			AfxMessageBox(_T("��ǰ�����ݻ�û��ʼ����ɣ����Ժ����ԣ�����"));
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
// չ�����ڵ�
void CCatalogTree::OnTvnItemexpandingCatalogtree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	HTREEITEM   pItem   =   (pNMTreeView-> itemNew).hItem;
	CTreeItemData* pItemData = (CTreeItemData*)m_CatalogTree.GetItemData(pItem);
	if (NULL == pItemData)
	{
		return;
	}

	// ������ڵ���ģ���
	if (pItemData->GetItemType() == TEMP_ITEM_TABLE)
	{
		CTemplateInfo* pTemplateInfo = (CTemplateInfo*)pItemData->GetData();
		// ���ģ����Ѿ���ʼ������˾Ͳ����ٴ��ļ����������
		if (NULL == pTemplateInfo || emLoadState_Inited == pTemplateInfo->m_emTempLoadState)
		{
			return;
		}

		// �������ˢģ��Ͳ����ٶ������ˣ���Ϊ�п����Ǳ�Ҳ�ڶ���
		if(CTemplateWrite::GetSingletonPtr()->GetWritingPB())
		{
			AfxMessageBox(_T("��������ģ���У��п��ܵ������ݶ�ȡ��ͻ������"));
			*pResult = 1;
			return;
		}
		if (m_bReadTempDataing)
		{
			AfxMessageBox(_T("��Ҫ���ڿ��ٴ򿪲�ͬ�ı�ڵ㣬���Ժ����ԣ�����"));
			*pResult = 1;
			return;
		}

		m_bReadTempDataing = TRUE;
		// ������̨�̶߳�ģ�������
		::CreateThread( NULL, 0, CCatalogTree::ReadTemplateDataThreadFunc, pItem, 0, NULL );
	}
	*pResult = 0;
}

// --------------------------------------------------------------------------------------------------------------------
// ����ģ���������ڵ�
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
	// ����ģ����������ģ������
	for (size_t i = 0; i < TemplateFieldData.size() && NULL != TemplateFieldData[i]; i++)
	{

		// û�ж�Ӧ��������ֱ�ӹ��ڱ�ڵ���
		mbstowcs(acDest,TemplateFieldData[i]->m_strTemplateName.c_str(),TemplateFieldData[i]->m_strTemplateName.size() + 1);
		_snwprintf(acDest, sizeof(acDest), _T("%s(%d)"), acDest, TemplateFieldData[i]->m_nTemplateID);
		pItem = m_CatalogTree.InsertItem(acDest, 1, 1, pParentItem);
		// �������ڵ�����
		CTreeItemData* pData = new CTreeItemData;
		if (pData == NULL)
		{
			// �����
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
// �Ҽ����
void CCatalogTree::OnNMRclickCatalogtree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// �������ˢģ�壬��������ʹ���Ҽ�
	if(CTemplateWrite::GetSingletonPtr()->GetWritingPB())
	{
		AfxMessageBox(_T("��������ģ���У��������޸����ݣ�"));
		return;
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	
	// ���ݲ�ͬ�����ڵ����ͼ��ز�ͬ���Ҽ��˵�
	switch (pData->GetItemType())
	{
		// ������
	case TEMP_ITEM_FILTER:
		{
			tMenu.LoadMenu(IDR_MENUFILTER);
			break;
		}
		// ԭʼģ��
	case TEMP_ITEM_ORIGIN:
		{
			tMenu.LoadMenu(IDR_MENUORIGIN);
			break;
		}
		// ģ���
	case TEMP_ITEM_TABLE:
		{
			tMenu.LoadMenu(IDR_MENUTEMPTABLE);
			break;
		}
		// ģ������
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

	//�����Ҽ�ѡ�н��
	if( nFlags & TVHT_ONITEM )
	{

		m_CatalogTree.SetFocus( );
		m_CatalogTree.SelectItem( pSelItem );
	}
	*pResult = 0;
}

// ����ģ��
void CCatalogTree::OnCreateTemplate()
{
	if(CTemplateWrite::GetSingletonPtr()->GetWritingPB())
	{
		AfxMessageBox(_T("��������ģ���У��������޸����ݣ�"));
		return;
	}
	// TODO: �ڴ���������������
	CCreateTemlateDialog CreateTemlateDialog;
	CreateTemlateDialog.DoModal();
}

// ��¡ģ��
void CCatalogTree::OnCloneTemplate()
{
	if(CTemplateWrite::GetSingletonPtr()->GetWritingPB())
	{
		AfxMessageBox(_T("��������ģ���У��������޸����ݣ�"));
		return;
	}
	// TODO: �ڴ���������������
	CTemplateCloneDialog CloneDialog;
	CloneDialog.DoModal();
}

// ����ģ���
void CCatalogTree::OnCreateTemplateTable()
{
	if(CTemplateWrite::GetSingletonPtr()->GetWritingPB())
	{
		AfxMessageBox(_T("��������ģ���У��������޸����ݣ�"));
		return;
	}
	CCreateTableDialog CreateTeamplateDialog;
	CreateTeamplateDialog.DoModal();
}

// ��Excel��ģ���
void CCatalogTree::OnOpenWithExcel()
{
	if(CTemplateWrite::GetSingletonPtr()->GetWritingPB())
	{
		AfxMessageBox(_T("��������ģ���У��������޸����ݣ�"));
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

	// �õ�ģ���·��
	TCHAR acFilePath[MAX_PATH] = {0};
	// ����ļ������пո�͵������������������Ըɴ඼������
	CString strFilePath = CString("\"") + pTempInfo->m_strPathFile + CString(_T("\""));
	wcsncpy(acFilePath, strFilePath, MAX_PATH);

	// ������������������
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
		AfxMessageBox(_T("��������ģ���У��������޸����ݣ�"));
		return;
	}
	CChangeTemplateNameDialog dialog;
	dialog.DoModal();
}

void CCatalogTree::OnDeleteTemplate()
{
	if(CTemplateWrite::GetSingletonPtr()->GetWritingPB())
	{
		AfxMessageBox(_T("��������ģ���У��������޸����ݣ�"));
		return;
	}

	if(IDOK != MessageBox(_T("�Ƿ���Ҫɾ����ģ��?"), _T("ɾ��ģ��"), MB_OKCANCEL))
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
		AfxMessageBox(_T("ģ���������δ��ʼ����ȫ,���Ժ�����"));
		return;
	}

	// ɾ������
	pTemplateInfo->RemoveFieldData(pFieldData);
	// ɾ�����ڵ�
	m_CatalogTree.DeleteItem(pSelItem);
	// ����
	CTemplateManager::GetSingletonPtr()->WriteTemplateInfoToFile(pTemplateInfo);
}

void CCatalogTree::OnChangeTableInfo()
{
	if(CTemplateWrite::GetSingletonPtr()->GetWritingPB())
	{
		AfxMessageBox(_T("��������ģ���У��������޸����ݣ�"));
		return;
	}
	CChangeTableInfo dialog;
	dialog.DoModal();
}

// ��ģ�����ݵĺ�̨�߳�
DWORD WINAPI CCatalogTree::ReadTemplateDataThreadFunc(LPVOID param)
{
	HTREEITEM   pItem   =   HTREEITEM(param);

	CTreeItemData* pItemData = (CTreeItemData*)CCatalogTree::GetSingletonPtr()->m_CatalogTree.GetItemData(pItem);
	if (NULL == pItemData)
	{
		return -1;
	}
	
	// ������ݲ�����ģ���ֱ�ӷ���
	if (pItemData->GetItemType() != TEMP_ITEM_TABLE)
	{
		return -1;
	}

	CString strFileName = CCatalogTree::GetSingletonPtr()->m_CatalogTree.GetItemText(pItem);
	CString strPath;
	CCatalogTree::GetSingletonPtr()->GetTemplatePath(pItem, strPath);


	// ��ģ���������ص�״̬
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

// ���ڵ�����ص�����
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

// ���������
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

// �ж��������ڵ��Ƿ���ͬһ��ģ�������
BOOL CCatalogTree::IsSameDataTable(HTREEITEM pSource, HTREEITEM pDest)
{
	if (NULL == pSource || NULL == pDest)
	{
		return FALSE;
	}

	// ���ݵ�ģ���
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

	// ���ݵ�ģ���
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
	
	// ����ģ����Ƿ���ͬ
	return pDestTable == pSourceTable;
}

// �ݹ��ͷ�����㶯̬������ڴ�
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
	// TODO: �ڴ˴������Ϣ����������
}

// ����������ļ�
void CCatalogTree::SaveFilterFile(HTREEITEM pTreeItem)
{
	HTREEITEM pTableItem = NULL;
	// �ȵõ�ģ���ڵ�
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

	// �õ�·��
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

