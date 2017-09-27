// SSHServerManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SSHServerManager.h"
#include "SSHServerManagerDlg.h"
#include "SSH2.h"
#include "AddServerInfoDlg.h"

BOOL WINAPI ConsoleHandler(DWORD dwMessage)
{
	return FALSE;
}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSSHServerManagerDlg �Ի���

DWORD WINAPI SSHServerManagerDlgThreadFunction(void* arg)
{
	CSSHServerManagerDlg *dlg = (CSSHServerManagerDlg*)arg;
	switch (dlg->mManagerType)
	{
	case emServerManagerType_Monitor:
		{
			dlg->AllMonitor();
			break;
		}
	case emServerManagerType_Stop:
		{
			dlg->AllStopServer();
			break;
		}
	case emServerManagerType_Update:
		{
			dlg->AllUpdate();
			break;
		}
	case emServerManagerType_Restart:
		{
			dlg->AllStartServer();
			break;
		}
	case emServerManagerType_UpdateDB:
		{
			dlg->AllUpdateDatebase();
			break;
		}
	case emServerManagerType_ChmodScript:
		{
			dlg->AllScriptExecute();
			break;
		}
	}

	return 0;
}


CSSHServerManagerDlg::CSSHServerManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSSHServerManagerDlg::IDD, pParent),mLogger("SSHServer.log")
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	mUserName[0] = '\0';
	mPassWord[0] = '\0';
}

void CSSHServerManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVER_LIST, mServerListCtrl);
}

BEGIN_MESSAGE_MAP(CSSHServerManagerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_MANUAL_CLOSE_SERVER, &CSSHServerManagerDlg::OnBnClickedManualCloseServer)
	ON_BN_CLICKED(IDC_MANUAL_RESTART_SERVER, &CSSHServerManagerDlg::OnBnClickedManualRestartServer)
	ON_BN_CLICKED(IDC_ADD_SERVER_LIST_ITEM, &CSSHServerManagerDlg::OnBnClickedAddServerListItem)
	ON_BN_CLICKED(IDC_EDIT_SERVER_LIST_ITEM, &CSSHServerManagerDlg::OnBnClickedEditServerListItem)
	ON_BN_CLICKED(IDC_DELETE_SERVER_LIST_ITEM, &CSSHServerManagerDlg::OnBnClickedDeleteServerListItem)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SERVER_LIST, &CSSHServerManagerDlg::OnLvnItemchangedServerList)
	ON_BN_CLICKED(IDC_MANUAL_MONITOR, &CSSHServerManagerDlg::OnBnClickedManualMonitor)
	ON_MESSAGE(WM_MYUPDATEDATA,OnUpdateMyData)  
	ON_BN_CLICKED(IDC_MANUAL_STOP_MONITOR, &CSSHServerManagerDlg::OnBnClickedManualStopMonitor)
	ON_BN_CLICKED(IDC_ALL_STOP_MONITOR, &CSSHServerManagerDlg::OnBnClickedAllStopMonitor)
	ON_BN_CLICKED(IDC_ALL_MONITOR, &CSSHServerManagerDlg::OnBnClickedAllMonitor)
	ON_BN_CLICKED(IDC_ALL_STOP_SERVER, &CSSHServerManagerDlg::OnBnClickedAllStopServer)
	ON_BN_CLICKED(IDC_ALL_UPDATE, &CSSHServerManagerDlg::OnBnClickedAllUpdate)
	ON_BN_CLICKED(IDC_ALL_START_SERVER, &CSSHServerManagerDlg::OnBnClickedAllStartServer)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHANGE_SERVER_TIME, &CSSHServerManagerDlg::OnBnClickedChangeServerTime)
	ON_BN_CLICKED(IDC_UPDATE_SERVER, &CSSHServerManagerDlg::OnBnClickedUpdateServer)
	ON_BN_CLICKED(IDC_ALL_UPDATE_DATEBASE, &CSSHServerManagerDlg::OnBnClickedAllUpdateDatebase)
	ON_BN_CLICKED(IDC_UPDATE_DATABASE, &CSSHServerManagerDlg::OnBnClickedUpdateDatabase)
	ON_BN_CLICKED(IDC_ALL_SCRIPT_EXECUTE, &CSSHServerManagerDlg::OnBnClickedAllScriptExecute)
	ON_BN_CLICKED(IDC_MANUAL_SCRIPT_EXECUTE, &CSSHServerManagerDlg::OnBnClickedManualScriptExecute)
END_MESSAGE_MAP()


// CSSHServerManagerDlg ��Ϣ�������

BOOL CSSHServerManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//��Ӧ�ó����½�һ��console������Ѿ����ڵ��û�ʧ��
	AllocConsole();
	//�ض���
	freopen("conout$","w",stdout) ;
	freopen("conout$","w",stderr) ;

	//if (!SetConsoleCtrlHandler(PHANDLER_ROUTINE(ConsoleHandler), TRUE))
	//{
	//	printf("Unalbe Set Console Handler");
	//	return -1;
	//}

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CSSH2::GlobalInit(0);
	InitAuthorizeInfo();
	InitManulServerInfo();

	DWORD dwStyle1;
	dwStyle1 = mServerListCtrl.GetStyle();
	//����listctrl��ʾ����
	dwStyle1 |= LVS_EX_FULLROWSELECT;
	mServerListCtrl.SetExtendedStyle(dwStyle1);

	mServerListCtrl.InsertColumn(0,L"ID");
	mServerListCtrl.SetColumnWidth(0,200);

	mServerListCtrl.InsertColumn(1,L"����IP");
	mServerListCtrl.SetColumnWidth(1,200);

	mServerListCtrl.InsertColumn(2,L"����IP");
	mServerListCtrl.SetColumnWidth(2,200);

	mServerListCtrl.InsertColumn(3,L"�û���");
	mServerListCtrl.SetColumnWidth(3,80);

	mServerListCtrl.InsertColumn(4,L"״̬");
	mServerListCtrl.SetColumnWidth(4,100);

	mServerListCtrl.InsertColumn(5,L"����");

	//����listvalue��Ŀ��
	CRect tListSheet;
	this->GetWindowRect( &tListSheet );
	ScreenToClient( tListSheet );

	InitServerListInfo();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSSHServerManagerDlg::InitAuthorizeInfo()
{
	tinyxml2::XMLDocument tDoc;
	bool bResult = tDoc.LoadFile("authorize.xml");
	if (!bResult)
	{
		::AfxMessageBox(_T("������Ȩ���ó���!"));
		return;
	}


	XMLElement *tpRoot = tDoc.FirstChildElement();
	if (NULL == tpRoot)
	{
		return;
	}

	XMLElement *pUserNode = tpRoot->FirstChildElement("user");
	if (NULL == pUserNode)
	{
		AfxMessageBox(_T("�Ҳ��������е��û���"));
		return;
	}

	XMLElement *pPasswdNode = tpRoot->FirstChildElement("passwd");
	if (NULL == pPasswdNode)
	{
		AfxMessageBox(_T("�Ҳ��������е�����"));
		return;
	}

	const char* pUserName = pUserNode->Attribute("value");
	if (NULL != pUserName)
	{
		strncpy(mUserName, pUserName, sizeof(mUserName) - 1);
	}
	else
	{
		AfxMessageBox(_T("�Ҳ��������е��û���"));
		return;
	}


	const char* pPassword = pPasswdNode->Attribute("value");
	if (NULL != pPassword)
	{
		strncpy(mPassWord, pPassword, sizeof(mPassWord) - 1);
	}
	else
	{
		AfxMessageBox(_T("�Ҳ��������е�����"));
		return;
	}
}

