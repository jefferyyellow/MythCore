#ifndef __MAILMODULE_H__
#define __MAILMODULE_H__
#include "singleton.h"
#include "logicmodule.h"
#include "commontype.h"
#include "mythlist.h"
#include "mail.h"
using namespace Myth;
class CEntityPlayer;
class CMail;
class CDBResponse;
class PBMailItemList;
class CMailModule : public CLogicModule, public CSingleton<CMailModule>
{
	friend class CSingleton<CMailModule>;
public:
	typedef CMythList<CMail, MAX_PLAYER_MAIL>	MAIL_LIST;

private:
	CMailModule()
	{
		mPlayerMailID = 0;
		mGlobalMailID = 0;
		mGlobalLoadComplete = false;
	}
	virtual ~CMailModule(){}

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
	/// 加载最大的邮件ID
	void		loadMaxMailID();
	/// 加载最大的邮件ID回调
	void		onLoadMaxMailID(CDBResponse& rResponse);
	/// 加载最大的全局邮件ID
	void		loadMaxGlobalMailID();
	/// 加载最大的全局邮件ID回调
	void		onLoadMaxGlobalMailID(CDBResponse& rResponse);

	/// 删除过期的邮件
	void		deleteExpireMail();
	/// 删除过期的全局邮件
	void		deleteExpireGlobalMail();
	/// 全局邮件列表
	void		sendGlobalMail(CMail& rMail);
	/// 给玩家发送邮件
	void		sendPlayerMail(uint nRoleID, CMail& rMail);
	/// 给玩家发送邮件
	void		sendPlayerMail(CEntityPlayer& rPlayer, CMail& rMail);
	/// 保存邮件到数据库
	void		saveMail2DB(uint nRoleID, CMail& rMail);
	/// 保存全局邮件到数据库
	void		saveGlobalMail2DB(CMail& rMail);
	/// 加载邮件
	void		loadPlayerMail(int nRoleID);
	/// 加载邮件数据库回调
	void		onLoadPlayerMail(CDBResponse& rResponse);
	/// 加载全局邮件
	void		loadGlobalMail();
	/// 加载全局邮件回调
	void		onLoadGlobalMail(CDBResponse& rResponse);
	/// 加载邮件道具列表
	void		loadMailItemList(CMail& rMail, PBMailItemList& pbMailItemList);
	/// 保存邮件道具列表
	void		saveMailItemList(CMail& rMail, PBMailItemList* pbMailItemList);
	/// 删除全局邮件
	void		deleteGlobalMail2DB(uint nMailID);
	/// 给玩家发全局邮件
	void		giveAllGlobalMail(CEntityPlayer& rPlayer);
	/// 给所以的玩家发全局邮件
	void		giveAllPlayerGlobalMail(CMail& rMail);
private:
	/// 玩家邮件ID生成器
	uint		mPlayerMailID;
	/// 全局邮件ID生成器
	uint		mGlobalMailID;
	/// 是否是否加载全局邮件完成
	bool		mGlobalLoadComplete;
	/// 全局邮件列表
	MAIL_LIST	mGlobalList;
};
#endif