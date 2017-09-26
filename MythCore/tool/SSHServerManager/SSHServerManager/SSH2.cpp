#include "StdAfx.h"
#include "SSH2.h"

const char *keyfile1="~/.ssh/id_rsa.pub";
const char *keyfile2="~/.ssh/id_rsa";
const char *pUserName="username";
const char *pPassWord="password";

static void kbd_callback(const char *name, int name_len,
						 const char *instruction, int instruction_len,
						 int num_prompts,
						 const LIBSSH2_USERAUTH_KBDINT_PROMPT *prompts,
						 LIBSSH2_USERAUTH_KBDINT_RESPONSE *responses,
						 void **abstract)
{
	(void)name;
	(void)name_len;
	(void)instruction;
	(void)instruction_len;
	if (num_prompts == 1) {
		responses[0].text = strdup(pPassWord);
		responses[0].length = strlen(pPassWord);
	}
	(void)prompts;
	(void)abstract;
} /* kbd_callback */





CSSH2::CSSH2(void)
{
	mChannel = NULL;
	mSession = NULL;
	mSocket = INVALID_SOCKET;
}

CSSH2::~CSSH2(void)
{
	if (NULL != mChannel)
	{
		while( libssh2_channel_close(mChannel) == LIBSSH2_ERROR_EAGAIN )
		{
			Sleep(100);
		}
		libssh2_channel_free(mChannel);
		mChannel = NULL;
	}

	if (NULL != mSession)
	{
		libssh2_session_disconnect(mSession, "Normal Shutdown, Thank you for playing");
		libssh2_session_free(mSession);
	}

	if (INVALID_SOCKET != mSocket)
	{
		closesocket(mSocket);
	}
}

bool CSSH2::GlobalInit(int nFlags)
{
	int nResult = libssh2_init(0);
	if (0 != nResult)
	{
		printf("SSH2 initialization failure\n");
		return false;
	}

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2,0), &wsadata);
	return true;
}

void CSSH2::GlobalUnInit()
{
	WSACleanup( );
	libssh2_exit();
}

bool CSSH2::InitSocket(const char* pIP)
{
	mSocket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(22);
	sin.sin_addr.s_addr = inet_addr(pIP);
	if (connect(mSocket, (struct sockaddr*)(&sin),
		sizeof(struct sockaddr_in)) != 0) {
			printf("Init Socket Failure\n");
			return false;
	}

	return true;
}

bool CSSH2::CreateSession(const char* pIP, const char* pUserName, const char* pPassWord)
{
	const char *fingerprint;
//	LIBSSH2_ERROR_EAGAIN
 	// 创建一个会话实例
    mSession = libssh2_session_init();
	// 握手
	int nResult = libssh2_session_handshake(mSession, mSocket);
    if (nResult) {

		if (NULL != mSession)
		{
			libssh2_session_disconnect(mSession, "Normal Shutdown, Thank you for playing");
			libssh2_session_free(mSession);
		}

		if (INVALID_SOCKET != mSocket)
		{
			closesocket(mSocket);
		}

		Sleep(200);
		bool bResult = InitSocket(pIP);
		if (!bResult)
		{
			wchar_t acBuffer[256] = { 0 };
			_snwprintf(acBuffer, sizeof(acBuffer), _T("%s"), s2ws(string(pIP)).c_str(), _T("连接服务器时创建套接字错误!"));
			::AfxMessageBox(acBuffer);
			return false;
		}
		mSession = libssh2_session_init();
		nResult = libssh2_session_handshake(mSession, mSocket);
		if (nResult)
		{
			printf("Failure establishing SSH session\n");
			return false;
		}
    }

    fingerprint = libssh2_hostkey_hash(mSession, LIBSSH2_HOSTKEY_HASH_SHA1);

	// 检查那个认证方法可用
	char *userauthlist;
	int auth_pw;
    userauthlist = libssh2_userauth_list(mSession, pUserName, strlen(pUserName));
    if (strstr(userauthlist, "password") != NULL) {
        auth_pw |= 1;
    }
    if (strstr(userauthlist, "keyboard-interactive") != NULL) {
        auth_pw |= 2;
    }
    if (strstr(userauthlist, "publickey") != NULL) {
        auth_pw |= 4;
    }

    if (auth_pw & 1) {
        if (libssh2_userauth_password(mSession, pUserName, pPassWord)) {
			printf("\tAuthentication by password failed!\n");
			return false;
        } 
		else 
		{
			printf("\tAuthentication by password succeeded.\n");
        }
    } else if (auth_pw & 2) {
        if (libssh2_userauth_keyboard_interactive(mSession, pUserName,
                                                  &kbd_callback) ) {
            printf("\tAuthentication by keyboard-interactive failed!\n");
			return false;
        } 
		else 
		{
           printf("\tAuthentication by keyboard-interactive succeeded.\n");
        }
    } else if (auth_pw & 4) {
        if (libssh2_userauth_publickey_fromfile(mSession, pUserName, keyfile1,
                                                keyfile2, pPassWord)) {
          printf("\tAuthentication by public key failed!\n");
          return false;
        } 
		else 
		{
           printf("\tAuthentication by public key succeeded.\n");
        }
    } else {
		printf("No supported authentication methods found!\n");
		return false;
    }
	return true;
}

bool CSSH2::OpenChannel()
{
	/* Request a shell */
	if (!(mChannel = libssh2_channel_open_session(mSession))) {
		printf("Unable to open a session\n");
		return false;
	}

	return true;
}

bool CSSH2::OperTerm()
{
    if (libssh2_channel_request_pty(mChannel, "vanilla")) {
        printf("Failed requesting pty\n");
        return false;
    }

    if (libssh2_channel_shell(mChannel)) {
        printf("Unable to request shell on allocated pty\n");
        return false;
    }

	libssh2_channel_set_blocking(mChannel, 0);
	return true;
}

int CSSH2::Write(const char* pData)
{
	int nOffset = 0;
	int nLen = strlen(pData);
	int nResult = 0;
	nResult = libssh2_channel_write(mChannel, pData + nOffset, nLen - nOffset);
	nOffset += nResult;
	while(nOffset < nLen)
	{
		nResult = libssh2_channel_write(mChannel, pData + nOffset, nLen - nOffset);
		nOffset += nResult;
	}
	return nOffset;
}

int CSSH2::Read(char* pData, int nDataLen)
{
	if (NULL == pData || nDataLen <= 0)
	{
		return 0;
	}
	int nResult = 0;

	nResult = libssh2_channel_read(mChannel, pData, nDataLen -1);
	return nResult;
}

int CSSH2::Exec(char* pData)
{
	int nResult = 0;
	while( (nResult = libssh2_channel_exec(mChannel, pData)) == LIBSSH2_ERROR_EAGAIN )
	{
		Sleep(100);
	}
	return nResult;
}