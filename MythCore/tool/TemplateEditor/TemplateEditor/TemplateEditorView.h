// TemplateEditorView.h : CTemplateEditorView ��Ľӿ�
//


#pragma once

class CTemplateEditorDoc;
class CTemplateInfo;
class CTemplateEditorView : public CListView
{
protected: // �������л�����
	CTemplateEditorView();
	DECLARE_DYNCREATE(CTemplateEditorView)

// ����
public:
	CTemplateEditorDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CTemplateEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	void	Update(CTemplateFieldData* pTemplateFieldData);

protected:

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // TemplateEditorView.cpp �еĵ��԰汾
inline CTemplateEditorDoc* CTemplateEditorView::GetDocument() const
   { return reinterpret_cast<CTemplateEditorDoc*>(m_pDocument); }
#endif

