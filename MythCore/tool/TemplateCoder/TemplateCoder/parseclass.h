#ifndef __PARSECLASS_H__
#define __PARSECLASS_H__
#include "commontype.h"
#include "class.h"

class CBuildInTypeToPB
{
public:
	CBuildInTypeToPB()
	{
		mBuildInType[0] = '\0';
		mPBType[0] = '\0';
	}
	~CBuildInTypeToPB(){}

	const char* getBuildInType() { return mBuildInType; }
	void setBuildInType(const char* pBuildInType)
	{
		if (NULL == pBuildInType)
		{
			return;
		}

		strncpy(mBuildInType, pBuildInType, sizeof(mBuildInType)-1);
	}

	const char* getPBType() { return mPBType; }
	void setPBType(const char* pPBType)
	{
		if (NULL == pPBType)
		{
			return;
		}
		strncpy(mPBType, pPBType, sizeof(mPBType)-1);
	}

private:
	char 			mBuildInType[MAX_NAME_LENGTH];
	char			mPBType[MAX_NAME_LENGTH];
};


class CParseClass
{
public:
	typedef vector<char*>				VEC_FILE;
	typedef vector<CBuildInTypeToPB>	VEC_BUILD_TYPE_TO_PB;

public:
	CParseClass()
	{
		init();
	}
	~CParseClass()
	{}
	void	clearContent();
	/// 初始
	void	init();
public:
	/// 分析头文件
	void parseClass(const char* pFilePath, const char* pClassName);
	/// 分析文件中的一行
	void parseLine(const char* pLine, int nLineLength);
	/// 得到第一个词
	void getFirstWord(const char* pLine, int& rStart, int nLineLength, char* pWord);
	/// 得到第一个变量
	void getFirstVariable(const char* pLine, int& rStart, int nLineLength, char* pWord);
	/// 得到第一个变量名的开始处
	bool getVariableTypeName(const char* pLine, int& rStart, int nLineLength, char* pVariableName);
	/// 处理变量名的空格
	void processVariableType(char* pSrc, char* pDst, int nLength);
	/// 是否是函数部分
	bool checkFunc(const char* pLine, int nLineLength);
	/// 是否是注释部分
	bool checkComment(const char* pLine, int nLineLength);
	/// 删除前导空格
	bool deleteHeadSpace(const char* pLine, int& rStart, int nLineLength);
	/// 计算数组维数
	int calcArrayDimension(CVariable* pVariable, const char* pLine, int nLineLength);
	/// 得到数组维数长度
	void getMaxArrayLen(const char* pSrc, int nLength, char* pMaxArrayLen);
	/// 得到类的真实名字（去掉前缀CTpl,C,CTemplate）
	void getRealClassName(const char* pClassName, char* pRealName, int nSize);
	/// 得到第一个大写字母的位置
	const char* getFirstUpcase(const char* pSrc);
	/// 通过build类型，得到pb类型
	const char* getPBTypeValue(const char* pBuildInType);
	/// 得到全类名
	void getClassDomainName(CClass* pClass,char* pName, int nSize);

	/// 写模板头文件
	void writeTempHeadFile();
	/// 写类的头文件
	void writeClassHeadFile(CClass* pClass, FILE* pFile);
	/// 写模板源文件
	void writeTempSourceFile();
	/// 写类的源文件
	void writeClassSourceFile(CClass* pClass, FILE* pFile);
	void writeSetFromPB(CClass* pClass, FILE* pFile);
	void writeCreatePB(CClass* pClass, FILE* pFile);
	/// 写模板PB文件
	void writeTempPBFile();
	/// 写类PB文件
	void writeClassPBFile(CClass* pClass, FILE* pFile);

private:
	VEC_FILE			mFileContent;				// 文件内容
	CClass*				mMainClass;					// 主要类
	CClass*				mCurClass;					// 当前处理类
	int					mCurLineIndex;				// 处理的当前行
	VEC_BUILD_TYPE_TO_PB mBuildTypeToPBList;		// 内置类型到PB类型
};
#endif