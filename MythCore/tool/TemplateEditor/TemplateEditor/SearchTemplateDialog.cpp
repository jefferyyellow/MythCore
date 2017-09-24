// SearchTemplateDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TemplateEditor.h"
#include "SearchTemplateDialog.h"
#include "MainFrm.h"

// CSearchTemplateDialog �Ի���

IMPLEMENT_DYNAMIC(CSearchTemplateDialog, CDialog)

CSearchTemplateDialog::CSearchTemplateDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSearchTemplateDialog::IDD, pParent)
	, m_nSearchID(0)
{

}

CSearchTemplateDialog::~CSearchTemplateDialog()
{
}

void CSearchTemplateDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SEARCHID, m_nSearchID);
}


BEGIN_MESSAGE_MAP(CSearchTemplateDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CSearchTemplateDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CSearchTemplateDialog ��Ϣ�������

void CSearchTemplateDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	CTemplateFieldData* pFieldData = CTemplateManager::GetSingletonPtr()->GetTemplateInfoByID(m_nSearchID);
	if(NULL == pFieldData)
	{
		MessageBox(_T("û����Ӧ��ģ������!!!"), _T("ģ������"), MB_OK);
		return;
	}

	CTemplateInfo* pTemplateInfo = pFieldData->GetParent();
	if(NULL == pTemplateInfo)
	{
		MessageBox(_T("û����Ӧ��ģ������!!!"), _T("ģ������"), MB_OK);
		return;
	}

	if(pFieldData->m_emTempLoadState == emLoadState_Null)
	{
		if(pFieldData->GetParent()->m_emTempLoadState == emLoadState_Initing)
		{
			MessageBox(_T("�����첽��ʼ��,���Ժ�����!!!"), _T("ģ������"), MB_OK);
			return;
		}


		if (pTemplateInfo->m_emTempLoadState == emLoadState_InitHead)
		{
			if (CCatalogTree::GetSingletonPtr()->GetReadTempDataing())
			{
				AfxMessageBox(_T("���ڽ��к�̨�����ݶ�ȡ�����Ժ����ԣ�����"));
				return;
			}
			if(pTemplateInfo->m_emTempLoadState == emLoadState_Initing)
			{
				AfxMessageBox(_T("���ڽ��к�̨�����ݶ�ȡ�����Ժ����ԣ�����"));
				return;
			}

			TmTempLoadState emNewState = emLoadState_Initing;
			InterlockedExchange(((LONG*)&pTemplateInfo->m_emTempLoadState), emNewState);

			if(!CTemplateManager::GetSingletonPtr()->ReadTemplate_CNext(pTemplateInfo, pTemplateInfo->m_strPathFile.GetBuffer()))
			{
				MessageBox(_T("��ʼ��ʧ��,������!!!"), _T("ģ������"), MB_OK);
				return ;
			}
			emNewState = emLoadState_Inited;
			InterlockedExchange(((LONG*)&pTemplateInfo->m_emTempLoadState), emNewState);


		}
	}
	
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	if(NULL == pMainFrame)
	{
		return;
	}
	pMainFrame->UpdateDataToView(pFieldData);

	OnOK();
}
