#pragma once
#include <WinSock2.h>
class CSSH2
{
public:
	CSSH2(void);
	~CSSH2(void);

public:
	static bool GlobalInit(int nFlags);
	static void GlobalUnInit();

public:
	bool InitSocket(const char* pIP);
	bool CreateSession(const char* pIP, const char* pUserName, const char* pPassWord);
	bool OpenChannel();
	bool OperTerm();
	int Write(const char* pData);
	int Read(char* pData, int nDataLen);
	int Exec(char* pData);

	public:
	SOCKET mSocket;
	LIBSSH2_SESSION *mSession;
	LIBSSH2_CHANNEL *mChannel;
};
