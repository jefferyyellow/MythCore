
// TaskEditorView.h : CTaskEditorView 类的接口
//

#pragma once
#include "GridCtrl.h"
class CTaskEditorDoc;
class CTaskEditorView : public CView
{
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

	CGridCtrl* m_pGrid;
	virtual BOOL DestroyWindow();
};

#ifndef _DEBUG  // TaskEditorView.cpp 中的调试版本
inline CTaskEditorDoc* CTaskEditorView::GetDocument() const
   { return reinterpret_cast<CTaskEditorDoc*>(m_pDocument); }
#endif

