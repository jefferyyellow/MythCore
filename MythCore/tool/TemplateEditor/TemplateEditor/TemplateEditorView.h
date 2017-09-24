// TemplateEditorView.h : CTemplateEditorView 类的接口
//


#pragma once

class CTemplateEditorDoc;
class CTemplateInfo;
class CTemplateEditorView : public CListView
{
protected: // 仅从序列化创建
	CTemplateEditorView();
	DECLARE_DYNCREATE(CTemplateEditorView)

// 属性
public:
	CTemplateEditorDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CTemplateEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	void	Update(CTemplateFieldData* pTemplateFieldData);

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

private:
	CImageList	m_ImageList;
	//CFont		m_Font; 
	
public:
	CTemplateFieldData* GetCurrTempData(){return m_CurrTempData;}
	string GetTypeString(string strType, string strOldValue);
	string GetComboBoxString(string strType, int nOldValue);
	string GetMutiString(string strType, int nOldValue);

private:
	CTemplateFieldData* m_CurrTempData;
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // TemplateEditorView.cpp 中的调试版本
inline CTemplateEditorDoc* CTemplateEditorView::GetDocument() const
   { return reinterpret_cast<CTemplateEditorDoc*>(m_pDocument); }
#endif

