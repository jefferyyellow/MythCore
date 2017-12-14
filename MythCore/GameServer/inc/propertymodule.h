#ifndef __PROPERTYMODULE_H__
#define __PROPERTYMODULE_H__
#include "singleton.h"
#include "logicmodule.h"
#include "gmcommandmanager.h"
#include "messagefactory.h"
#include "timemanager.h"

using namespace Myth;
class CEntityPlayer;
class CDBResponse;
class CPropertyModule : public CLogicModule, public CSingleton < CPropertyModule >
{
	friend class CSingleton < CPropertyModule >;
private:
	CPropertyModule();
	virtual ~CPropertyModule()
	{
	}

public:
	/// ʱ�亯��
	virtual		void OnTimer(unsigned int nTickOffset);

public:
	void		onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage);

public:
	/// ���������Ϣ
	void			onLoadPlayerInfo(CDBResponse& rResponse);
	/// ������һ�������
	void			onLoadPlayerBaseProperty(CDBResponse& rResponse);
	/// ������Լ������
	void			onLoadComplete(CEntityPlayer* pPlayer);
	/// ��Ҵ���
	void			savePlayer(CEntityPlayer* pPlayer);
	/// ���������Ϣ
	void			savePlayerInfo(CEntityPlayer* pPlayer);
	/// ������һ�������
	void			savePlayerBaseProperty(CEntityPlayer* pPlayer);
	/// ��Ҵ������
	void			onSavePlayerComplete(CEntityPlayer* pPlayer);
	/// ����뿪��Ϸ
	void			onPlayerLeaveGame(CEntityPlayer* pPlayer);

public:
	CGMCommandManager&		getGMCmdManager(){return mGMCmdManager;}

private:
	/// GM��������
	void		onGMCommandRequest(CEntityPlayer* pPlayer, Message* pMessage);
	/// ����뿪��Ϸ������
	void		onLeaveGameRequest(CEntityPlayer* pPlayer, Message* pMessage);
	
private:
	CGMCommandManager		mGMCmdManager;
	CAutoResetTimer			mSavePlayerTimer;
};
#endif