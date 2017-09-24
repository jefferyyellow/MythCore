#pragma once


// CProgStatusBar

class CProgStatusBar : public CStatusBar
{
	DECLARE_DYNAMIC(CProgStatusBar)

public:
	CProgStatusBar();
	virtual ~CProgStatusBar();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CProgressCtrl	m_wndProgBar;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	void OnProgress(int nProgressValue);
	void AdjustProgressPosition();
};