void CSSHServerManagerDlg::InitManulServerInfo()
{
	tinyxml2::XMLDocument tDoc;
	bool bResult = tDoc.LoadFile( "manul.xml" );
	if (!bResult)
	{
		::AfxMessageBox(_T("�����ֶ��������������ó���!"));
		return;
	}

	XMLElement *tpRoot = tDoc.FirstChildElement();
	if (NULL == tpRoot)
	{
		return;
	}

	XMLElement *pCommandElement = tpRoot->FirstChildElement( "command" );
	if (NULL == pCommandElement)
	{
		AfxMessageBox(_T("�Ҳ��������е�������"));
		return;
	}
	
	GetCommandInfo(pCommandElement, "restart", gRestartCommand, sizeof(gRestartCommand) - 1, _T("�Ҳ���������������������,���������ļ�!"));
	GetCommandInfo(pCommandElement, "stop", gStopCommand, sizeof(gStopCommand) - 1, _T("�Ҳ��Źرշ�������������,���������ļ�!"));
	GetCommandInfo(pCommandElement, "monitor1", gMonitor1Command, sizeof(gMonitor1Command) - 1, _T("�Ҳ��ż��ӷ�������������,���������ļ�!"));
	GetCommandInfo(pCommandElement, "monitor2", gMonitor2Command, sizeof(gMonitor2Command) - 1, _T("�Ҳ��ż��ӷ�������������,���������ļ�!"));
	GetCommandInfo(pCommandElement, "update", gUpdateCommand, sizeof(gUpdateCommand) - 1, _T("�Ҳ��Ÿ��·�������������,���������ļ�!"));
	GetCommandInfo(pCommandElement, "updatedb", gUpdatedbCommand, sizeof(gUpdatedbCommand) - 1, _T("�Ҳ��Ÿ��·�������������,���������ļ�!"));
	GetCommandInfo(pCommandElement, "chmodscript", gChmodScriptCommand, sizeof(gChmodScriptCommand) - 1, _T("�Ҳ����޸Ľű�Ȩ������,���������ļ�!"));

	XMLElement *pServerTypeElement = tpRoot->FirstChildElement( "servertype" );
	if (NULL == pServerTypeElement)
	{
		AfxMessageBox(_T("�Ҳ��������еķ�����������"));
		return;
	}
	const char* pServerType = pServerTypeElement->Attribute("value");
	if (NULL != pServerType)
	{
		gServerType = atoi(pServerType);
	}
	else
	{
		AfxMessageBox(_T("�Ҳ��������еķ�����������"));
		return;
	}

	XMLElement *pRestartTimeElement = tpRoot->FirstChildElement( "restartwaittime" );
	if (NULL == pRestartTimeElement)
	{
		AfxMessageBox(_T("�Ҳ��������еķ�����������"));
		return;
	}
	const char* pRestartTime = pRestartTimeElement->Attribute("value");
	if (NULL != pRestartTime)
	{
		gRestartWaitTime = atoi(pRestartTime);
	}
	else
	{
		AfxMessageBox(_T("�Ҳ��������е��������������ݵȴ�ʱ����"));
		return;
	}

	XMLElement *pStopTimeElement = tpRoot->FirstChildElement( "stopwaittime" );
	if (NULL == pStopTimeElement)
	{
		AfxMessageBox(_T("�Ҳ��������еķ�����������"));
		return;
	}
	const char* pStopTime = pStopTimeElement->Attribute("value");
	if (NULL != pStopTime)
	{
		gStopWaitTime = atoi(pStopTime);
	}
	else
	{
		AfxMessageBox(_T("�Ҳ��������е��������������ݵȴ�ʱ����"));
		return;
	}

	XMLElement *pUpdateTimeElement = tpRoot->FirstChildElement( "updatewaittime" );
	if (NULL == pUpdateTimeElement)
	{
		AfxMessageBox(_T("�Ҳ��������еķ�����������"));
		return;
	}
	const char* pUpdateTime = pUpdateTimeElement->Attribute("value");
	if (NULL != pUpdateTime)
	{
		gUpdateWaitTime = atoi(pUpdateTime);
	}
	else
	{
		AfxMessageBox(_T("�Ҳ��������еĸ��·��������ݵȴ�ʱ����"));
		return;
	}

	XMLElement *pUpdateDBTimeElement = tpRoot->FirstChildElement( "updatedbwaittime" );
	if (NULL == pUpdateDBTimeElement)
	{
		AfxMessageBox(_T("�Ҳ��������еķ�����������"));
		return;
	}
	const char* pUpdateDBTime = pUpdateDBTimeElement->Attribute("value");
	if (NULL != pUpdateDBTime)
	{
		gUpdateDBWaitTime = atoi(pUpdateDBTime);
	}
	else
	{
		AfxMessageBox(_T("�Ҳ��������еĸ������ݿ����ݵȴ�ʱ����"));
		return;
	}
}

void CSSHServerManagerDlg::GetCommandInfo(XMLElement *pCommandElement, char* pCommand, char* pTargetCommand, int nLength, CString strErrMessage)
{
	XMLElement* pSubCommandElement = pCommandElement->FirstChildElement(pCommand);
	if (NULL == pSubCommandElement)
	{
		AfxMessageBox(strErrMessage);
		return;
	}
	const char* pValue = pSubCommandElement->Attribute("value");
	if (NULL != pValue)
	{
		strncpy(pTargetCommand, pValue, nLength);
	}
	else
	{
		AfxMessageBox(strErrMessage);
		return;
	}
}

void CSSHServerManagerDlg::InitServerListInfo()
{
	tinyxml2::XMLDocument tDoc;
	bool bResult = tDoc.LoadFile( "serverlist.xml" );
	if (!bResult)
	{
		::AfxMessageBox(_T("�����ֶ��������������ó���!"));
		return;
	}

	XMLElement *tpRoot = tDoc.FirstChildElement();
	if (NULL == tpRoot)
	{
		return;
	}
	XMLElement *pServerElement = tpRoot->FirstChildElement( "server" );
	for (; NULL != pServerElement; pServerElement = pServerElement->NextSiblingElement("server"))
	{
		const char* pID = pServerElement->Attribute("id");
		if (NULL == pID)
		{
			AfxMessageBox(_T("�����������ļ��е�IDû������!"));
			exit(1);
			return;
		}
		const char* pIP = pServerElement->Attribute("internetip");
		if (NULL ==  pIP)
		{
			AfxMessageBox(_T("�����������ļ��е�����IPû������!"));
			exit(1);
			return;
		}

		const char* pLocalIP = pServerElement->Attribute("localip");
		if (NULL == pLocalIP)
		{
			AfxMessageBox(_T("�����������ļ��е�����IPû������!"));
			exit(1);
			return;
		}

		int nCount = mServerListCtrl.GetItemCount();
		mServerListCtrl.InsertItem(nCount, s2ws(pID).c_str());
		mServerListCtrl.SetItemText(nCount, 1, s2ws(pIP).c_str());
		mServerListCtrl.SetItemText(nCount, 2, s2ws(pLocalIP).c_str());
		mServerListCtrl.SetItemText(nCount, 4, _T("δ���"));
		
		CServerInfo Info;
		Info.SetServerID(s2ws(pID).c_str());
		Info.SetInternetIP(s2ws(pIP).c_str());
		Info.SetLocalIP(s2ws(pLocalIP).c_str());
		mServerListInfo.push_back(Info);
		mServerExecute.push_back(NULL);
	}
}

