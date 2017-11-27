#ifndef __CLASS_H__
#define __CLASS_H__

#include "commontype.h"
#include "variable.h"
#include "tinyxml2.h"
using namespace tinyxml2;

class CXmlElement
{
public:
	CXmlElement()
	{
		mCount = 0;
		mAlreadyWrite = false;
		mRoot = false;
	}

	~CXmlElement()
	{}


	bool getAlreadyWrite() const { return mAlreadyWrite; }
	void setAlreadyWrite(bool val) { mAlreadyWrite = val; }

	int getCount() const { return mCount; }
	void setCount(int val) { mCount = val; }

	string& getName() { return mName; }
	void setName(string& val) { mName = val; }

	bool getRoot() const { return mRoot; }
	void setRoot(bool val) { mRoot = val; }

private:
	string				mName;							// �ڵ������
	int					mCount;							// �ڵ����Ŀ
	bool				mAlreadyWrite;					// �Ƿ��Ѿ�д
	bool				mRoot;							// �Ƿ��Ǹ��ڵ�

};

class CClass
{
public:
	CClass()
	{
		mName[0] = '\0';
		mNeedGeneral = false;
		mCount = 0;
		mBuffer[0] = '\0';
	}
	~CClass()
	{
		clear();
	}

	void clear()
	{
		for (int i = 0; i < mVariableList.size(); ++i)
		{
			delete mVariableList[i];
		}
	}
public:
	typedef vector<CVariable*>		VARIABLE_VECTOR;
	typedef vector<CClass*>			CLASS_VECTOR;


public:
	void formatSourceFile(const char* pName, const char* pPath);
	// д����xml�ڵ�
	void formatParseXmlElement(XMLElement* pElement, XMLElement* pParentElement, CClass* pParent, int nIndentNum, bool bRoot);
	//
	void formatClass(XMLElement* pElement, XMLElement* pParentElement, bool bClass, int nIndentNum);
	// 
	void formatAttribute(XMLElement* pElement, int nIndentNum, bool bRoot);
	void formatVariable(CVariable* pVariable, XMLElement* pElement, const XMLAttribute* pAttribute, bool bRoot);
	void fromatVariableArray(CVariable* pVariable, XMLElement* pElement, const XMLAttribute* pAttribute, bool bRoot);
	// дԴ�ļ�
	void writeSourceFile(FILE* pFile);
	// �������ֵõ���Ӧ�ı���
	CVariable* getVariable(const char* pName);
	// �������ֵõ���Ӧ����
	CClass* getSubClass(const char* pName);
	// �Ѿ���ʽ���ڵ������
	CXmlElement* getXmlElement(const char* pName);
	// ����ڵ�
	void pushBack(CXmlElement& rElement)
	{
		mXmlElement.push_back(rElement);
	}
	// ��������
	void appendIndent(int nNum);

public:
	VARIABLE_VECTOR&		getVariableList(){ return mVariableList; }
	CLASS_VECTOR&			getSubClassList(){return mvecSubClassList;}

	const char* getName() { return mName; }
	void setName(const char* pName) 
	{ 
		if (NULL == pName)
		{
			return;
		}
		strncpy(mName, pName, sizeof(mName) - 1);
	}

	bool getNeedGeneral() const { return mNeedGeneral; }
	void setNeedGeneral(bool nValue) { mNeedGeneral = nValue; }

	int getCount() const { return mCount; }
	void setCount(int nValue) { mCount = nValue; }


	char* getBuffer(){return mBuffer;}
private:
	CLASS_VECTOR			mvecSubClassList;				// �����б�
	VARIABLE_VECTOR			mVariableList;					// �����б�
	char					mName[CLASS_NAME_LENGTH];		// ����
	bool					mNeedGeneral;					// �ⲿ����
	int						mCount;							// ����ά��
	char					mBuffer[MAX_WRITE_BUFFER];		// дsource file BUFF
	vector<CXmlElement>		mXmlElement;					// Xml�ڵ�
};

#endif