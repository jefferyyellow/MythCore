#pragma once


// CAddServerInfoDlg �Ի���

class CAddServerInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddServerInfoDlg)

public:
	CAddServerInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddServerInfoDlg();

// �Ի�������
	enum { IDD = IDD_ADD_SERVER_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

public:
	bool GetAdd(){return mAdd;}
	void SetAdd(int nAdd){mAdd = nAdd;}
	void SetServerID(CString strServerID);
	void SetServerIP(CString strServerIP);
	void SetServerLocalIP(CString strServerLocalIP);
	void SetUserName(CString strUserName);
	void SetPassWord(CString strPassWord);

private:
	bool mAdd;		// ���Ϊ��������ӷ�������Ϣ�����Ϊ�پ����޸ķ�������Ϣ
public:
	virtual BOOL OnInitDialog();
};