void CSSHServerManagerDlg::SaveServerListInfoToFile()
{
	tinyxml2::XMLDocument* pFilterDoc = new tinyxml2::XMLDocument;
	XMLDeclaration* pDeclaration = pFilterDoc->NewDeclaration();
	pFilterDoc->LinkEndChild(pDeclaration);
	XMLElement* pRootElem = pFilterDoc->NewElement("serverlist");
	pFilterDoc->LinkEndChild(pRootElem);
	for (int i = 0; i < mServerListInfo.size(); ++ i)
	{
		XMLElement* pServerElem = pFilterDoc->NewElement("server");
		if (NULL == pServerElem)
		{
			continue;
		}
		pServerElem->SetAttribute("id", ws2s(mServerListInfo[i].GetServerID().GetBuffer()).c_str());
		pServerElem->SetAttribute("internetip", ws2s(mServerListInfo[i].GetInternetIP().GetBuffer()).c_str());
		pServerElem->SetAttribute("localip", ws2s(mServerListInfo[i].GetLocalIP().GetBuffer()).c_str());
		pRootElem->LinkEndChild(pServerElem);
	}

	pFilterDoc->SaveFile("serverlist.xml");
	delete pFilterDoc;
}

int CSSHServerManagerDlg::GetMonitorIndex()
{
	for (int i = 0; i < mServerMonitor.size(); ++ i)
	{
		if (mServerMonitor[i]->GetServerManagerType() == emServerManagerType_None)
		{
			return i;
		}
	}

	mServerMonitor.push_back(new CServerManager());
	int nIndex = mServerMonitor.size() - 1;
	mServerMonitor[nIndex]->SetMonitorIndex(nIndex);
	return nIndex;
}

void CSSHServerManagerDlg::GetExecute(int nIndex)
{
	if(NULL == mServerExecute[nIndex])
	{
		mServerExecute[nIndex] = new CServerManager;
	}
}

void CSSHServerManagerDlg::GetServerInfo(char* pIP, int nIPLength, char* pLocalIP, int nLocalIPLength, char* pUserName, int nUserNameLength, char* pPassWord, int nPassWordLength)
{
	CIPAddressCtrl* pIPAddress = (CIPAddressCtrl* )GetDlgItem(IDC_MANUAL_SERVER_IP);
	if (NULL == pIPAddress)
	{
		return;
	}

	CIPAddressCtrl* pLocalIPAddressCtrl = (CIPAddressCtrl*)GetDlgItem(IDC_MANUAL_SERVER_LOCAL_IP);
	if (NULL == pLocalIPAddressCtrl)
	{
		return;
	}

	CEdit* pUserNameEdit = (CEdit*)GetDlgItem(IDC_MANUAL_USER_NAME);
	if (NULL == pUserNameEdit)
	{
		return;
	}

	CEdit* pPassWordEdit = (CEdit*)GetDlgItem(IDC_MANUAL_PASSWORD);
	if (NULL == pPassWordEdit)
	{
		return;
	}


	DWORD dwIPAddress = 0;
	pIPAddress->GetAddress(dwIPAddress);
	if (0 == dwIPAddress)
	{
		::AfxMessageBox(_T("����д����IP��ַ!"));
		return;
	}

	snprintf(pIP, nIPLength - 1, "%d.%d.%d.%d", (0xFF000000&dwIPAddress)>>24,(0xFF0000&dwIPAddress)>>16,(0xFF00&dwIPAddress)>>8,0xFF&dwIPAddress);


	DWORD dwLocalIPAddress = 0;
	pLocalIPAddressCtrl->GetAddress(dwLocalIPAddress);
	if (0 == dwLocalIPAddress)
	{
		::AfxMessageBox(_T("����д����IP��ַ!"));
		return;
	}
	snprintf(pLocalIP, nLocalIPLength - 1, "%d.%d.%d.%d", (0xFF000000&dwLocalIPAddress)>>24,(0xFF0000&dwLocalIPAddress)>>16,(0xFF00&dwLocalIPAddress)>>8,0xFF&dwLocalIPAddress);


	strncpy(pUserName, mUserName, nUserNameLength - 1);
	strncpy(pPassWord, mPassWord, nPassWordLength - 1);
}

void CSSHServerManagerDlg::OnServerStateChange(int nMonitorIndex, TmServerState eState)
{
	mLock.Lock();
	if (nMonitorIndex < 0 || nMonitorIndex >= mServerMonitor.size())
	{
		mLock.UnLock();
		return;
	}
	for (int i = 0; i < mServerListInfo.size(); ++ i)
	{
		if (mServerListInfo[i].GetMonitorIndex() == nMonitorIndex)
		{
			switch(eState)
			{
			case emServerState_None:
				{
					mServerListCtrl.SetItemText(i, 4, _T("δ����"));
					//UpdateData(FALSE);
					break;
				}
			case emServerState_Running:
				{
					mServerListCtrl.SetItemText(i, 4, _T("������"));
					//UpdateData(FALSE);
					break;
				}
			case emServerState_Stop:
				{
					mServerListCtrl.SetItemText(i, 4, _T("��ͣ��"));
					//UpdateData(FALSE);
					break;
				}
			}
		}
	}
	SendMessage(WM_MYUPDATEDATA,FALSE);
	mLock.UnLock();
}

void CSSHServerManagerDlg::OnServerManagerTypeChange(int nMonitorIndex, TmServerManagerType eType)
{
	mLock.Lock();
	if (nMonitorIndex < 0)
	{
		mLock.UnLock();
		return;
	}
	for (int i = 0; i < mServerListInfo.size(); ++ i)
	{
		if (mServerListInfo[i].GetMonitorIndex() == nMonitorIndex)
		{
			if (eType == emServerManagerType_None)
			{
				mServerListCtrl.SetItemText(i, 4, _T("δ���"));
				mServerListInfo[i].SetMonitorIndex(-1);
			}
			else if (eType == emServerManagerType_Monitor)
			{
				mServerListCtrl.SetItemText(i, 4, _T("�����"));
			}
			mLock.UnLock();
			return;
		}
	}

	SendMessage(WM_MYUPDATEDATA,FALSE);
	mLock.UnLock();
}

