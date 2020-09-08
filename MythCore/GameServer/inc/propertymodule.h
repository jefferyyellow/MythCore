#ifndef __PROPERTYMODULE_H__
#define __PROPERTYMODULE_H__
#include "singleton.h"
#include "logicmodule.h"
#include "gmcommandmanager.h"
#include "messagefactory.h"
#include "timeutility.h"
#include "logintype.h"
using namespace Myth;
class CEntityPlayer;
class CEntity;
class CDBResponse;
class CEntityCreator;
class CPropertyModule : public CLogicModule, public CSingleton < CPropertyModule >
{
	friend class CSingleton < CPropertyModule >;
private:
	CPropertyModule();
	virtual ~CPropertyModule()
	{
	}

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
	virtual		void onTimer(unsigned int nTickOffset);
	/// ���������ļ�
	virtual		void onLoadConfig();

public:
	void		onClientMessage(CEntityPlayer& rPlayer, unsigned int nMessageID, Message* pMessage);

public:
	void			checkSavePlayerTimer();
public:
	/// ���������Ϣ
	void			onLoadPlayerInfo(CDBResponse& rResponse);
	/// ������һ�������
	void			onLoadPlayerBaseProperty(CDBResponse& rResponse);
	/// ������Լ������
	void			onLoadComplete(CEntityPlayer* pPlayer);
public:
	/// ��Ҵ���
	void			savePlayer(CEntityPlayer& rPlayer);
	/// ���������Ϣ
	void			savePlayerInfo(CEntityPlayer& rPlayer);
	/// ������һ�������
	void			savePlayerBaseProperty(CEntityPlayer& rPlayer);
	/// ��Ҵ������
	void			onSavePlayerComplete(CEntityPlayer& rPlayer);

public:
	/// ����뿪��Ϸ
	void			playerLeaveGame(CEntityPlayer& rPlayer, EmLeaveReason eReason);
	/// ɾ�����ʵ��
	void			destroyPlayer(CEntityPlayer& rPlayer);
	/// ����Ҵ���
	void			setNewPlayerValue(CEntityPlayer* pPlayer);
	/// ����ʵ���Ĵ���
	void			onCreateEntity(CEntityCreator* pCreator, CEntity* pEntity);
private:
	/// GM��������
	void		onGMCommandRequest(CEntityPlayer& rPlayer, Message* pMessage);
	/// ����뿪��Ϸ������
	void		onLeaveGameRequest(CEntityPlayer& rPlayer, Message* pMessage);
	/// ������һ�����Ϣ֪ͨ
	void		sendPlayerBaseInfoNotify(CEntityPlayer* pPlayer);
	/// ������ҵ�����Ϣ֪ͨ
	void		sendPlayerItemInfoNotify(CEntityPlayer* pPlayer);
	/// �������������Ϣ֪ͨ
	void		sendPlayerTaskInfoNotify(CEntityPlayer* pPlayer);
	/// ��Ҽ�����Ϣ֪ͨ
	void		sendPlayerSkillInfoNotify(CEntityPlayer* pPlayer);
	//// �õ������������
	void		onGetPlayerPropertyRequest(CEntityPlayer* pPlayer, Message* pMessage);

public:
	CGMCommandManager&		getGMCmdManager(){ return mGMCmdManager; }
	/// �߳��������
	void		kickAllPlayer();

private:
	CGMCommandManager		mGMCmdManager;
	CAutoResetTimer			mSavePlayerTimer;
};
#endif