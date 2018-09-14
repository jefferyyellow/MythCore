#ifndef __ENTITYTIMER_H__
#define __ENTITYTIMER_H__
#include "obj.h"
#include "commontype.h"
#include <list>
#define PARAM_NUM	4	// ������Ŀ

class CEntityTimer :  public CObj
{
public:
	/// ���˽�ɫID
	int		mOwerObjID;
	/// ������ʱ��
	uint64	mTickCount;
	///	ԭʼ�ĺ�����
	int		mInitialMilliSec;
	/// ������Ŀ
	int		mParam[PARAM_NUM];
	/// ���ô���
	short	mCallTimes;
	/// ģ��
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
	/// ���ú��뼶��ʱ��(���ܳ���24��)
	int		setTimer(int nOwerObjID, int nModule, int nMilliSec, const int* pParam, int nParamNum, int nCallTimes);
	/// ����ָ���ļ�ʱ��
	void	clearTimer(int nObjID);
	/// �������еļ�ʱ��
	void	clearAllTimer();
	/// ����ʱ��
	void	update(uint64 nNowTickCount);

public:
	static void	setTimeOutFunc(TimeOutFunc* pFunc){mpTimeOutFunc = pFunc;}
	int		getTimeListSize(){return mTimerList.size();}

private:
	/// Ϊ���ٶȣ�ֱ�Ӵ���ָ���ַ
	std::list<CEntityTimer*>	mTimerList;
	/// ʱ�䵽�˵Ļص�����,default:ignore
	static	TimeOutFunc*		mpTimeOutFunc;
};
#endif