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
	typedef vector<CDailyActTime> DAILY_ACT_TIME_LIST;
	friend class CSingleton <CDailyActModule> ;
private:
	CDailyActModule();
	~CDailyActModule();

	void init()
	{
		for (int i = 0; i < MAX_DAILY_ACT_ID; ++i)
		{
			mActivity[i] = NULL;
		}
        mTimeListIndex = 0;
        mLastTime = 0;
	}
	
	void clear()
	{
		mLastTime = 0;
		mTimeListIndex = 0;
		mTimeList.clear();
		for (int i = 0; i < MAX_DAILY_ACT_ID; ++i)
		{
			if (NULL != mActivity[i])
			{
				delete mActivity[i];
				mActivity[i] = NULL;
			}
		}
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
	/// ���������ļ�
	virtual	void onLoadConfig();
	/// ���¼��������ļ�
	virtual	void onReloadConfig();

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
        if(nIndex < 0 || nIndex >= MAX_DAILY_ACT_ID)
        {
            return NULL;
        }
        return mActivity[nIndex];
    }
    void setActivity(int nIndex, CDailyActivity* value)
    {
        if(nIndex < 0 || nIndex >= MAX_DAILY_ACT_ID)
        {
            return;
        }
        mActivity[nIndex] = value;
    }

    DAILY_ACT_TIME_LIST& getTimeList(){ return mTimeList;}

    unsigned int getTimeListIndex(){ return mTimeListIndex;}
    void setTimeListIndex(unsigned int value){ mTimeListIndex = value;}

    time_t getLastTime(){ return mLastTime;}
    void setLastTime(time_t value){ mLastTime = value;}
	/// autocode,don't edit !!!

private:
	CDailyActivity*		mActivity[MAX_DAILY_ACT_ID];
	DAILY_ACT_TIME_LIST	mTimeList;
	/// ��ǰ�Ѿ�������ʱ���б�����
	unsigned int		mTimeListIndex;
	/// ÿ����һ��
	time_t				mLastTime;
};
#endif