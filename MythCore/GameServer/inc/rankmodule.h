#ifndef __RANKMODULE_H__
#define __RANKMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "ranktype.h"
#include "rank.h"
#include "messagefactory.h"
using namespace Myth;

typedef CRank<MAX_RANK_NUM> CRankList;
class CEntityPlayer;
class CRankModule : public CLogicModule, public CSingleton<CRankModule>
{
	friend class CSingleton<CRankModule>;
private:
	CRankModule();
	~CRankModule();

public:
	/// 启动服务器
	virtual void onLaunchServer();
	/// 启动完成检查
	virtual bool onCheckLaunch();
	/// 服务器启动成功
	virtual void onLaunchComplete();
	/// 退出服务器
	virtual void onExitServer();
	/// 新一天到来
	virtual void onNewDayCome();
	/// 新一周到来
	virtual void onNewWeekCome();
	/// 建立实体
	virtual void onCreatePlayer(CEntityPlayer* pPlayer);
	/// 销毁实体
	virtual void onDestroyPlayer(CEntityPlayer* pPlayer);
	/// 时间函数
	virtual	void onTimer(unsigned int nTickOffset);

public:
	void onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage);
	void onGetRankInfoRequest(CEntityPlayer* pPlayer, Message* pMessage);
	void sendGetRankInfoResponse();

public:
	// 更新玩家的排行榜
	void updateRoleRank(EmRankType eType, CEntityPlayer* pPlayer, int nValue);

private:
	CRankList		mRankList[emRankTypeMax];
};
#endif