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
		HANDLE hHandle = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, (unsigned int)nSize, szKeyBuffer);
		if (INVALID_HANDLE_VALUE == hHandle)
		{
			return nullptr;
		}

		// ��֪����εõ������ڴ�Ĵ�С������û���Դ�С��У�飬����ʱ�����ɣ�
		// ����Ժ�֪����ô�����ټ���
		unsigned int nErrno = GetLastError();
		if (nErrno != 0)
		{
			bCreate = false;
		}

		return (byte *)MapViewOfFile(hHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	}

	int CShareMemory::destroyShareMemory(int nKey, const byte* pShmPoint)
	{
		if (nullptr != pShmPoint)
		{
			UnmapViewOfFile(pShmPoint);
		}

		char szKeyBuffer[MAX_PATH] = { 0 };
		snprintf(szKeyBuffer, sizeof(szKeyBuffer) - 1, "%d", nKey);
		HANDLE hHandle = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, 0, szKeyBuffer);
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
			return nullptr;
		}
		// ֱ�Ӵ���
		int nShmID = shmget(tKey, nShmSize, IPC_CREAT | IPC_EXCL | 0666);
		if (nShmID < 0)
		{
			if (EEXIST != errno)
			{
				return nullptr;
			}
			bCreate = false;
			// ��Ӧ�ļ�ֵ�Ѿ�����
			// ȡnShmSize��С�Ĺ����ڴ�
			nShmID = shmget(tKey, nShmSize, 0666);
			if (nShmID < 0)
			{
				// ���ܴ�С,���ݼ�ȡ�����ڴ�
				nShmID = shmget(tKey, 0, 0666);
				if (nShmID < 0)
				{
					return nullptr;
				}
				else
				{
					bCreate = true;
					// �������ڴ�ɾ��
					if (shmctl(tKey, IPC_RMID, nullptr))
					{
						return nullptr;
					}
					// ���������ڴ�
					nShmID = shmget(tKey, nShmSize, IPC_CREAT | IPC_EXCL | 0666);
					if (nShmID < 0)
					{
						return nullptr;
					}
				}
			}
		}


		return (byte*)shmat(nShmID, nullptr, 0);
	}

	int CShareMemory::destroyShareMemory(int nKey, const byte* pShmPoint)
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
			if (shmctl(nShmID, IPC_RMID, nullptr))
			{
				return -1;
			}
		}

		return 0;
	}

#endif
}
