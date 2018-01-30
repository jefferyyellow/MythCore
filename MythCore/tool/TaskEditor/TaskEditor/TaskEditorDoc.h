
// TaskEditorDoc.h : CTaskEditorDoc ��Ľӿ�
//


#pragma once

#include "commondefine.h"
class CGridCtrl;
class CTaskEditorDoc : public CDocument
{
protected: // �������л�����
	CTaskEditorDoc();
	DECLARE_DYNCREATE(CTaskEditorDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	//virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CTaskEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);

public:
	void	OpenDocument();
	XMLElement* SaveMainNode(CGridCtrl* pGridCtrl, int nRowNum, int nColumnNum,
		tinyxml2::XMLDocument& tDocument, XMLElement* pParentElem, bool bAttribute);
	void	SaveDataNode(CGridCtrl* pGridCtrl, int nRowNum, int nColumnNum,
		tinyxml2::XMLDocument& tDocument, XMLElement* pParentElem, int nParamNum);

	XMLElement*	LoadMainNode(CGridCtrl* pGridCtrl, int nRowNum, int nColumnNum,
		tinyxml2::XMLDocument& tDocument, XMLElement* pParentElem, bool bAttribute);

	int		LoadDataNode(CGridCtrl* pGridCtrl, int nRowNum, int nColumnNum,
		tinyxml2::XMLDocument& tDocument, XMLElement* pParentElem, int nParamNum);

private:
	CString	mOpenFilePath;
};
