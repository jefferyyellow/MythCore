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
	emFileState_Delete		= 0,	// 删除
	emFileState_Update		= 1,	// 更新
	emFileState_Add			= 2,	// 增加
	emFileState_NoModify	= 3,	// 无修改
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
	// 文件状态
	EmFileState	mFileState;
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
	void listFileDirectory(const wchar_t* pParentFilePath, const wchar_t* pRelativePath);
	// 得到文件大小
	bool getFileInfo(wchar_t *pFilePath, CFileData* pFileData, uint32 nFileSize);
	// 将字符转换成16进制打印字符
	char c2Hex(char c);
	// 将FileTime转换成time_t
	void fileTimeToTime_t(FILETIME ft, time_t *t);
	// 将time_t转换成FileTime
	void timetToFileTime(time_t t, LPFILETIME pft);
	// 将当前的文件列表写入XML
	void saveFileList(int nCurVersion);
	// 加载当前的文件列表
	void loadFileList();
	// 生成差异表
	void generateDiffList(int nStartVersion, int nCurVersion);
	// 修改所有版本的差异文件
	void modifyAllDiffXml(int nStartVersion, int nCurVersion);
	// 修改版本差异文件
	void modifyVersionDiffXml(const char* pDiffXml);
public:
	FILE_LIST&		getFileList(){return mFileList;}

private:
	// 文件列表
	FILE_LIST		mFileList;
	// 差异文件列表
	FILE_LIST		mDiffList;
};
#endif
