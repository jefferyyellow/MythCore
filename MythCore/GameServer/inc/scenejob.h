#ifndef __SCENEJOB_H__
#define __SCENEJOB_H__
#include "job.h"
#include "sharememory.h"
#include "socketstream.h"
#include "messagefactory.h"
#include "singleton.h"
#include "logintype.h"
#include "dbmessage.h"
#include <map>
using namespace Myth;

#define PIPE_SIZE					((int)0x1000000)	/*�ڴ�ܵ��Ĵ�С*/
#define MAX_SOCKET_BUFF_SIZE		4096				// Socket��������С

class CEntityPlayer;
class CSceneJob : public CJob < 1000, 100 >, public CSingleton<CSceneJob>
{
	friend class CSingleton<CSceneJob>;
	/// ����role id,ֵ��obj id
	typedef std::map<uint32, uint32> PLAYER_LIST;
	/// ����socket index, ֵ��obj id
	typedef std::map<uint32, uint32> PLAYER_SOCKET_LIST;

public:
	CSceneJob(){}
	~CSceneJob(){}

public:
	bool		init(int nDBBuffSize);

public:
	virtual void doing(uint32 uParam);

public:
	/// ����ǰ����Ϣ
	void		sendClientMessage(CExchangeHead& rExchangeHead, unsigned short nMessageID, Message* pMessage);
	void		sendClientMessage(CEntityPlayer* pPlayer, unsigned short nMessageID, Message* pMessage);

private:
	void		onTask(CInternalMsg* pMsg);
	/// ����ǰ����Ϣ
	void		processClientMessage();
	/// �ַ�ǰ����Ϣ
	void		dispatchClientMessage(CEntityPlayer* pPlayer, unsigned short nMessageID, Message* pMessage);
	/// ��ʼ�������ڴ�
	bool		initShareMemory();
	/// ѹ��DB����
	void		pushDBData(uint8* pData, int nDataLength);
	/// ȡ��DB����
	void		popDBData(uint8* pData, int &rLength);
	/// ѹ��DB����
	void		pushDBTask(int nPlayerID, int nSessionType, int nParam1, int nParam2, char* pSql, ...);

private:
	CShareMemory*			mShareMemory;
	CSocketStream*			mTcp2ServerMemory;
	CSocketStream*			mServer2TcpMemory;
	char					mBuffer[MAX_SOCKET_BUFF_SIZE + sizeof(CExchangeHead)];

	PLAYER_LIST				mPlayerList; 
	PLAYER_SOCKET_LIST		mPlayerSocketList;


	CSocketStream			mDBStream;
	uint8*					mDBBuffer;
	CSimpleLock				mDBStreamLock;

	CDBRequest				mDBRequest;
	CDBResponse				mDBResponse;
};
#endif