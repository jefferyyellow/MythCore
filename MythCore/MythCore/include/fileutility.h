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
		static uint32		GetFileSize(const char* pFilePath);
		static uint32		GetFileSize(FILE* pFile);
		static uint32		GetFileModifyTime(const char* pFilePath);
		static bool			SetFileModifyTime(const char* pFilePath, uint32 uModifyTime);
		static uint32		GetFileCreateDate(const char* pFilePath);
		static bool			CreateEmptyFile(const char* pFilePath);
		static bool			DeleteFile(const char* pFilePath);
		static bool			CreateDir(const char* pDirName);
		static bool			DeleteDir(const char* pDirName);
		static bool			CreateDirTree(const char* pDirName);
		static bool			DeleteDirTree(const char* pDirName);
	};
}


#endif
