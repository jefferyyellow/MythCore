#pragma once
/********************************************************************
	created:	16/12/2011   12:43
	filename: 	TemplateCloneDialog.h
	author:		jeffery

	purpose:	��¡ģ�����ݵĶԻ���
*********************************************************************/

// CTemplateCloneDialog �Ի���

class CTemplateCloneDialog : public CDialog
{
	DECLARE_DYNAMIC(CTemplateCloneDialog)

public:
	CTemplateCloneDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTemplateCloneDialog();

// �Ի�������
	enum { IDD = IDD_TEMPLATECLONEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ��¡��ģ�������
	CString m_strCloneName;
	// ��¡���ģ��ID
	//CString m_strCloneID;
	afx_msg void OnBnClickedOk();
	// ��¡���ģ��
	int m_nCloneID;
};
