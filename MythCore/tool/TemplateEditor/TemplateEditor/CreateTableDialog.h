#pragma once
#include "afxwin.h"
/********************************************************************
	created:	21/12/2011   18:28
	filename: 	CreateTableDialog.h
	author:		jeffery

	purpose:	����ģ���Ի���
*********************************************************************/

// CCreateTableDialog �Ի���

class CCreateTableDialog : public CDialog
{
	DECLARE_DYNAMIC(CCreateTableDialog)

public:
	CCreateTableDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCreateTableDialog();

// �Ի�������
	enum { IDD = IDD_CREATETABLEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ģ��������
	CString m_strTableName;
	afx_msg void OnBnClickedCreatetable();
	virtual BOOL OnInitDialog();
};
