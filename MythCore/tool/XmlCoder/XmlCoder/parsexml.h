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
	// ����Xml
	void parseXml(const char* pPath);
	// ����Xml�ڵ�
	void parseXmlElement(XMLElement* pElement, CClass* pParent);
	// �������ֵõ���Ӧ�ı���
	CVariable* getVariable(CClass* pClass, const char* pName);
	// �������ֵõ���Ӧ����
	CClass* getSubClass(CClass* pClass, const char* pName);
	// дͷ�ļ�
	void writeHeadFile(const char* pName);
	// д�������
	void writeClassFile(CClass* pClass, FILE* pFile);

	// дԴ�ļ�
	void writeSourceFile(const char* pName);

	// autocode don't edit
    CClass* getRootClass(){ return mRootClass;}
    void setRootClass(CClass* value){ mRootClass = value;}
	// autocode
private:
	CClass*				mRootClass;
};
#endif