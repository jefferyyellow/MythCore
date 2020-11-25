#ifndef __RANKMODULE_H__
#define __RANKMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "messagefactory.h"
#include "ranktype.h"
#include "commontype.h"
#include "template.h"
#include "logintype.h"
#include <map>

using namespace Myth;

class CEntityPlayer;
class CInternalMsg;

class CPlayerRankInfo
{
public:
	CPlayerRankInfo()
	{}
	~CPlayerRankInfo()
	{}

public:
	/// 名字
	char*		getName(){return mName;}
	void		setName(const char* pName)
	{
		if (NULL == pName)
		{
			return;
		}
		strncpy(mName, pName, sizeof(MAX_PLAYER_NAME_LEN) - 1);
	}

	/// 头像ID
	uint		getHeadID(){return mHeadID;}
	void		setHeadID(uint nHeadID){mHeadID = nHeadID;}

private:
	/// 玩家名称
	char		mName[MAX_PLAYER_NAME_LEN];
	/// 头像ID
	uint		mHeadID;
};

class CRankModule : public CLogicModule, public CSingleton<CRankModule>
{
	friend class CSingleton<CRankModule>;
	typedef std::map<uint, CPlayerRankInfo>	HASH_RANK_INFO;

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
	virtual void onCreatePlayer(CEntityPlayer& rPlayer);
	/// 销毁实体
	virtual void onDestroyPlayer(CEntityPlayer& rPlayer);
	/// 时间函数
	virtual	void onTimer(unsigned int nTickOffset);
	/// 加载配置文件
	virtual	void onLoadConfig();

public:
	void onClientMessage(CEntityPlayer& rPlayer, unsigned int nMessageID, Message* pMessage);
	void onGetRankInfoRequest(CEntityPlayer& rPlayer, Message* pMessage);

public:
	void onIMGetRankInfoResponse(CInternalMsg* pIMMsg);
	void onIMUpdateRankResponse(CInternalMsg* pIMMsg);

public:
	/// 更新玩家的排行榜
	void updateRoleRank(EmRankType eType, CEntityPlayer& rPlayer, int nValue);
	/// 更新玩家排行榜信息
	void updateRoleInfo(CEntityPlayer& rPlayer);
	/// 更新玩家排行榜信息
	void updateRoleInfo(CEntityPlayer& rPlayer, CPlayerRankInfo& rRankInfo);
	/// 增加玩家排行榜信息
	void addRoleInfo(CEntityPlayer& rPlayer);
private:
	HASH_RANK_INFO	mHashRankInfo;
};
#endif