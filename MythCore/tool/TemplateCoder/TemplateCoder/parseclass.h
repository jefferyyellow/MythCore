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
	/// ��ʼ
	void	init();
public:
	/// ����ͷ�ļ�
	void parseClass(const char* pFilePath, const char* pClassName);
	/// �����ļ��е�һ��
	void parseLine(const char* pLine, int nLineLength);
	/// �õ���һ����
	void getFirstWord(const char* pLine, int& rStart, int nLineLength, char* pWord);
	/// �õ���һ������
	void getFirstVariable(const char* pLine, int& rStart, int nLineLength, char* pWord);
	/// �õ���һ���������Ŀ�ʼ��
	bool getVariableTypeName(const char* pLine, int& rStart, int nLineLength, char* pVariableName);
	/// ����������Ŀո�
	void processVariableType(char* pSrc, char* pDst, int nLength);
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
	/// �õ������ʵ���֣�ȥ��ǰ׺CTpl,C,CTemplate��
	void getRealClassName(const char* pClassName, char* pRealName, int nSize);
	/// �õ���һ����д��ĸ��λ��
	const char* getFirstUpcase(const char* pSrc);
	/// ͨ��build���ͣ��õ�pb����
	const char* getPBTypeValue(const char* pBuildInType);
	/// �õ�ȫ����
	void getClassDomainName(CClass* pClass,char* pName, int nSize);

	/// дģ��ͷ�ļ�
	void writeTempHeadFile();
	/// д���ͷ�ļ�
	void writeClassHeadFile(CClass* pClass, FILE* pFile);
	/// дģ��Դ�ļ�
	void writeTempSourceFile();
	/// д���Դ�ļ�
	void writeClassSourceFile(CClass* pClass, FILE* pFile);
	void writeSetFromPB(CClass* pClass, FILE* pFile);
	void writeCreatePB(CClass* pClass, FILE* pFile);
	/// дģ��PB�ļ�
	void writeTempPBFile();
	/// д��PB�ļ�
	void writeClassPBFile(CClass* pClass, FILE* pFile);

private:
	VEC_FILE			mFileContent;				// �ļ�����
	CClass*				mMainClass;					// ��Ҫ��
	CClass*				mCurClass;					// ��ǰ������
	int					mCurLineIndex;				// ����ĵ�ǰ��
	VEC_BUILD_TYPE_TO_PB mBuildTypeToPBList;		// �������͵�PB����
};
#endif