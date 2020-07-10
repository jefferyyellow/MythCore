#ifndef __GAMECLIENT_H__
#define __GAMECLIENT_H__
#include "log.h"
#include "tcpsocket.h"
#include "selectmodel.h"
#include "google/protobuf/message.h"
using namespace Myth;
typedef ::google::protobuf::Message Message;

#define MAX_SOCKET_BUFF_SIZE		4096				// Socket缓冲区大小
#define MAX_PLAYER_NAME_LEN			32					// 玩家姓名长度
class CGameClient
{
public:
	CGameClient();
	~CGameClient(){}

public:
	/// 初始化
	bool		init();
	/// 初始日志
	bool		initLog();
	/// 运行
	void		run();
	/// 处理服务器消息
	void		processServerMessage();
	/// 发送消息给服务器
	void		sendMessage(unsigned short uMessageID, Message* pMessage);

	/// 当收到服务器消息
	void		onServerMessage(CTcpSocket* pTcpSocket);

public:
	void		LoginServer(char *pAccountName, char* pRoleName);
	void		sendGMCommandRequest(char* pCommandString);

private:
	void		onMessageLoginResponse(Message* pMessage);
	void		onCreateRoleResponse(Message* pMessage);
private:
	CLog*					mDefaultLog;
	CTcpSocket				mTcpSocket;
	CSelectModel			mSelectModel;
	byte					mTcpRecData[MAX_SOCKET_BUFF_SIZE];
	byte					mBuffer[MAX_SOCKET_BUFF_SIZE];
	unsigned int			mAccountID;
	int						mChannelID;
	int						mServerID;
	unsigned int			mRoleID;

	char					mAccountName[MAX_PLAYER_NAME_LEN];
	char					mRoleName[MAX_PLAYER_NAME_LEN];
};
#endif