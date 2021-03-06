#include "commontype.h"
#include "sharememory.h"
using namespace Myth;

#ifdef MYTH_OS_UNIX
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#endif

class CMemUnit
{
public:
	CMemUnit()
	{
		mPlayerID = 0;
		mExp = 0;
		mLevel = 0;
	}
	~CMemUnit()
	{

	}

public:
	int GetPlayerID() const { return mPlayerID; }
	void SetPlayerID(int nValue) { mPlayerID = nValue; }

	int GetExp() const { return mExp; }
	void SetExp(int nValue) { mExp = nValue; }

	int GetLevel() const { return mLevel; }
	void SetLevel(int nValue) { mLevel = nValue; }

private:
	int mPlayerID;
	int mExp;
	int	mLevel;
};
#ifdef MYTH_OS_WINDOWS
LONG WINAPI DumpException(struct _EXCEPTION_POINTERS *pExceptionInfo)
{
	//WinExec("cmd.exe", SW_SHOWNORMAL);
	ShellExecute(NULL, _T("open"), _T("CrashResave.exe"), NULL, NULL, SW_SHOWNORMAL);
	return 0;
}
#else
void sigCrash(int signo)
{
	//system("./CrashResave");
	//signal(SIGSEGV, SIG_DFL);

	pid_t pid;
	if ((pid = fork()) < 0)
	{
		return;
	}
	else if (pid == 0)
	{
		printf("\n\n--------------excele  Crash   Resave  --------------\n");
		if (execle("./CrashResave", (char*)0) < 0)
		{
				return;
		}
		// 注意下面这行是打印不了的，因为execle成功了就不会返回，失败了就return返回了
		printf("222222222222excele  Crash   Resave  ");
	}
	else
	{
		signal(SIGSEGV, SIG_DFL);
		return;
	}
	//pid_t pid = fork();
	//if (pid < 0)
	//{
	//	printf("出错啦");
	//	return;
	//}
	//if(pid == 0)
	//{
	//	if (execle("CrashResave", (char*)0) < 0)
	//	{
	//		return;
	//	}
	//}
	//else
	//{
	//	signal(SIGSEGV, SIG_DFL);
	//}
}
#endif // MYTH_OS_WINDOWS

int main()
{
	printf("\n\n **********************  CrashSave ***********************\n");

#ifdef MYTH_OS_WINDOWS
	SetUnhandledExceptionFilter(DumpException);
#else
	if (signal(SIGSEGV, sigCrash) == SIG_ERR)
	{
		printf("can't catch SIGUSR1");
		return -1;
	}
#endif // MYTH_OS_WINDOWS

	bool bCreate = true;

	int nShareMemorySize = sizeof(int) + sizeof(CMemUnit) * 10;
	byte* pSharePoint = CShareMemory::createShareMemory(12343433, nShareMemorySize, bCreate);
	if (NULL == pSharePoint)
	{
		return false;
	}

	printf("%x\n", pSharePoint);

	int* pNum = (int*)pSharePoint;

	CMemUnit* pUnit = (CMemUnit*)(pSharePoint + sizeof(int));


	*pNum = 5;
	CMemUnit* pTempUnit = pUnit;
	for (int i = 0; i < *pNum; ++ i)
	{
		pTempUnit->SetPlayerID(100000 + i);
		pTempUnit->SetLevel(i);
		pTempUnit->SetExp(32134 + i);
		printf("PlayerID:	%d\n", pTempUnit->GetPlayerID());
		printf("Level:	%d\n", pTempUnit->GetLevel());
		printf("Exp:	%d\n", pTempUnit->GetExp());
		pTempUnit += 1;
	}
	int *p = NULL;
	*p = 100;
	return 0;
}
