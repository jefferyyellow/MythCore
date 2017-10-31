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
		init();
	}
	void	init();

public:
	void parseHeaderFile(const char* pFilePath);
	void parseLine(const char* pLine, int nLineLength);
	void getFirstWord(const char* pLine, int& rStart, int nLineLength, char* pWord);
	bool checkFunc(const char* pLine, int nLineLength);
	bool checkComment(const char* pLine, int nLineLength);
	bool deleteHeadSpace(const char* pLine, int& rStart, int nLineLength);

	void parseSourceFile(const char* pFilePath);
	void writeHeaderFile();
	void writeHeaderLine(FILE* pFile, const char* pLine, int nLineLength);
	void writeVariableInit(FILE* pFile, int nSpaceNum);
	void writeContent(FILE* pFile, int nStartIndex, int nEndIndex);
	void writeSourceFile();
	void writeSourceLine(FILE* pFile, const char* pLine, int nLineLength);
	CPlusClass* getClass(const char* pLine, int nLineLength);
	CPlusClass* getClassByName(char* pClassName, CPlusClass* pOutClass);
	const char* getDefaultValue(const char* pVariableType);
private:
	VEC_FILE			mFileContent;
	CLASS_VECTOR		mClassList;

	CPlusClass*			mCurClass;
	int					mCurLineIndex;
	int					mCurClassIndex;
	VARIABLE_DEFAULT	mVariableDefaultList;
};