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
	string				mName;							// 节点的名字
	int					mCount;							// 节点的数目
	bool				mAlreadyWrite;					// 是否已经写
	bool				mRoot;							// 是否是根节点

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
	// 写分析xml节点
	void formatParseXmlElement(XMLElement* pElement, XMLElement* pParentElement, CClass* pParent, int nIndentNum, bool bRoot);
	//
	void formatClass(XMLElement* pElement, XMLElement* pParentElement, bool bClass, int nIndentNum);
	// 
	void formatAttribute(XMLElement* pElement, int nIndentNum, bool bRoot);
	void formatVariable(CVariable* pVariable, XMLElement* pElement, const XMLAttribute* pAttribute, bool bRoot);
	void fromatVariableArray(CVariable* pVariable, XMLElement* pElement, const XMLAttribute* pAttribute, bool bRoot);
	// 写源文件
	void writeSourceFile(FILE* pFile);
	// 根据名字得到对应的变量
	CVariable* getVariable(const char* pName);
	// 根据名字得到对应的类
	CClass* getSubClass(const char* pName);
	// 已经格式化节点的名字
	CXmlElement* getXmlElement(const char* pName);
	// 加入节点
	void pushBack(CXmlElement& rElement)
	{
		mXmlElement.push_back(rElement);
	}
	// 增加缩进
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
	CLASS_VECTOR			mvecSubClassList;				// 子类列表
	VARIABLE_VECTOR			mVariableList;					// 变量列表
	char					mName[CLASS_NAME_LENGTH];		// 类名
	bool					mNeedGeneral;					// 外部引用
	int						mCount;							// 数组维数
	char					mBuffer[MAX_WRITE_BUFFER];		// 写source file BUFF
	vector<CXmlElement>		mXmlElement;					// Xml节点
};

#endif