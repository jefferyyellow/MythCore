#include "fileutility.h"
#ifdef MYTH_OS_WINDOWS
#include <direct.h>
#include <sys/stat.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace Myth
{
	/// get the pos of the last '/' or '\'
	const char* CFileUtility::GetLastSeparator(const char* pFilePath)
	{
		const char* pFind = strrchr(pFilePath, '/');
		if (NULL == pFind)
		{
			pFind = strrchr(pFilePath, '\\');
		}
		return pFind;

	}

	/// get the file name from file path
	char* CFileUtility::GetFileName(const char* pFilePath, char* pFileName, int nSize)
	{
		if (NULL == pFilePath || NULL == pFileName)
		{
			return NULL;
		}

		const char* pFind = GetLastSeparator(pFilePath);
		if (NULL == pFind)
		{
			strncpy(pFileName, pFilePath, size_t(nSize - 1));
		}
		else
		{
			strncpy(pFileName, pFind + 1, size_t(nSize - 1));
		}
		return pFileName;
	}

	/// get the path name from file path
	char* CFileUtility::GetPathName(const char* pFilePath, char* pPathName, int nSize)
	{
		if (NULL == pFilePath || NULL == pPathName)
		{
			return NULL;
		}

		const char* pFind = GetLastSeparator(pFilePath);
		if (NULL == pFind)
		{
			strncpy(pPathName, pFilePath, size_t(nSize - 1));
		}
		else
		{
			int nBufferSize = pFind - pFilePath;
			if (nBufferSize >= nSize)
			{
				nBufferSize = nSize - 1;
			}
			strncpy(pPathName, pFilePath, (size_t)nBufferSize);
			pPathName[nBufferSize] = '\0';
		}

		return pPathName;
	}

	/// get the file extension from file path
	char* CFileUtility::GetExtension(const char* pFilePath, char* pExtension, int nSize)
	{
		const char* pFind = strrchr(pFilePath, '.');
		if (NULL == pFind)
		{
			strncpy(pExtension, "", size_t(nSize - 1));
		}
		else
		{
			strncpy(pExtension, pFind + 1, (size_t)(nSize - 1));
		}
		return pExtension;
	}

	/// get file name exclude extension from file path
	char* CFileUtility::GetFileNameWithoutExtension(const char* pFilePath, char* pNewFileName, int nSize)
	{
		char tBuffer[STR_LENGTH_64] = { 0 };
		GetFileName(pFilePath, tBuffer, STR_LENGTH_64);

		char* pFind = strrchr(tBuffer, '.');
		if (NULL == pFind)
		{
			strncpy(pNewFileName, pFilePath, size_t(nSize - 1));
		}
		else
		{
			int nBufferSize = pFind - tBuffer;
			if (nBufferSize >= nSize)
			{
				nBufferSize = nSize - 1;
			}
			strncpy(pNewFileName, tBuffer, size_t(nBufferSize));
			pNewFileName[nBufferSize] = '\0';
		}
		return pNewFileName;
	}

	char* CFileUtility::GetFilePathWithoutExtension(const char* pFilePath, char* pNewFilePath, int nSize)
	{
		if (NULL == pFilePath || NULL == pNewFilePath)
		{
			return NULL;
		}

		const char* pFind = strrchr(pFilePath, '.');
		if (NULL == pFind)
		{
			strncpy(pNewFilePath, pFilePath, size_t(nSize - 1));
		}
		else
		{
			int nBufferSize = pFind - pFilePath;
			if (nBufferSize >= nSize)
			{
				nBufferSize = nSize - 1;
			}
			strncpy(pNewFilePath, pFilePath, (size_t)nBufferSize);
			pNewFilePath[nBufferSize] = '\0';
		}

		return pNewFilePath;
	}


	bool CFileUtility::CheckFileExist(const char* pFilePath)
	{
#ifdef MYTH_OS_WINDOWS
		return GetFileAttributes(pFilePath) != INVALID_FILE_ATTRIBUTES;
#else
		struct stat tTempStat;
		return stat(pFilePath, &tTempStat) == 0;
#endif
	}

	bool CFileUtility::IsDirectory(const char* pFilePath)
	{
#ifdef MYTH_OS_WINDOWS
		DWORD dwResult = GetFileAttributes(pFilePath);
		if (INVALID_FILE_ATTRIBUTES == dwResult)
		{
			return false;
		}
		return (FILE_ATTRIBUTE_DIRECTORY & dwResult) != 0;
#else 
		struct stat tTempStat;
		int nResult = stat(pFilePath, &tTempStat);
		if (-1 == nResult)
		{
			return false;
		}
		if (S_ISDIR(tTempStat.st_mode))
		{
			return true;
		}
		return false;
#endif
	}

	unsigned int CFileUtility::GetFileSize(const char* pFilePath)
	{
#ifdef MYTH_OS_WINDOWS
		struct _stat tTempStat;
		int nResult = _stat(pFilePath, &tTempStat);
#else
		struct stat tTempStat;
		int nResult = stat(pFilePath, &tTempStat);
#endif
		if (0 != nResult)
		{
			return 0;
		}
		return (unsigned int)tTempStat.st_size;
	}

	unsigned int CFileUtility::GetFileSize(FILE* pFile)
	{
#ifdef MYTH_OS_WINDOWS
		struct _stat tTempStat;
		int nResult = _fstat(_fileno(pFile), &tTempStat);
#else
		struct stat tTempStat;
		int nFd = fileno(pFile);
		int nResult = fstat((nFd), &tTempStat);
#endif
		if (0 != nResult)
		{
			return 0;
		}
		return (unsigned int)tTempStat.st_size;
	}

	unsigned int CFileUtility::GetFileModifyTime(const char* pFilePath)
	{
		//	if(NULL == pFilePath)
		//	{
		//		return 0;
		//	}
		//#ifdef MYTH_OS_WINDOWS
		//	HANDLE handle = CreateFile(pFilePath, 0, 0, NULL, OPEN_EXISTING, 0, 0);
		//	FILETIME tCreationTime;
		//	FILETIME tAccessTime; 
		//	FILETIME tModTime;
		//
		//	BOOL bResult = GetFileTime(handle, &tCreationTime, &tAccessTime, &tModTime);
		//	if(0 == bResult)
		//	{
		//		CloseHandle(handle);
		//		return 0;
		//	}
		//	CloseHandle(handle);
		//
		return 0;
	}
	bool CFileUtility::SetFileModifyTime(const char* pFilePath, unsigned int uModifyTime)
	{
		if (NULL == pFilePath)
		{
			return false;
		}
		return true;
	}

	unsigned int CFileUtility::GetFileCreateDate(const char* pFilePath)
	{
		if (NULL == pFilePath)
		{
			return 0;
		}

#ifdef MYTH_OS_WINDOWS
		struct _stat tTempStat;
		int nResult = _stat(pFilePath, &tTempStat);
#else
		struct stat tTempStat;
		int nResult = stat(pFilePath, &tTempStat);
#endif
		if (nResult != 0)
		{
			return 0;
		}
		return (unsigned int)tTempStat.st_ctime;

	}

	bool CFileUtility::CreateEmptyFile(const char* pFilePath)
	{
		if (NULL == pFilePath)
		{
			return false;
		}
		FILE* pFile = fopen(pFilePath, "wb");
		if (NULL != pFile)
		{
			fclose(pFile);
			return true;
		}
		return false;
	}

	bool CFileUtility::DeleteFile(const char* pFilePath)
	{
		int nResult = unlink(pFilePath);
		if (-1 == nResult)
		{
			return false;
		}
		return true;
	}

	bool CFileUtility::CreateDir(const char* pDirName)
	{
		if (NULL == pDirName)
		{
			return false;
		}
#ifdef MYTH_OS_WINDOWS
		return _mkdir(pDirName) == 0;
#else
		return mkdir(pDirName, 0xFFFF) == 0;
#endif
	}

	bool CFileUtility::DeleteDir(const char* pDirName)
	{
		if (NULL == pDirName)
		{
			return false;
		}
		int nResult = rmdir(pDirName);
		if (0 != nResult)
		{
			return false;
		}

		return true;
	}

	bool CFileUtility::CreateDirTree(const char* pDirName)
	{
		if (NULL == pDirName)
		{
			return false;
		}

		bool bLastResult = true;

		unsigned int i = 0;
		char tBuffer[STR_LENGTH_256] = { 0 };
		unsigned int nLength = strlen(pDirName);

		// skip windows drive name, for example "C:"
		if (nLength > 1 && ':' == pDirName[1])
		{
			i = 2;
			tBuffer[0] = pDirName[0];
			tBuffer[1] = pDirName[1];
		}

		while (i < nLength)
		{
			// skip the head of "\" or "/"
			for (; i < nLength; ++i)
			{
				if ('/' != pDirName[i] && '\\' != pDirName[i])
				{
					break;
				}
				tBuffer[i] = pDirName[i];
			}

			// end with "\" or "/"
			if (i == nLength)
			{
				break;
			}

			for (; i < nLength; ++i)
			{
				if ('/' == pDirName[i] || '\\' == pDirName[i])
				{
					break;
				}
				tBuffer[i] = pDirName[i];
			}
			bLastResult = CreateDir(tBuffer);
		}

		return bLastResult;
	}

	bool CFileUtility::DeleteDirTree(const char* pDirName)
	{
		if (NULL == pDirName)
		{
			return false;
		}

#ifdef MYTH_OS_WINDOWS
		WIN32_FIND_DATA tFindFileData;
		DWORD dwError;
		char tBuffer[STR_LENGTH_256] = { 0 };
		snprintf(tBuffer, sizeof(tBuffer) - 1, "%s\\*", pDirName);

		HANDLE hFind = FindFirstFile(tBuffer, &tFindFileData);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			return false;
		}
		else
		{
			do{
				if ((tFindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
				{
					if (strcmp(tFindFileData.cFileName, ".") != 0 && strcmp(tFindFileData.cFileName, "..") != 0)
					{
						snprintf(tBuffer, sizeof(tBuffer) - 1, "%s\\%s", pDirName, tFindFileData.cFileName);
						DeleteDirTree(tBuffer);
					}
				}
				else
				{
					snprintf(tBuffer, sizeof(tBuffer) - 1, "%s\\%s", pDirName, tFindFileData.cFileName);
					::DeleteFile(tBuffer);
				}
			} while (FindNextFile(hFind, &tFindFileData) != 0);

			dwError = GetLastError();
			FindClose(hFind);
			// 如果错误不是已经没有其它文件了,就表示出现了其它的错误
			if (dwError != ERROR_NO_MORE_FILES)
			{
				return false;
			}
			return RemoveDirectory(pDirName) != 0;
		}
#else
		DIR	*pDir;
		struct	dirent *pDirent;
		struct 	stat tStatBuf;
		char tBuffer[STR_LENGTH_256] = { 0 };

		if ((pDir = opendir(pDirName)) == NULL)
		{
			return false;
		}

		while ((pDirent = readdir(pDir)) != NULL)
		{
			if (strcmp(".", pDirent->d_name) == 0
				|| strcmp("..", pDirent->d_name) == 0)
			{
				continue;
			}

			snprintf(tBuffer, sizeof(tBuffer) - 1, "%s/%s", pDirName, pDirent->d_name);
			int nReulst = lstat(tBuffer, &tStatBuf);

			if (S_ISDIR(tStatBuf.st_mode))
			{
				DeleteDirTree(tBuffer);
			}
			else
			{
				unlink(tBuffer);
			}

		}

		closedir(pDir);
		return rmdir(pDirName) == 0;
#endif
	}


	void CDir::findFirstFile(const char* pFilePath, char* pFileName, int nNameSize)
	{
#ifdef MYTH_OS_UNIX
		if ((mDir = opendir (pFilePath)) == NULL) 
		{
			pFileName[0] = '\0';
			return;
		}
		return nextFile(pFileName, nNameSize);
#else
		mFindHandle = FindFirstFile(pFilePath, &mFindData);
		if (INVALID_HANDLE_VALUE == mFindHandle)
		{
			pFileName[0] = '\0';
			return;
		}
		// 只处理非文件夹文件
		if (!(mFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			strncpy(pFileName, mFindData.cFileName, (size_t)(nNameSize - 1));
			return;
		}
		return nextFile(pFileName, nNameSize);
#endif
	}

	void CDir::nextFile(char* pFileName, int nNameSize)
	{
#ifdef MYTH_OS_UNIX
		struct 	stat tStatBuf;
		char tBuffer[STR_LENGTH_256] = { 0 };
		struct dirent *dp = NULL;

		while((dp = readdir (mDir)) != NULL)
		{
			int nReulst = lstat(tBuffer, &tStatBuf);
			// 不处理子文件夹
			if (S_ISDIR(tStatBuf.st_mode))
			{
				continue;
			}
			strncpy(pFileName, dp->d_name, nNameSize - 1);
			return;
		}
		pFileName[0] = '\0';
		return;
#else
		while (FindNextFile(mFindHandle, &mFindData) != 0)
		{
			// 只处理非文件夹文件
			if ((mFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				continue;
			}
			strncpy(pFileName, mFindData.cFileName, (size_t)(nNameSize - 1));
			return;
		}

		pFileName[0] = '\0';
		return;
#endif
	}

	const char* CDir::findFirstFileLua(const char* pFilePath)
	{
#ifdef MYTH_OS_UNIX
		if ((mDir = opendir(pFilePath)) == NULL)
		{
			return NULL;
		}
		return nextFileLua();
#else
		mFindHandle = FindFirstFile(pFilePath, &mFindData);
		if (INVALID_HANDLE_VALUE == mFindHandle)
		{
			return NULL;
		}
		// 只处理非文件夹文件
		if (!(mFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			return mFindData.cFileName;
		}
		return nextFileLua();
#endif
	}

	const char* CDir::nextFileLua()
	{
#ifdef MYTH_OS_UNIX
		struct 	stat tStatBuf;
		char tBuffer[STR_LENGTH_256] = { 0 };
		struct dirent *dp = NULL;

		while ((dp = readdir(mDir)) != NULL)
		{
			int nReulst = lstat(tBuffer, &tStatBuf);
			// 不处理子文件夹
			if (S_ISDIR(tStatBuf.st_mode))
			{
				continue;
			}
			return dp->d_name;
		}
		return NULL;
#else
		while (FindNextFile(mFindHandle, &mFindData) != 0)
		{
			// 只处理非文件夹文件
			if ((mFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				continue;
			}
			return mFindData.cFileName;
		}

		return NULL;
#endif
	}
}
