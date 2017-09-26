// RestartServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RestartServer.h"
#include "RestartServerDlg.h"
#include "message.h"
#include <string>
using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_ADDTRAYICON WM_USER+101	// 为添加托盘自定义的消息

// 多字节转宽字符，这个函数效率很低，高效率函数不同调用
wstring s2ws(const string& s)
{
	string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";

	setlocale(LC_ALL, "chs"); 

	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;

	TCHAR acDest[MAX_PATH] = {0};

	mbstowcs(acDest,_Source,_Dsize);
	wstring result = acDest;
	setlocale(LC_ALL, curLocale.c_str());

	return result;
}
// 宽字符转多字节，这个函数效率很低，高效率函数不同调用
string ws2s(const wstring& ws)
{
	string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";

	setlocale(LC_ALL, "chs"); 
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = ws.size() * 2 + 1;
	char acDest[MAX_PATH * 2] = {0};

	wcstombs(acDest, _Source, _Dsize);
	string result = acDest;


	setlocale(LC_ALL, curLocale.c_str());
	return result;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CRestartServerDlg 对话框




CRestartServerDlg::CRestartServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRestartServerDlg::IDD, pParent)
	, mEditHour(0)
	, mEditMinute(0)
	, mEditSecond(0)
	, mStaticServerTime(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	mSockfd = 0;
	memset(mBuffer, 0, sizeof(mBuffer));
	mBuffLength = 0;
	mCurrentServerTime = 0;
	mbGetServerName = false;
	mbDisConnect = false;
	flag=false;
}

void CRestartServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HOUR, mEditHour);
	DDV_MinMaxInt(pDX, mEditHour, 0, 23);
	DDX_Text(pDX, IDC_EDIT_MINUTE, mEditMinute);
	DDV_MinMaxInt(pDX, mEditMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SECOND, mEditSecond);
	DDV_MinMaxInt(pDX, mEditSecond, 0, 59);
	DDX_Control(pDX, IDC_SET_SERVER_DATE_TIME, mDateTimeCtrl);
	DDX_Text(pDX, IDC_CURRENT_SERVER_TIME, mStaticServerTime);
	DDX_Control(pDX, IDC_CURRENT_SERVER_TIME, mServerTimeControl);
}

BEGIN_MESSAGE_MAP(CRestartServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ADDTRAYICON, OnAddTrayIcon)//添加消息映射
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_GET_SERVER_DATE_TIME, &CRestartServerDlg::OnBnClickedGetServerDateTime)
	ON_BN_CLICKED(IDC_SET_SERVER_TIME, &CRestartServerDlg::OnBnClickedSetServerTime)
	ON_BN_CLICKED(IDC_RESTART_SERVER0, &CRestartServerDlg::OnBnClickedRestartServer0)
	ON_BN_CLICKED(IDC_STOP_SERVER0, &CRestartServerDlg::OnBnClickedStopServer0)
	ON_BN_CLICKED(IDC_SELECT_SERVER0, &CRestartServerDlg::OnBnClickedSelectServer0)
	ON_BN_CLICKED(IDC_SELECT_SERVER1, &CRestartServerDlg::OnBnClickedSelectServer1)
	ON_BN_CLICKED(IDC_SELECT_SERVER2, &CRestartServerDlg::OnBnClickedSelectServer2)
	ON_BN_CLICKED(IDC_SELECT_SERVER3, &CRestartServerDlg::OnBnClickedSelectServer3)
	ON_BN_CLICKED(IDC_SELECT_SERVER4, &CRestartServerDlg::OnBnClickedSelectServer4)
	ON_BN_CLICKED(IDC_SELECT_SERVER5, &CRestartServerDlg::OnBnClickedSelectServer5)
	ON_BN_CLICKED(IDC_SELECT_SERVER11, &CRestartServerDlg::OnBnClickedSelectServer11)
	ON_BN_CLICKED(IDC_SELECT_SERVER10, &CRestartServerDlg::OnBnClickedSelectServer10)
	ON_BN_CLICKED(IDC_SELECT_SERVER9, &CRestartServerDlg::OnBnClickedSelectServer9)
	ON_BN_CLICKED(IDC_SELECT_SERVER6, &CRestartServerDlg::OnBnClickedSelectServer6)
	ON_BN_CLICKED(IDC_SELECT_SERVER7, &CRestartServerDlg::OnBnClickedSelectServer7)
	ON_BN_CLICKED(IDC_SELECT_SERVER8, &CRestartServerDlg::OnBnClickedSelectServer8)
