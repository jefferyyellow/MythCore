#pragma once
#include "afxwin.h"


// CChangeTableInfo �Ի���

class CChangeTableInfo : public CDialog
{
	DECLARE_DYNAMIC(CChangeTableInfo)

public:
	CChangeTableInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChangeTableInfo();

// �Ի�������
	enum { IDD = IDD_CHANGETABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ģ�������
	CString m_strTableName;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
