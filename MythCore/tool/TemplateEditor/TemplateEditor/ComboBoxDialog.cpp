// ComboBoxDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "TemplateEditor.h"
#include "ComboBoxDialog.h"
#include "TemplateEditorView.h"
#include "MainFrm.h"
#include "CommonDefine.h"


// CComboBoxDialog 对话框

IMPLEMENT_DYNAMIC(CComboBoxDialog, CDialog)

CComboBoxDialog::CComboBoxDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CComboBoxDialog::IDD, pParent)
	, m_SelectDescribe(_T(""))
{

}

CComboBoxDialog::~CComboBoxDialog()
{
}

void CComboBoxDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_SelectComboBox);
	DDX_Text(pDX, IDC_SELECTDESCRIBE, m_SelectDescribe);
}


BEGIN_MESSAGE_MAP(CComboBoxDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CComboBoxDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CComboBoxDialog 消息处理程序

BOOL CComboBoxDialog::OnInitDialog()
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
	
	m_SelectDescribe = s2ws(pOriginTemp->m_vecOriginalField[nSelectIndex].m_strFieldName + ":").c_str();
	string strType = pOriginTemp->m_vecOriginalField[nSelectIndex].m_strType;
	
	int nOldValue = -1;
	string strOldValue = pFieldData->m_vecValue[nSelectIndex];
	strOldValue = GetFirstWord(strOldValue);
	if(strOldValue != "")
	{
		nOldValue = atoi(strOldValue.c_str());
	}
	 
	size_t nIndex = strType.find("(");
	if (nIndex != string::npos)
	{
		strType = strType.substr(nIndex + 1);
	}

	size_t nValueIndex = 0;
	string strName;
	int nValue = 0;
	m_vecComboBoxData.clear();
	while((nIndex = strType.find(",")) != string::npos)
	{
		strName = strType.substr(0, nIndex);
		nValueIndex = strName.find("=");
		if (nValueIndex != string::npos)
		{
			nValue = atoi(strName.substr(nValueIndex + 1).c_str());
			
			strName = strName.substr(0, nValueIndex);
		}
		else
		{
			if (((int)m_vecComboBoxData.size()) - 1 >= 0)
			{
				nValue = m_vecComboBoxData[m_vecComboBoxData.size() - 1] + 1;
			}
			else
			{
				nValue = 1;
			}
		}

		m_vecComboBoxData.push_back(nValue);
		m_SelectComboBox.AddString(s2ws(strName).c_str());
		if (nValue == nOldValue)
		{
			m_SelectComboBox.SetCurSel(m_SelectComboBox.GetCount() - 1);
		}
		strType = strType.substr(nIndex + 1);
	}


	nIndex = strType.find(")");
	if (nIndex != string::npos)
	{
		strName = strType.substr(0, nIndex);
		nValueIndex = strName.find("=");
		if (nValueIndex != string::npos)
		{
			nValue = atoi(strName.substr(nValueIndex + 1).c_str());
			m_vecComboBoxData.push_back(nValue);
			strName = strName.substr(0, nValueIndex);
		}
		else
		{
			if (((int)m_vecComboBoxData.size()) - 1 >= 0)
			{
				m_vecComboBoxData.push_back(m_vecComboBoxData[m_vecComboBoxData.size() - 1] + 1);
			}
			else
			{
				m_vecComboBoxData.push_back(1);
			}
		}
		m_SelectComboBox.AddString(s2ws(strName).c_str());
		if (nValue == nOldValue)
		{
			m_SelectComboBox.SetCurSel(m_SelectComboBox.GetCount() - 1);
		}
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CComboBoxDialog::OnBnClickedOk()
{
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

	int nSelectIndex = m_SelectComboBox.GetCurSel();
	if (nSelectIndex == -1)
	{
		return;
	}
	
	
	int nValue = m_vecComboBoxData[nSelectIndex];
	char acBuffer[MAX_PATH] = {0};
	_itoa(nValue, acBuffer, 10);

	pFieldData->m_vecValue[nFieldIndex] = acBuffer;
	CString strSelectText;
	m_SelectComboBox.GetWindowText(strSelectText);
	pList->SetItemText(nFieldIndex + 1, 1, s2ws(acBuffer).c_str() + CString(_T(" ,")) + strSelectText);

	pFieldData->GetParent()->m_bDirty = TRUE;
	OnOK();
}
