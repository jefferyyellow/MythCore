// TemplateEditorDoc.h : CTemplateEditorDoc ��Ľӿ�
//


#pragma once


class CTemplateEditorDoc : public CDocument
{
protected: // �������л�����
	CTemplateEditorDoc();
	DECLARE_DYNCREATE(CTemplateEditorDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CTemplateEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


