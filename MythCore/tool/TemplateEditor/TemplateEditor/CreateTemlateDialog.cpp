// CreateTemlateDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "TemplateEditor.h"
#include "CreateTemlateDialog.h"
#include "MainFrm.h"
#include "CommonDefine.h"


// CCreateTemlateDialog 对话框

IMPLEMENT_DYNAMIC(CCreateTemlateDialog, CDialog)

CCreateTemlateDialog::CCreateTemlateDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateTemlateDialog::IDD, pParent)
	, m_strTemplateName(_T(""))
	, m_nTemplateID(0)
{

}

CCreateTemlateDialog::~CCreateTemlateDialog()
{
}

void CCreateTemlateDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TEAMLATENAME, m_strTemplateName);
	DDX_Text(pDX, IDC_TEMPLATEID, m_nTemplateID);
	DDV_MinMaxInt(pDX, m_nTemplateID, 0, 65535);
}


BEGIN_MESSAGE_MAP(CCreateTemlateDialog, CDialog)
	ON_BN_CLICKED(ID_CREATETEMPLATE, &CCreateTemlateDialog::OnBnClickedCreatetemplate)
END_MESSAGE_MAP()


// CCreateTemlateDialog 消息处理程序

void CCreateTemlateDialog::OnBnClickedCreatetemplate()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_nTemplateID <= 0)
	{
		AfxMessageBox(_T("请填写正确的模板ID!"));
		return;
	}


	if (CTemplateManager::GetSingletonPtr()->IsExistTemplate(m_nTemplateID))
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

	CTreeItemData* pData = (CTreeItemData*)CatalogTree.GetItemData(pSelectItem);
	if( pData->GetItemType() != TEMP_ITEM_TABLE)
	{
		return;
	}


	CTemplateInfo* pTemplateInfo = (CTemplateInfo*)pData->GetData();
	if (NULL == pTemplateInfo)
	{
		return;
	}

	// 模板表正在读取中,不能创建新模板
	if(pTemplateInfo->m_emTempLoadState != emLoadState_Inited)
	{
		AfxMessageBox(_T("模板表尚未初始化,不能创建新模板"));
		return;
	}

	// 分配模板内存
	CTemplateFieldData* pNewFieldData = new CTemplateFieldData;
	if (NULL == pNewFieldData)
	{
		return;
	}


	//pTemplateInfo->m_vecTemplateFieldData.resize(CTemplateManager::GetSingletonPtr()->GetTemplateInfoFieldNum(pTemplateInfo));
	// 设置模板字段数目
	pNewFieldData->m_vecValue.resize(CTemplateManager::GetSingletonPtr()->GetTemplateInfoFieldNum(pTemplateInfo));
	pNewFieldData->m_nTemplateID = m_nTemplateID;
	pNewFieldData->m_emTempLoadState = emLoadState_Inited;
	pNewFieldData->m_strTemplateName = ws2s(m_strTemplateName.GetBuffer());
	pNewFieldData->m_pParentInfo = pTemplateInfo;
	pTemplateInfo->m_vecTemplateFieldData.push_back(pNewFieldData);
	pTemplateInfo->m_bDirty = TRUE;
	// 加载表中
	CTemplateManager::GetSingletonPtr()->AddNewTemplateField(pNewFieldData);


	TCHAR acTreeItemName[MAX_PATH] = {0};
	_snwprintf(acTreeItemName, sizeof(acTreeItemName), _T("%s(%d)"), m_strTemplateName.GetBuffer(), m_nTemplateID);

	// 创建树节点
	HTREEITEM pNewItem = CatalogTree.InsertItem(acTreeItemName, 1, 1, pSelectItem);
	CTreeItemData* pNewData = new CTreeItemData;
	if (NULL == pNewData)
	{
		return;
	}
	pNewData->SetItemType(TEMP_ITEM_DATA);
	pNewData->SetData(pNewFieldData);

	CatalogTree.SetItemData( pNewItem, DWORD_PTR(pNewData));
	CatalogTree.SetItemImage( pNewItem, 1, 1 );
	CatalogTree.Expand(pSelectItem, TVE_EXPAND);
	OnOK();
	UpdateData(TRUE);
}
