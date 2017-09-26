#pragma once

#include <vector>
#include <map>
using namespace std;

class CTemplateInfo;

#define ONE_TAB "\t"
#define TWO_TAB "\t\t"

enum TmFieldType
{
	emTplField_None = 0,
	emTplField_Int = 1, //int 
	emTplField_IntArray = 2,// int array
	emTplField_String = 3, //String
	emTplField_StringArray = 4, //String array
	emTplField_Class = 5, //class 
	emTplField_ClassArray = 6,// class array
	emTplField_Function = 7 //函数
};

class CFieldInfo
{
public:
	int mType;
	int mDimensional;
	string mFieldType;
	string mTypeKeyWord;
	string mFieldName;
	string mKeyWord;  //关键字，用于生成文件时命名
};

typedef map<string, CTemplateInfo> ClassMap;
//基本数据
class CTemplateInfo
{
public:
	string mClassName;  //类名
	string mParentName; //父类
	string mParentKeyNameWord; //父类的关键字，用于生成文件时命名
	string mKeyNameWord; //类名的关键字，用于生成文件时命名
	vector<CFieldInfo> mFiledInfoList;  //字段列表
	vector<CFieldInfo> mNotInTemplateEdit; //不在模板编辑器的字段信息
	ClassMap mNestClassMap;  //嵌套类
};

typedef vector<CTemplateInfo> CTemplateData;

enum
{
	// 每从文件中取一行用的BUFF大小
	emLine_Max_Char = 1024,
};

//分析.h
class CParseCodeFile
{
public:
	bool BeginParseFile(string & rStrPath, CTemplateData & rTemplateData);
	bool ParseClass(CTemplateInfo & rTemplateInfo, string & rCurStrLine, ifstream& InFile);
	string	GetFirstWord(string& strSource);
	string	GetFieldKeyWord(string& strFieldName);
	string	GetClassNameKeyWord(string& strClassName);
};

//生成cpp
class CBuildCodeFile
{
public:
	bool BeginBuildCode(const string & rStrPath, string & rClassName,CTemplateData & rTemplateData); //生成目录，生成类名，要生成的数据
	string GetLowString(const string &strSrc);
	void WriteFieldToInitCpp(vector<CFieldInfo> & rFieldInfoList, vector<string> & rWriteList);
	void WriteClassFieldSetFromPB(CTemplateInfo & rTemplateInfo, const string & pbData, const string & strPrefix, const string & strPreData,vector<string> & rWriteList);
	void WriteClassFieldCreateFromPB(CTemplateInfo & rTemplateInfo, const string & pbData, const string & strPrefix, const string & strPreData, vector<string> & rWriteList);
	void WriteClassFieldPB(CTemplateInfo & rTemplateInfo, vector<string> & rWriteList);
	void WriteFile(const string & rStrPath, vector<string> & rWriteList);
	string Int2String(int nValue);
public:
};