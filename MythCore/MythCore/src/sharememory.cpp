#include "sharememory.h"
#include <stdio.h>
#ifdef MYTH_OS_UNIX
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#endif


namespace Myth
{
#ifdef MYTH_OS_WINDOWS
	byte* CShareMemory::createShareMemory(int nKey, int nSize, bool& bCreate)
	{
		bCreate = true;
		char szKeyBuffer[MAX_PATH] = { 0 };
		snprintf(szKeyBuffer, sizeof(szKeyBuffer) - 1, "%d", nKey);
		HANDLE hHandle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, nSize, szKeyBuffer);
		if (INVALID_HANDLE_VALUE == hHandle)
		{
			return NULL;
		}

		// 不知道如何得到共享内存的大小，所以没法对大小做校验，先暂时这样吧，
		// 如果以后知道怎么处理，再加上
		int nErrno = GetLastError();
		if (nErrno != 0)
		{
			bCreate = false;
		}

		return (byte *)MapViewOfFile(hHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	}

	int CShareMemory::destroyShareMemory(int nKey, byte* pShmPoint)
	{
		if (NULL != pShmPoint)
		{
			UnmapViewOfFile(pShmPoint);
		}

		char szKeyBuffer[MAX_PATH] = { 0 };
		snprintf(szKeyBuffer, sizeof(szKeyBuffer) - 1, "%d", nKey);
		HANDLE hHandle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 0, szKeyBuffer);
		if (INVALID_HANDLE_VALUE == hHandle)
		{
			return 0;
		}
		CloseHandle(hHandle);
		return 0;
	}
#else
	byte* CShareMemory::createShareMemory(int nKey, int nSize, bool& bCreate)
	{
		bCreate = true;
		key_t tKey = (key_t)nKey;
		size_t nShmSize = (size_t)nSize;
		if (nKey < 0)
		{
			return NULL;
		}
		// 直接创建
		int nShmID = shmget(tKey, nShmSize, IPC_CREAT | IPC_EXCL | 0666);
		if (nShmID < 0)
		{
			if (EEXIST != errno)
			{
				return NULL;
			}
			bCreate = false;
			// 对应的键值已经存在
			// 取nShmSize大小的共享内存
			nShmID = shmget(tKey, nShmSize, 0666);
			if (nShmID < 0)
			{
				// 不管大小,根据键取共享内存
				nShmID = shmget(tKey, 0, 0666);
				if (nShmID < 0)
				{
					return NULL;
				}
				else
				{
					bCreate = true;
					// 将共享内存删除
					if (shmctl(tKey, IPC_RMID, NULL))
					{
						return NULL;
					}
					// 创建共享内存
					nShmID = shmget(tKey, nShmSize, IPC_CREAT | IPC_EXCL | 0666);
					if (nShmID < 0)
					{
						return NULL;
					}
				}
			}
		}


		return (byte*)shmat(nShmID, NULL, 0);
	}

	int CShareMemory::destroyShareMemory(int nKey, char* pShmPoint)
	{
		key_t tKey = (key_t)nKey;
		if (tKey < 0)
		{
			return -1;
		}

		int nShmID = shmget(tKey, 0, 0666);
		if (nShmID < 0)
		{
			return -1;
		}
		else
		{
			if (shmctl(nShmID, IPC_RMID, NULL))
			{
				return -1;
			}
		}

		return 0;
	}

#endif
}
