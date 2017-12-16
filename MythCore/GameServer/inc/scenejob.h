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
#include <list>
using namespace Myth;

#define PIPE_SIZE					((int)0x1000000)	/*�ڴ�ܵ��Ĵ�С*/
#define MAX_SOCKET_BUFF_SIZE		4096				// Socket��������С

class CEntityPlayer;
class CLogicModule;
class CSceneJob : public CJob < 1000, 100 >, public CSingleton<CSceneJob>
{
public:
	friend class CSingleton<CSceneJob>;
	/// ����role id,ֵ��obj id
	typedef std::map<unsigned int, int> PLAYER_LIST;
	/// ����socket index, ֵ��obj id
	typedef std::map<unsigned int, int> PLAYER_SOCKET_LIST;
	/// �߼�ģ���б�
	typedef std::list<CLogicModule*> LOGIC_MODULE_LIST;
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
	void		send2Player(CExchangeHead& rExchangeHead, unsigned short nMessageID, Message* pMessage);
	void		send2Player(CEntityPlayer* pPlayer, unsigned short nMessageID, Message* pMessage);
	/// �Ͽ���ҵ�����
	void		disconnectPlayer(CExchangeHead& rExchangeHead);
	void		disconnectPlayer(CEntityPlayer* pPlayer);
	/// ��¼��һ����ң�ֻ�ǵ�¼У����ɣ����ݻ�û�м�����ɣ�
	bool		onPlayerLogin(CEntityPlayer* pNewPlayer);
	/// �뿪��һ�����
	void		onPlayerLeaveGame(CEntityPlayer* pPlayer);
	/// ʱ�亯��
	void		OnTimer(unsigned int nTickOffset);
	/// �õ����е�����б�
	PLAYER_LIST& getPlayerList(){	return mPlayerList;	}
	/// �õ�������ҵ�socket�б�
	PLAYER_SOCKET_LIST& getPlayerSocketList(){return mPlayerSocketList;};
	/// ͨ����ɫID�õ����
	CEntityPlayer* getPlayerByRoleID(unsigned int nRoleID);
	/// ͨ��SocketIndex�õ����
	CEntityPlayer* getPlayerBySocketIndex(short nSocketIndex);
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
	LOGIC_MODULE_LIST		mLogicModuleList;
	/// ��һ��ˢ�¼�ʱ����ʱ��
	uint64					mLastTimerTick;
};
#endif