#ifndef __FILELIST_H__
#define __FILELIST_H__
#include <hash_map>
#include <string>
#include "common.h"
#include <windows.h>
#include <time.h>
using namespace std;

#define MD5_STR_LENGTH	33
#define MD5_LENGTH		16

enum EmFileState
{
	emFileState_Delete		= 0,	// ɾ��
	emFileState_Update		= 1,	// ����
	emFileState_Add			= 2,	// ����
	emFileState_NoModify	= 3,	// ���޸�
};

class CFileData
{
public:
	CFileData()
	{
		memset(mMd5, 0, sizeof(mMd5));
		mModifyTime = 0;
		mFileSize = 0;
		mFileState = emFileState_Delete;
	}
	~CFileData(){}

public:
	// �ļ�״̬
	EmFileState	mFileState;
	// Md5��
	char		mMd5[MD5_STR_LENGTH];
	// �޸�ʱ��
	time_t		mModifyTime;
	// �ļ���С
	uint32		mFileSize;
};

class CFileList
{
public:
	// �ļ���Ե�ַ��md5���hash
	typedef hash_map<string, CFileData*>	FILE_LIST;

public:
	CFileList()
	{
	}
	~CFileList()
	{
	}

public:
	void listFileDirectory(const wchar_t* pParentFilePath, const wchar_t* pRelativePath);
	// �õ��ļ���С
	bool getFileInfo(wchar_t *pFilePath, CFileData* pFileData, uint32 nFileSize);
	// ���ַ�ת����16���ƴ�ӡ�ַ�
	char c2Hex(char c);
	// ��FileTimeת����time_t
	void fileTimeToTime_t(FILETIME ft, time_t *t);
	// ��time_tת����FileTime
	void timetToFileTime(time_t t, LPFILETIME pft);
	// ����ǰ���ļ��б�д��XML
	void saveFileList(int nCurVersion);
	// ���ص�ǰ���ļ��б�
	void loadFileList();
	// ���ɲ����
	void generateDiffList(int nStartVersion, int nCurVersion);
	// �޸����а汾�Ĳ����ļ�
	void modifyAllDiffXml(int nStartVersion, int nCurVersion);
	// �޸İ汾�����ļ�
	void modifyVersionDiffXml(const char* pDiffXml);
public:
	FILE_LIST&		getFileList(){return mFileList;}

private:
	// �ļ��б�
	FILE_LIST		mFileList;
	// �����ļ��б�
	FILE_LIST		mDiffList;
};
#endif
