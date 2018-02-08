
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
// ����
public:
	CFileView();

	void AdjustLayout();
	void OnChangeVisualStyle();
public:
	/// �����ļ��ڵ�
	void AddFileItem(tinyxml2::XMLDocument& tDocument, CString strFileName);
	void AddFileItem(CString strType, CString strName, CString strFileName);
	/// �ļ��ڵ���Ҷ�ӽڵ�����
	void FileViewSort(HTREEITEM hParentItem);
	/// ����ļ����ڵ�
	void FillFileView();
	/// ͨ�������������ڵ���ӽڵ�
	HTREEITEM GetRootChildItem(CString strItemName);
	/// չ�����еĽڵ�
	void ExtendAllItem(HTREEITEM hItem);
	/// ��������ID��Ӧ���������ͺ���������
	void UpdateTreeItem(CString& strTaskID, CString& strTaskType, CString& strTaskName);
	/// ͨ�����������ڵ���ӽڵ�
	HTREEITEM FindTreeItem(HTREEITEM hTreeItem, CString& strItemName);
// ����
protected:

	CViewTree m_wndFileView;
	CImageList m_FileViewImages;
	CFileViewToolBar m_wndToolBar;

// ʵ��
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
	/// ��ѡ����ļ�
	void OpenSelectFile();
	/// ��xml����õ��ļ������ͺ�����
	void GetTaskTypeAndName(tinyxml2::XMLDocument& rDocument, CString& rStrType, CString& rStrName);
	/// ���������ڵ�����ı� 
	CString		mCopyItemString;
};

