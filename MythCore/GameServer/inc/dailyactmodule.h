#ifndef __DAILYACTMODULE_H__
#define __DAILYACTMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "messagefactory.h"
#include "dailyactivity.h"

using namespace Myth;
class CEntityPlayer;
// �ճ��ģ��
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
	virtual	void onTimer(unsigned int nTickOffset);

public:
	void onActivityStart(int nActivityID);
	void onActivityEnd(int nActivityID);
	void onActivityNotice(int nActivityID);

public:
	void onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage);
	/// ���ػ����
	void loadActivityConfig(const char* pConfigFile);
	/// ����Ѿ�����/��ʱ�Ļ
	void checkPassedActivity();
	/// ���ʱ��
	void checkActivityTime();
	/// ���ʱ���Ƿ��Ѿ���ȥ(ע�⣺ActTime�ĸ�ʽ�ǣ�1630��ʾ16ʱ30��)
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
	/// ��ǰ�Ѿ�������ʱ���б�����
	unsigned int		mTimeListIndex;
	/// ÿ���Ӽ��һ��
	int					mLastMinute;
	/// ÿ����һ��
	time_t				mLastTime;
};
#endif