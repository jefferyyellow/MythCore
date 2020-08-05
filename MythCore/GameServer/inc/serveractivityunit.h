#ifndef __SERVERACTIVITYUNIT_H__
#define __SERVERACTIVITYUNIT_H__
#include "serveractivitytype.h"
#include "playersubunit.h"
#include "servercommon.h"
/// 活动单元
class CServerActivityUnit : public CPlayerSubUnit
{
public:
	CServerActivityUnit(CEntityPlayer& rPlayer)
		:CPlayerSubUnit(rPlayer)
	{
		memset(mActTime, 0, sizeof(mActTime));
	}
	~CServerActivityUnit()
	{
		init();
	}

	void	init()
	{
       
	}

public:
	void				dailyRefresh();
	// autocode
	// end autocode
private:
	// 记录每个活动的开始时间
	int					mActTime[MAX_SERVER_ACT_NUM];
};
#endif