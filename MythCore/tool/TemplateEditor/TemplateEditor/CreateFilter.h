#pragma once
/********************************************************************
	created:	21/12/2011   18:24
	filename: 	CreateFilter.h
	author:		jeffery

	purpose:	�����������Ի�����
*********************************************************************/

// CCreateFilter �Ի���

class CCreateFilter : public CDialog
{
	DECLARE_DYNAMIC(CCreateFilter)

public:
	CCreateFilter(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCreateFilter();

// �Ի�������
	enum { IDD = IDD_CREATEFILTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_strFilterName;

};
