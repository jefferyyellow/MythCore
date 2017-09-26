// AddServerInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SSHServerManager.h"
#include "AddServerInfoDlg.h"
#include "SSHServerManagerDlg.h"


// CAddServerInfoDlg 对话框

IMPLEMENT_DYNAMIC(CAddServerInfoDlg, CDialog)

CAddServerInfoDlg::CAddServerInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddServerInfoDlg::IDD, pParent)
{

}

CAddServerInfoDlg::~CAddServerInfoDlg()
{
}

void CAddServerInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAddServerInfoDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAddServerInfoDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddServerInfoDlg 消息处理程序

void CAddServerInfoDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	CEdit* pServerID = (CEdit*)GetDlgItem(IDC_ADD_SERVER_ID);
	if (NULL == pServerID)
	{
		return;
	}

	CIPAddressCtrl* pIPAddress = (CIPAddressCtrl* )GetDlgItem(IDC_ADD_SERVER_IP);
	if (NULL == pIPAddress)
	{
		return;
	}

	CEdit* pUserNameEdit = (CEdit*)GetDlgItem(IDC_ADD_SERVER_USER_NAME);
	if (NULL == pUserNameEdit)
	{
		return;
	}

	CEdit* pPassWord = (CEdit*)GetDlgItem(IDC_ADD_SERVER_PASSWORD);
	if (NULL == pPassWord)
	{
		return;
	}

	CIPAddressCtrl* pLocalIPAddress = (CIPAddressCtrl*)GetDlgItem(IDC_ADD_SERVER_LOCAL_IP);
	if (NULL == pLocalIPAddress)
	{
		return;
	}

	CString wsStrServerID;
	pServerID->GetWindowText(wsStrServerID);
	if (wsStrServerID == "")
	{
		::AfxMessageBox(_T("请填写服务器ID"));
		return;
	}

	DWORD dwIPAddress = 0;
	pIPAddress->GetAddress(dwIPAddress);
	if (0 == dwIPAddress)
	{
		::AfxMessageBox(_T("请填写外网IP地址!"));
		return;
	}

	wchar_t acIPAddress[MAX_PATH] = {0};
	_snwprintf(acIPAddress, sizeof(acIPAddress) - 1, _T("%d.%d.%d.%d"), (0xFF000000&dwIPAddress)>>24,(0xFF0000&dwIPAddress)>>16,(0xFF00&dwIPAddress)>>8,0xFF&dwIPAddress);

	
	DWORD dwLocalIPAddress = 0;
	pLocalIPAddress->GetAddress(dwLocalIPAddress);
	if (0 == dwLocalIPAddress)
	{
		::AfxMessageBox(_T("请填写内网IP地址!"));
		return;
	}
	wchar_t acLocalIPAddress[MAX_PATH] = {0};
	_snwprintf(acLocalIPAddress, sizeof(acLocalIPAddress) - 1, _T("%d.%d.%d.%d"), (0xFF000000&dwLocalIPAddress)>>24,(0xFF0000&dwLocalIPAddress)>>16,(0xFF00&dwLocalIPAddress)>>8,0xFF&dwLocalIPAddress);


	CString wstrUserName;
	pUserNameEdit->GetWindowText(wstrUserName);
	if (wstrUserName == "")
	{
		::AfxMessageBox(_T("请填写用户名!"));
		return;
	}

	CString wsPassWord;
	pPassWord->GetWindowText(wsPassWord);
	if (wsPassWord == "")
	{
		::AfxMessageBox(_T("请填写密码!"));
		return;
	}
	
	CSSHServerManagerDlg* dlg = (CSSHServerManagerDlg*)GetParent();
	if (mAdd)
	{
		dlg->AddServerInfo(wsStrServerID, acIPAddress, acLocalIPAddress, wstrUserName, wsPassWord);
	}
	else
	{
		dlg->SetServerInfo(wsStrServerID, acIPAddress, acLocalIPAddress, wstrUserName, wsPassWord);
	}
	
	OnOK();
}

void CAddServerInfoDlg::SetServerID(CString strServerID)
{
	CEdit* pIDEdit = (CEdit*)GetDlgItem(IDC_ADD_SERVER_ID);
	if (NULL != pIDEdit)
	{
		pIDEdit->SetWindowText(strServerID);
	}
}

void CAddServerInfoDlg::SetServerIP(CString strServerIP)
{
	CIPAddressCtrl* pIPCtrl = (CIPAddressCtrl*)GetDlgItem(IDC_ADD_SERVER_IP);
	if (NULL != pIPCtrl)
	{
		char acBuffer[MAX_PATH] = {0};
		strncpy(acBuffer, ws2s(strServerIP.GetBuffer()).c_str(), sizeof(acBuffer) - 1);
		int nByte1 = 0, nByte2 = 0, nByte3 = 0, nByte4 = 0;
		sscanf(acBuffer, "%d.%d.%d.%d", &nByte1, &nByte2, &nByte3, &nByte4);
		pIPCtrl->SetAddress(nByte1, nByte2, nByte3, nByte4);
	}
}

void CAddServerInfoDlg::SetServerLocalIP(CString strServerLocalIP)
{
	CIPAddressCtrl* pIPCtrl = (CIPAddressCtrl*)GetDlgItem(IDC_ADD_SERVER_LOCAL_IP);
	if(NULL != pIPCtrl)
	{
		char acBuffer[MAX_PATH] = {0};
		strncpy(acBuffer, ws2s(strServerLocalIP.GetBuffer()).c_str(), sizeof(acBuffer) - 1);
		int nByte1 = 0, nByte2 = 0, nByte3 = 0, nByte4 = 0;
		sscanf(acBuffer, "%d.%d.%d.%d", &nByte1, &nByte2, &nByte3, &nByte4);
		pIPCtrl->SetAddress(nByte1, nByte2, nByte3, nByte4);
	}
}

void CAddServerInfoDlg::SetUserName(CString strUserName)
{
	CEdit* pIDEdit = (CEdit*)GetDlgItem(IDC_ADD_SERVER_USER_NAME);
	if (NULL != pIDEdit)
	{
		pIDEdit->SetWindowText(strUserName);
	}
}

void CAddServerInfoDlg::SetPassWord(CString strPassWord)
{
	CEdit* pIDEdit = (CEdit*)GetDlgItem(IDC_ADD_SERVER_PASSWORD);
	if (NULL != pIDEdit)
	{
		pIDEdit->SetWindowText(strPassWord);
	}
}
BOOL CAddServerInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (!mAdd)
	{
		// TODO:  在此添加额外的初始化
		CSSHServerManagerDlg *dlg = (CSSHServerManagerDlg*)AfxGetApp()->m_pMainWnd;
		if (NULL == dlg)
		{
			return TRUE;
		}
		CSSHServerManagerDlg::VEC_SERVER_INFO rServerListInfo = dlg->GetServerListInfo();
		CListCtrl& rServerListCtrl = dlg->GetServerListCtrl();
		int nItemIndex = rServerListCtrl.GetSelectionMark();
		if (nItemIndex >= 0 && nItemIndex < rServerListInfo.size())
		{
			SetServerID(rServerListInfo[nItemIndex].GetServerID());
			SetServerIP(rServerListInfo[nItemIndex].GetInternetIP());
			SetServerLocalIP(rServerListInfo[nItemIndex].GetLocalIP());
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
