// ProgStatusBar.cpp : ʵ���ļ�
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



// CProgStatusBar ��Ϣ�������



int CProgStatusBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//lpCreateStruct->style |= WS_CLIPCHILDREN;

	if (CStatusBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	VERIFY(m_wndProgBar.Create(WS_CHILD | WS_VISIBLE, CRect(), this, 1));
	// TODO:  �ڴ������ר�õĴ�������
	m_wndProgBar.SetRange(0,100);

	return 0;
}

void CProgStatusBar::OnSize(UINT nType, int cx, int cy)
{
	CStatusBar::OnSize(nType, cx, cy);

	//// TODO: �ڴ˴������Ϣ����������
	AdjustProgressPosition();
}

void CProgStatusBar::AdjustProgressPosition()
{
	//// TODO: �ڴ˴������Ϣ����������
	CRect rc;
	GetItemRect(2, &rc);//��ȡ״̬���ĵ�һ�����ڵĳߴ磻
	m_wndProgBar.MoveWindow(&rc,TRUE);//�ƶ���������״̬���ĵ�һ�����ڣ�
}

void CProgStatusBar::OnProgress(int nProgressValue)
{
	m_wndProgBar.SetPos(nProgressValue);
}