#pragma once
#include <Windows.h>
// CriticalSectionËø
class CCSLock
{
public:
	CCSLock(void){::InitializeCriticalSection(&m_CriticalSection);}
	~CCSLock(void){::DeleteCriticalSection(&m_CriticalSection);}


	void Lock(){::EnterCriticalSection(&m_CriticalSection);}
	void UnLock(){::LeaveCriticalSection(&m_CriticalSection);}

private:
	CRITICAL_SECTION m_CriticalSection;
};
