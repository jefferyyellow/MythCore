#include "log.h"
#include "logdisplayer.h"
/// Initialize the log
int CLog::Init()
{
	mLogName[0] = '\0';
	mDisplayerSize = 0;

	return 0;
}

/// Finalization, when release the log
void CLog::Finial()
{

}

/// Add a displayer
bool CLog::AddDisplayer(CLogDisplayer* pDisplayer)
{
	if(mDisplayerSize >= MAX_NUM_LOG_DISPLAYER)
	{
		return false;
	}	
	mDisplayer[mDisplayerSize] = pDisplayer;

	++ mDisplayerSize;
	return true;
}

/// Get a displayer
CLogDisplayer* CLog::GetDisplayer(uint32 uIndex)
{
	if(uIndex >= MAX_NUM_LOG_DISPLAYER)
	{
		return NULL;
	}
	return mDisplayer[uIndex];	
}

/// display log in the all log displayer
void CLog::DisplayLog(char* pLogMessage)
{
	for (int i = 0; i < mDisplayerSize; ++ i)
	{
		mDisplayer[i]->DisplayLog(pLogMessage);
	}
}
