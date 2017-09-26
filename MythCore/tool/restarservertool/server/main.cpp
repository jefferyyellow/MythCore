#include <stdio.h>
#include <signal.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <sys/file.h>
#include <fcntl.h>
#include <string.h>
#include "EpollTcp.h"
#include "NetWorkDefine.h"
#include "config.h"

CEpollTcp tEpollTcp;

static void sig_usr(int nSigNo)
{
	CMessageCompileCompleteNotify tCompileCompleteNotify;
	tCompileCompleteNotify.ID = emS2CNotify_CompileComplete;
	tCompileCompleteNotify.Size= sizeof(CMessageCompileCompleteNotify);

	int tFd = tEpollTcp.mEpollEventFd;
	if ( tFd < 0 || tFd > MAX_SOCKET_NUM )
	{
		return;
	}

	CTcpSocket* pSocket = tEpollTcp.mpTcpSocket[tFd];
	tEpollTcp.OnMessage( &tCompileCompleteNotify, tFd );
}

void ignore_pipe()
{
	struct sigaction sig;
	sig.sa_handler = SIG_IGN;
	sig.sa_flags = 0;
	sigemptyset(&sig.sa_mask);
	sigaction(SIGPIPE, &sig, NULL);
}

// 设置描述符不能被继承
int SetFdNotInherited(int nFd)
{
	int nFlags = fcntl(nFd, F_GETFD, 0);
	fcntl(nFd, F_SETFD, nFlags | FD_CLOEXEC);	
}

int init_daemon(void)
{
	// 没看到这个文件？
	int lock_fd = open("./restartlock.lock", O_RDWR | O_CREAT, 0640);
	if(lock_fd < 0)
	{
		printf("Open Lock File Failed\n");
		exit(0);
	}

	// flock 函数：
	// 依参数operation所指定的方式对参数fd所指的文件做各种锁定或解除锁定的动作。
	// 此函数只能锁定整个文件，无法锁定文件的某一区域。
	// LOCK_EX : 建立互斥锁定。一个文件同时只有一个互斥锁定
	// LOCK_NB : 无法建立锁定时，此操作可不被阻断，马上返回进程
	int ret = flock( lock_fd, LOCK_EX | LOCK_NB );
	if(ret < 0)
	{
		printf("Lock File Failed, The Tool is already Running!\n");
		exit(0);
	}
	SetFdNotInherited(lock_fd);


	pid_t pid = 0;
	if(pid = fork() != 0)
	{
		exit(0);
	}
	setsid();

	signal(SIGINT, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
	signal(SIGTERM, SIG_IGN);

	ignore_pipe();

	if((pid = fork()) != 0)
	{
		exit(0);
	}

	umask(0);
	return 0;
}


int main(int argc, char** argv)
{
	if ( signal( SIGUSR1, sig_usr ) == SIG_ERR )
	{
		printf("can't catch SIG_USR1\n");
	}

	// 先屏蔽 SIG_USR1，等 tEpollTcp 初始化后再解除屏蔽
	sigset_t tNewMask, tOldMask;
	sigemptyset( &tNewMask );
	sigaddset( &tNewMask, SIGUSR1 );

	if ( sigprocmask( SIG_BLOCK, &tNewMask, &tOldMask ) < 0 )
	{
		printf("SIG_BLOCK error !\n");
	}

	// 版本信息
	if(argc > 1)
	{
		if(strcasecmp(argv[1], "-v") == 0)
		{
			printf("RestartTool debug %s build at %s %s\n", "1.0", __DATE__, __TIME__);
			exit(0);
		}
	}

	bool bDaemon = false;
	for(int i = 1; i < argc; ++ i)
	{
		if(strcasecmp(argv[i], "-D") == 0)
		{
			bDaemon = true;
		}

	}

	if(bDaemon)
	{
	    init_daemon();
	}

	tEpollTcp.Init();
	tEpollTcp.InitEpoll();

	char szConfigSection[MAX_STR_NUM] = {0}; 
	snprintf(szConfigSection, sizeof(szConfigSection), "Port");
	char szPort[MAX_STR_NUM] = {0};
	tEpollTcp.mConfigFile.GetItemValue(szConfigSection, "port", szPort, sizeof(szPort));
	int tzPort = atoi( szPort );

	tEpollTcp.Listen(tzPort);

	// 解除屏蔽
	if ( sigprocmask( SIG_SETMASK, &tOldMask, NULL ) < 0 )
	{
		printf("SIG_UNBLOCK error !\n");
	}

	while(true)
	{
		tEpollTcp.CheckServerRestart();
		tEpollTcp.ReadInput();
	}
}