void CSSHServerManagerDlg::OnBatchComplete(bool bSuccess)
{
	mLock.Lock();
	-- mBatchNum;
	if (bSuccess)
	{
		-- mBatchSuccess;
	}
	if (0 == mBatchNum)
	{
		switch(mBatchOperType)
		{
		case emBatchOperType_Restart:
			{
				if (mBatchSuccess == 0)
				{
					AfxMessageBox(_T("���з������������!"));
				}
				else
				{
					AfxMessageBox(_T("��������������ʧ��,������ִ��!"));
				}
				
				break;
			}
		case emBatchOperType_Stop:
			{
				if (mBatchSuccess == 0)
				{
					AfxMessageBox(_T("���з�����ͣ�����!"));
				}
				else
				{
					AfxMessageBox(_T("����������ͣ��ʧ��,������ִ��!"));
				}
				break;
			}
		case emBatchOperType_Update:
			{
				if (mBatchSuccess == 0)
				{
					AfxMessageBox(_T("���з������������!"));
				}
				else
				{
					AfxMessageBox(_T("��������������ʧ��,������ִ��!"));
				}
				break;
			}
		case emBatchOperType_UpdateDB:
			{
				if (mBatchSuccess == 0)
				{
					AfxMessageBox(_T("���з������������ݿ�����!"));
				}
				else
				{
					AfxMessageBox(_T("�����������������ݿ��ʧ��,������ִ��!"));
				}
				break;
			}
		case emBatchOperType_ChmodScript:
			{
				if (mBatchSuccess == 0)
				{
					AfxMessageBox(_T("���з������޸Ľű�Ȩ�����!"));
				}
				else
				{
					AfxMessageBox(_T("�����������޸Ľű�Ȩ��ʧ��,������ִ��!"));
				}
				break;
			}
		}
		BatchEnableCtrl(true);
		mBatchOperType = emBatchOperType_None;
	}
	mLock.UnLock();
}

void CSSHServerManagerDlg::BatchEnableCtrl(bool bEnable)
{
	CButton* pAddServerListBtn = (CButton*)GetDlgItem(IDC_ADD_SERVER_LIST_ITEM);
	if (NULL != pAddServerListBtn)
	{
		pAddServerListBtn->EnableWindow(bEnable);
	}

	CButton* pEditServerListBtn = (CButton*)GetDlgItem(IDC_EDIT_SERVER_LIST_ITEM);
	if (NULL != pEditServerListBtn)
	{
		pEditServerListBtn->EnableWindow(bEnable);
	}

	CButton* pDeleteServerListBtn = (CButton*)GetDlgItem(IDC_DELETE_SERVER_LIST_ITEM);
	if (NULL != pDeleteServerListBtn)
	{
		pDeleteServerListBtn->EnableWindow(bEnable);
	}

	CButton* pStartMonitorBtn = (CButton*)GetDlgItem(IDC_MANUAL_MONITOR);
	if (NULL != pStartMonitorBtn)
	{
		pStartMonitorBtn->EnableWindow(bEnable);
	}

	CButton* pStopMonitorBtn = (CButton*)GetDlgItem(IDC_MANUAL_STOP_MONITOR);
	if (NULL != pStopMonitorBtn)
	{
		pStopMonitorBtn->EnableWindow(bEnable);
	}

	CButton* pAllMonitorBtn = (CButton*)GetDlgItem(IDC_ALL_MONITOR);
	if (NULL != pAllMonitorBtn)
	{
		pAllMonitorBtn->EnableWindow(bEnable);
	}

	CButton* pAllStopMonitorBtn = (CButton*)GetDlgItem(IDC_ALL_STOP_MONITOR);
	if (NULL != pAllStopMonitorBtn)
	{
		pAllStopMonitorBtn->EnableWindow(bEnable);
	}

	CButton* pAllStopServerBtn = (CButton*)GetDlgItem(IDC_ALL_STOP_SERVER);
	if (NULL != pAllStopServerBtn)
	{
		pAllStopServerBtn->EnableWindow(bEnable);
	}

	CButton* pAllUpdateBtn = (CButton*)GetDlgItem(IDC_ALL_UPDATE);
	if (NULL != pAllUpdateBtn)
	{
		pAllUpdateBtn->EnableWindow(bEnable);
	}

	CButton* pAllUpdateDataBaseBtn = (CButton*)GetDlgItem(IDC_ALL_UPDATE_DATEBASE);
	if (NULL != pAllUpdateDataBaseBtn)
	{
		pAllUpdateDataBaseBtn->EnableWindow(bEnable);
	}

	CButton* pAllReStartServerBtn = (CButton*)GetDlgItem(IDC_ALL_START_SERVER);
	if (NULL != pAllReStartServerBtn)
	{
		pAllReStartServerBtn->EnableWindow(bEnable);
	}

	CButton* pStopServerBtn = (CButton*)GetDlgItem(IDC_MANUAL_CLOSE_SERVER);
	if (NULL != pStopServerBtn)
	{
		pStopServerBtn->EnableWindow(bEnable);
	}

	CButton* pRestartBtn = (CButton*)GetDlgItem(IDC_MANUAL_RESTART_SERVER);
	if (NULL != pRestartBtn)
	{
		pRestartBtn->EnableWindow(bEnable);
	}

	CButton* pUpdateBtn = (CButton*)GetDlgItem(IDC_UPDATE_SERVER);
	if (NULL != pUpdateBtn)
	{
		pUpdateBtn->EnableWindow(bEnable);
	}

	CButton* pUpdateDataBase = (CButton*)GetDlgItem(IDC_UPDATE_DATABASE);
	if (NULL != pUpdateDataBase)
	{
		pUpdateDataBase->EnableWindow(bEnable);
	}

	CButton* pAllChmodScriptBtn = (CButton*)GetDlgItem(IDC_ALL_SCRIPT_EXECUTE);
	if (NULL != pAllChmodScriptBtn)
	{
		pAllChmodScriptBtn->EnableWindow(bEnable);
	}

	CButton* pChmodScriptBtn = (CButton*)GetDlgItem(IDC_MANUAL_SCRIPT_EXECUTE);
	if (NULL != pChmodScriptBtn)
	{
		pChmodScriptBtn->EnableWindow(bEnable);
	}
}

void CSSHServerManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSSHServerManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CSSHServerManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSSHServerManagerDlg::OnBnClickedManualCloseServer()
{
	if (IDOK != MessageBox( _T("ȷ��ֹͣ������?"), _T("ֹͣ������"), MB_OKCANCEL))
	{
		return;
	}
	BatchEnableCtrl(false);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char acIPAddress[MAX_PATH] = {0};
	char acLocalIPAddress[MAX_PATH] = {0};
	char acUserName[MAX_PATH] = {0};
	char acPassWord[MAX_PATH] = {0};
	GetServerInfo(acIPAddress, sizeof(acIPAddress) - 1, acLocalIPAddress, sizeof(acLocalIPAddress), acUserName, sizeof(acUserName) - 1, acPassWord, sizeof(acPassWord) - 1);

	mServerManager.SetBatch(false);
	mServerManager.SetManagerData(emServerManagerType_Stop, "", acIPAddress, acLocalIPAddress, acUserName, acPassWord);
	mServerManager.CreateThread();
}

void CSSHServerManagerDlg::OnBnClickedManualRestartServer()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//mServerManager.SetManagerData(emServerManagerType_Restart, "192.168.7.84", "huangjh", "huangjh123");
	if (IDOK != MessageBox( _T("ȷ������������?"), _T("����������"), MB_OKCANCEL))
	{
		return;
	}
	BatchEnableCtrl(false);

	char acIPAddress[MAX_PATH] = {0};
	char acLocalIPAddress[MAX_PATH] = {0};
	char acUserName[MAX_PATH] = {0};
	char acPassWord[MAX_PATH] = {0};
	GetServerInfo(acIPAddress, sizeof(acIPAddress) - 1, acLocalIPAddress, sizeof(acLocalIPAddress) - 1, acUserName, sizeof(acUserName) - 1, acPassWord, sizeof(acPassWord) - 1);

	mServerManager.SetBatch(false);
	mServerManager.SetManagerData(emServerManagerType_Restart, "", acIPAddress, acLocalIPAddress, acUserName, acPassWord);
	mServerManager.CreateThread();
}

