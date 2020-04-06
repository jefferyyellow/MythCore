#ifndef __DIR_H__
#define __DIR_H__
#include <hash_map>
#include <string>
#include "common.h"
#include <windows.h>
#include <time.h>
using namespace std;

#define MD5_STR_LENGTH	33
#define MD5_LENGTH		16
class CFileData
{
public:
	CFileData()
	{
		memset(mMd5, 0, sizeof(mMd5));
		mModifyTime = 0;
		mFileSize = 0;
	}
	~CFileData(){}

public:
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
	void ListFileDirectory(const wchar_t* pParentFilePath, const wchar_t* pRelativePath);
	// �õ��ļ���С
	bool GetFileInfo(wchar_t *pFilePath, CFileData* pFileData, uint32 nFileSize);
	// ���ַ�ת����16���ƴ�ӡ�ַ�
	char C2Hex(char c);
	// ��FileTimeת����time_t
	void FileTimeToTime_t(FILETIME ft, time_t *t);
	// ��time_tת����FileTime
	void TimetToFileTime(time_t t, LPFILETIME pft);
	// ����ǰ���ļ��б�д��XML
	void SaveFileList();
	// ���ص�ǰ���ļ��б�
	void LoadFileList();

private:
	// �ļ��б�
	FILE_LIST		mFileList;
};
#endif
