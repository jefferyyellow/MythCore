// ChangeTableInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "TemplateEditor.h"
#include "ChangeTableInfo.h"
#include "MainFrm.h"


// CChangeTableInfo 对话框

IMPLEMENT_DYNAMIC(CChangeTableInfo, CDialog)

CChangeTableInfo::CChangeTableInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CChangeTableInfo::IDD, pParent)
	, m_strTableName(_T(""))
{

}

CChangeTableInfo::~CChangeTableInfo()
{
}

void CChangeTableInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CHANGETABLENAME, m_strTableName);
}


BEGIN_MESSAGE_MAP(CChangeTableInfo, CDialog)
	ON_BN_CLICKED(IDOK, &CChangeTableInfo::OnBnClickedOk)
END_MESSAGE_MAP()


// CChangeTableInfo 消息处理程序

void CChangeTableInfo::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();

	if(m_strTableName == "")
	{
		AfxMessageBox(_T("请填写模板表名称！"));
		return;
	}

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

	if(pData->GetItemType() != TEMP_ITEM_TABLE)
	{
		return;
	}

	CTemplateInfo* pTemplateInfo = (CTemplateInfo*)(pData->GetData());
	if (NULL == pTemplateInfo)
	{
		return;
	}

	CString strOldConfigPath = pTemplateInfo->m_strPathFile.Left(pTemplateInfo->m_strPathFile.ReverseFind('.')) + _T("_Config.xml");
	CString strNewPath = pTemplateInfo->m_strPathFile.Left(pTemplateInfo->m_strPathFile.ReverseFind('\\') + 1);
	CString strNewFilePath = strNewPath + m_strTableName + _T(".txt");
	CString strConfigFilePath = strNewPath + m_strTableName + _T("_Config.xml");
	
	if(0 == MoveFile(pTemplateInfo->m_strPathFile, strNewFilePath))
	{
		AfxMessageBox(_T("修改文件名出错!"));
		return;
	}
	if(0 == MoveFile(strOldConfigPath, strConfigFilePath))
	{
		AfxMessageBox(_T("修改文件名出错!"));
		return;
	}
	pTemplateInfo->m_strPathFile = strNewFilePath;

	CatalogTree.SetItemText(pSelItem, m_strTableName + _T(".txt"));
}

BOOL CChangeTableInfo::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
