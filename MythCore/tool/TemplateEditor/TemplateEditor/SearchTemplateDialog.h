#pragma once


// CSearchTemplateDialog �Ի���

class CSearchTemplateDialog : public CDialog
{
	DECLARE_DYNAMIC(CSearchTemplateDialog)

public:
	CSearchTemplateDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSearchTemplateDialog();

// �Ի�������
	enum { IDD = IDD_SEARCHTEMPLATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	int m_nSearchID;
};
