
// GameClientDlgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GameClientDlg.h"
#include "GameClientDlgDlg.h"
#include "afxdialogex.h"
#include "i18n.h"
#include "./gameclient.h"
#include "itemmodule.hxx.pb.h"
#include "propertymodule.hxx.pb.h"
using namespace Myth;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGameClientDlgDlg 对话框



CGameClientDlgDlg::CGameClientDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGameClientDlgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGameClientDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGameClientDlgDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_PLAYER_LOGIN, &CGameClientDlgDlg::OnBnClickedPlayerLogin)
	ON_BN_CLICKED(IDC_SENDGMCOMMAND, &CGameClientDlgDlg::OnBnClickedSendgmcommand)
	ON_BN_CLICKED(IDC_EQUIP, &CGameClientDlgDlg::OnBnClickedEquip)
	ON_BN_CLICKED(IDC_UNEQUIP, &CGameClientDlgDlg::OnBnClickedUnequip)
	ON_BN_CLICKED(IDC_GET_PLAYER_PROPERTY, &CGameClientDlgDlg::OnBnClickedGetPlayerProperty)
	ON_BN_CLICKED(IDC_ACCEPT_TASK, &CGameClientDlgDlg::OnBnClickedAcceptTask)
	ON_BN_CLICKED(IDC_COMPLETE_TASK, &CGameClientDlgDlg::OnBnClickedCompleteTask)
	ON_BN_CLICKED(IDC_CLEAR_LOG, &CGameClientDlgDlg::OnBnClickedClearLog)
	ON_BN_CLICKED(IDC_GET_CUMUL_PRIZE, &CGameClientDlgDlg::OnBnClickedGetCumulPrize)
END_MESSAGE_MAP()


// CGameClientDlgDlg 消息处理程序

BOOL CGameClientDlgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	mpClient = new CGameClient();
	mpClient->init();
	mpClient->setDlg(this);
	SetTimer(GAME_MAIN_TIMER, 30, NULL);
	SetTimer(GAME_HEART_BEAT, 6000, NULL);
	((CEdit*)GetDlgItem(IDC_LOG))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_LOG))->SetWindowText(_T("我不吃"));
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGameClientDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGameClientDlgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGameClientDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGameClientDlgDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == GAME_MAIN_TIMER)
	{
		mpClient->run();
	}
	else if (nIDEvent == GAME_HEART_BEAT)
	{
		mpClient->sendHeartBeatRequest();
	}
	CDialogEx::OnTimer(nIDEvent);
}


HBRUSH CGameClientDlgDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_LOG)
	{
		pDC->SetTextColor(RGB(255, 0, 0));//设置编辑框字体的颜色
		//pDC->SetBkColor(RGB(255, 255, 0));//设置字体背景颜色
		//CFont font;
		////font.CreatePointFont(100, "华文楷体");
		//pDC->SelectObject(&font);//设置字体        
		return m_brush;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CGameClientDlgDlg::OnBnClickedPlayerLogin()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strAccName;
	((CEdit*)GetDlgItem(IDC_PLAYER_ACCOUNT))->GetWindowText(strAccName);
	CString stRoleName;
	((CEdit*)GetDlgItem(IDC_ROLE_NAME))->GetWindowText(stRoleName);

	mpClient->LoginServer(strAccName.GetBuffer(), stRoleName.GetBuffer());
}


void CGameClientDlgDlg::OnBnClickedSendgmcommand()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strCommand;
	((CEdit*)GetDlgItem(IDC_GM_COMMAND))->GetWindowText(strCommand);

	mpClient->sendGMCommandRequest(strCommand.GetBuffer());
}

void CGameClientDlgDlg::DisplayLog(char* pLog)
{
	if (NULL == pLog)
	{
		return;
	}
	if (pLog[0] == '\0')
	{
		return;
	}

	mStrLog += pLog;
	mStrLog += "\r\n";
	((CEdit*)GetDlgItem(IDC_LOG))->SetWindowText(mStrLog);
	UpdateData(TRUE);
}

void CGameClientDlgDlg::OnBnClickedEquip()
{
	// TODO:  在此添加控件通知处理程序代码
	CEquipItemRequest tRequest;

	CString stEquipIndex;
	((CEdit*)GetDlgItem(IDC_EQUIP_INDEX))->GetWindowText(stEquipIndex);
	int nIndex = atoi(stEquipIndex.GetBuffer());
	tRequest.set_itemindex(nIndex);
	mpClient->sendMessage(ID_C2S_REQUEST_EQUIP_ITEM, &tRequest);
}


void CGameClientDlgDlg::OnBnClickedUnequip()
{
	// TODO:  在此添加控件通知处理程序代码
	CString stBuffer;
	((CEdit*)GetDlgItem(IDC_UNEQUIP_PART))->GetWindowText(stBuffer);
	int nEquipPart = atoi(stBuffer.GetBuffer());

	((CEdit*)GetDlgItem(IDC_UNEQUIP_INDEX))->GetWindowText(stBuffer);
	int nItemIndex = atoi(stBuffer.GetBuffer());

	CUnEquipItemRequest tRequest;
	tRequest.set_equippart(nEquipPart);
	tRequest.set_itemindex(nItemIndex);

	mpClient->sendMessage(ID_C2S_REQUEST_UNEQUIP_ITEM, &tRequest);
}


void CGameClientDlgDlg::OnBnClickedGetPlayerProperty()
{
	// TODO:  在此添加控件通知处理程序代码
	CGetPlayerPropertyRequest tRequest;
	mpClient->sendMessage(ID_C2S_REQUEST_GET_PLAYER_PROPERTY, &tRequest);
}


void CGameClientDlgDlg::OnBnClickedAcceptTask()
{
	// TODO:  在此添加控件通知处理程序代码
	CString stBuffer;
	((CEdit*)GetDlgItem(IDC_ACCEPT_TASK_ID))->GetWindowText(stBuffer);
	int nTaskID = atoi(stBuffer.GetBuffer());

	mpClient->sendAcceptTaskRequest(nTaskID);
}

void CGameClientDlgDlg::OnBnClickedCompleteTask()
{
	// TODO:  在此添加控件通知处理程序代码
	CString stBuffer;
	((CEdit*)GetDlgItem(IDC_COMPLETE_TASK_ID))->GetWindowText(stBuffer);
	int nTaskID = atoi(stBuffer.GetBuffer());

	mpClient->sendSubmitTaskRequest(nTaskID);
}


void CGameClientDlgDlg::OnBnClickedClearLog()
{
	// TODO:  在此添加控件通知处理程序代码
	mStrLog ="";
	((CEdit*)GetDlgItem(IDC_LOG))->SetWindowText(mStrLog);
	UpdateData(TRUE);
}


void CGameClientDlgDlg::OnBnClickedGetCumulPrize()
{
	// TODO:  在此添加控件通知处理程序代码
	CString stBuffer;
	((CEdit*)GetDlgItem(IDC_CUMUL_ACT_ID))->GetWindowText(stBuffer);
	int nActivityID = atoi(stBuffer.GetBuffer());
	((CEdit*)GetDlgItem(IDC_CUMUL_PRIZE_INDEX))->GetWindowText(stBuffer);
	int nIndex = atoi(stBuffer.GetBuffer());

	mpClient->sendGetCumulRechargePrizeRequest(nActivityID, nIndex);
}
