#ifndef __SCENEJOB_H__
#define __SCENEJOB_H__
#include "job.h"
#include "sharememory.h"
#include "socketstream.h"
#include "messagefactory.h"
#include "singleton.h"
#include "logintype.h"
#include "bytestream.h"
#include "timemanager.h"
#include <map>
#include <list>

using namespace Myth;

#define PIPE_SIZE					((int)0x1000000)	/*�ڴ�ܵ��Ĵ�С*/
#define MAX_SOCKET_BUFF_SIZE		4096				// Socket��������С
#define CHECK_NEW_DAY_INTERVAL		30000				// 30����һ���µ�һ��
enum EmServerState
{
	emServerStateInit		= 0,	// ��ʼ״̬
	emServerStateLaunch		= 1,	// ����״̬
	emServerStateRun		= 2,	// ��Ϸ״̬
	emServerStateExit		= 3,	// �˳�״̬
};

class CEntityPlayer;
class CLogicModule;
class CEntity;
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
	CSceneJob();
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
	// ״̬�٣���ʱ����״̬����
	void		doInit();
	void		doLaunch();
	void		doRun();
	void		doExit();

public:
	void		launchServer();
	bool		checkLaunch();
	void		launchComplete();
	void		exitServer();
	void		newDayCome();
	void		newWeekCome();
	void		createPlayer(CEntityPlayer* pPlayer);
	void		destroyPlayer(CEntityPlayer* pPlayer);
	void		timer(unsigned int nTickOffset);

public:
	void		checkNewDayCome();

public:
	/// ����ǰ����Ϣ
	void		send2Player(CExchangeHead& rExchangeHead, unsigned short nMessageID, Message* pMessage);
	void		send2Player(CEntityPlayer* pPlayer, unsigned short nMessageID, Message* pMessage);
	/// ���������е������Ϣ
	void		send2AllPlayer(unsigned short nMessageID, Message* pMessage);
	/// �Ͽ���ҵ�����
	void		disconnectPlayer(CExchangeHead& rExchangeHead);
	void		disconnectPlayer(CEntityPlayer* pPlayer);
	/// ��¼��һ����ң�ֻ�ǵ�¼У����ɣ����ݻ�û�м�����ɣ�
	bool		onPlayerLogin(CEntityPlayer* pNewPlayer);
	/// �뿪��һ�����
	void		onPlayerLeaveGame(CEntityPlayer* pPlayer);
	/// һ��Socket�Ͽ�
	void		onSocketDisconnect(int nSocketIndex);
	/// �õ����е�����б�
	PLAYER_LIST& getPlayerList(){	return mPlayerList;	}
	/// �õ�������ҵ�socket�б�
	PLAYER_SOCKET_LIST& getPlayerSocketList(){return mPlayerSocketList;};
	/// ͨ����ɫID�õ����
	CEntityPlayer* getPlayerByRoleID(unsigned int nRoleID);
	/// ͨ��SocketIndex�õ����
	CEntityPlayer* getPlayerBySocketIndex(short nSocketIndex);
	/// ���socket index
	bool			addPlayerSocketIndex(short nSocketIndex, int nObjID);
	/// ɾ��socket index
	void			removePlayerSocketIndex(short nSocketIndex);
private:
	void		onTask(CInternalMsg* pMsg);
	/// ����ǰ����Ϣ
	void		processClientMessage();
	/// �ַ�ǰ����Ϣ
	void		dispatchClientMessage(CEntityPlayer* pPlayer, unsigned short nMessageID, Message* pMessage);
	/// ��ʼ�������ڴ�
	bool		initShareMemory();

public:
	/// ������״̬
	EmServerState getServerState() const { return mServerState; }
	void setServerState(EmServerState nValue) { mServerState = nValue; }

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
	/// ������״̬
	EmServerState			mServerState;
	/// �µ�һ�����ʱ��
	CAutoResetTimer			mNewDayTimer;
};
#endif