
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
	/// 填充选项界面的树节点
	void FillTempOptionView();
	/// 增加选项节点
	void AddOptionItem(HTREEITEM hTreeItem, COptionNameItem* pTempName);
	/// 通过名字得到根节点的子节点
	HTREEITEM GetRootChildItem(CString strItemName);
	/// 显示选项界面
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
	/// 双击树节点，将对应的节点应用到选择的Cell上
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
	/// 从哪个窗口，那个控件的哪行，哪列导致该窗口的现实
	HWND		mHwnd;
	CGridCtrl*	mGridCtrl;
	int			mRowNum;
	int			mColumnNum;
};

