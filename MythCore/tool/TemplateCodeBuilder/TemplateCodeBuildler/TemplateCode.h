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
	emTplField_Function = 7 //����
};

class CFieldInfo
{
public:
	int mType;
	int mDimensional;
	string mFieldType;
	string mTypeKeyWord;
	string mFieldName;
	string mKeyWord;  //�ؼ��֣����������ļ�ʱ����
};

typedef map<string, CTemplateInfo> ClassMap;
//��������
class CTemplateInfo
{
public:
	string mClassName;  //����
	string mParentName; //����
	string mParentKeyNameWord; //����Ĺؼ��֣����������ļ�ʱ����
	string mKeyNameWord; //�����Ĺؼ��֣����������ļ�ʱ����
	vector<CFieldInfo> mFiledInfoList;  //�ֶ��б�
	vector<CFieldInfo> mNotInTemplateEdit; //����ģ��༭�����ֶ���Ϣ
	ClassMap mNestClassMap;  //Ƕ����
};

typedef vector<CTemplateInfo> CTemplateData;

enum
{
	// ÿ���ļ���ȡһ���õ�BUFF��С
	emLine_Max_Char = 1024,
};

//����.h
class CParseCodeFile
{
public:
	bool BeginParseFile(string & rStrPath, CTemplateData & rTemplateData);
	bool ParseClass(CTemplateInfo & rTemplateInfo, string & rCurStrLine, ifstream& InFile);
	string	GetFirstWord(string& strSource);
	string	GetFieldKeyWord(string& strFieldName);
	string	GetClassNameKeyWord(string& strClassName);
};

//����cpp
class CBuildCodeFile
{
public:
	bool BeginBuildCode(const string & rStrPath, string & rClassName,CTemplateData & rTemplateData); //����Ŀ¼������������Ҫ���ɵ�����
	string GetLowString(const string &strSrc);
	void WriteFieldToInitCpp(vector<CFieldInfo> & rFieldInfoList, vector<string> & rWriteList);
	void WriteClassFieldSetFromPB(CTemplateInfo & rTemplateInfo, const string & pbData, const string & strPrefix, const string & strPreData,vector<string> & rWriteList);
	void WriteClassFieldCreateFromPB(CTemplateInfo & rTemplateInfo, const string & pbData, const string & strPrefix, const string & strPreData, vector<string> & rWriteList);
	void WriteClassFieldPB(CTemplateInfo & rTemplateInfo, vector<string> & rWriteList);
	void WriteFile(const string & rStrPath, vector<string> & rWriteList);
	string Int2String(int nValue);
public:
};