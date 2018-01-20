#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "tcpsocket.h"
using namespace Myth;
#include "google/protobuf/message.h"
typedef ::google::protobuf::Message Message;

#define MAX_PLAYER_SOCKET_BUFF 4096
#define MAX_NAME_LENGTH 32
class CPlayer
{
public:
	CPlayer()
	{
		mAccountID = 0;
		mChannelID = 0;
		mServerID = 0;
		mRoleID = 0;
		mAccountName[0] = '\0';
		mTcpIndex = -1;
	}
	~CPlayer()
	{
	}

public:
	void			onServerMessage(short nMessageID, Message* pMessage);
	void			loginServer();
	void			onMessageLoginResponse(Message* pMessage);
	void			onCreateRoleResponse(Message* pMessage);

public:
	unsigned int	getAccountID(){return mAccountID;}
	void			setAccountID(unsigned int nAccountID){mAccountID = nAccountID;}

	int				getChannelID(){return mChannelID;}
	void			setChannelID(int nChannelID){mChannelID = nChannelID;}

	int				getServerID(){return mServerID;}
	void			setServerID(int nServerID){mServerID = nServerID;}

	unsigned int	getRoleID(){return mRoleID;}
	void			setRoleID(int nRoleID){mRoleID = nRoleID;}

	char*			getAccountName(){return mAccountName;}
	void			setAccountName(const char* pName)
	{
		strncpy(mAccountName, pName, sizeof(mAccountName) - 1);
		mAccountName[sizeof(mAccountName) - 1] = '\0';
	}

	int				getTcpIndex() const { return mTcpIndex; }
	void			setTcpIndex(int nValue) { mTcpIndex = nValue; }

private:
	unsigned int			mAccountID;
	int						mChannelID;
	int						mServerID;
	unsigned int			mRoleID;
	char					mAccountName[MAX_NAME_LENGTH];
	int						mTcpIndex;
};
#endif