
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
	/// 增加文件节点
	void AddFileItem(tinyxml2::XMLDocument& tDocument, CString strFileName);
	void AddFileItem(CString strType, CString strName, CString strFileName);
	/// 文件节点树叶子节点排序
	void FileViewSort(HTREEITEM hParentItem);
	/// 填充文件树节点
	void FillFileView();
	/// 通过名字搜索根节点的子节点
	HTREEITEM GetRootChildItem(CString strItemName);
	/// 展开所有的节点
	void ExtendAllItem(HTREEITEM hItem);
	/// 更新任务ID对应的任务类型和任务名字
	void UpdateTreeItem(CString& strTaskID, CString& strTaskType, CString& strTaskName);
	/// 通过名字找树节点的子节点
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
	/// 打开选择的文件
	void OpenSelectFile();
	/// 从xml里面得到文件的类型和名字
	void GetTaskTypeAndName(tinyxml2::XMLDocument& rDocument, CString& rStrType, CString& rStrName);
	/// 拷贝的树节点项的文本 
	CString		mCopyItemString;
};

