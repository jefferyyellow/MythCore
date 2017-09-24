#pragma once
#include "afxcmn.h"


// CTempPathDialog dialog

class CTempPathDialog : public CDialog
{
	DECLARE_DYNAMIC(CTempPathDialog)

public:
	CTempPathDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTempPathDialog();

// Dialog Data
	enum { IDD = IDD_TEMPPATHDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	CTreeCtrl m_TempPathTree;
	CImageList	mTreeImages;
	CBitmap		mDirBitmap;
	CBitmap		mDirTBitmap;
	CBitmap		mFileBitmap;
	CBitmap		mFileCOBitmap;

public:
	virtual BOOL OnInitDialog();

public:
	void CopyTree(CTreeCtrl& rSrcTree, CTreeCtrl& rDstTree);
	void CopyTreeItem(CTreeCtrl& rSrcTree, HTREEITEM pSrcItem, CTreeCtrl& rDstTree, HTREEITEM pDstParentItem);
public:
	afx_msg void OnBnClickedOk();
};
