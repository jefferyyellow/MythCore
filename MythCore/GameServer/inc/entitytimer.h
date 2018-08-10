#ifndef __ENTITYTIMER_H__
#define __ENTITYTIMER_H__
#include "obj.h"
#include "commontype.h"
#include <list>
#define PARAM_NUM	4	// 参数数目

class CEntityTimer :  public CObj
{
public:
	/// 主人角色ID
	int		mOwerObjID;
	/// 毫秒数
	int		mElapseMilliSec;
	///	原始的毫秒数
	int		mInitialMilliSec;
	/// 参数数目
	int		mParam[PARAM_NUM];
	/// 秒级计时器还是毫秒级的计时器
	bool	mSecElapse;
	/// 调用次数
	short	mCallTimes;
	/// 模块
	int	mModule;
};

class CTimerList
{
public:
	typedef void TimeOutFunc(CEntityTimer* pTimer);
public:
	CTimerList()
	{}
	~CTimerList()
	{}

public:
	/// 设置毫秒级计时器(不能超过24天)
	int		setTimer(int nOwerObjID, int nModule, int nMilliSec, const int* pParam, int nParamNum, int nCallTimes);
	/// 清理指定的计时器
	void	clearTimer(int nObjID);
	/// 清理所有的计时器
	void	clearAllTimer();
	/// 流逝时间
	void	elapseTime(unsigned int nTickOffset);

public:
	static void	setTimeOutFunc(TimeOutFunc* pFunc){mpTimeOutFunc = pFunc;}
	int		getTimeListSize(){return mTimerList.size();}

private:
	/// 为了速度，直接存了指针地址
	std::list<CEntityTimer*>	mTimerList;
	/// 时间到了的回调函数,default:ignore
	static	TimeOutFunc*		mpTimeOutFunc;
};
#endif