#pragma once
/********************************************************************
	created:	21/12/2011   16:35
	filename: 	MultiCheckDialog.h
	author:		jeffery

	purpose:	��ѡ�����ݱ༭�Ի���
*********************************************************************/

// CMultiCheckDialog �Ի���

class CMultiCheckDialog : public CDialog
{
	DECLARE_DYNAMIC(CMultiCheckDialog)

public:
	CMultiCheckDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMultiCheckDialog();

// �Ի�������
	enum { IDD = IDD_MULTICHECKDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
