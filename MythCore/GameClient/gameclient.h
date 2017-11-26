#ifndef __GAMECLIENT_H__
#define __GAMECLIENT_H__
#include "log.h"
#include "tcpsocket.h"
#include "selectmodel.h"
#include "google/protobuf/message.h"
using namespace Myth;
typedef ::google::protobuf::Message Message;

#define MAX_SOCKET_BUFF_SIZE		4096				// Socket缓冲区大小
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
	void		LoginServer();


private:
	void		onMessageLoginResponse(Message* pMessage);

private:
	CLog*					mDefaultLog;
	CTcpSocket				mTcpSocket;
	CSelectModel			mSelectModel;
	byte					mTcpRecData[MAX_SOCKET_BUFF_SIZE];
	byte					mBuffer[MAX_SOCKET_BUFF_SIZE];
};
#endif