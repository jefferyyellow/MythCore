#ifndef __PARSEHEADER_H__
#define __PARSEHEADER_H__

#include "commontype.h"
#include "class.h"

class CParseHeader
{
public:
	typedef vector<char*>				VEC_FILE;
	typedef vector<CPlusClass*>			CLASS_VECTOR;
	typedef vector<CVariableDefault>	VARIABLE_DEFAULT;

public:
	CParseHeader()
	{
		mCurClass = NULL;
		mCurLineIndex = 0;
		mCurClassIndex = 0;
	}
	~CParseHeader()
	{
		clear();
	}
	void	clear();
	void	clearContent();

public:
	/// 加载默认值配置文件
	bool loadDefaultValueXml(const char* pXmlFile);
	// ******分析头文件和CPP文件******
	/// 分析头文件
	void parseHeaderFile(const char* pFilePath);
	/// 分析文件中的一行
	void parseLine(const char* pLine, int nLineLength);
	/// 分析源文件
	void parseSourceFile(const char* pFilePath);
	/// 得到第一个词
	void getFirstWord(const char* pLine, int& rStart, int nLineLength, char* pWord);
	/// 得到第一个变量
	void getFirstVariable(const char* pLine, int& rStart, int nLineLength, char* pWord);
	/// 得到变量的类型名
	bool getVariableTypeName(const char* pLine, int& rStart, int nLineLength, char* pVariableName);
	/// 处理变量的类型
	void processVariableType(char* pSrc, char* pDst, int nLength);
	/// 是否是函数部分
	bool checkFunc(const char* pLine, int nLineLength);
	/// 是否是注释部分
	bool checkComment(const char* pLine, int nLineLength);
	/// 删除前导空格
	bool deleteHeadSpace(const char* pLine, int& rStart, int nLineLength);
	/// 分析文件中写明的默认值
	bool getDefaultValue(const char* pLine, int nLineLength, char* pDefaultValue);
	/// 计算数组维数
	int calcArrayDimension(CVariable* pVariable, const char* pLine, int nLineLength);
	/// 得到数组维数长度
	void getMaxArrayLen(const char* pSrc, int nLength, char* pMaxArrayLen);


	// ******写初始化函数相关******
	/// 写头文件
	void writeHeaderFile(const char* pHeadFile);
	/// 写头文件的一行
	void writeHeaderLine(FILE* pFile, const char* pLine, int nLineLength);
	/// 写源文件
	void writeSourceFile(const char* pSrcFile);
	/// 写源文件的一行
	void writeSourceLine(FILE* pFile, const char* pLine, int nLineLength);
	/// 得到init函数的类
	CPlusClass* getClass(const char* pLine, int nLineLength);
	/// 通过名字得到类
	CPlusClass* getClassByName(char* pClassName, CPlusClass* pOutClass);
	/// 得到类型对应的默认值
	const char* getDefaultValue(const char* pVariableType);
	/// 写变量的初始化
	void writeVariableInit(FILE* pFile, int nSpaceNum);
	/// 直接写文件的内容
	void writeContent(FILE* pFile, int nStartIndex, int nEndIndex);

	// ******写get/set函数******
	/// 写变量的get/set函数
	bool writeGetSetMethod(FILE* pFile, const char* pLine, int nLineLength);
	/// 写非数组的内置类型
	void writeBuiltInType(bool bBuiltIn, CVariable* pVariable, FILE* pFile, int nSpaceNum, int &rCount);
	/// 写内置数组
	void writeBuiltInArray(bool bBuiltIn, CVariable* pVariable, FILE* pFile, int nSpaceNum, int &rCount);
	/// 写二维内置类型数组
	void writeBuiltInArray2(bool bBuiltIn, CVariable* pVariable, FILE* pFile, int nSpaceNum, int &rCount);
	/// 写字符数组
	void writeCharArray(CVariable* pVariable, FILE* pFile, int nSpaceNum, int &rCount);
	/// 写二维字符串数组 
	void writeCharArray2(CVariable* pVariable, FILE* pFile, int nSpaceNum, int &rCount);

	/// 得到第一个大写字母的位置
	const char* getFirstUpcase(const char* pLine);
	/// 写指定空格
	void writeSpace(char* pBuffer, int nSpaceNum);
	/// 写入指定空行
	void writeSpaceLine(char* pBuffer, int nSpaceLineNum);
private:
	VEC_FILE			mFileContent;
	CLASS_VECTOR		mClassList;
	VARIABLE_DEFAULT	mVariableDefaultList;

	CPlusClass*			mCurClass;
	int					mCurLineIndex;
	int					mCurClassIndex;
};
#endif