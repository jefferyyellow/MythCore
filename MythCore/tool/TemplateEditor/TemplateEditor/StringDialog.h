#pragma once
/********************************************************************
	created:	21/12/2011   16:36
	filename: 	StringDialog.h
	author:		jeffery

	purpose:	�ַ����ݱ༭�Ի���
*********************************************************************/

// CStringDialog �Ի���

class CStringDialog : public CDialog
{
	DECLARE_DYNAMIC(CStringDialog)

public:
	CStringDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStringDialog();

// �Ի�������
	enum { IDD = IDD_STRINGDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CString m_strString;
};
