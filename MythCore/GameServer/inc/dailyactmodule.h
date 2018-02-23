#ifndef __DAILYACTMODULE_H__
#define __DAILYACTMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "messagefactory.h"
#include "dailyactivity.h"

using namespace Myth;
class CEntityPlayer;
// 日常活动模块
class CDailyActModule : public CLogicModule, public CSingleton <CDailyActModule>
{
public:
	typedef vector<CActivityTime> DAILY_ACT_TIME_LIST;
	friend class CSingleton <CDailyActModule> ;
private:
	CDailyActModule();
	~CDailyActModule();

	void init()
	{
        mTimeListIndex = 0;
        mLastMinute = 0;
        mLastTime = 0;
	}
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
	void onActivityStart(int nActivityID);
	void onActivityEnd(int nActivityID);
	void onActivityNotice(int nActivityID);

public:
	void onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage);
	/// 加载活动配置
	void loadActivityConfig(const char* pConfigFile);
	/// 检测已经开启/过时的活动
	void checkPassedActivity();
	/// 检测活动时间
	void checkActivityTime();
	/// 检测时间是否已经过去(注意：ActTime的格式是：1630表示16时30分)
	bool checkTimePassed(int nActTime, tm& rTm);

public:
	void sendActivityStatusNotify(int nActivityID, int nStatus);

public:
	/// autocode,don't edit !!!
    CDailyActivity* getActivity(int nIndex)
    {
        if(nIndex < 0 || nIndex >= emDailyActIDMax)
        {
            return NULL;
        }
        return &mActivity[nIndex];
    }
    void setActivity(int nIndex, CDailyActivity& value)
    {
        if(nIndex < 0 || nIndex >= emDailyActIDMax)
        {
            return;
        }
        mActivity[nIndex] = value;
    }

    DAILY_ACT_TIME_LIST& getTimeList(){ return mTimeList;}

    unsigned int getTimeListIndex(){ return mTimeListIndex;}
    void setTimeListIndex(unsigned int value){ mTimeListIndex = value;}

    int getLastMinute(){ return mLastMinute;}
    void setLastMinute(int value){ mLastMinute = value;}

    time_t getLastTime(){ return mLastTime;}
    void setLastTime(time_t value){ mLastTime = value;}
	/// autocode,don't edit !!!

	///
private:
	CDailyActivity		mActivity[emDailyActIDMax];
	DAILY_ACT_TIME_LIST	mTimeList;
	/// 当前已经处理到的时间列表索引
	unsigned int		mTimeListIndex;
	/// 每分钟检测一下
	int					mLastMinute;
	/// 每秒检测一下
	time_t				mLastTime;
};
#endif