void CSSHServerManagerDlg::OnBnClickedAddServerListItem()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAddServerInfoDlg dlg;
	dlg.SetAdd(TRUE);
	dlg.DoModal();
}

void CSSHServerManagerDlg::OnBnClickedEditServerListItem()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nItemIndex = mServerListCtrl.GetSelectionMark();
	if (nItemIndex < 0 || nItemIndex >= mServerListCtrl.GetItemCount())
	{
		AfxMessageBox(_T("��ѡ��Ҫ�༭�ķ�����!"));
		return;
	}
	//mServerListInfo[nItemIndex].GetLocalIP()

	CAddServerInfoDlg dlg;
	//dlg.SetServerID(mServerListInfo[nItemIndex].GetServerID());
	//dlg.SetServerIP(mServerListInfo[nItemIndex].GetInternetIP());
	//dlg.SetServerLocalIP(mServerListInfo[nItemIndex].GetLocalIP());
	//dlg.SetUserName(mServerListInfo[nItemIndex].GetUserName());
	//dlg.SetPassWord(mServerListInfo[nItemIndex].GetPassWord());
	dlg.SetAdd(FALSE);
	dlg.DoModal();
}

void CSSHServerManagerDlg::OnBnClickedDeleteServerListItem()
{
	if (IDOK != MessageBox( _T("ȷ��ɾ��ָ��������?"), _T("ɾ��������"), MB_OKCANCEL))
	{
		return;
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nItemIndex = mServerListCtrl.GetSelectionMark();
	if (nItemIndex <0 || nItemIndex >= mServerListInfo.size())
	{
		::AfxMessageBox(_T("��ѡ��Ҫɾ���ķ�����"));
		return;
	}	
	int nMonitorIndex = mServerListInfo[nItemIndex].GetMonitorIndex();
	if (nMonitorIndex >= 0 && nMonitorIndex < mServerMonitor.size())
	{
		if (mServerMonitor[nMonitorIndex]->GetServerManagerType() == emServerManagerType_Monitor)
		{
			AfxMessageBox(_T("�÷��������ڼ���״̬��,����ʱ��غ���ɾ��!"));
			return;
		}
	}

	mServerListInfo.erase(mServerListInfo.begin() + mServerListCtrl.GetSelectionMark());
	mServerListCtrl.DeleteItem(mServerListCtrl.GetSelectionMark());
	UpdateData(FALSE);
	if (mServerListInfo.size() != mServerListCtrl.GetItemCount())
	{
		AfxMessageBox(_T("�������BUG,���Զ��ر�!"));
		exit(1);
		return;
	}
	SaveServerListInfoToFile();
}

void CSSHServerManagerDlg::AddServerInfo(CString strID, CString strIP, CString strLocalIP, CString strUserName, CString strPassWord)
{
	int nCount = mServerListCtrl.GetItemCount();
	mServerListCtrl.InsertItem(nCount, strID);
	mServerListCtrl.SetItemText(nCount, 1, strIP);
	mServerListCtrl.SetItemText(nCount, 2, strLocalIP);
	mServerListCtrl.SetItemText(nCount, 3, strUserName);
	mServerListCtrl.SetItemText(nCount, 4, _T("δ���"));
	mServerListCtrl.SetItemText(nCount, 5, strPassWord);

	CServerInfo Info;
	Info.SetServerID(strID);
	Info.SetInternetIP(strIP);
	Info.SetLocalIP(strLocalIP);
	mServerListInfo.push_back(Info);
	mServerExecute.push_back(NULL);

	if (mServerListInfo.size() != mServerListCtrl.GetItemCount())
	{
		AfxMessageBox(_T("�������BUG,���Զ��ر�!"));
		exit(1);
		return;
	}
	SaveServerListInfoToFile();
	UpdateData(FALSE);
}

void CSSHServerManagerDlg::SetServerInfo(CString strID, CString strIP, CString strLocalIP, CString strUserName, CString strPassWord)
{
	int nItemIndex = mServerListCtrl.GetSelectionMark();
	if (nItemIndex < 0 || nItemIndex >= mServerListCtrl.GetItemCount())
	{
		return;
	}

	mServerListInfo[nItemIndex].SetServerID(strID);
	mServerListInfo[nItemIndex].SetInternetIP(strIP);
	mServerListInfo[nItemIndex].SetLocalIP(strLocalIP);

	mServerListCtrl.SetItemText(nItemIndex, 0, strID);
	mServerListCtrl.SetItemText(nItemIndex, 1, strIP);
	mServerListCtrl.SetItemText(nItemIndex, 2, strLocalIP);
	mServerListCtrl.SetItemText(nItemIndex, 3, strUserName);
	mServerListCtrl.SetItemText(nItemIndex, 5, strPassWord);
	SaveServerListInfoToFile();
	UpdateData(FALSE);
}

void CSSHServerManagerDlg::OnLvnItemchangedServerList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// �����ʧ��ѡ�е��ֱ�ӷ���
	if (!pNMLV->uNewState)
	{
		return;
	}

	CIPAddressCtrl* pIPAddress = (CIPAddressCtrl* )GetDlgItem(IDC_MANUAL_SERVER_IP);
	if (NULL == pIPAddress)
	{
		return;
	}

	CIPAddressCtrl* pLocalIPAddress = (CIPAddressCtrl* )GetDlgItem(IDC_MANUAL_SERVER_LOCAL_IP);
	if (NULL == pLocalIPAddress)
	{
		return;
	}
	CEdit* pUserNameEdit = (CEdit*)GetDlgItem(IDC_MANUAL_USER_NAME);
	if (NULL == pUserNameEdit)
	{
		return;
	}

	CEdit* pPassWord = (CEdit*)GetDlgItem(IDC_MANUAL_PASSWORD);
	if (NULL == pPassWord)
	{
		return;
	}

	int nItemIndex = pNMLV->iItem;
	CString strIP = mServerListCtrl.GetItemText(nItemIndex, 1);
	CString strLocalIP = mServerListCtrl.GetItemText(nItemIndex, 2);
	CString strUserName = mServerListCtrl.GetItemText(nItemIndex, 3);
	CString strPassWord = mServerListCtrl.GetItemText(nItemIndex, 5);
	
	char acBuffer[MAX_PATH] = {0};
	strncpy(acBuffer, ws2s(strIP.GetBuffer()).c_str(), sizeof(acBuffer) - 1);
	int nByte1 = 0, nByte2 = 0, nByte3 = 0, nByte4 = 0;
	sscanf(acBuffer, "%d.%d.%d.%d", &nByte1, &nByte2, &nByte3, &nByte4);
	pIPAddress->SetAddress(nByte1, nByte2, nByte3, nByte4);

	
	char acLocalIP[MAX_PATH] = {0};
	strncpy(acLocalIP, ws2s(strLocalIP.GetBuffer()).c_str(), sizeof(acLocalIP) - 1);
	int nLocalByte1 = 0, nLocalByte2 = 0, nLocalByte3 = 0, nLocalByte4 = 0;
	sscanf(acLocalIP, "%d.%d.%d.%d", &nLocalByte1, &nLocalByte2, &nLocalByte3, &nLocalByte4);
	pLocalIPAddress->SetAddress(nLocalByte1, nLocalByte2, nLocalByte3, nLocalByte4);

	pUserNameEdit->SetWindowText(strUserName);
	pPassWord->SetWindowText(strPassWord);
	*pResult = 0;
}

