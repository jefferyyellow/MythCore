
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
};
class CGridData
{
public:
	EmGridDataType	mDataType;
	void*			mData;
};

class CTaskEditorView : public CView
{
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

	CGridCtrl*	mMainGrid;
	CGridCtrl*	mCondGrid;
	int			mCondSelectRow;
	int			mDiagSelectRow;

	CGridCtrl*	mDiagGrid;
	virtual BOOL DestroyWindow();

	void OnCondGridClickDown(NMHDR* pNMHDR, LRESULT* pResult);
	void OnComboSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	void OnStartEdit(NMHDR* pNMHDR, LRESULT* pResult);
	void AddCondRow(CGridCtrl* pGridCtrl, int nRowNum, CTaskMainNode* pMainNode, wstring& strCondType, CStringArray& strParaValue, int nParamNum);
	void SetCondParam(CGridCtrl* pGridCtrl, int nRowNum, TASK_NODE_LIST& rNodeList, CStringArray& strParaValue, int nParamNum);
	CGridCellCombo* AddComboBox(CGridCtrl* pGridCtrl, OPTION_LIST& rOptionList, int nRowNum, int nColumnNum, wstring& strDefaultValue, int& rOptionIndex);

	void OnDiagGridClickDown(NMHDR* pNMHDR, LRESULT* pResult);	
	void SaveToXml();
};

#ifndef _DEBUG  // TaskEditorView.cpp �еĵ��԰汾
inline CTaskEditorDoc* CTaskEditorView::GetDocument() const
   { return reinterpret_cast<CTaskEditorDoc*>(m_pDocument); }
#endif

