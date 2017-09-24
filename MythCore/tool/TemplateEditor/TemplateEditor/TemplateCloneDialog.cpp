// TemplateCloneDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "TemplateEditor.h"
#include "TemplateCloneDialog.h"
#include "MainFrm.h"
#include "TemplateManager.h"
#include "CommonDefine.h"

// CTemplateCloneDialog 对话框

IMPLEMENT_DYNAMIC(CTemplateCloneDialog, CDialog)

CTemplateCloneDialog::CTemplateCloneDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTemplateCloneDialog::IDD, pParent)
	, m_strCloneName(_T(""))
	//, m_strCloneID(_T(""))
	, m_nCloneID(0)
{

}

CTemplateCloneDialog::~CTemplateCloneDialog()
{
}

void CTemplateCloneDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CLONENAME, m_strCloneName);
	//DDX_Text(pDX, IDC_CLONEID, m_strCloneID);
	DDX_Text(pDX, IDC_CLONEID, m_nCloneID);
}


BEGIN_MESSAGE_MAP(CTemplateCloneDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CTemplateCloneDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CTemplateCloneDialog 消息处理程序

void CTemplateCloneDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_nCloneID <= 0)
	{
		AfxMessageBox(_T("请填写正确的模板ID!"));
		return;
	}
	if (CTemplateManager::GetSingletonPtr()->IsExistTemplate(m_nCloneID))
	{
		AfxMessageBox(_T("已经有相同的模板ID存在，请重新填写模板ID"));
		return;
	}
	CMainFrame* pMainFrame = (CMainFrame*) ( (CMainFrame*) AfxGetMainWnd( ) );
	CTreeCtrl& CatalogTree = pMainFrame->GetCatalogTreeDialog().GetCatalogTree();

	HTREEITEM pSelectItem = CatalogTree.GetSelectedItem();
	if (NULL == pSelectItem)
	{
		return;
	}

	// 得到克隆的源数据
	CTreeItemData* pData = (CTreeItemData*)CatalogTree.GetItemData(pSelectItem);
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
	if(pFieldData->m_emTempLoadState != emLoadState_Inited)
	{
		AfxMessageBox(_T("拷贝的目标模板还没有初始化完成,请稍后再试!!!"));
		return;
	}


	CTemplateInfo* pTemplateInfo = pFieldData->GetParent();
	if (NULL == pTemplateInfo)
	{
		return;
	}

	// 模板表正在读取中,不能创建新模板
	if(pTemplateInfo->m_emTempLoadState != emLoadState_Inited)
	{
		AfxMessageBox(_T("模板表正在读取中,不能创建新模板!!!"));
		return;
	}

	HTREEITEM pParentItem = CatalogTree.GetParentItem(pSelectItem);
	if (NULL == pParentItem)
	{
		return;
	}

	// 创建目标数据
	CTemplateFieldData* pNewFieldData = new CTemplateFieldData(*pFieldData);
	if (NULL == pNewFieldData)
	{
		return;
	}
	
	// 模板名和模板ID不能克隆
	pNewFieldData->m_nTemplateID = m_nCloneID;
	pNewFieldData->m_emTempLoadState = emLoadState_Inited;
	pNewFieldData->m_strTemplateName = ws2s(m_strCloneName.GetBuffer());
	pTemplateInfo->m_bDirty = FALSE;
	pTemplateInfo->m_vecTemplateFieldData.push_back(pNewFieldData);
	CTemplateManager::GetSingletonPtr()->AddNewTemplateField(pNewFieldData);

	TCHAR acTreeItemName[MAX_PATH] = {0};
	_snwprintf(acTreeItemName, sizeof(acTreeItemName), _T("%s(%d)"), m_strCloneName.GetBuffer(), m_nCloneID);

	// 创建模板数据的树节点
	HTREEITEM pNewItem = CatalogTree.InsertItem(acTreeItemName, 1, 1, pParentItem);
	CTreeItemData* pNewData = new CTreeItemData;
	if (NULL == pNewData)
	{
		// Error 
		return;
	}
	pNewData->SetData(pNewFieldData);
	pNewData->SetItemType(TEMP_ITEM_DATA);
	CatalogTree.SetItemData( pNewItem, (DWORD_PTR)pNewData );
	CatalogTree.SetItemImage( pNewItem, 1, 1 );

	CTemplateManager::GetSingletonPtr()->WriteTemplateInfoToFile(pTemplateInfo);

	OnOK();
}
