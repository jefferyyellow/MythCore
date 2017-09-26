#pragma once


// CAddServerInfoDlg 对话框

class CAddServerInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddServerInfoDlg)

public:
	CAddServerInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddServerInfoDlg();

// 对话框数据
	enum { IDD = IDD_ADD_SERVER_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	bool mAdd;		// 如果为真就是增加服务器信息，如果为假就是修改服务器信息
public:
	virtual BOOL OnInitDialog();
};
