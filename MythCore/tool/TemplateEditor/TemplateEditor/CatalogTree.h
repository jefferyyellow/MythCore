#pragma once
#include "afxcmn.h"
#include "XTreeCtrl.h"
#include "resource.h"
/********************************************************************
	created:	21/12/2011   16:32
	filename: 	CatalogTree.h
	author:		jeffery

	purpose:	模板数目树类
*********************************************************************/
// 树结点数据类
class CTreeItemData
{
public:
	CTreeItemData()
	{
		m_ItemType	= TEMP_ITEM_NULL;
		m_pData		= NULL;
	}

public:
	TmTempItemType	GetItemType(){return m_ItemType;}
	void			SetItemType(TmTempItemType type){m_ItemType = type;}
	
	void*			GetData(){return m_pData;}
	void			SetData(void* pData){m_pData = pData;}

private:
	TmTempItemType	m_ItemType;		// 当前树节点的类型
	void*			m_pData;		// 主要用于计录模板，原始模板，模板字段对应的指针
};

// CCatalogTree 对话框
class CTemplateEditorView;
class CCatalogTree : public CDialogBar
{
	DECLARE_DYNAMIC(CCatalogTree)

public:
	CCatalogTree(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCatalogTree();

// 对话框数据
	enum { IDD = IDD_CATALOGTREE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	LONG OnInitDialog ( UINT wParam, LONG lParam);

public:
	BOOL	BuildCatalogTree(TCHAR* pServerPath, HTREEITEM vParent);
	BOOL	BuildTreeItem(TCHAR* pServerPath, TCHAR* pFileName, HTREEITEM vParent, bool bIsDir);
	BOOL	BuildTemplateItem(CTemplateInfo* pNewTemplate, HTREEITEM pParentItem);
	BOOL	GetTemplatePath(HTREEITEM pTreeItem, CString& strPath);

public:
	CTreeCtrl&	GetCatalogTree(){return m_CatalogTree;}
	static DWORD WINAPI	ReadTemplateDataThreadFunc(LPVOID param);

private:
	// 目录树控件
	CTreeCtrl m_CatalogTree;
public:
	afx_msg void OnNMDblclkCatalogtree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnItemexpandingCatalogtree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickCatalogtree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCreateTemplate();
	afx_msg void OnCloneTemplate();
	afx_msg void OnCreateTemplateTable();
	afx_msg void OnOpenWithExcel();
	afx_msg void OnCreateFilter();
	afx_msg void OnChangeTemplateName();
	afx_msg void OnDeleteTemplate();
	afx_msg void OnChangeTableInfo();

public:
	static CCatalogTree* GetSingletonPtr(){return m_pInst;}
	static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	void SortTreeItem(HTREEITEM pParent);
	void CCatalogTree::FreeTree(HTREEITEM pParent);
	BOOL IsSameDataTable(HTREEITEM pSource, HTREEITEM pDest);
	void SaveFilterFile(HTREEITEM pTreeItem);

public:
	void SetReadTempDataing(BOOL bReadTempDataing){m_bReadTempDataing = bReadTempDataing;}
	BOOL GetReadTempDataing(){return m_bReadTempDataing;}

private:

private:
	CImageList	mTreeImages;
	CBitmap		mDirBitmap;
	CBitmap		mDirTBitmap;
	CBitmap		mFileBitmap;
	CBitmap		mFileCOBitmap;

	// 是否是后台读取数据中
	BOOL		m_bReadTempDataing;


	static CCatalogTree* m_pInst;
protected:
//	virtual void PostNcDestroy();
public:
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
