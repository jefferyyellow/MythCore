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
	// GM��������
	void		onMessageGMCommandRequest(CEntityPlayer* pPlayer, Message* pMessage);

public:
	/// ���������Ϣ
	void			onLoadPlayerInfo(CDBResponse& rResponse);
	/// ������һ�������
	void			onLoadPlayerBaseProperty(CDBResponse& rResponse);
	/// ������Լ������
	void			onLoadComplete(CEntityPlayer* pPlayer);
	/// ��Ҵ���
	void		SavePlayer(CEntityPlayer* pPlayer);
	/// ���������Ϣ
	void		SavePlayerInfo(CEntityPlayer* pPlayer);
	/// ������һ�������
	void		SavePlayerBaseProperty(CEntityPlayer* pPlayer);

public:
	CGMCommandManager&		getGMCmdManager(){return mGMCmdManager;}

private:
	CGMCommandManager		mGMCmdManager;
	CAutoResetTimer			mSavePlayerTimer;
};
#endif