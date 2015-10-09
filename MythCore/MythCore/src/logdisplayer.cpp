#include "logdisplayer.h"
#include <stdio.h>

/// display log message
void CStdDisplayer::DisplayLog(char* pLogMessage)
{
	printf("%s", pLogMessage);
}

CRollFileDisplayer::CRollFileDisplayer(char* pFileName, uint32 uMaxFileSize, uint uMaxBackNum)
{
	if(NULL != pFileName)
	{
		mFd = fopen(pFileName, "a+");
		fseek(mFd, 0, SEEK_END);
		strncpy(mFileName, pFileName, sizeof(mFileName) - 1);
	}
	mMaxFileSize = uMaxFileSize;
	mMaxBackNum = uMaxBackNum;
}

CRollFileDisplayer::~CRollFileDisplayer()
{
	if (NULL != mFd)
	{
		fclose(mFd);
	}
	
}

/// display log message
void CRollFileDisplayer::DisplayLog(char* pLogMessage)
{
	if(NULL == mFd || NULL == pLogMessage)
	{
		return;
	}
	fwrite(pLogMessage, strlen(pLogMessage), 1, mFd);
	//fseek(mFd, 0, SEEK_END);
	uint32 nFileSize = ftell(mFd);
	if(nFileSize >= mMaxFileSize)
	{
		RollOver();
	}
}

/// role over when a log file is full
void CRollFileDisplayer::RollOver()
{
	if(mMaxBackNum > 0)
	{
		fclose(mFd);
		char tNewBuffer[STRING_LENGTH_256] = {0};
		snprintf(tNewBuffer, sizeof(tNewBuffer) - 1, "%s.%d", mFileName, mMaxBackNum);
		remove(tNewBuffer);

		char tOldBuffer[STRING_LENGTH_256] = {0};
		for(int i = mMaxBackNum - 1; i >= 1; -- i)
		{
			snprintf(tOldBuffer, sizeof(tOldBuffer) - 1, "%s.%d", mFileName, i);
			rename(tOldBuffer, tNewBuffer);
			strncpy(tNewBuffer, tOldBuffer, sizeof(tNewBuffer) - 1);
		}
		rename(mFileName, tNewBuffer);

		mFd = fopen(mFileName, "a+");
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
