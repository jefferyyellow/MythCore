#pragma once
/********************************************************************
	created:	21/12/2011   16:35
	filename: 	NumberDialog.h
	author:		jeffery

	purpose:	�������ݱ༭�Ի���
*********************************************************************/

// CNumberDialog �Ի���

class CNumberDialog : public CDialog
{
	DECLARE_DYNAMIC(CNumberDialog)

public:
	CNumberDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNumberDialog();

// �Ի�������
	enum { IDD = IDD_NUMBERDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strMinMaxDescribe;
	CString m_strIntValue;
	afx_msg void OnBnClickedOk();
};
