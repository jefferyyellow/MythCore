// SearchTemplateDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "TemplateEditor.h"
#include "SearchTemplateDialog.h"
#include "MainFrm.h"

// CSearchTemplateDialog 对话框

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


// CSearchTemplateDialog 消息处理程序

void CSearchTemplateDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	CTemplateFieldData* pFieldData = CTemplateManager::GetSingletonPtr()->GetTemplateInfoByID(m_nSearchID);
	if(NULL == pFieldData)
	{
		MessageBox(_T("没有相应的模板数据!!!"), _T("模板搜索"), MB_OK);
		return;
	}

	CTemplateInfo* pTemplateInfo = pFieldData->GetParent();
	if(NULL == pTemplateInfo)
	{
		MessageBox(_T("没有相应的模板数据!!!"), _T("模板搜索"), MB_OK);
		return;
	}

	if(pFieldData->m_emTempLoadState == emLoadState_Null)
	{
		if(pFieldData->GetParent()->m_emTempLoadState == emLoadState_Initing)
		{
			MessageBox(_T("正在异步初始化,请稍后再试!!!"), _T("模板搜索"), MB_OK);
			return;
		}


		if (pTemplateInfo->m_emTempLoadState == emLoadState_InitHead)
		{
			if (CCatalogTree::GetSingletonPtr()->GetReadTempDataing())
			{
				AfxMessageBox(_T("正在进行后台的数据读取，请稍后再试！！！"));
				return;
			}
			if(pTemplateInfo->m_emTempLoadState == emLoadState_Initing)
			{
				AfxMessageBox(_T("正在进行后台的数据读取，请稍后再试！！！"));
				return;
			}

			TmTempLoadState emNewState = emLoadState_Initing;
			InterlockedExchange(((LONG*)&pTemplateInfo->m_emTempLoadState), emNewState);

			if(!CTemplateManager::GetSingletonPtr()->ReadTemplate_CNext(pTemplateInfo, pTemplateInfo->m_strPathFile.GetBuffer()))
			{
				MessageBox(_T("初始化失败,请重试!!!"), _T("模板搜索"), MB_OK);
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