void CSSHServerManagerDlg::OnBnClickedManualMonitor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nItemIndex = mServerListCtrl.GetSelectionMark();
	if (nItemIndex <0 || nItemIndex >= mServerListInfo.size())
	{
		AfxMessageBox(_T("ѡ��������Ƿ�!"));
		return;
	}	
	int nMonitorIndex = mServerListInfo[nItemIndex].GetMonitorIndex();
	if (nMonitorIndex >= 0 && nMonitorIndex < mServerMonitor.size())
	{
		if (mServerMonitor[nMonitorIndex]->GetServerManagerType() == emServerManagerType_Monitor)
		{
			AfxMessageBox(_T("�÷������Ѿ����ڼ���״̬��!"));
			return;
		}
	}


	int nIndex = GetMonitorIndex();


	mServerListInfo[nItemIndex].SetMonitorIndex(nIndex);
	string strID = ws2s(mServerListInfo[nItemIndex].GetServerID().GetBuffer());
	string strInterIP = ws2s(mServerListInfo[nItemIndex].GetInternetIP().GetBuffer());
	string strLocalIP = ws2s(mServerListInfo[nItemIndex].GetLocalIP().GetBuffer());
	string strUserName(mUserName);
	string strPassWord(mPassWord);


	mServerMonitor[nIndex]->SetBatch(false);
	mServerMonitor[nIndex]->SetManagerData(emServerManagerType_Monitor, strID.c_str(), strInterIP.c_str(), strLocalIP.c_str(), strUserName.c_str(), strPassWord.c_str());
	mServerMonitor[nIndex]->CreateThread();
	
}

 LRESULT CSSHServerManagerDlg::OnUpdateMyData(WPARAM wParam, LPARAM lParam)
 {
	UpdateData(wParam);
	return 1;
 }
 void CSSHServerManagerDlg::OnBnClickedManualStopMonitor()
 {
	 // TODO: �ڴ���ӿؼ�֪ͨ����������
	 int nItemIndex = mServerListCtrl.GetSelectionMark();
	 if (nItemIndex <0 || nItemIndex >= mServerListInfo.size())
	 {
		 AfxMessageBox(_T("ѡ��������Ƿ�!"));
		 return;
	 }
	 int nMonitorIndex = mServerListInfo[nItemIndex].GetMonitorIndex();
	 if (nMonitorIndex < 0 || nMonitorIndex >= mServerMonitor.size())
	 {
		 AfxMessageBox(_T("���������Ѿ�����δ����״̬!"));
		 return;
	 }
	if (mServerMonitor[nMonitorIndex]->GetServerManagerType() == emServerManagerType_None
		|| mServerMonitor[nMonitorIndex]->GetThreadState() == emThreadState_Stopping)
	{
		AfxMessageBox(_T("���������Ѿ�����δ����״̬!"));
		return;
	}
	mServerMonitor[nMonitorIndex]->SetBatch(false);
	mServerMonitor[nMonitorIndex]->StopThread(0, NULL);
 }

 void CSSHServerManagerDlg::OnBnClickedAllStopMonitor()
 {
	 if (IDOK != MessageBox( _T("ȷ��ֹͣ������з�����?"), _T("ֹͣ��ط�����"), MB_OKCANCEL))
	 {
		 return;
	 }
	 // TODO: �ڴ���ӿؼ�֪ͨ����������
	 for (int i = 0; i < mServerListInfo.size(); i++)
	 {
		 int nMonitorIndex = mServerListInfo[i].GetMonitorIndex();
		 if (nMonitorIndex < 0 || nMonitorIndex >= mServerMonitor.size())
		 {
			 continue;
		 }
		 if (mServerMonitor[nMonitorIndex]->GetServerManagerType() == emServerManagerType_None)
		 {
			 continue;
		 }
		 mServerMonitor[nMonitorIndex]->SetBatch(false);
		 mServerMonitor[nMonitorIndex]->StopThread(0, NULL);
		 mServerMonitor[nMonitorIndex]->SetServerState(emServerState_Stop);
		 mServerMonitor[nMonitorIndex]->SetServerManagerType(emServerManagerType_None, true);
	 }
 }

 void CSSHServerManagerDlg::OnBnClickedAllMonitor()
 {
	if (IDOK != MessageBox( _T("ȷ��������з�����?"), _T("��ط�����"), MB_OKCANCEL))
	{
		return;
	}

	DWORD dwThreadID = 0;
	mManagerType = emServerManagerType_Monitor;
	HANDLE tThreadHand = ::CreateThread(NULL, 0, SSHServerManagerDlgThreadFunction, this, 0, (LPDWORD)&dwThreadID);
	if(NULL == tThreadHand)
	{
		AfxMessageBox(_T("������з�����ʧ��"));
	}
	CloseHandle(tThreadHand);
 }

void CSSHServerManagerDlg::AllMonitor()
{
	for (int i = 0; i < mServerListInfo.size(); i++)
	{
		int nMonitorIndex = mServerListInfo[i].GetMonitorIndex();
		if (nMonitorIndex >= 0 && nMonitorIndex < mServerMonitor.size())
		{
			if (mServerMonitor[nMonitorIndex]->GetServerManagerType() == emServerManagerType_Monitor)
			{
				continue;
			}
		}


		int nIndex = GetMonitorIndex();

		mServerListInfo[i].SetMonitorIndex(nIndex);
		string strID = ws2s(mServerListInfo[i].GetServerID().GetBuffer());
		string strIP = ws2s(mServerListInfo[i].GetInternetIP().GetBuffer());
		string strLocalIP = ws2s(mServerListInfo[i].GetLocalIP().GetBuffer());
		string strUserName(mUserName);
		string strPassWord(mPassWord);


		mServerMonitor[nIndex]->SetBatch(false);
		mServerMonitor[nIndex]->SetManagerData(emServerManagerType_Monitor, strID.c_str(), strIP.c_str(), strLocalIP.c_str(), strUserName.c_str(), strPassWord.c_str());
		mServerMonitor[nIndex]->CreateThread();
		Sleep(200);
	}
}

 void CSSHServerManagerDlg::OnBnClickedAllStopServer()
 {
	 if (IDOK != MessageBox( _T("�ط���Ҫ������,�����ĵȴ�,ȷ��ֹͣ���з�����?"), _T("ֹͣ������"), MB_OKCANCEL))
	 {
		 return;
	 }


	 DWORD dwThreadID = 0;
	 mManagerType = emServerManagerType_Stop;
	 HANDLE tThreadHand = ::CreateThread(NULL, 0, SSHServerManagerDlgThreadFunction, this, 0, (LPDWORD)&dwThreadID);
	 if(NULL == tThreadHand)
	 {
		 AfxMessageBox(_T("ֹͣ���з�����ʧ��"));
	 }
	 CloseHandle(tThreadHand);
 }

 void CSSHServerManagerDlg::AllStopServer()
 {
	 // TODO: �ڴ���ӿؼ�֪ͨ����������
	 mBatchOperType = emBatchOperType_Stop;
	 mBatchNum = mServerListInfo.size();
	 mBatchSuccess = mBatchNum;
	 BatchEnableCtrl(false);
	 for (int i = 0; i < mServerListInfo.size(); i++)
	 {
		 GetExecute(i);

		 string strID = ws2s(mServerListInfo[i].GetServerID().GetBuffer());
		 string strIP = ws2s(mServerListInfo[i].GetInternetIP().GetBuffer());
		 string strLocalIP = ws2s(mServerListInfo[i].GetLocalIP().GetBuffer());
		 string strUserName(mUserName);
		 string strPassWord(mPassWord);


		 mServerExecute[i]->SetBatch(true);
		 mServerExecute[i]->SetManagerData(emServerManagerType_Stop, strID.c_str(), strIP.c_str(), strLocalIP.c_str(), strUserName.c_str(), strPassWord.c_str());
		 mServerExecute[i]->CreateThread();
		 Sleep(200);
	 }
 }

 void CSSHServerManagerDlg::OnBnClickedAllUpdate()
 {
	 if (IDOK != MessageBox( _T("ȷ���������з�����?"), _T("���·�����"), MB_OKCANCEL))
	 {
		 return;
	 }

	 DWORD dwThreadID = 0;
	 mManagerType = emServerManagerType_Update;
	 HANDLE tThreadHand = ::CreateThread(NULL, 0, SSHServerManagerDlgThreadFunction, this, 0, (LPDWORD)&dwThreadID);
	 if(NULL == tThreadHand)
	 {
		 AfxMessageBox(_T("�������з�����ʧ��"));
	 }
	 CloseHandle(tThreadHand);
 }


