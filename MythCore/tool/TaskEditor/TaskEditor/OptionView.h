
#pragma once

#include "ViewTree.h"
#include "GridCtrl.h"
class COptionViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class COptionView : public CDockablePane
{
// 构造
public:
	COptionView();

	void AdjustLayout();
	void OnChangeVisualStyle();
	void FillTempOptionView();
	void AddTempOptionItem(HTREEITEM hTreeItem, COptionNameItem* pTempName);
	HTREEITEM GetRootChildItem(CString strItemName);
	void ShowOptionView(HWND hWnd, CString& strConfigName, CGridCtrl* pGridCtrl, int nRowNum, int nColumnNum);
// 特性
protected:

	CViewTree m_wndOptionView;
	CImageList m_TempViewImages;
	COptionViewToolBar m_wndToolBar;

// 实现
public:
	virtual ~COptionView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnDblClkFileView(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

public:
	HWND GetHwnd() const { return mHwnd; }
	void SetHwnd(HWND nValue) { mHwnd = nValue; }

	CGridCtrl* GetGridCtrl() const { return mGridCtrl; }
	void SetGridCtrl(CGridCtrl* nValue) { mGridCtrl = nValue; }

	int GetRowNum() const { return mRowNum; }
	void SetRowNum(int nValue) { mRowNum = nValue; }

	int GetColumnNum() const { return mColumnNum; }
	void SetColumnNum(int nValue) { mColumnNum = nValue; }

private:
	HWND		mHwnd;
	CGridCtrl*	mGridCtrl;
	int			mRowNum;
	int			mColumnNum;
};

