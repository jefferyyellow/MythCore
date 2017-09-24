// StringDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "TemplateEditor.h"
#include "StringDialog.h"
#include "MainFrm.h"
#include "TemplateEditorView.h"
#include "CommonDefine.h"

// CStringDialog 对话框

IMPLEMENT_DYNAMIC(CStringDialog, CDialog)

CStringDialog::CStringDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CStringDialog::IDD, pParent)
	, m_strString(_T(""))
{

}

CStringDialog::~CStringDialog()
{
}

void CStringDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STRINGEDIT, m_strString);
}


BEGIN_MESSAGE_MAP(CStringDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CStringDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CStringDialog 消息处理程序

BOOL CStringDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CTemplateEditorView* pView = (CTemplateEditorView*) ( (CMainFrame*) AfxGetMainWnd( ) )->MDIGetActive( )->GetActiveView( );
	if (NULL == pView)
	{
		return FALSE;
	}

	CTemplateFieldData* pFieldData = pView->GetCurrTempData();
	if (NULL == pFieldData)
	{
		return FALSE;
	}

	CListCtrl* pList		= (CListCtrl*)pView;
	COriginalTemplate* pOriginTemp = CTemplateManager::GetSingletonPtr()->GetOriginalTemplate(pFieldData->GetParent());
	if (NULL == pOriginTemp)
	{
		return FALSE;
	}

	int nSelectIndex	= pList->GetSelectionMark() - 1;
	if (nSelectIndex >= (int)pOriginTemp->m_vecOriginalField.size())
	{
		return FALSE;
	}
	
	string strType = pOriginTemp->m_vecOriginalField[nSelectIndex].m_strFieldType;
	size_t nIndex = string::npos;
	string strNum;
	int nValue = 0;
	if ((nIndex = strType.find("STRING")) != string::npos)
	{
		strNum = strType.substr(nIndex + 6);
		nValue = atoi(strNum.c_str());
		CStatic* pLengthDescribe = (CStatic*)GetDlgItem(IDC_LENGTHDESCRIBE);
		if (NULL != pLengthDescribe)
		{
			pLengthDescribe->SetWindowText(CString(_T("请输入长度不大于")) + s2ws(strNum).c_str() + CString(_T("的字符串！")));
		}
	}

	CStatic* pLengthDescribe = (CStatic*)GetDlgItem(IDC_STRINGFIELDDESCRIBE);
	if (NULL != pLengthDescribe)
	{
		pLengthDescribe->SetWindowText(CString(s2ws(pOriginTemp->m_vecOriginalField[nSelectIndex].m_strFieldName).c_str()) + CString(_T(":")));
	}

	CEdit* pStringEdit = (CEdit*)GetDlgItem(IDC_STRINGEDIT);
	if(NULL != pStringEdit)
	{
		pStringEdit->SetWindowText(CString(s2ws(pFieldData->m_vecValue[nSelectIndex]).c_str()));
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CStringDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CTemplateEditorView* pView = (CTemplateEditorView*) ( (CMainFrame*) AfxGetMainWnd( ) )->MDIGetActive( )->GetActiveView( );
	if (NULL == pView)
	{
		return ;
	}

	CTemplateFieldData* pFieldData = pView->GetCurrTempData();
	if (NULL == pFieldData)
	{
		return ;
	}

	CListCtrl* pList		= (CListCtrl*)pView;
	int nFieldIndex	= pList->GetSelectionMark() - 1;
	if (nFieldIndex >= (int)pFieldData->m_vecValue.size())
	{
		return;
	}
	pFieldData->m_vecValue[nFieldIndex] = ws2s(m_strString.GetBuffer()).c_str();
	pList->SetItemText(nFieldIndex + 1, 1, m_strString);

	pFieldData->GetParent()->m_bDirty = TRUE;
	OnOK();
}
