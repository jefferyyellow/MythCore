// CreateFilter.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TemplateEditor.h"
#include "CreateFilter.h"
#include "TemplateEditorView.h"
#include "MainFrm.h"
#include "CommonDefine.h"


// CCreateFilter �Ի���

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


// CCreateFilter ��Ϣ�������

void CCreateFilter::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	
	// �������ģ�����߹������´�����������ֱ�ӷ���
	if(pData->GetItemType() != TEMP_ITEM_TABLE 
		&& pData->GetItemType() != TEMP_ITEM_FILTER)
	{
		return;
	}
	// Ϊ����������һ�����ڵ�
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

	// չ�����ڵ�
	CatalogTree.Expand(pSelectItem, TVE_EXPAND);
	// �����ڵ��������
	pMainFrame->GetCatalogTreeDialog().SortTreeItem(pSelectItem);
	// ����������ļ�
	pMainFrame->GetCatalogTreeDialog().SaveFilterFile(pNewItem);
	OnOK();
}