void CSSHServerManagerDlg::AllUpdate()
{
	mBatchOperType = emBatchOperType_Update;
	mBatchNum = mServerListInfo.size();
	mBatchSuccess = mBatchNum;
	BatchEnableCtrl(false);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int i = 0; i < mServerListInfo.size(); i++)
	{
		GetExecute(i);

		string strID = ws2s(mServerListInfo[i].GetServerID().GetBuffer());
		string strIP = ws2s(mServerListInfo[i].GetInternetIP().GetBuffer());
		string strLocalIP = ws2s(mServerListInfo[i].GetLocalIP().GetBuffer());
		string strUserName(mUserName);
		string strPassWord(mPassWord);

		mServerExecute[i]->SetBatch(true);
		mServerExecute[i]->SetManagerData(emServerManagerType_Update, strID.c_str(), strIP.c_str(), strLocalIP.c_str(), strUserName.c_str(), strPassWord.c_str());
		mServerExecute[i]->CreateThread();
		Sleep(200);
	}
}

 void CSSHServerManagerDlg::OnBnClickedAllStartServer()
 {
	 if (IDOK != MessageBox( _T("������Ҫ������,�����ĵȴ�,ȷ���������з�����?"), _T("����������"), MB_OKCANCEL))
	 {
		 return;
	 }

	 DWORD dwThreadID = 0;
	 mManagerType = emServerManagerType_Restart;
	 HANDLE tThreadHand = ::CreateThread(NULL, 0, SSHServerManagerDlgThreadFunction, this, 0, (LPDWORD)&dwThreadID);
	 if(NULL == tThreadHand)
	 {
		 AfxMessageBox(_T("�������з�����ʧ��"));
	 }
	 CloseHandle(tThreadHand);
 }

 void CSSHServerManagerDlg::AllStartServer()
 {
	 // TODO: �ڴ���ӿؼ�֪ͨ����������
	 mBatchOperType = emBatchOperType_Restart;
	 mBatchNum = mServerListInfo.size();
	 mBatchSuccess = mBatchNum;
	 BatchEnableCtrl(false);
	 for (int i = 0; i < mServerListInfo.size(); i++)
	 {
		 GetExecute(i);

		 string strID = ws2s(mServerListInfo[i].GetServerID().GetBuffer());
		 string strIP = ws2s(mServerListInfo[i].GetInternetIP().GetBuffer());
		 string strLocalIP = ws2s(mServerListInfo[i].GetLocalIP().GetBuffer());
		 string strUserName(mUserName);
		 string strPassWord(mPassWord);

		 mServerExecute[i]->SetBatch(true);
		 mServerExecute[i]->SetManagerData(emServerManagerType_Restart, strID.c_str(), strIP.c_str(), strLocalIP.c_str(), strUserName.c_str(), strPassWord.c_str());
		 mServerExecute[i]->CreateThread();
		 Sleep(200);
	 }
 }

 BOOL CSSHServerManagerDlg::DestroyWindow()
 {
	 // TODO: �ڴ����ר�ô����/����û���
	for (int i = 0; i < mServerMonitor.size(); ++ i)
	{
		if (NULL != mServerMonitor[i])
		{
			delete mServerMonitor[i];
			mServerMonitor[i] = NULL;
		}
	}
	mServerMonitor.clear();


	for (int i = 0; i < mServerExecute.size(); ++ i)
	{
		if (NULL != mServerExecute[i])
		{
			delete mServerExecute[i];
			mServerExecute[i] = NULL;
		}
	}
	mServerExecute.clear();
	 return CDialog::DestroyWindow();
 }

 void CSSHServerManagerDlg::OnClose()
 {
	 // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	 CDialog::OnClose();
 }
 void CSSHServerManagerDlg::OnBnClickedChangeServerTime()
 {
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDateTimeCtrl* pDateCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_SERVER_DATE);
	if(NULL == pDateCtrl)
	{
		return;
	}

	CEdit* pTimeEdit = (CEdit*)GetDlgItem(IDC_SERVER_TIME);
	if (NULL == pTimeEdit)
	{
		return;
	}
	CString strTime;
	pTimeEdit->GetWindowText(strTime);
	if (strTime == "")
	{
		AfxMessageBox(_T("����дʱ��!"));
		return;
	}

	int nHour = 0;
	int nMin = 0;
	int nSecond = 0;
	if (wcschr(strTime.GetBuffer(), _T(':')))
	{
		swscanf(strTime.GetBuffer(), _T("%d:%d:%d"), &nHour, &nMin, &nSecond);
	}
	else if (wcschr(strTime.GetBuffer(), _T('��')))
	{
		swscanf(strTime.GetBuffer(), _T("%d��%d��%d"), &nHour, &nMin, &nSecond);
	}
	else
	{
		AfxMessageBox(_T("ʱ���ʽ����!"));
		return;
	}
	if (nHour < 0 || nHour >= 24 || nMin < 0 || nMin >= 60 || nSecond < 0 || nSecond >= 60)
	{
		AfxMessageBox(_T("ʱ���ʽ����!"));
		return;
	}


	CTime date;
	pDateCtrl->GetTime(date);
	snprintf(gTime, sizeof(gTime) -1, "%d-%d-%d %d:%d:%d", date.GetYear(), date.GetMonth(), date.GetDay(), nHour, nMin, nSecond);

	char acIPAddress[MAX_PATH] = {0};
	char acLocalIPAddress[MAX_PATH] = {0};
	char acUserName[MAX_PATH] = {0};
	char acPassWord[MAX_PATH] = {0};
	GetServerInfo(acIPAddress, sizeof(acIPAddress) - 1, acLocalIPAddress, sizeof(acLocalIPAddress) - 1, acUserName, sizeof(acUserName) - 1, acPassWord, sizeof(acPassWord) - 1);

	mServerManager.SetManagerData(emServerManagerType_SetTime, "", acIPAddress, acLocalIPAddress, acUserName, acPassWord);
	mServerManager.CreateThread();
 }

 void CSSHServerManagerDlg::OnBnClickedUpdateServer()
 {
	 if (IDOK != MessageBox( _T("ȷ�����·�����?"), _T("���·�����"), MB_OKCANCEL))
	 {
		 return;
	 }
	 BatchEnableCtrl(false);

	 // TODO: �ڴ���ӿؼ�֪ͨ����������
	 char acIPAddress[MAX_PATH] = {0};
	 char acLocalIPAddress[MAX_PATH] = {0};
	 char acUserName[MAX_PATH] = {0};
	 char acPassWord[MAX_PATH] = {0};
	 GetServerInfo(acIPAddress, sizeof(acIPAddress) - 1, acLocalIPAddress, sizeof(acLocalIPAddress) - 1, acUserName, sizeof(acUserName) - 1, acPassWord, sizeof(acPassWord) - 1);

	 mServerManager.SetBatch(false);
	 mServerManager.SetManagerData(emServerManagerType_Update, "", acIPAddress, acLocalIPAddress, acUserName, acPassWord);
	 mServerManager.CreateThread();
 }

 void CSSHServerManagerDlg::OnBnClickedAllUpdateDatebase()
 {
	 if (IDOK != MessageBox( _T("ȷ���������з��������ݿ��?"), _T("�������ݿ��"), MB_OKCANCEL))
	 {
		 return;
	 }

	 DWORD dwThreadID = 0;
	 mManagerType = emServerManagerType_UpdateDB;
	 HANDLE tThreadHand = ::CreateThread(NULL, 0, SSHServerManagerDlgThreadFunction, this, 0, (LPDWORD)&dwThreadID);
	 if(NULL == tThreadHand)
	 {
		 AfxMessageBox(_T("�������з��������ݿ��ʧ��"));
	 }
	 CloseHandle(tThreadHand);

 }

