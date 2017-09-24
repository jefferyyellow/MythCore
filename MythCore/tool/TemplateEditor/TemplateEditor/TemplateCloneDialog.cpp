// TemplateCloneDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TemplateEditor.h"
#include "TemplateCloneDialog.h"
#include "MainFrm.h"
#include "TemplateManager.h"
#include "CommonDefine.h"

// CTemplateCloneDialog �Ի���

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


// CTemplateCloneDialog ��Ϣ�������

void CTemplateCloneDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_nCloneID <= 0)
	{
		AfxMessageBox(_T("����д��ȷ��ģ��ID!"));
		return;
	}
	if (CTemplateManager::GetSingletonPtr()->IsExistTemplate(m_nCloneID))
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

	// �õ���¡��Դ����
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
		AfxMessageBox(_T("������Ŀ��ģ�廹û�г�ʼ�����,���Ժ�����!!!"));
		return;
	}


	CTemplateInfo* pTemplateInfo = pFieldData->GetParent();
	if (NULL == pTemplateInfo)
	{
		return;
	}

	// ģ������ڶ�ȡ��,���ܴ�����ģ��
	if(pTemplateInfo->m_emTempLoadState != emLoadState_Inited)
	{
		AfxMessageBox(_T("ģ������ڶ�ȡ��,���ܴ�����ģ��!!!"));
		return;
	}

	HTREEITEM pParentItem = CatalogTree.GetParentItem(pSelectItem);
	if (NULL == pParentItem)
	{
		return;
	}

	// ����Ŀ������
	CTemplateFieldData* pNewFieldData = new CTemplateFieldData(*pFieldData);
	if (NULL == pNewFieldData)
	{
		return;
	}
	
	// ģ������ģ��ID���ܿ�¡
	pNewFieldData->m_nTemplateID = m_nCloneID;
	pNewFieldData->m_emTempLoadState = emLoadState_Inited;
	pNewFieldData->m_strTemplateName = ws2s(m_strCloneName.GetBuffer());
	pTemplateInfo->m_bDirty = FALSE;
	pTemplateInfo->m_vecTemplateFieldData.push_back(pNewFieldData);
	CTemplateManager::GetSingletonPtr()->AddNewTemplateField(pNewFieldData);

	TCHAR acTreeItemName[MAX_PATH] = {0};
	_snwprintf(acTreeItemName, sizeof(acTreeItemName), _T("%s(%d)"), m_strCloneName.GetBuffer(), m_nCloneID);

	// ����ģ�����ݵ����ڵ�
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
