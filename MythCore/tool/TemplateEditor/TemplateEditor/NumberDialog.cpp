// NumberDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "TemplateEditor.h"
#include "NumberDialog.h"
#include "TemplateEditorView.h"
#include "MainFrm.h"
#include "CommonDefine.h"

// CNumberDialog 对话框

IMPLEMENT_DYNAMIC(CNumberDialog, CDialog)

CNumberDialog::CNumberDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNumberDialog::IDD, pParent)
	, m_strMinMaxDescribe(_T(""))
	, m_strIntValue(_T(""))
{

}

CNumberDialog::~CNumberDialog()
{
}

void CNumberDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NUMBERDESCRIBE, m_strMinMaxDescribe);
	DDX_Text(pDX, IDC_NUMBER, m_strIntValue);
}


BEGIN_MESSAGE_MAP(CNumberDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CNumberDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CNumberDialog 消息处理程序

BOOL CNumberDialog::OnInitDialog()
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

	string strType = pOriginTemp->m_vecOriginalField[nSelectIndex].m_strType;
	size_t nStartIndex = strType.find("(");
	size_t nEndIndex = strType.find(")");
	strType = strType.substr(nStartIndex + 1, nEndIndex - nStartIndex - 1);
	size_t nInterval = strType.find(",");
	string strMin = strType.substr(0, nInterval);
	string strMax = strType.substr(nInterval + 1);
	
	//m_strMinMaxDescribe = ;
	CStatic* pStatic = ( CStatic *)GetDlgItem( IDC_NUMBERDESCRIBE );
	if (NULL != pStatic)
	{
		pStatic->SetWindowText(CString(_T("请输入")) + CString(s2ws(strMin).c_str()) + CString(_T("-")) + CString(s2ws(strMax).c_str()) + CString(_T("之间的整数")));
	}
	CEdit * pEdit	= ( CEdit *)GetDlgItem( IDC_NUMBER );
	if (NULL != pEdit)
	{
		pEdit->SetWindowText(s2ws(pFieldData->m_vecValue[nSelectIndex]).c_str());
		pEdit->SetFocus( );
		pEdit->SetSel( 0, -1 );
	}

	//UpdateData(FALSE);
	return FALSE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CNumberDialog::OnBnClickedOk()
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

	pFieldData->m_vecValue[nFieldIndex] = ws2s(m_strIntValue.GetBuffer()).c_str();
	pList->SetItemText(nFieldIndex + 1, 1, m_strIntValue);

	pFieldData->GetParent()->m_bDirty = TRUE;
	OnOK();
}
