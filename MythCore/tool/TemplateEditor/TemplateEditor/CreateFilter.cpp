// CreateFilter.cpp : 实现文件
//

#include "stdafx.h"
#include "TemplateEditor.h"
#include "CreateFilter.h"
#include "TemplateEditorView.h"
#include "MainFrm.h"
#include "CommonDefine.h"


// CCreateFilter 对话框

IMPLEMENT_DYNAMIC(CCreateFilter, CDialog)

CCreateFilter::CCreateFilter(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateFilter::IDD, pParent)
	, m_strFilterName(_T(""))
{

}

CCreateFilter::~CCreateFilter()
{
}

void CCreateFilter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILTERNAME, m_strFilterName);
}


BEGIN_MESSAGE_MAP(CCreateFilter, CDialog)
	ON_BN_CLICKED(IDOK, &CCreateFilter::OnBnClickedOk)
END_MESSAGE_MAP()


// CCreateFilter 消息处理程序

void CCreateFilter::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CMainFrame* pMainFrame = (CMainFrame*) ( (CMainFrame*) AfxGetMainWnd( ) );
	CTreeCtrl& CatalogTree = pMainFrame->GetCatalogTreeDialog().GetCatalogTree();

	HTREEITEM pSelectItem = CatalogTree.GetSelectedItem();
	if (NULL == pSelectItem)
	{
		return;
	}

	CTreeItemData* pData = (CTreeItemData*)CatalogTree.GetItemData(pSelectItem);
	if (NULL == pData)
	{
		return;
	}
	
	// 如果不是模板表或者过滤器下创建过滤器，直接返回
	if(pData->GetItemType() != TEMP_ITEM_TABLE 
		&& pData->GetItemType() != TEMP_ITEM_FILTER)
	{
		return;
	}
	// 为过滤器创建一个树节点
	HTREEITEM pNewItem = CatalogTree.InsertItem(m_strFilterName, 1, 1, pSelectItem, TVI_SORT);
	if (NULL == pNewItem)
	{
		return;
	}

	CTreeItemData* pNewData = new CTreeItemData;
	if (NULL == pNewData)
	{
		return;
	}

	pNewData->SetData(NULL);
	pNewData->SetItemType(TEMP_ITEM_FILTER);
	CatalogTree.SetItemData( pNewItem, (DWORD_PTR)pNewData);
	CatalogTree.SetItemImage( pNewItem, 0, 0);

	// 展开树节点
	CatalogTree.Expand(pSelectItem, TVE_EXPAND);
	// 对树节点进入排序
	pMainFrame->GetCatalogTreeDialog().SortTreeItem(pSelectItem);
	// 保存过滤器文件
	pMainFrame->GetCatalogTreeDialog().SaveFilterFile(pNewItem);
	OnOK();
}

