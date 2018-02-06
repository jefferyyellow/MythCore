
// TaskEditorView.h : CTaskEditorView 类的接口
//

#pragma once
#include "GridCtrl.h"
#include "TaskTemplate.h"
class CTaskEditorDoc;
class CTaskMainNode;
class CGridCellCombo;
enum EmGridDataType
{
	emDataType_None				= 0,		// 无
	emDataType_MainNode			= 1,		// 主节点
	emDataType_DataNode			= 2,		// 条件节点
	emDataType_ParamNode		= 3,		// 参数节点
};
class CGridData
{
public:
	EmGridDataType	mDataType;
	void*			mData;
};

class CTaskEditorView : public CView
{
public:
	typedef vector<CGridData*> GRID_DATA_LIST;
protected: // 仅从序列化创建
	CTaskEditorView();
	DECLARE_DYNCREATE(CTaskEditorView)

// 特性
public:
	CTaskEditorDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CTaskEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	void InitialMainNode();
	void InitialCondNode();
	void InitialDiagNode();
	void InitCondDiagNode(CGridCtrl*& pGridCtrl, int nGridCtrlID, TASK_NODE_LIST& rNodeList, int nColumnNum);
	CGridData* NewGridData();
	void DeleteGridData(CGridData* pGridData);


	CGridCtrl*	mMainGrid;
	CGridCtrl*	mCondGrid;
	CGridCtrl*	mDiagGrid;
	int			mCondSelectRow;
	int			mDiagSelectRow;
	GRID_DATA_LIST mGridDataList;

	virtual BOOL DestroyWindow();

	void OnCondGridClickDown(NMHDR* pNMHDR, LRESULT* pResult);
	void OnDiagGridClickDown(NMHDR* pNMHDR, LRESULT* pResult);
	void CondDiagGridButtonDown(CGridCtrl* pGridCtrl, int nRow, int nColumn, int nOldSelect, int nParamNum);
	void OnComboSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	void OnCondComboDropDown(NMHDR* pNMHDR, LRESULT* pResult);
	void OnCondStartEdit(NMHDR* pNMHDR, LRESULT* pResult);
	void OnMainStartEdit(NMHDR* pNMHDR, LRESULT* pResult);
	void OnMainEndEdit(NMHDR* pNMHDR, LRESULT* pResult);
	void MainCondStartEdit(CGridCtrl* pGridCtrl, int nRow, int nColumn);
	void AddCondRow(CGridCtrl* pGridCtrl, int nRowNum, CTaskMainNode* pMainNode, wstring& strCondType, CStringArray& strParaValue, int nParamNum);
	void SetCondParam(CGridCtrl* pGridCtrl, int nRowNum, TASK_NODE_LIST& rNodeList, CStringArray& strParaValue, int nParamNum);
	CGridCellCombo* AddComboBox(CGridCtrl* pGridCtrl, CTaskMainNode* pMainNode, int nRowNum, int nColumnNum, wstring& strDefaultValue, int& rOptionIndex, int nStyle);
	CString GetMainGridText(CString strName);

	void SaveToXml();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // TaskEditorView.cpp 中的调试版本
inline CTaskEditorDoc* CTaskEditorView::GetDocument() const
   { return reinterpret_cast<CTaskEditorDoc*>(m_pDocument); }
#endif

