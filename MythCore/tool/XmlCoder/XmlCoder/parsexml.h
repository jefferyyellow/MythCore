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
	// ����Xml
	void parseXml(const char* pPath);
	// ����Xml�ڵ�
	void parseXmlElement(XMLElement* pElement);
	// �������ֵõ���Ӧ�ı���
	CVariable* getVariable(CClass* pClass, const char* pName);
	// �������ֵõ���Ӧ����
	CClass* getClass(const char* pName);
	// дͷ�ļ�
	void writeHeadFile(const char* pName);
	// д�������
	void writeClassFile(CClass* pClass, FILE* pFile);

	// дԴ�ļ�
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