#include <stdio.h>
#include <string.h>
#ifdef MYTH_OS_UNIX
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>
#endif
#include "commontype.h"
#include "tcpserver.h"

#define _GAME_VER_   "Version 1.0.0"

#ifdef MYTH_OS_UNIX
void ignorePipe()
{
	struct sigaction sig;

	sig.sa_handler = SIG_IGN;
	sig.sa_flags = 0;
	sigemptyset(&sig.sa_mask);
	sigaction(SIGPIPE, &sig, NULL);
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
	ignorePipe();
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
		printf("Tcp server already start!");
		exit(0);
	}
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
			printf("Tcp Server debug %s  build at %s %s\n", _GAME_VER_, __DATE__, __TIME__);
#else
			printf("Tcp Server release %s  build at %s %s\n", _GAME_VER_, __DATE__, __TIME__);
#endif // __DEBUG__
			return true;
		}
	}

	return false;
}

int main(int argc, char* argv[])
{
#ifdef MYTH_OS_WINDOWS
	int nMemCheckFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	nMemCheckFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(nMemCheckFlag);
	//_CrtSetBreakAlloc(79);
#endif
	bool bExit = ParseParam(argc, argv);
	if (bExit)
	{
		return 0;
	}

#ifdef MYTH_OS_UNIX
	// 设置信号
	//setSignal();
	//initDaemon();
#endif

	setExclusive("tcpserver.lock");

	CTcpServer tServer;
	tServer.init();
	tServer.run();
	tServer.clear();
	tServer.exit();

	return 0;
}
