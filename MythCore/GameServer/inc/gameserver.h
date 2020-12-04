#ifndef __GAMESERVER_H__
#define __GAMESERVER_H__

#include "log.h"
#include "singleton.h"
#include "timeutility.h"
#include "gameservernet.h"
using namespace Myth;

class CJobManager;

enum EmServerState
{
	emServerStateInit		= 0,	// ��ʼ״̬
	emServerStateLaunch		= 1,	// ����״̬
	emServerStateRun		= 2,	// ��Ϸ״̬
	emServerStateExit		= 3,	// �˳�״̬
};

class CGameServer : public CSingleton<CGameServer>
{
	friend class CSingleton<CGameServer>;
public:
	/// ����role id,ֵ��obj id
	typedef std::map<unsigned int, int> PLAYER_LIST;
	/// ����socket index, ֵ��obj id
	typedef std::map<unsigned int, int> PLAYER_SOCKET_LIST;

private:
	CGameServer();
	~CGameServer(){}

public:
	/// ��ʼ��
	void		init();
	/// ��ʼ��
	bool		initAll();
	/// ��ʼ��־
	bool		initLog();
	/// ��ʼ�߼�ģ��
	bool		initLogicModule();
	/// ��ʼ����Ϸ��̬����
	bool		initStaticData();
	/// ��ʼ������������
	bool		initNet();

	/// ����
	void		run();

	/// ��ʼΪ�˳���׼��
	void		clear();
	/// ������־��Դ
	void		clearLog();
	/// �����߼�ģ��
	void		clearLogicModule();
	/// ����̬����
	void		clearStaticData();
	/// �˳�
	void		exit();
	/// ��¼���е�Perf�ļ�¼
	void		logPerf();

public:
	short		getServerID() const { return mServerID; }
	void		setServerID(short nValue) { mServerID = nValue; }

	bool		getExit() const { return mExit; }
	void		setExit(bool nValue) { mExit = nValue; }

	time_t		GetGameTimeOffset() const { return mGameTimeOffset; }
	void		SetGameTimeOffset(time_t nValue) { mGameTimeOffset = nValue; }



	///// �Ͽ���ҵ�����
	//void		disconnectPlayer(CExchangeHead& rExchangeHead);
	//void		disconnectPlayer(CEntityPlayer& rPlayer);
	///// ��¼��һ����ң�ֻ�ǵ�¼У����ɣ����ݻ�û�м�����ɣ�
	//bool		onPlayerLogin(CEntityPlayer* pNewPlayer);
	///// �뿪��һ�����
	//void		destroyPlayerObject(CEntityPlayer& rPlayer);
	///// һ��Socket�Ͽ�
	//void		onSocketDisconnect(int nSocketIndex);
	///// ͨ����ɫID�õ����
	//CEntityPlayer* getPlayerByRoleID(unsigned int nRoleID);
	///// ͨ��SocketIndex�õ����
	//CEntityPlayer* getPlayerBySocketIndex(short nSocketIndex);
	///// ���socket index
	//bool			addPlayerSocketIndex(short nSocketIndex, int nObjID);
	///// ɾ��socket index
	//void			removePlayerSocketIndex(short nSocketIndex);

private:
	CGameServerNet			mGameServerNet;
#ifdef __DEBUG__
	CLog*					mDefaultLog;
#endif
	CJobManager*			mpJobManager;
	// ��������Ӧ�ķ�����ID����������ΨһID
	short					mServerID;
	// �˳�״̬
	bool					mExit;
	/// ���ʱ��,�������
	CAutoResetTimer			mMinuteTimer;
	/// �ϴ�ˢ�µ�ʱ��
	time_t					mLastTime;
	/// ��Ϸ������ƫ��ʱ��
	time_t					mGameTimeOffset;

	PLAYER_LIST				mPlayerList;
	PLAYER_SOCKET_LIST		mPlayerSocketList;

};
#endif