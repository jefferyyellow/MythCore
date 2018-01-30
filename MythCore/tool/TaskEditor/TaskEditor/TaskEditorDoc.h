
// TaskEditorDoc.h : CTaskEditorDoc 类的接口
//


#pragma once

#include "commondefine.h"
class CGridCtrl;
class CTaskEditorDoc : public CDocument
{
protected: // 仅从序列化创建
	CTaskEditorDoc();
	DECLARE_DYNCREATE(CTaskEditorDoc)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	//virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CTaskEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
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