void CSSHServerManagerDlg::AllUpdateDatebase()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	mBatchOperType = emBatchOperType_UpdateDB;
	mBatchNum = mServerListInfo.size();
	mBatchSuccess = mBatchNum;
	BatchEnableCtrl(false);
	for (int i = 0; i < mServerListInfo.size(); i++)
	{
		GetExecute(i);

		string strID = ws2s(mServerListInfo[i].GetServerID().GetBuffer());
		string strIP = ws2s(mServerListInfo[i].GetInternetIP().GetBuffer());
		string strLocalIP = ws2s(mServerListInfo[i].GetLocalIP().GetBuffer());
		string strUserName(mUserName);
		string strPassWord(mPassWord);

		mServerExecute[i]->SetBatch(true);
		mServerExecute[i]->SetManagerData(emServerManagerType_UpdateDB, strID.c_str(), strIP.c_str(), strLocalIP.c_str(), strUserName.c_str(), strPassWord.c_str());
		mServerExecute[i]->CreateThread();
		Sleep(200);
	}
}

 void CSSHServerManagerDlg::OnBnClickedUpdateDatabase()
 {
	 if (IDOK != MessageBox( _T("ȷ�����·��������ݿ��?"), _T("���·��������ݿ��"), MB_OKCANCEL))
	 {
		 return;
	 }
	 BatchEnableCtrl(false);

	 // TODO: �ڴ���ӿؼ�֪ͨ����������
	 char acIPAddress[MAX_PATH] = {0};
	 char acLocalIPAddress[MAX_PATH] = {0};
	 char acUserName[MAX_PATH] = {0};
	 char acPassWord[MAX_PATH] = {0};
	 GetServerInfo(acIPAddress, sizeof(acIPAddress) - 1, acLocalIPAddress, sizeof(acLocalIPAddress) - 1, acUserName, sizeof(acUserName) - 1, acPassWord, sizeof(acPassWord) - 1);

	 mServerManager.SetBatch(false);
	 mServerManager.SetManagerData(emServerManagerType_UpdateDB, "", acIPAddress, acLocalIPAddress, acUserName, acPassWord);
	 mServerManager.CreateThread();
 }

 void CSSHServerManagerDlg::OnBnClickedAllScriptExecute()
 {
	 // TODO: �ڴ���ӿؼ�֪ͨ����������
	 if (IDOK != MessageBox( _T("ȷ���޸����з������ű�Ȩ��?"), _T("�޸Ľű�Ȩ��"), MB_OKCANCEL))
	 {
		 return;
	 }

	 DWORD dwThreadID = 0;
	 mManagerType = emServerManagerType_ChmodScript;
	 HANDLE tThreadHand = ::CreateThread(NULL, 0, SSHServerManagerDlgThreadFunction, this, 0, (LPDWORD)&dwThreadID);
	 if(NULL == tThreadHand)
	 {
		 AfxMessageBox(_T("�޸����з������ű�Ȩ��ʧ��"));
	 }
	 CloseHandle(tThreadHand);
 }

void CSSHServerManagerDlg::AllScriptExecute()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	mBatchOperType = emBatchOperType_ChmodScript;
	mBatchNum = mServerListInfo.size();
	mBatchSuccess = mBatchNum;
	BatchEnableCtrl(false);
	for (int i = 0; i < mServerListInfo.size(); i++)
	{
		GetExecute(i);

		string strID = ws2s(mServerListInfo[i].GetServerID().GetBuffer());
		string strIP = ws2s(mServerListInfo[i].GetInternetIP().GetBuffer());
		string strLocalIP = ws2s(mServerListInfo[i].GetLocalIP().GetBuffer());
		string strUserName(mUserName);
		string strPassWord(mPassWord);

		mServerExecute[i]->SetBatch(true);
		mServerExecute[i]->SetManagerData(emServerManagerType_ChmodScript, strID.c_str(), strIP.c_str(), strLocalIP.c_str(), strUserName.c_str(), strPassWord.c_str());
		mServerExecute[i]->CreateThread();
		Sleep(200);
	}
}

 void CSSHServerManagerDlg::OnBnClickedManualScriptExecute()
 {
	 // TODO: �ڴ���ӿؼ�֪ͨ����������
	 if (IDOK != MessageBox( _T("ȷ���޸Ľű�Ȩ��?"), _T("�޸Ľű�Ȩ��"), MB_OKCANCEL))
	 {
		 return;
	 }
	 BatchEnableCtrl(false);

	 // TODO: �ڴ���ӿؼ�֪ͨ����������
	 char acIPAddress[MAX_PATH] = {0};
	 char acLocalIPAddress[MAX_PATH] = {0};
	 char acUserName[MAX_PATH] = {0};
	 char acPassWord[MAX_PATH] = {0};
	 GetServerInfo(acIPAddress, sizeof(acIPAddress) - 1, acLocalIPAddress, sizeof(acLocalIPAddress) - 1, acUserName, sizeof(acUserName) - 1, acPassWord, sizeof(acPassWord) - 1);

	 mServerManager.SetBatch(false);
	 mServerManager.SetManagerData(emServerManagerType_ChmodScript, "", acIPAddress, acLocalIPAddress, acUserName, acPassWord);
	 mServerManager.CreateThread();
 }
