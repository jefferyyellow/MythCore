#pragma once
/********************************************************************
	created:	21/12/2011   16:31
	filename: 	CreateTemlateDialog.h
	author:		jeffery

	purpose:	����ģ��ĵ����Ի�����
*********************************************************************/

// CCreateTemlateDialog �Ի���

class CCreateTemlateDialog : public CDialog
{
	DECLARE_DYNAMIC(CCreateTemlateDialog)

public:
	CCreateTemlateDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCreateTemlateDialog();

// �Ի�������
	enum { IDD = IDD_CREATETEMPLATEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ģ������
	CString m_strTemplateName;
	// ģ��ID
	int m_nTemplateID;
	afx_msg void OnBnClickedCreatetemplate();
};
