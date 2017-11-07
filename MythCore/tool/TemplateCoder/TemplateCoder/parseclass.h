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
	/// ����ͷ�ļ�
	void parseClass(const char* pFilePath, const char* pClassName);
	/// �����ļ��е�һ��
	void parseLine(const char* pLine, int nLineLength);
	/// �õ���һ����
	void getFirstWord(const char* pLine, int& rStart, int nLineLength, char* pWord);
	/// �õ���һ������
	void getFirstVariable(const char* pLine, int& rStart, int nLineLength, char* pWord);
	/// �Ƿ��Ǻ�������
	bool checkFunc(const char* pLine, int nLineLength);
	/// �Ƿ���ע�Ͳ���
	bool checkComment(const char* pLine, int nLineLength);
	/// ɾ��ǰ���ո�
	bool deleteHeadSpace(const char* pLine, int& rStart, int nLineLength);
	/// ��������ά��
	int calcArrayDimension(CVariable* pVariable, const char* pLine, int nLineLength);
	/// �õ�����ά������
	void getMaxArrayLen(const char* pSrc, int nLength, char* pMaxArrayLen);


private:
	VEC_FILE			mFileContent;				// �ļ�����
	CClass*				mMainClass;					// ��Ҫ��
	CClass*				mCurClass;					// ��ǰ������
	int					mCurLineIndex;				// ����ĵ�ǰ��
};
#endif