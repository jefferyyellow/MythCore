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
class CLoginPlayer;
class CEntityTimer;
class CSceneJob : public CJob < 100, 100 >, public CSingleton<CSceneJob>
{
public:
	friend class CSingleton<CSceneJob>;
	/// ����role id,ֵ��obj id
	typedef std::map<unsigned int, int> PLAYER_LIST;
	/// ����socket index, ֵ��obj id
	typedef std::map<unsigned int, int> PLAYER_SOCKET_LIST;
	/// �߼�ģ���б�
	typedef std::list<CLogicModule*> LOGIC_MODULE_LIST;
private:
	CSceneJob();
	~CSceneJob(){}

public:
	void		init()
	{
        mShareMemory = NULL;
        mTcp2ServerMemory = NULL;
        mServer2TcpMemory = NULL;
        mDBBuffer = NULL;
        mLastTimerTick = 0;
        mLastTime = 0;
        mServerState = emServerStateInit;
        mMorningTime = 0;
	}
	bool		initBase(int nDBBuffSize);
	void		clearBase();

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
	void		onNewDayCome();

public:
	void		send2Player(CLoginPlayer* pLoginPlayer, unsigned short nMessageID, Message* pMessage);
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
	/// ͨ����ɫID�õ����
	CEntityPlayer* getPlayerByRoleID(unsigned int nRoleID);
	/// ͨ��SocketIndex�õ����
	CEntityPlayer* getPlayerBySocketIndex(short nSocketIndex);
	/// ���socket index
	bool			addPlayerSocketIndex(short nSocketIndex, int nObjID);
	/// ɾ��socket index
	void			removePlayerSocketIndex(short nSocketIndex);
	/// ��¼perf��־
	void			logPerf();
private:
	/// ����ǰ����Ϣ
	void		send2Player(CExchangeHead& rExchangeHead, unsigned short nMessageID, Message* pMessage);
	void		onTask(CInternalMsg* pMsg);
	/// ����ǰ����Ϣ
	void		processClientMessage();
	/// �ַ�ǰ����Ϣ
	void		dispatchClientMessage(CEntityPlayer* pPlayer, unsigned short nMessageID, Message* pMessage);
	/// ��ʼ�������ڴ�
	bool		initShareMemory();

public:
	/// ��ʱ����ʱ�ص�����
	static void		timeOutCallFunc(CEntityTimer* pTimer);
public:
	tm&				getTmNow(){ return mTmNow; }
	void			setTmNow(time_t tTimeNow)
	{
#ifdef MYTH_OS_WINDOWS
		localtime_s(&mTmNow, &tTimeNow);
#else
		localtime_r(&tTimeNow, &mTmNow);
#endif // MYTH_OS_WINDOWS
	}
public:
	/// autocode don't edit
    CShareMemory* getShareMemory(){ return mShareMemory;}
    void setShareMemory(CShareMemory* value){ mShareMemory = value;}

    CSocketStream* getTcp2ServerMemory(){ return mTcp2ServerMemory;}
    void setTcp2ServerMemory(CSocketStream* value){ mTcp2ServerMemory = value;}

    CSocketStream* getServer2TcpMemory(){ return mServer2TcpMemory;}
    void setServer2TcpMemory(CSocketStream* value){ mServer2TcpMemory = value;}

    PLAYER_LIST& getPlayerList(){ return mPlayerList;}

    PLAYER_SOCKET_LIST& getPlayerSocketList(){ return mPlayerSocketList;}

    CByteStream& getDBStream(){ return mDBStream;}

    byte* getDBBuffer(){ return mDBBuffer;}
    void setDBBuffer(byte* value){ mDBBuffer = value;}

    CSimpleLock& getDBStreamLock(){ return mDBStreamLock;}

    LOGIC_MODULE_LIST& getLogicModuleList(){ return mLogicModuleList;}

    uint64 getLastTimerTick(){ return mLastTimerTick;}
    void setLastTimerTick(uint64 value){ mLastTimerTick = value;}

    time_t getLastTime(){ return mLastTime;}
    void setLastTime(time_t value){ mLastTime = value;}

    EmServerState getServerState(){ return mServerState;}
    void setServerState(EmServerState value){ mServerState = value;}

    time_t getMorningTime(){ return mMorningTime;}
    void setMorningTime(time_t value){ mMorningTime = value;}

    CAutoResetTimer& getMinuteTimer(){ return mMinuteTimer;}
	/// end autocode
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
	/// �ϴ�ˢ�µ�ʱ��
	time_t					mLastTime;
	/// ������״̬,default:emServerStateInit
	EmServerState			mServerState;
	/// ��ǰ��ʱ��
	struct tm				mTmNow;
	/// ָ��ʱ����������ϣ�00��00��00��
	time_t					mMorningTime;
	/// ���ʱ��,�������
	CAutoResetTimer			mMinuteTimer;
};
#endif