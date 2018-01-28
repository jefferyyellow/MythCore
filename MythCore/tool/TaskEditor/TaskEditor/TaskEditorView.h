
// TaskEditorView.h : CTaskEditorView ��Ľӿ�
//

#pragma once
#include "GridCtrl.h"
#include "TaskTemplate.h"
class CTaskEditorDoc;
class CTaskMainNode;
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

	CGridCtrl*	m_pGrid;
	CGridCtrl*	m_pCondGrid;
	int			mCondSelectRow;

	CGridCtrl*	m_pDiagGrid;
	virtual BOOL DestroyWindow();

	void OnCondGridClickDown(NMHDR* pNMHDR, LRESULT* pResult);
	void OnCondEndEdit(NMHDR* pNMHDR, LRESULT* pResult);
	void AddCondRow(int nRowNum, CTaskMainNode* pMainNode);
	void SetCondParam(int nRowNum, TASK_NODE_LIST& rNodeList);

	void OnDiagGridClickDown(NMHDR* pNMHDR, LRESULT* pResult);
};

#ifndef _DEBUG  // TaskEditorView.cpp �еĵ��԰汾
inline CTaskEditorDoc* CTaskEditorView::GetDocument() const
   { return reinterpret_cast<CTaskEditorDoc*>(m_pDocument); }
#endif

