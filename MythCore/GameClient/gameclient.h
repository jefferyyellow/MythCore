#ifndef __GAMECLIENT_H__
#define __GAMECLIENT_H__
#include "log.h"
#include "tcpsocket.h"
#include "selectmodel.h"
#include "google/protobuf/message.h"
using namespace Myth;
typedef ::google::protobuf::Message Message;

#define MAX_SOCKET_BUFF_SIZE		4096				// Socket��������С
class CGameClient
{
public:
	CGameClient();
	~CGameClient(){}

public:
	/// ��ʼ��
	bool		init();
	/// ��ʼ��־
	bool		initLog();
	/// ����
	void		run();
	/// �����������Ϣ
	void		processServerMessage();
	/// ������Ϣ��������
	void		sendMessage(unsigned short uMessageID, Message* pMessage);

	/// ���յ���������Ϣ
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