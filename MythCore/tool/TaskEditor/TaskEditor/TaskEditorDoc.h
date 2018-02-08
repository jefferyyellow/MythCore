
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
	/// 打开任务文档
	void	OpenDocument();
	/// 从XML中加载任务配置
	void	LoadFromXml(tinyxml2::XMLDocument& tDocument, bool bCopy);
	/// 将任务保存到xml文件中去
	void	SaveToXml(tinyxml2::XMLDocument& tDocument);
	/// 保持主节点
	XMLElement* SaveMainNode(CGridCtrl* pGridCtrl, int nRowNum, int nColumnNum,
		tinyxml2::XMLDocument& tDocument, XMLElement* pParentElem, bool bAttribute);
	/// 保存数据节点
	void	SaveDataNode(CGridCtrl* pGridCtrl, int nRowNum, int nColumnNum,
		tinyxml2::XMLDocument& tDocument, XMLElement* pParentElem, int nParamNum);
	/// 加载主节点
	XMLElement*	LoadMainNode(CGridCtrl* pGridCtrl, int nRowNum, int nColumnNum,
		tinyxml2::XMLDocument& tDocument, XMLElement* pParentElem, bool bAttribute);
	/// 加载数据节点
	int		LoadDataNode(CGridCtrl* pGridCtrl, int nRowNum, int nColumnNum,
		tinyxml2::XMLDocument& tDocument, XMLElement* pParentElem, int nParamNum);
	/// 拷贝老的粘贴到一个新的界面上
	void	PasteNew(LPCTSTR pStrName);
private:
};
