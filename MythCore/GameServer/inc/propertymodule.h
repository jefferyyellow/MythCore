#ifndef __PROPERTYMODULE_H__
#define __PROPERTYMODULE_H__
#include "singleton.h"
#include "logicmodule.h"
#include "gmcommandmanager.h"
#include "messagefactory.h"

using namespace Myth;
class CEntityPlayer;
class CPropertyModule : public CLogicModule, public CSingleton < CPropertyModule >
{
	friend class CSingleton < CPropertyModule >;
private:
	CPropertyModule()
	{
	}
	~CPropertyModule()
	{
	}

public:
	/// ʱ�亯��
	virtual		void OnTimer(unsigned int nTickOffset);

public:
	void		onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage);

public:
	/// ��һ�þ���
	void		obtainExp(CEntityPlayer* pPlayer, int nExp);
	/// ��ҵȼ�����
	void		onPlayerLevelUp(CEntityPlayer* pPlayer, int nLevel);

public:
	CGMCommandManager&		getGMCmdManager(){return mGMCmdManager;}

private:
	CGMCommandManager		mGMCmdManager;
};
#endif