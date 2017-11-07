#ifndef __PARSECLASS_H__
#define __PARSECLASS_H__
#include "commontype.h"
#include "class.h"

class CParseClass
{
public:
	typedef vector<char*>			VEC_FILE;

	void	clearContent();

public:
	/// 分析头文件
	void parseClass(const char* pFilePath, const char* pClassName);
	/// 分析文件中的一行
	void parseLine(const char* pLine, int nLineLength);
	/// 得到第一个词
	void getFirstWord(const char* pLine, int& rStart, int nLineLength, char* pWord);
	/// 得到第一个变量
	void getFirstVariable(const char* pLine, int& rStart, int nLineLength, char* pWord);
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


private:
	VEC_FILE			mFileContent;				// 文件内容
	CClass*				mMainClass;					// 主要类
	CClass*				mCurClass;					// 当前处理类
	int					mCurLineIndex;				// 处理的当前行
};
#endif