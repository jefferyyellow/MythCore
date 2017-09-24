// ProgStatusBar.cpp : 实现文件
//

#include "stdafx.h"
#include "TemplateEditor.h"
#include "ProgStatusBar.h"


// CProgStatusBar

IMPLEMENT_DYNAMIC(CProgStatusBar, CStatusBar)

CProgStatusBar::CProgStatusBar()
{

}

CProgStatusBar::~CProgStatusBar()
{
}


BEGIN_MESSAGE_MAP(CProgStatusBar, CStatusBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CProgStatusBar 消息处理程序



int CProgStatusBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//lpCreateStruct->style |= WS_CLIPCHILDREN;

	if (CStatusBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	VERIFY(m_wndProgBar.Create(WS_CHILD | WS_VISIBLE, CRect(), this, 1));
	// TODO:  在此添加您专用的创建代码
	m_wndProgBar.SetRange(0,100);

	return 0;
}

void CProgStatusBar::OnSize(UINT nType, int cx, int cy)
{
	CStatusBar::OnSize(nType, cx, cy);

	//// TODO: 在此处添加消息处理程序代码
	AdjustProgressPosition();
}

void CProgStatusBar::AdjustProgressPosition()
{
	//// TODO: 在此处添加消息处理程序代码
	CRect rc;
	GetItemRect(2, &rc);//获取状态条的第一个窗口的尺寸；
	m_wndProgBar.MoveWindow(&rc,TRUE);//移动进度条到状态条的第一个窗口；
}

void CProgStatusBar::OnProgress(int nProgressValue)
{
	m_wndProgBar.SetPos(nProgressValue);
}