END_MESSAGE_MAP()


// CRestartServerDlg 消息处理程序

BOOL CRestartServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// 创建托盘图标
	mNotifyIconData.cbSize = sizeof(NOTIFYICONDATA);//为notifyicondata结构体分配空间
	//为notifyicondata结构体各个参数赋值
	mNotifyIconData.hWnd = m_hWnd;
	mNotifyIconData.uID = IDI_ICON_SMILE; //ID标识
	mNotifyIconData.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP;
	mNotifyIconData.uCallbackMessage = WM_ADDTRAYICON;
	mNotifyIconData.hIcon=AfxGetApp()->LoadIcon(IDI_ICON1);//托盘图标


	std::wstring tIconTipStr(L"");

	// 读取配置文件
	wchar_t szWIP[256] = {0};
	GetPrivateProfileString(_T("serverinfo"), _T("ip"), _T(""), szWIP, sizeof(szWIP)/2, _T("./restartserverconfig.ini"));
	char szIP[256] = {0};
	_snprintf(szIP, sizeof(szIP), "%s", ws2s(szWIP).c_str());
	tIconTipStr.append( szWIP );
	tIconTipStr.append(L" : ");

	// 读取连接重启工具端口
	wchar_t szWPort[256] = {0};
	GetPrivateProfileString(_T("serverinfo"), _T("port"), _T(""), szWPort, sizeof(szWPort)/2, _T("./restartserverconfig.ini"));
	char szPort[256] = {0};
	_snprintf(szPort, sizeof(szPort), "%s", ws2s(szWPort).c_str());
	int tzPort = atoi( szPort );
	tIconTipStr.append( szWPort );

	wcscpy( mNotifyIconData.szTip, tIconTipStr.c_str() );
	//装载托盘图标
	Shell_NotifyIcon(NIM_ADD, &mNotifyIconData);

	// 读取服务器名称
	wchar_t szWIP_Svr0[256] = {0};
	GetPrivateProfileString(_T("Server_0"), _T("name"), _T(""), szWIP_Svr0, sizeof(szWIP_Svr0)/2, _T("./restartserverconfig.ini"));

	GetDlgItem( IDC_RESTART_SERVER0 )->ShowWindow(FALSE);
	GetDlgItem( IDC_STOP_SERVER0 )->ShowWindow(FALSE);
	int tServerID[]  = {IDC_SERVER0, IDC_SERVER1, IDC_SERVER2, IDC_SERVER3,IDC_SERVER4,IDC_SERVER5, IDC_SERVER6, IDC_SERVER7, IDC_SERVER8, IDC_SERVER9, IDC_SERVER10, IDC_SERVER11};
	const int tServerNum = sizeof(tServerID) / sizeof(int);
	for (int i = 0; i < tServerNum; ++ i)
	{
		GetDlgItem( tServerID[i] )->EnableWindow(FALSE);
	}

	int tSelectServerID[] = { IDC_SELECT_SERVER0, IDC_SELECT_SERVER1, IDC_SELECT_SERVER2, IDC_SELECT_SERVER3, IDC_SELECT_SERVER4, IDC_SELECT_SERVER5,
							  IDC_SELECT_SERVER6, IDC_SELECT_SERVER7, IDC_SELECT_SERVER8, IDC_SELECT_SERVER9, IDC_SELECT_SERVER10, IDC_SELECT_SERVER11};
	const int tSelectServerNum = sizeof(tSelectServerID) / sizeof(int);
	for (int i = 0; i < tSelectServerNum; ++ i)
	{
		GetDlgItem(tSelectServerID[i])->EnableWindow(FALSE);
	}

	//GetDlgItem(IDC_NAME_SERVER0)->EnableWindow(FALSE);
	//GetDlgItem(IDC_NAME_SERVER1)->EnableWindow(FALSE);
	//GetDlgItem(IDC_NAME_SERVER2)->EnableWindow(FALSE);
	//GetDlgItem(IDC_NAME_SERVER3)->EnableWindow(FALSE);

	//CString tBtnStartName("重启");
	//tBtnStartName.Append( szWIP_Svr0 );
	//GetDlgItem( IDC_RESTART_SERVER0 )->SetWindowText( tBtnStartName );
	//CString tBtnStopName("停止");
	//tBtnStopName.Append( szWIP_Svr0 );
	//GetDlgItem( IDC_STOP_SERVER0 )->SetWindowText( tBtnStopName );

	// 初始化 socket 连接
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(1, 1), &wsaData))
	{
		return -1;
	}

	struct sockaddr_in tSerAdd;

	mSockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(mSockfd < 0)
	{
		printf("socket error");
		return -1;
	}

	memset( (void*)&tSerAdd, sizeof(sockaddr_in), 0 );

	tSerAdd.sin_family = AF_INET;
	tSerAdd.sin_port = htons(tzPort);
	tSerAdd.sin_addr.s_addr = inet_addr(szIP);

	//设置非阻塞方式连接（如果去掉看不出效果？）
	// FIONBIO : 允许非阻塞模式则非零，如禁止非阻塞模式则为零
	unsigned long ul = 1;//1为非阻塞,0为阻塞
	int nResult = ioctlsocket( mSockfd, FIONBIO, (unsigned long*)&ul );
	if (nResult == SOCKET_ERROR)
	{
		WSACleanup();
		return 1;
	}

	int tRet = connect( mSockfd, (struct sockaddr*)&tSerAdd, sizeof(sockaddr) );
	int nError = WSAGetLastError();

	for (int i = 0; i < MAX_SERVER_NUM; ++i)
	{
		mSelectServer[i] = false;
	}
	mCurrRestartNum = 0;
	SetTimer(WM_TIMER, 1000, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRestartServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRestartServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CRestartServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRestartServerDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	closesocket(mSockfd);
	mSockfd = 0;
	WSACleanup();
}

void CRestartServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// 获取按钮上的服务器名称
	if ( !mbDisConnect )
	{
		SendGetServerNameRequest();
	}
	
	if (mCurrentServerTime > 0)
	{
		++ mCurrentServerTime;
		tm* pTm = gmtime(&mCurrentServerTime);
		wchar_t acTime[256] = {0};
		swprintf(acTime, sizeof(acTime)/2, _T("%d年%d月%d日 %d时:%d分:%d秒"), pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
		mServerTimeControl.SetWindowText(acTime);
	}

	if(mSockfd <= 0)
	{
		return;
	}

	int nRet = recv(mSockfd, mBuffer + mBuffLength, MAX_BUFF_SIZE - mBuffLength, 0);
	if (nRet == SOCKET_ERROR)
	{
		int nError = WSAGetLastError();
		switch (nError)
		{
		case WSAEWOULDBLOCK:
			{
				break;
			}
		default:
			{
				mbDisConnect = true;
				mSockfd = 0;
				closesocket(mSockfd);
				WSACleanup();
				MessageBox(_T("服务器断开连接，请重起程序!"));
				break;
			}
		}
	}
	else if(nRet == 0)
	{
		mSockfd = 0;
		closesocket(mSockfd);
		WSACleanup();
		MessageBox(_T("服务器断开连接，请重起程序!"));
		mbDisConnect = true;
	}
	else
	{
		mBuffLength += nRet;
		const int tMsgSize = sizeof(CMessage);
		while(true)
		{
			if (mBuffLength >= tMsgSize)
			{
				char nBuffer[MAX_BUFF_SIZE];
				memmove(nBuffer, mBuffer, mBuffLength);
				CMessage* pMessage = (CMessage*)nBuffer;
				int nPackageSize = pMessage->Size;
				if (nPackageSize <= mBuffLength)
				{
					mBuffLength -= nPackageSize;
					memmove(mBuffer, mBuffer + nPackageSize, mBuffLength);
					OnMessage(pMessage);
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}

	}

	CDialog::OnTimer(nIDEvent);
}

void CRestartServerDlg::OnBnClickedGetServerDateTime()
{
	// TODO: 在此添加控件通知处理程序代码
	CMessageGetServerDateTimeRequest tGetServerDataTimeRequest;
	tGetServerDataTimeRequest.ID = emC2SRequest_GetServerDateTime;
	tGetServerDataTimeRequest.Size = sizeof(CMessageGetServerDateTimeRequest);
	int nResult = send(mSockfd, (char*)&tGetServerDataTimeRequest, sizeof(CMessageGetServerDateTimeRequest), 0);
	if(-1 == nResult)
	{
		SendMsgBoxFailed();
	}
}


void CRestartServerDlg::OnMessage(CMessage* pMessage)
{
	switch(pMessage->ID)
	{
	case emS2CResponse_RestartServer:
	{
		OnMsgServerRestartResponse(pMessage);
		break;
	}
	case emS2CResponse_StopServer:
		{
			MessageBox(_T("停服完成"));
			break;
		}
	case emS2CResponse_GetServerDateTime:
		{
			CMessageGetServerDateTimeResponse* pGetServerDateTimeResponse = (CMessageGetServerDateTimeResponse*)pMessage;
			time_t nServerTime = (time_t)pGetServerDateTimeResponse->DateTime;
			mCurrentServerTime = nServerTime;
			tm* pTm = gmtime(&nServerTime);
			wchar_t acTime[256] = {0};
			swprintf(acTime, sizeof(acTime)/2, _T("当前服务器时间 %d年%d月%d日 %d时:%d分:%d秒"), pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
			MessageBox(acTime);
			break;
		}
	case emS2CResponse_SetServerDateTime:
		{
			CMessageSetServerDateTimeResponse* pSetServerDateTimeResponse = (CMessageSetServerDateTimeResponse*)pMessage;
			time_t nServerTime = (time_t)pSetServerDateTimeResponse->DateTime;
			mCurrentServerTime = nServerTime;
			tm* pTm = gmtime(&nServerTime);
			wchar_t acTime[256] = {0};
			swprintf(acTime, sizeof(acTime)/2, _T("当前服务器时间 %d年%d月%d日 %d时:%d分:%d秒"), pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
			MessageBox(acTime);			
			break;
		}
	case emS2CNotify_CompileComplete:
		{
			//AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);//显示窗口
			//AfxGetApp()->m_pMainWnd->BringWindowToTop();
			//SwitchToThisWindow( AfxGetApp()->m_pMainWnd->m_hWnd, TRUE );
			MessageBox(_T("编译完成！请检查是否编译成功。"), NULL, MB_SYSTEMMODAL);
			break;
		}
	case emS2CResponse_GetServerName:
		{
			OnMsgGetServerNameResponse(pMessage);
			break;
		}
	default:
		break;
	}
}
void CRestartServerDlg::OnBnClickedSetServerTime()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bCheck = UpdateData(TRUE);
	if (!bCheck)
	{
		return;
	}
	CTime time;
	mDateTimeCtrl.GetTime(time);
	int nYear = time.GetYear();
	int nMonth = time.GetMonth();
	int nDay = time.GetDay();

	tm tTm;
	tTm.tm_year = nYear - 1900;
	tTm.tm_mon = nMonth - 1;
	tTm.tm_mday = nDay;
	tTm.tm_hour = mEditHour;
	tTm.tm_min = mEditMinute;
	tTm.tm_sec = mEditSecond;

	time_t datetime = _mkgmtime(&tTm);
	CMessageSetServerDateTimeRequest tSetServerDateTimeRequest;
	tSetServerDateTimeRequest.ID = emC2SRequest_SetServerDateTime;
	tSetServerDateTimeRequest.Size = sizeof(CMessageSetServerDateTimeRequest);
	tSetServerDateTimeRequest.DateTime = datetime;

	for (int i = 0; i < MAX_SERVER_NUM; ++i)
	{
		tSetServerDateTimeRequest.SelectServer[i] = mSelectServer[i];
	}

	CString str("重启服务器\n");
	UpdateSelectServerItemText(str);

	int nResult = send(mSockfd, (char*)&tSetServerDateTimeRequest, sizeof(CMessageSetServerDateTimeRequest), 0);
	if(-1 == nResult)
	{
		SendMsgBoxFailed();
	}
}

void CRestartServerDlg::OnBnClickedRestartServer0()
{
	CString str("重启服务器\n");
	UpdateSelectServerItemText(str);
	OnClickedRestartServer();
}


void CRestartServerDlg::OnBnClickedStopServer0()
{
	OnClickedStopServer();
}


// 重启服务器事件响应核心处理
void CRestartServerDlg::OnClickedRestartServer()
{
	if (mCurrRestartNum > 0)
	{
		MessageBox(_T("正在重启服务器中，请耐心等待!"));
		return;
	}

	CMessageRestartServerRequest tRestartServerRequest;
	tRestartServerRequest.ID = emC2SRequest_RestartServer;
	tRestartServerRequest.Size = sizeof(CMessageRestartServerRequest);

	mCurrRestartNum = 0;
	for (int i = 0; i < MAX_SERVER_NUM; ++i)
	{
		tRestartServerRequest.SelectServer[i] = mSelectServer[i];

		if (mSelectServer[i])
		{
			++mCurrRestartNum;
		}

	}

	int nResult = send(mSockfd, (char*)&tRestartServerRequest, sizeof(CMessageRestartServerRequest), 0);
	if(-1 == nResult)
	{
		SendMsgBoxFailed();
	}
}

// 停止服务器事件响应核心处理
void CRestartServerDlg::OnClickedStopServer()
{
	if (mCurrRestartNum > 0)
	{
		MessageBox(_T("正在重启服务器中，请耐心等待!"));
		return;
	}

	// TODO: 在此添加控件通知处理程序代码
	CMessageStopServerRequest tStopServerRequest;
	tStopServerRequest.ID = emC2SRequest_StopServer;
	tStopServerRequest.Size = sizeof(CMessageStopServerRequest);

	for (int i = 0; i < MAX_SERVER_NUM; ++i)
	{
		tStopServerRequest.SelectServer[i] = mSelectServer[i];
	}

	int nResult = send(mSockfd, (char*)&tStopServerRequest, sizeof(CMessageStopServerRequest), 0);
	if(-1 == nResult)
	{
		SendMsgBoxFailed();
	}
	CString str("关闭服务器");
	UpdateSelectServerItemText(str);
}


void CRestartServerDlg::SendGetServerNameRequest()
{
	if ( mbGetServerName )
	{
		return;
	}

	CMessageGetServerNameRequest tGetServerNameRequest;
	tGetServerNameRequest.ID = emC2SRequest_GetServerName;
	tGetServerNameRequest.Size = sizeof(CMessageGetServerNameRequest);
	int nResult = send(mSockfd, (char*)&tGetServerNameRequest, sizeof(CMessageGetServerNameRequest), 0);
	if(-1 == nResult)
	{
		//SendMsgBoxFailed();
	}
}

void CRestartServerDlg::OnMsgGetServerNameResponse(CMessage* pMessage)
{
	if ( pMessage == NULL )
	{
		return;
	}
	CMessageGetServerNameResponse* tpMsgResponse = (CMessageGetServerNameResponse*)pMessage;

	//wchar_t szWIP_Svr1[256] = {0};

	int tServerNameID[] = { IDC_NAME_SERVER0, IDC_NAME_SERVER1, IDC_NAME_SERVER2, IDC_NAME_SERVER3, IDC_NAME_SERVER4, IDC_NAME_SERVER5,
							IDC_NAME_SERVER6, IDC_NAME_SERVER7, IDC_NAME_SERVER8, IDC_NAME_SERVER9, IDC_NAME_SERVER10, IDC_NAME_SERVER11};
	int tServerID[] = { IDC_SERVER0, IDC_SERVER1, IDC_SERVER2, IDC_SERVER3, IDC_SERVER4, IDC_SERVER5, IDC_SERVER6, IDC_SERVER7, IDC_SERVER8, IDC_SERVER9, IDC_SERVER10, IDC_SERVER11 };
	int tSelectServerID[] = { IDC_SELECT_SERVER0, IDC_SELECT_SERVER1, IDC_SELECT_SERVER2, IDC_SELECT_SERVER3, IDC_SELECT_SERVER4, IDC_SELECT_SERVER5,
		IDC_SELECT_SERVER6, IDC_SELECT_SERVER7, IDC_SELECT_SERVER8, IDC_SELECT_SERVER9, IDC_SELECT_SERVER10, IDC_SELECT_SERVER11 };
	const int tServerNum = sizeof(tServerID) / sizeof(int);
	
	GetDlgItem( IDC_GETSERVERLIST )->ShowWindow(FALSE);

	int nServerCount = 0;
	for (int i = 0; i < MAX_SERVER_NUM && i < tServerNum; ++i)
	{
		int tIDC_SERVER_NAME = tServerNameID[i];
		int tIDC_SERVER = tServerID[i];
		int tIDC_SELECT_SERVER = tSelectServerID[i];

		char tSvrNameChar[MAX_STR_LEN] = { 0 };
		strncpy( tSvrNameChar, tpMsgResponse->mServerName[i], sizeof(tSvrNameChar) - 1);
		tSvrNameChar[MAX_STR_LEN - 1] = '\0';
		int nSvrNameLen = strlen(tSvrNameChar);

		std::string tSvrNameStr( tSvrNameChar );
		std::wstring tConvertNameStr = s2ws( tSvrNameStr );
		const wchar_t* tpWCharSvrName = tConvertNameStr.c_str();
		if ( tpWCharSvrName == NULL )
		{
			return;
		}
		if (nSvrNameLen == 0)
		{
			break;
		}
		GetDlgItem(tIDC_SERVER_NAME)->EnableWindow(TRUE);
		GetDlgItem(tIDC_SERVER)->EnableWindow(TRUE);
		GetDlgItem(tIDC_SELECT_SERVER)->EnableWindow(TRUE);

		GetDlgItem(tIDC_SERVER_NAME)->SetWindowText(tpWCharSvrName);

		++ nServerCount;
	}

	GetDlgItem(IDC_RESTART_SERVER0)->ShowWindow(TRUE);
	GetDlgItem(IDC_STOP_SERVER0)->ShowWindow(TRUE);

	if (nServerCount == 1)
	{
		mSelectServer[0] = TRUE;
		GetDlgItem(IDC_SELECT_SERVER0)->ShowWindow(FALSE);
		UpdateSelectServerItemText();
	}
	else
	{
		GetDlgItem( IDC_SETTIMERESTART )->SetWindowText(_T("请选择服务器!!!"));
	}

	mbGetServerName = true;
}

void CRestartServerDlg::SendMsgBoxFailed()
{
	MessageBox(_T("服务器连接失败!"));
}

// 托盘图标消息响应函数
LRESULT CRestartServerDlg::OnAddTrayIcon(WPARAM wParam,LPARAM lParam)
{

	if (wParam == IDI_ICON_SMILE)//为创建的托盘图标
	{
		switch(lParam)
		{
		case WM_LBUTTONDOWN://单击左键
			{
				flag = !flag;
				if(flag)
				{
					AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);//隐藏窗口
				}
				else
				{
					AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);//显示窗口
				}
				break;
			}
		case WM_RBUTTONDBLCLK://双击右键
			{
				AfxGetApp()->m_pMainWnd->DestroyWindow();//关闭窗口
				break;
			}
		}
	}
	return 1;
}
void CRestartServerDlg::OnBnClickedSelectServer0()
{
	// TODO: 在此添加控件通知处理程序代码
	mSelectServer[0] = !mSelectServer[0];
}

void CRestartServerDlg::OnBnClickedSelectServer1()
{
	// TODO: 在此添加控件通知处理程序代码
	mSelectServer[1] = !mSelectServer[1];
}

void CRestartServerDlg::OnBnClickedSelectServer2()
{
	// TODO: 在此添加控件通知处理程序代码
	mSelectServer[2] = !mSelectServer[2];
}

void CRestartServerDlg::OnBnClickedSelectServer3()
{
	// TODO: 在此添加控件通知处理程序代码
	mSelectServer[3] = !mSelectServer[3];
}

void CRestartServerDlg::OnBnClickedSelectServer4()
{
	// TODO: 在此添加控件通知处理程序代码
	mSelectServer[4] = !mSelectServer[4];
}

void CRestartServerDlg::OnBnClickedSelectServer5()
{
	// TODO: 在此添加控件通知处理程序代码
	mSelectServer[5] = !mSelectServer[5];
}

void CRestartServerDlg::UpdateSelectServerItemText()
{
	CString strRestartText = CString(_T("设置服务器时间时\n会重启所选择的服务器")) ;
	GetDlgItem(IDC_SETTIMERESTART)->SetWindowText(strRestartText);
	UpdateData(TRUE);
}

void CRestartServerDlg::UpdateSelectServerItemText(const CString & Str)
{
	GetDlgItem(IDC_SETTIMERESTART)->SetWindowText(Str);
	UpdateData(TRUE);
}

void CRestartServerDlg::OnMsgServerRestartResponse(CMessage * pMessage)
{
	if (pMessage == NULL)
	{
		return;
	}
	CString strResult;
	CMessageRestartServerResponse * pMsg = (CMessageRestartServerResponse *)pMessage;

	GetDlgItem(IDC_SETTIMERESTART)->GetWindowText(strResult);

	strResult += pMsg->ServerName;

	if (pMsg->Result)
	{
		strResult += _T("重启失败\n");
	}
	else
	{
		strResult += _T("重启成功\n");
	}

	UpdateSelectServerItemText(strResult);
	GetDlgItem(IDC_SETTIMERESTART)->SetWindowText(strResult);
	UpdateData(TRUE);

	--mCurrRestartNum;

	if (mCurrRestartNum <= 0)
	{
		MessageBox(strResult);
	}
}

void CRestartServerDlg::OnBnClickedSelectServer6()
{
	// TODO:  在此添加控件通知处理程序代码
	mSelectServer[6] = !mSelectServer[6];
}

void CRestartServerDlg::OnBnClickedSelectServer7()
{
	// TODO:  在此添加控件通知处理程序代码
	mSelectServer[7] = !mSelectServer[7];
}

void CRestartServerDlg::OnBnClickedSelectServer8()
{
	// TODO:  在此添加控件通知处理程序代码
	mSelectServer[8] = !mSelectServer[8];
}

void CRestartServerDlg::OnBnClickedSelectServer9()
{
	// TODO:  在此添加控件通知处理程序代码
	mSelectServer[9] = !mSelectServer[9];
}

void CRestartServerDlg::OnBnClickedSelectServer10()
{
	// TODO:  在此添加控件通知处理程序代码
	mSelectServer[10] = !mSelectServer[10];
}

void CRestartServerDlg::OnBnClickedSelectServer11()
{
	// TODO:  在此添加控件通知处理程序代码
	mSelectServer[11] = !mSelectServer[11];
}
