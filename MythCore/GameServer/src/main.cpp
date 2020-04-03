#include <stdio.h>
#include <string.h>
#include "i18n.h"
#include "gameserver.h"
#include "google/protobuf/stubs/common.h"
#define _GAME_VER_   "Version 1.0.0"
#ifdef MYTH_OS_UNIX
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>
bool gDaemon = false;
#endif
#include "gtest/gtest.h"

#ifdef MYTH_OS_UNIX
void sigCrash(int signo)
{
	pid_t pid;
	if ((pid = fork()) < 0)
	{
		return;
	}
	else if (pid == 0)
	{
		printf("\n\n--------------excele  Crash   Resave  --------------\n");
		if (execle("./ServerCrashResave", (char*)0) < 0)
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
}
void ignorePipe()
{
	struct sigaction sig;

	sig.sa_handler = SIG_IGN;
	sig.sa_flags = 0;
	sigemptyset(&sig.sa_mask);
	sigaction(SIGPIPE, &sig, NULL);
}

void sigusr1_handle(int iSigVal)
{
	CGameServer::Inst()->setExit(true);
	signal(SIGUSR1, sigusr1_handle);
}

void sigusr2_handle(int iSigVal)
{
	//CSceneServer::GetSingletonPtr()->SetRunFlag(CSceneServer::EFLG_CTRL_QUIT);
	signal(SIGUSR2, sigusr2_handle);
}

void setSignal()
{
	//signal(SIGINT, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
	if (signal(SIGSEGV, sigCrash) == SIG_ERR)
	{
		printf("can't catch SIGUSR1");
	}
	ignorePipe();
	signal(SIGUSR1, sigusr1_handle);
	signal(SIGUSR2, sigusr2_handle);
}

int initDaemon()
{
	pid_t pid;
	if ((pid = fork()) != 0)
	{
		exit(0);
	}
	setsid();

	if ((pid = fork()) != 0)
	{
		exit(0);
	}

	umask(0);
	return 0;

}

void setExclusive(const char* pLockFile)
{
	if (NULL == pLockFile)
	{
		exit(0);
	}

	// 避免一个程序开启多个
	int lock_fd;
	lock_fd = open(pLockFile, O_RDWR | O_CREAT, 0640);
	if (lock_fd < 0)
	{
		printf("Open Lock File Failed, Scene Server Init Failed!\n");
		exit(0);
	}
	int ret;
	ret = flock(lock_fd, LOCK_EX | LOCK_NB);
	if (ret < 0)
	{
		printf("Lock File Failed, Scene Server is already Running!\n");
		exit(0);
	}
}
#else
void setExclusive(char* pLockFile)
{
	HANDLE hFile = CreateFile(pLockFile, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		exit(0);
	}
	CloseHandle(hFile);
}

// 崩溃转存
LONG WINAPI DumpException(struct _EXCEPTION_POINTERS *pExceptionInfo)
{
	ShellExecute(NULL, _T("open"), _T("ServerCrashResave.exe"), NULL, NULL, SW_SHOWNORMAL);
	return 0;
}
#endif




/// 处理传人参数
/// 返回true,程序直接退出
bool ParseParam(int argc, char* argv[])
{
	if (argc > 1)
	{
#ifdef MYTH_OS_WINDOWS
		// 支持版本信息查询
		if (0 == _stricmp(argv[1], "-v"))
#else
		if (0 == strcasecmp(argv[1], "-v"))
#endif
		{
#ifdef __DEBUG__	
			printf("GameServer debug %s  build at %s %s\n", _GAME_VER_, __DATE__, __TIME__);
#else
			printf("GameServer release %s  build at %s %s\n", _GAME_VER_, __DATE__, __TIME__);
#endif // __DEBUG__
			return true;
		}

#ifdef MYTH_OS_UNIX
		else if (0 == strcasecmp(argv[1], "-d"))
		{
			gDaemon = true;
		}
#endif
	}

	return false;
}

#ifdef MYTH_OS_UNIX
	
#endif

int main(int argc, char* argv[])
{
#ifdef MYTH_OS_WINDOWS
	int nMemCheckFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	nMemCheckFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(nMemCheckFlag);
	//_CrtSetBreakAlloc(19992);
	// 崩溃转存
	SetUnhandledExceptionFilter(DumpException);
#endif

	bool bExit = ParseParam(argc, argv);
	if (bExit)
	{
		return 0;
	}

	setExclusive("gameserver.lock");
#ifdef MYTH_OS_UNIX
	if (gDaemon)
	{
		initDaemon();
	}
	// 设置信号
	setSignal();
	//initDaemon();
#endif

	CGameServer::createInst();
 	bool bResult = CGameServer::Inst()->initAll();
	if (!bResult)
	{
		return -1;
	}

	/// ***********************************************************
	/// gtest代码

#ifdef GOOGLE_TEST_ALL
	testing::InitGoogleTest(&argc, argv);
	int nResult = RUN_ALL_TESTS();


	CGameServer::destroyInst();
	// 释放protobuf中lib和msg占用的内存
	::google::protobuf::ShutdownProtobufLibrary();
	//_CrtDumpMemoryLeaks();
	return nResult;
#endif
	/// ***********************************************************

	//return RUN_ALL_TESTS();
	CGameServer::Inst()->run();
	CGameServer::Inst()->exit();
	CGameServer::destroyInst();

	// 释放protobuf中lib和msg占用的内存
	::google::protobuf::ShutdownProtobufLibrary();
	//_CrtDumpMemoryLeaks();

	return 0;
}