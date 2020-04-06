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
	// Md5码
	char		mMd5[MD5_STR_LENGTH];
	// 修改时间
	time_t		mModifyTime;
	// 文件大小
	uint32		mFileSize;
};

class CFileList
{
public:
	// 文件相对地址和md5码的hash
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
	// 得到文件大小
	bool GetFileInfo(wchar_t *pFilePath, CFileData* pFileData, uint32 nFileSize);
	// 将字符转换成16进制打印字符
	char C2Hex(char c);
	// 将FileTime转换成time_t
	void FileTimeToTime_t(FILETIME ft, time_t *t);
	// 将time_t转换成FileTime
	void TimetToFileTime(time_t t, LPFILETIME pft);
	// 将当前的文件列表写入XML
	void SaveFileList();
	// 加载当前的文件列表
	void LoadFileList();

private:
	// 文件列表
	FILE_LIST		mFileList;
};
#endif
