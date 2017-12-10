#ifndef __PROPERTYMODULE_H__
#define __PROPERTYMODULE_H__
#include "singleton.h"
#include "logicmodule.h"
#include "gmcommandmanager.h"
#include "messagefactory.h"

using namespace Myth;
class CEntityPlayer;
class CDBResponse;
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
	/// 时间函数
	virtual		void OnTimer(unsigned int nTickOffset);

public:
	void		onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage);

public:
	// GM命令请求
	void		onMessageGMCommandRequest(CEntityPlayer* pPlayer, Message* pMessage);

public:
	/// 加载玩家信息
	void			onLoadPlayerInfo(CDBResponse& rResponse);
	/// 加载玩家基础属性
	void			onLoadPlayerBaseProperty(CDBResponse& rResponse);
	/// 玩家属性加载完成
	void			onLoadComplete(CEntityPlayer* pPlayer);
public:
	CGMCommandManager&		getGMCmdManager(){return mGMCmdManager;}

private:
	CGMCommandManager		mGMCmdManager;
};
#endif