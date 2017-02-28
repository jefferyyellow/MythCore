#include "logdisplayer.h"
#include <stdio.h>
#ifdef MYTH_OS_WINDOWS
#include <io.h>
#else
#include <unistd.h>
#endif
#include <fcntl.h>
namespace Myth
{
	/// display log message
	void CStdDisplayer::DisplayLog(char* pLogMessage)
	{
		printf("%s", pLogMessage);
	}

	CRollFileDisplayer::CRollFileDisplayer(char* pFileName, uint32 uMaxFileSize, uint uMaxBackNum)
	{
		if (NULL != pFileName)
		{
			mFd = open(pFileName, O_CREAT | O_APPEND | O_WRONLY, 00644);
			strncpy(mFileName, pFileName, sizeof(mFileName) - 1);
		}
		mMaxFileSize = uMaxFileSize;
		mMaxBackNum = uMaxBackNum;
	}

	CRollFileDisplayer::~CRollFileDisplayer()
	{
		if (0 != mFd)
		{
			close(mFd);
		}

	}

	/// display log message
	void CRollFileDisplayer::DisplayLog(char* pLogMessage)
	{
		if (0 == mFd || NULL == pLogMessage)
		{
			return;
		}

		// 开始用的是fwrite,fopen,flcose系列的函数，由于不能支持行缓冲，
		// 所以改成write,open,close系统函数
		write(mFd, pLogMessage, strlen(pLogMessage));
		
		uint32 nFileSize = ::lseek(mFd, 0, SEEK_END);
		if (nFileSize >= mMaxFileSize)
		{
			RollOver();
		}
	}

	/// role over when a log file is full
	void CRollFileDisplayer::RollOver()
	{
		if (mMaxBackNum > 0)
		{
			close(mFd);
			char tNewBuffer[STRING_LENGTH_256] = { 0 };
			snprintf(tNewBuffer, sizeof(tNewBuffer) - 1, "%s.%d", mFileName, mMaxBackNum);
			remove(tNewBuffer);

			char tOldBuffer[STRING_LENGTH_256] = { 0 };
			for (int i = mMaxBackNum - 1; i >= 1; --i)
			{
				snprintf(tOldBuffer, sizeof(tOldBuffer) - 1, "%s.%d", mFileName, i);
				rename(tOldBuffer, tNewBuffer);
				strncpy(tNewBuffer, tOldBuffer, sizeof(tNewBuffer) - 1);
			}
			rename(mFileName, tNewBuffer);

			mFd = ::open(mFileName, O_CREAT | O_APPEND | O_WRONLY, 00644);
		}
	}

	/// display log message
	void CDebuggerOutputDisplayer::DisplayLog(char* pLogMessage)
	{
#ifdef MYTH_OS_WINDOWS
		//int nLenth = MultiByteToWideChar(CP_ACP,0, pLogMessage, -1, NULL, 0);
		//wchar_t* pWideChar = new wchar_t[nLenth + 1];
		//MultiByteToWideChar(CP_ACP, 0, pLogMessage, -1, (LPWSTR)pWideChar, nLenth);
		OutputDebugString(pLogMessage);
		//delete []pWideChar;
#endif
	}

}
