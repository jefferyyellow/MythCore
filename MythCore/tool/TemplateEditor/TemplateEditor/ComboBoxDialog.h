#pragma once
#include "afxwin.h"
/********************************************************************
	created:	21/12/2011   16:34
	filename: 	ComboBoxDialog.h
	author:		jeffery

	purpose:	��ѡһ���ݱ༭�Ի���
*********************************************************************/

// CComboBoxDialog �Ի���

class CComboBoxDialog : public CDialog
{
	DECLARE_DYNAMIC(CComboBoxDialog)

public:
	CComboBoxDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CComboBoxDialog();

// �Ի�������
	enum { IDD = IDD_COMBOBOXDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_SelectComboBox;
	CString m_SelectDescribe;

	vector<DWORD>	m_vecComboBoxData;
	afx_msg void OnBnClickedOk();
};
