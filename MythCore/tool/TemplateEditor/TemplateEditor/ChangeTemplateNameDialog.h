#pragma once


// CChangeTemplateNameDialog �Ի���

class CChangeTemplateNameDialog : public CDialog
{
	DECLARE_DYNAMIC(CChangeTemplateNameDialog)

public:
	CChangeTemplateNameDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChangeTemplateNameDialog();

// �Ի�������
	enum { IDD = IDD_CHANGETEMPLATENAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_strTemplateName;
};
