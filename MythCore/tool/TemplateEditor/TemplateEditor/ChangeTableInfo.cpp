// ChangeTableInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TemplateEditor.h"
#include "ChangeTableInfo.h"
#include "MainFrm.h"


// CChangeTableInfo �Ի���

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


// CChangeTableInfo ��Ϣ�������

void CChangeTableInfo::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();

	if(m_strTableName == "")
	{
		AfxMessageBox(_T("����дģ������ƣ�"));
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
		AfxMessageBox(_T("�޸��ļ�������!"));
		return;
	}
	if(0 == MoveFile(strOldConfigPath, strConfigFilePath))
	{
		AfxMessageBox(_T("�޸��ļ�������!"));
		return;
	}
	pTemplateInfo->m_strPathFile = strNewFilePath;

	CatalogTree.SetItemText(pSelItem, m_strTableName + _T(".txt"));
}

BOOL CChangeTableInfo::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
