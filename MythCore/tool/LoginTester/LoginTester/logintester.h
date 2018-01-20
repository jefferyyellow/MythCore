#ifndef __LOGINTESTER_H__
#define __LOGINTESTER_H__
#include "commontype.h"
#include "tcpsocket.h"
#include "selectmodel.h"
#include "timemanager.h"
#include "google/protobuf/message.h"
#include "log.h"
#include "singleton.h"
#include <map>
#define MAX_SOCKET_NUM				4096				// 最大的socket数目
#define MAX_SOCKET_BUFF_SIZE		40960				// Socket缓冲区大小
using namespace Myth;
typedef ::google::protobuf::Message Message;
class CPlayer;
class CLoginTester :  public CSingleton <CLoginTester>
{
public:
	friend class CSingleton <CLoginTester> ;

public:
	// key: socket fd, value: CPlayer*
	typedef std::map<int, CPlayer*> PLAYER_SOCKET_LIST;
private:
	CLoginTester();
	~CLoginTester();

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
	void		sendMessage(int nTcpIndex, unsigned short uMessageID, Message* pMessage);

	/// 当收到服务器消息
	void		onServerMessage(CTcpSocket* pTcpSocket, CPlayer* pPlayer);
	/// 创建消息
	Message*	createMessage(unsigned short nMessageID);
public:
	void		newPlayer(const char* pAccountName, int nChannelID, int nServerID);

	void		setServerIP(const char* pServerIP)
	{
		strncpy(mServerIP, pServerIP, sizeof(mServerIP) - 1);
		mServerIP[sizeof(mServerIP) - 1] = '\0';
	}
	char*		getServerIP(){return mServerIP;}

	void		setServerPort(int nServerPort){mServerPort = nServerPort;}
	int			getServerPort(){return mServerPort;}

	uint64		getTickCount();
	void		onTime(int nElapseTime);
	
public:
	CLog*					mDefaultLog;
	CSelectModel			mSelectModel;
	char					mServerIP[32];
	int						mServerPort;
	byte					mBuffer[MAX_SOCKET_BUFF_SIZE];
	CTcpSocket				mTcpSocket[MAX_SOCKET_NUM];
	PLAYER_SOCKET_LIST		mPlayerSocketList;
	uint64					mLastTickCount;
	CAutoResetTimer			mResetTimer;
	int						mAccountNameCount;
};
#endif