#ifndef __DBMODULE_H__
#define __DBMODULE_H__
#include "dbmessage.h"
#include "loginmodule.h"
#include "messagefactory.h"
class CDBModule : public CLogicModule, public CSingleton < CDBModule >
{
	friend class CSingleton < CDBModule >;
private:
	CDBModule(){};
	~CDBModule(){};

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
	/// 时间
	virtual void onTimer(unsigned int nTickOffset);
	/// 加载配置文件
	virtual	void onLoadConfig();

public:
	/// 压入DB任务
	void			pushDBTask(int nJobID, int nPlayerID, int nSessionType, int nParam1, int nParam2, const char* pSql, ...);
	void			pushDBTask(int nJobID, int nPlayerID, int nSessionType, int nParam1, int nParam2, Message* pMessage);
	CDBResponse*	getDBResponse(){return &mDBResponse;}
	void			onDBSession();
private:

private:
	CDBRequest		mDBRequest;
	CDBResponse		mDBResponse;
};
#endif