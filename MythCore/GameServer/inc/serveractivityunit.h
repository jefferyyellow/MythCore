#ifndef __SERVERACTIVITYUNIT_H__
#define __SERVERACTIVITYUNIT_H__
#include "serveractivitytype.h"
#include "playersubunit.h"
#include "servercommon.h"
/// ���Ԫ
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
	// ��¼ÿ����Ŀ�ʼʱ��
	int					mActTime[MAX_SERVER_ACT_NUM];
};
#endif