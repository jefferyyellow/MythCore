#ifndef __FILEUTILITY_H__
#define __FILEUTILITY_H__
#include "commontype.h"
#include <stdio.h>

namespace Myth
{
	class CFileUtility
	{
	public:
		CFileUtility();
		~CFileUtility();


	public:
		/// get the pos of the last '/' or '\'
		static const char*	GetLastSeparator(const char* pFilePath);
		/// get the file name from file path
		static char* 		GetFileName(const char* pFilePath, char* pFileName, int nSize);
		/// get the path name from file path
		static char* 		GetPathName(const char* pFilePath, char* pPathName, int nSize);
		/// get the file extension from file path
		static char* 		GetExtension(const char* pFilePath, char* pExtension, int nSize);
		/// get file name exclude extension from file path
		static char*		GetFileNameWithoutExtension(const char* pFilePath, char* pNewFilePath, int nSize);
		/// 
		static bool			CheckFileExist(const char* pFilePath);
		static bool			IsDirectory(const char* pFilePath);
		static unsigned int	GetFileSize(const char* pFilePath);
		static unsigned int	GetFileSize(FILE* pFile);
		static unsigned int	GetFileModifyTime(const char* pFilePath);
		static bool			SetFileModifyTime(const char* pFilePath, unsigned int uModifyTime);
		static unsigned int	GetFileCreateDate(const char* pFilePath);
		static bool			CreateEmptyFile(const char* pFilePath);
		static bool			DeleteFile(const char* pFilePath);
		static bool			CreateDir(const char* pDirName);
		static bool			DeleteDir(const char* pDirName);
		static bool			CreateDirTree(const char* pDirName);
		static bool			DeleteDirTree(const char* pDirName);
	};

	class CDir
	{
	public:
		CDir()
		{
#ifdef MYTH_OS_UNIX
			mDir = NULL;
#else
			mFindHandle = INVALID_HANDLE_VALUE;
#endif
		}
		~CDir()
		{
#ifdef MYTH_OS_UNIX
			if (NULL != mDir)
			{
				closedir(mDir);
				mDir = NULL;
			}
#else
			if (INVALID_HANDLE_VALUE != mFindHandle)
			{
				FindClose(mFindHandle);
			}
#endif
		}

	public:
		void findFirstFile(const char* pFilePath, char* pFileName, int nNameSize);
		void nextFile(char* pFileName, int nNameSize);

	private:
#ifdef MYTH_OS_UNIX
		DIR*	mDir;
#else
		HANDLE	mFindHandle;
#endif
	};
}


#endif
