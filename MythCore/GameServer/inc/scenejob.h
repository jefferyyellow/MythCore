#ifndef __SCENEJOB_H__
#define __SCENEJOB_H__
#include "job.h"
#include "sharememory.h"
#include "socketstream.h"
#include "messagefactory.h"
#include "singleton.h"
#include "logintype.h"
#include "bytestream.h"
#include <map>
using namespace Myth;

#define PIPE_SIZE					((int)0x1000000)	/*�ڴ�ܵ��Ĵ�С*/
#define MAX_SOCKET_BUFF_SIZE		4096				// Socket��������С

class CEntityPlayer;
class CSceneJob : public CJob < 1000, 100 >, public CSingleton<CSceneJob>
{
	friend class CSingleton<CSceneJob>;
	/// ����role id,ֵ��obj id
	typedef std::map<int, int> PLAYER_LIST;
	/// ����socket index, ֵ��obj id
	typedef std::map<int, int> PLAYER_SOCKET_LIST;

public:
	CSceneJob(){}
	~CSceneJob(){}

public:
	bool		init(int nDBBuffSize);

public:
	/// ѹ��DB����
	void		pushBackDBData(byte* pData, int nDataLength);
	/// ȡ��DB����
	void		popUpDBData(byte* pData, int &rLength);
	/// ���DB��
	void		checkDBStream();

public:
	virtual void doing(int uParam);

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

private:
	CShareMemory*			mShareMemory;
	CSocketStream*			mTcp2ServerMemory;
	CSocketStream*			mServer2TcpMemory;
	char					mBuffer[MAX_SOCKET_BUFF_SIZE + sizeof(CExchangeHead)];

	PLAYER_LIST				mPlayerList; 
	PLAYER_SOCKET_LIST		mPlayerSocketList;


	CByteStream				mDBStream;
	byte*					mDBBuffer;
	CSimpleLock				mDBStreamLock;
};
#endif