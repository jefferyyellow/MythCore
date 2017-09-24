// CreateTemlateDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TemplateEditor.h"
#include "CreateTemlateDialog.h"
#include "MainFrm.h"
#include "CommonDefine.h"


// CCreateTemlateDialog �Ի���

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


// CCreateTemlateDialog ��Ϣ�������

void CCreateTemlateDialog::OnBnClickedCreatetemplate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_nTemplateID <= 0)
	{
		AfxMessageBox(_T("����д��ȷ��ģ��ID!"));
		return;
	}


	if (CTemplateManager::GetSingletonPtr()->IsExistTemplate(m_nTemplateID))
	{
		AfxMessageBox(_T("�Ѿ�����ͬ��ģ��ID���ڣ���������дģ��ID"));
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

	// ģ������ڶ�ȡ��,���ܴ�����ģ��
	if(pTemplateInfo->m_emTempLoadState != emLoadState_Inited)
	{
		AfxMessageBox(_T("ģ�����δ��ʼ��,���ܴ�����ģ��"));
		return;
	}

	// ����ģ���ڴ�
	CTemplateFieldData* pNewFieldData = new CTemplateFieldData;
	if (NULL == pNewFieldData)
	{
		return;
	}


	//pTemplateInfo->m_vecTemplateFieldData.resize(CTemplateManager::GetSingletonPtr()->GetTemplateInfoFieldNum(pTemplateInfo));
	// ����ģ���ֶ���Ŀ
	pNewFieldData->m_vecValue.resize(CTemplateManager::GetSingletonPtr()->GetTemplateInfoFieldNum(pTemplateInfo));
	pNewFieldData->m_nTemplateID = m_nTemplateID;
	pNewFieldData->m_emTempLoadState = emLoadState_Inited;
	pNewFieldData->m_strTemplateName = ws2s(m_strTemplateName.GetBuffer());
	pNewFieldData->m_pParentInfo = pTemplateInfo;
	pTemplateInfo->m_vecTemplateFieldData.push_back(pNewFieldData);
	pTemplateInfo->m_bDirty = TRUE;
	// ���ر���
	CTemplateManager::GetSingletonPtr()->AddNewTemplateField(pNewFieldData);


	TCHAR acTreeItemName[MAX_PATH] = {0};
	_snwprintf(acTreeItemName, sizeof(acTreeItemName), _T("%s(%d)"), m_strTemplateName.GetBuffer(), m_nTemplateID);

	// �������ڵ�
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
