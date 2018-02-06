
// TaskEditorView.h : CTaskEditorView ��Ľӿ�
//

#pragma once
#include "GridCtrl.h"
#include "TaskTemplate.h"
class CTaskEditorDoc;
class CTaskMainNode;
class CGridCellCombo;
enum EmGridDataType
{
	emDataType_None				= 0,		// ��
	emDataType_MainNode			= 1,		// ���ڵ�
	emDataType_DataNode			= 2,		// �����ڵ�
	emDataType_ParamNode		= 3,		// �����ڵ�
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
protected: // �������л�����
	CTaskEditorView();
	DECLARE_DYNCREATE(CTaskEditorView)

// ����
public:
	CTaskEditorDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CTaskEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // TaskEditorView.cpp �еĵ��԰汾
inline CTaskEditorDoc* CTaskEditorView::GetDocument() const
   { return reinterpret_cast<CTaskEditorDoc*>(m_pDocument); }
#endif

