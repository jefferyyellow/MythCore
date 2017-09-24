// ChangeTemplateNameDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "TemplateEditor.h"
#include "ChangeTemplateNameDialog.h"
#include "MainFrm.h"


// CChangeTemplateNameDialog 对话框

IMPLEMENT_DYNAMIC(CChangeTemplateNameDialog, CDialog)

CChangeTemplateNameDialog::CChangeTemplateNameDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CChangeTemplateNameDialog::IDD, pParent)
	, m_strTemplateName(_T(""))
{

}

CChangeTemplateNameDialog::~CChangeTemplateNameDialog()
{
}

void CChangeTemplateNameDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TEMPLATERENAME, m_strTemplateName);
}


BEGIN_MESSAGE_MAP(CChangeTemplateNameDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CChangeTemplateNameDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CChangeTemplateNameDialog 消息处理程序

void CChangeTemplateNameDialog::OnBnClickedOk()
{
	UpdateData(TRUE);
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainFrame = (CMainFrame*) ( (CMainFrame*) AfxGetMainWnd( ) );
	CTreeCtrl& CatalogTree = pMainFrame->GetCatalogTreeDialog().GetCatalogTree();

	HTREEITEM pSelItem	= CatalogTree.GetSelectedItem();
	if(NULL == pSelItem)
	{
		return;
	}

	CTreeItemData* pData = (CTreeItemData*)CatalogTree.GetItemData(pSelItem);
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

	TCHAR acTreeItemName[MAX_PATH] = {0};
	_snwprintf(acTreeItemName, sizeof(acTreeItemName), _T("%s(%d)"), m_strTemplateName.GetBuffer(), pFieldData->m_nTemplateID);

	pFieldData->m_strTemplateName = ws2s(m_strTemplateName.GetBuffer());
	CatalogTree.SetItemText(pSelItem, acTreeItemName);

	if(pTemplateInfo->m_emTempLoadState != emLoadState_Inited)
	{
		AfxMessageBox(_T("模板表数据尚未初始化完全,请稍后再试"));
		return;
	}
	CTemplateManager::GetSingletonPtr()->WriteTemplateInfoToFile(pTemplateInfo);
	OnOK();
}
