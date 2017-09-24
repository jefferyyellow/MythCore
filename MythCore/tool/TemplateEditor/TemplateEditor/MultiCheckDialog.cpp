// MultiCheckDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "TemplateEditor.h"
#include "MultiCheckDialog.h"
#include "TemplateEditorView.h"
#include "MainFrm.h"
#include "CommonDefine.h"

// CMultiCheckDialog 对话框

IMPLEMENT_DYNAMIC(CMultiCheckDialog, CDialog)

CMultiCheckDialog::CMultiCheckDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMultiCheckDialog::IDD, pParent)
{

}

CMultiCheckDialog::~CMultiCheckDialog()
{
}

void CMultiCheckDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMultiCheckDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CMultiCheckDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CMultiCheckDialog 消息处理程序

BOOL CMultiCheckDialog::OnInitDialog()
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
	int nOldValue = atoi(pFieldData->m_vecValue[nSelectIndex].c_str());

	string strType = pOriginTemp->m_vecOriginalField[nSelectIndex].m_strType;

	size_t nIndex = strType.find("(");
	if (nIndex != string::npos)
	{
		strType = strType.substr(nIndex + 1);
	}

	size_t nValueIndex = 0;
	string strName;
	int nValue = 0;
	int nCheckCount = 0;
	while((nIndex = strType.find(",")) != string::npos && nCheckCount < 32)
	{
		strName = strType.substr(0, nIndex);
		strType = strType.substr(nIndex + 1);
		CButton *pCheckButton	= ( CButton * )GetDlgItem( IDC_CHECK1 + nCheckCount );
		if (NULL != pCheckButton)
		{
			pCheckButton->SetWindowText( s2ws(strName).c_str( ) );
		}
		nCheckCount ++;
	}


	nIndex = strType.find(")");
	if (nIndex != string::npos && nCheckCount <= 32)
	{
		strName = strType.substr(0, nIndex);
		CButton *pCheckButton	= ( CButton * )GetDlgItem( IDC_CHECK1 + nCheckCount );
		if (NULL != pCheckButton)
		{
			pCheckButton->SetWindowText( s2ws(strName).c_str( ) );
		}
		nCheckCount++;
	}

	for (int i = nCheckCount; i < 32; i++)
	{
		CButton *pCheckButton	= ( CButton * )GetDlgItem( IDC_CHECK1 + i );
		if (NULL != pCheckButton)
		{
			pCheckButton->ShowWindow(SW_HIDE);
		}
	}


	for (int i = 0; i < 32; i++)
	{
		BOOL bCheck = nOldValue & 0x01;
		if (bCheck)
		{
			CButton *pCheckButton	= ( CButton * )GetDlgItem( IDC_CHECK1 + i);
			if (NULL != pCheckButton)
			{
				pCheckButton->SetCheck(TRUE);
				pCheckButton->ShowWindow(SW_SHOW);
			}
		}
		nOldValue = nOldValue >> 1;
	}

	CStatic* pMultiDescribe = (CStatic*)GetDlgItem(IDC_MULTIDESCRIBE);
	if (NULL != pMultiDescribe)
	{
		pMultiDescribe->SetWindowText(CString(s2ws(pOriginTemp->m_vecOriginalField[nSelectIndex].m_strFieldName).c_str()) + CString(_T("：")));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMultiCheckDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	// TODO: 在此添加控件通知处理程序代码
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

	int nValue = 0;
	int nOffset = 1;
	CString strAppend = _T("(");
	CString strWindowsText;
	for (int i = 0; i < 32; i++)
	{
		CButton *pCheckButton	= ( CButton * )GetDlgItem( IDC_CHECK1 + i);
		if (NULL != pCheckButton && pCheckButton->IsWindowVisible() && pCheckButton->GetCheck())
		{
			nValue |= nOffset;

			TCHAR acBuffer[MAX_PATH] = {0};
			_itow(i + 1, acBuffer, 10);
			pCheckButton->GetWindowText(strWindowsText);
			strAppend += acBuffer + CString(_T(":")) + strWindowsText + CString(_T(","));
		}
		nOffset = nOffset << 1;
	}

	strAppend += _T(")");
	char acBuffer[MAX_PATH] = {0};
	_itoa(nValue, acBuffer, 10);
	pFieldData->m_vecValue[nFieldIndex] = acBuffer;

	pList->SetItemText(nFieldIndex + 1, 1, s2ws(acBuffer).c_str() + CString(_T(" ,")) + strAppend);
	pFieldData->GetParent()->m_bDirty = TRUE;
	OnOK();
}
