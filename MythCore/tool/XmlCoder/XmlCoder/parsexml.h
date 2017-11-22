#ifndef __PARSEXML_H__
#define __PARSEXML_H__
#include "commontype.h"
#include "class.h"
#include "tinyxml2.h"
using namespace tinyxml2;
class CParseXml
{
public:
	typedef vector<CClass*>		CLASS_VECTOR;

public:
	CParseXml()
	{
	}
	~CParseXml(){}

	void init()
	{
        mRootClass = NULL;
	}
public:
	// 分析Xml
	void parseXml(const char* pPath);
	// 分析Xml节点
	void parseXmlElement(XMLElement* pElement, CClass* pParent);
	// 写头文件
	void writeHeadFile(const char* pName);
	// 写类的声明
	void writeClassFile(CClass* pClass, FILE* pFile);

	// 写源文件
	void writeSourceFile(const char* pName, const char* pPath);
	// autocode don't edit
    CClass* getRootClass(){ return mRootClass;}
    void setRootClass(CClass* value){ mRootClass = value;}
	// autocode
private:
	CClass*				mRootClass;
};
#endif