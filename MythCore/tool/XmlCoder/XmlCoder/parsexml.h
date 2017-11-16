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
        mCurClass = NULL;
	}
public:
	// 分析Xml
	void parseXml(const char* pPath);
	// 分析Xml节点
	void parseXmlElement(XMLElement* pElement);
	// 根据名字得到对应的变量
	CVariable* getVariable(CClass* pClass, const char* pName);
	// 根据名字得到对应的类
	CClass* getClass(const char* pName);
	// 写头文件
	void writeHeadFile(const char* pName);
	// 写类的声明
	void writeClassFile(CClass* pClass, FILE* pFile);

	// 写源文件
	void writeSourceFile(const char* pName);

	// autocode don't edit
    CLASS_VECTOR& getClassList(){ return mvecClassList;}

    CClass* getCurClass(){ return mCurClass;}
    void setCurClass(CClass* value){ mCurClass = value;}
	// autocode
private:
	CLASS_VECTOR		mvecClassList;
	CClass*				mCurClass;
};
#endif