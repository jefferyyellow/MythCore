
#pragma once

#include "ViewTree.h"
#include "commondefine.h"

class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFileView : public CDockablePane
{
// 构造
public:
	CFileView();

	void AdjustLayout();
	void OnChangeVisualStyle();
public:
	void AddFileItem(tinyxml2::XMLDocument& tDocument, CString strFileName);
	void AddFileItem(CString strType, CString strName, CString strFileName);
	void FileViewSort(HTREEITEM hParentItem);
	void FillFileView();
	HTREEITEM GetRootChildItem(CString strItemName);
	void ExtendAllItem(HTREEITEM hItem);
	void UpdateTreeItem(CString& strTaskID, CString& strTaskType, CString& strTaskName);
	HTREEITEM FindTreeItem(HTREEITEM hTreeItem, CString& strItemName);
// 特性
protected:

	CViewTree m_wndFileView;
	CImageList m_FileViewImages;
	CFileViewToolBar m_wndToolBar;

// 实现
public:
	virtual ~CFileView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnFileOpen();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditClear();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnDblClkFileView(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
private:
	void OpenSelectFile();
	void GetTaskTypeAndName(tinyxml2::XMLDocument& rDocument, CString& rStrType, CString& rStrName);
	CString		mCopyItemString;
};

