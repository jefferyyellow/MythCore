// TempPathDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TemplateEditor.h"
#include "TempPathDialog.h"
#include "CatalogTree.h"
#include "MainFrm.h"
#include "TemplateEditorView.h"

// CTempPathDialog dialog

IMPLEMENT_DYNAMIC(CTempPathDialog, CDialog)

CTempPathDialog::CTempPathDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTempPathDialog::IDD, pParent)
{

}

CTempPathDialog::~CTempPathDialog()
{
}

void CTempPathDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEMPPATHTREE, m_TempPathTree);
}


BEGIN_MESSAGE_MAP(CTempPathDialog, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CTempPathDialog::OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDOK, &CTempPathDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CTempPathDialog message handlers

void CTempPathDialog::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

BOOL CTempPathDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

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
	CTreeCtrl* pTree = (CTreeCtrl*) GetDlgItem( IDC_TEMPPATHTREE );
	pTree->SetImageList( &mTreeImages, LVSIL_NORMAL );

	CTreeCtrl& rSrcTree = CCatalogTree::GetSingletonPtr()->GetCatalogTree();
	CopyTree(rSrcTree, m_TempPathTree);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CTempPathDialog::CopyTree(CTreeCtrl& rSrcTree, CTreeCtrl& rDstTree)
{
	HTREEITEM pSrcRoot = rSrcTree.GetRootItem();
	if (NULL == pSrcRoot)
	{
		return;
	}

	CopyTreeItem(rSrcTree, pSrcRoot, rDstTree, NULL);
}

void CTempPathDialog::CopyTreeItem(CTreeCtrl& rSrcTree, HTREEITEM pSrcItem, CTreeCtrl& rDstTree, HTREEITEM pDstParentItem)
{
	int nImage = 0;
	int nSelectImage = 0;
	rSrcTree.GetItemImage(pSrcItem, nImage, nSelectImage);
	HTREEITEM pNewItem = rDstTree.InsertItem(rSrcTree.GetItemText(pSrcItem), nImage, nSelectImage, pDstParentItem);
	rDstTree.SetItemData(pNewItem, rSrcTree.GetItemData(pSrcItem));

	HTREEITEM pChildItem = rSrcTree.GetChildItem(pSrcItem);
	for (; NULL != pChildItem; pChildItem = rSrcTree.GetNextSiblingItem(pChildItem))
	{
		CopyTreeItem(rSrcTree, pChildItem, rDstTree, pNewItem);
	}
}
void CTempPathDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	HTREEITEM pSelectItem = m_TempPathTree.GetSelectedItem();
	if (NULL == pSelectItem)
	{
		return;
	}

	CTreeItemData* pData = (CTreeItemData*)m_TempPathTree.GetItemData(pSelectItem);
	if(NULL == pData)
	{
		return;
	}
	if (pData->GetItemType() != TEMP_ITEM_DATA)
	{
		AfxMessageBox(_T("请选择模板数据！"));
		return;
	}

	CTemplateFieldData* pTemplateFieldData = (CTemplateFieldData*)pData->GetData();
	if (NULL == pTemplateFieldData)
	{
		return;
	}

	CTemplateEditorView* pView = (CTemplateEditorView*) ( (CMainFrame*) AfxGetMainWnd( ) )->MDIGetActive( )->GetActiveView( );
	if (NULL == pView)
	{
		return ;
	}

	CTemplateFieldData* pFieldData = pView->GetCurrTempData();
	if (NULL == pFieldData)
	{
		return ;
	}

	CListCtrl* pList		= (CListCtrl*)pView;
	int nFieldIndex	= pList->GetSelectionMark() - 1;
	if (nFieldIndex >= (int)pFieldData->m_vecValue.size())
	{
		return;
	}


	char acBuffer[MAX_PATH] = {0};
	_snprintf(acBuffer, MAX_PATH, "%d", pTemplateFieldData->m_nTemplateID);

	pFieldData->m_vecValue[nFieldIndex] = acBuffer;
	pList->SetItemText(nFieldIndex + 1, 1, s2ws(acBuffer).c_str());

	pFieldData->GetParent()->m_bDirty = TRUE;

	OnOK();
}
