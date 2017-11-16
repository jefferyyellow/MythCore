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
	/// ����Ĭ��ֵ�����ļ�
	bool loadDefaultValueXml(const char* pXmlFile);
	// ******����ͷ�ļ���CPP�ļ�******
	/// ����ͷ�ļ�
	void parseHeaderFile(const char* pFilePath);
	/// �����ļ��е�һ��
	void parseLine(const char* pLine, int nLineLength);
	/// ����Դ�ļ�
	void parseSourceFile(const char* pFilePath);
	/// �õ���һ����
	void getFirstWord(const char* pLine, int& rStart, int nLineLength, char* pWord);
	/// �õ���һ������
	void getFirstVariable(const char* pLine, int& rStart, int nLineLength, char* pWord);
	/// �õ�������������
	bool getVariableTypeName(const char* pLine, int& rStart, int nLineLength, char* pVariableName);
	/// �������������
	void processVariableType(char* pSrc, char* pDst, int nLength);
	/// �Ƿ��Ǻ�������
	bool checkFunc(const char* pLine, int nLineLength);
	/// �Ƿ���ע�Ͳ���
	bool checkComment(const char* pLine, int nLineLength);
	/// ɾ��ǰ���ո�
	bool deleteHeadSpace(const char* pLine, int& rStart, int nLineLength);
	/// �����ļ���д����Ĭ��ֵ
	bool getDefaultValue(const char* pLine, int nLineLength, char* pDefaultValue);
	/// ��������ά��
	int calcArrayDimension(CVariable* pVariable, const char* pLine, int nLineLength);
	/// �õ�����ά������
	void getMaxArrayLen(const char* pSrc, int nLength, char* pMaxArrayLen);


	// ******д��ʼ���������******
	/// дͷ�ļ�
	void writeHeaderFile(const char* pHeadFile);
	/// дͷ�ļ���һ��
	void writeHeaderLine(FILE* pFile, const char* pLine, int nLineLength);
	/// дԴ�ļ�
	void writeSourceFile(const char* pSrcFile);
	/// дԴ�ļ���һ��
	void writeSourceLine(FILE* pFile, const char* pLine, int nLineLength);
	/// �õ�init��������
	CPlusClass* getClass(const char* pLine, int nLineLength);
	/// ͨ�����ֵõ���
	CPlusClass* getClassByName(char* pClassName, CPlusClass* pOutClass);
	/// �õ����Ͷ�Ӧ��Ĭ��ֵ
	const char* getDefaultValue(const char* pVariableType);
	/// д�����ĳ�ʼ��
	void writeVariableInit(FILE* pFile, int nSpaceNum);
	/// ֱ��д�ļ�������
	void writeContent(FILE* pFile, int nStartIndex, int nEndIndex);

	// ******дget/set����******
	/// д������get/set����
	bool writeGetSetMethod(FILE* pFile, const char* pLine, int nLineLength);
	/// д���������������
	void writeBuiltInType(bool bBuiltIn, CVariable* pVariable, FILE* pFile, int nSpaceNum, int &rCount);
	/// д��������
	void writeBuiltInArray(bool bBuiltIn, CVariable* pVariable, FILE* pFile, int nSpaceNum, int &rCount);
	/// д��ά������������
	void writeBuiltInArray2(bool bBuiltIn, CVariable* pVariable, FILE* pFile, int nSpaceNum, int &rCount);
	/// д�ַ�����
	void writeCharArray(CVariable* pVariable, FILE* pFile, int nSpaceNum, int &rCount);
	/// д��ά�ַ������� 
	void writeCharArray2(CVariable* pVariable, FILE* pFile, int nSpaceNum, int &rCount);

	/// �õ���һ����д��ĸ��λ��
	const char* getFirstUpcase(const char* pLine);
	/// дָ���ո�
	void writeSpace(char* pBuffer, int nSpaceNum);
	/// д��ָ������
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