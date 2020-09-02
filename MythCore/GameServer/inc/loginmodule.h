#ifndef __LOGINMODULE_H__
#define __LOGINMODULE_H__
#include "singleton.h"
#include "messagefactory.h"
#include "servercommon.h"
#include "logintype.h"
#include "logicmodule.h"
#include "timeutility.h"
#include <map>

using namespace Myth;
class CDBResponse;
class CLoginPlayer;
#define  MAKE_LOGIN_KEY(AccountID, ChannelID, ServerID) ((ChannelID << 48) | (ServerID << 32) | AccountID)
class CLoginModule : public CLogicModule, public CSingleton < CLoginModule >
{
	friend class CSingleton < CLoginModule > ;
public:
	typedef std::map<short, int> LOGIN_LIST;
	typedef std::map<uint64, int> VERIFY_LIST;

private:
	CLoginModule();
	~CLoginModule();
	void init();

public:
	/// ����������
	virtual void onLaunchServer();
	/// ������ɼ��
	virtual bool onCheckLaunch();
	/// �����������ɹ�
	virtual void onLaunchComplete();
	/// �˳�������
	virtual void onExitServer();
	/// ��һ�쵽��
	virtual void onNewDayCome();
	/// ��һ�ܵ���
	virtual void onNewWeekCome();
	/// ����ʵ��
	virtual void onCreatePlayer(CEntityPlayer* pPlayer);
	/// ����ʵ��
	virtual void onDestroyPlayer(CEntityPlayer* pPlayer);
	/// ʱ�亯��
	virtual	void onTimer(unsigned int nTickOffset);
	/// ���������ļ�
	virtual	void onLoadConfig();

public:
	/// ����ͻ�����Ϣ
	void		onClientMessage(CExchangeHead& rExchangeHead, unsigned int nMessageID, Message* pMessage);
	/// �������ݿ���Ϣ
	void		OnDBMessage(CDBResponse* pMsg);
	/// ����ȴ���ҽ�����Ϸ
	void		processWaitEnterGame(CLoginPlayer* pLoginPlayer, Message* pMessage);
	///  һ��Socket�Ͽ�
	void		onSocketDisconnect(int nSocketIndex);
	/// �������ݿ�������ɫID���ֶ�
	void		loadAllocateRoleId();
	/// �������ݿ�������ɫID�Ĵ���
	void		onLoadAllocateRoleId(CDBResponse& rResponse);
	/// ��������У������
	bool		addVerifyPlayer(short nChannelId, short nServerId, int nAccountId, int nLoginObjId);
	/// �Ƿ�������У������
	bool		checkVerifyPlayer(short nChannelId, short nServerId, int nAccountId);
	/// ɾ������У������
	void		removeVerifyPlayer(short nChannelId, short nServerId, int nAccountId);
	/// �õ���¼�������
	CLoginPlayer* getLoginPlayer(int nSocketIndex);

	unsigned int getBeginRoleID(int nServerId)
	{
		if (nServerId <= 0 || nServerId > MAX_ROLE_PER_SERVER)
		{
			return 0;
		}
		return (nServerId - 1) * MAX_SERVER_ID + 1;
	}

	unsigned int allocateRoleID(int nServerId);

public:
	unsigned int getAllocateRoleId(int nIndex)
	{
		if (nIndex < 0 || nIndex >= MAX_SERVER_ID)
		{
			return 0;
		}

		return mAllocateRoleId[nIndex];
	}

	void		setAllocateRoleId(int nIndex, int nRoleID)
	{
		if (nIndex < 0 || nIndex >= MAX_SERVER_ID)
		{
			return;
		}
		mAllocateRoleId[nIndex] = nRoleID;
	}
private:
	LOGIN_LIST			mLoginList;						// ��¼�б�
	VERIFY_LIST			mVerifyList;					// У���б�
	CAutoResetTimer		mLoginCheckTime;				// ��¼У��ʱ��
	unsigned int		mAllocateRoleId[MAX_SERVER_ID];	// ��ɫID������
	bool				mCheckResult;
};
#endif