#ifndef __PERFMANAGER_H__
#define __PERFMANAGER_H__
#define PERF_MIN_TIME		1000000	// Perf的时间单位是us，1000000us = 1s
#include <map>
#include "singleton.h"
#include "simplelock.h"
using namespace Myth;

class CPerfData
{
public:
	CPerfData()
		:mCallTimes(0), 
		mCostTime(0),
		mMaxTime(0),
		mMinTime(PERF_MIN_TIME)
	{
	}
	~CPerfData()
	{}

public:
	void	addCall(int nElapseTime);
	void	reset();

public:
	// autocode don't edit!!!
    int getCallTimes(){ return mCallTimes;}
    void setCallTimes(int value){ mCallTimes = value;}

    int getCostTime(){ return mCostTime;}
    void setCostTime(int value){ mCostTime = value;}

    int getMaxTime(){ return mMaxTime;}
    void setMaxTime(int value){ mMaxTime = value;}

    int getMinTime(){ return mMinTime;}
    void setMinTime(int value){ mMinTime = value;}
	// end autocode 

private:
	int		mCallTimes;
	int		mCostTime;
	int		mMaxTime;
	int		mMinTime;
};


class CPerfManager : public CSingleton <CPerfManager>
{
public:
	friend class CSingleton<CPerfManager>;
	typedef std::map<std::string, CPerfData> PERF_HASH;
public:
	CPerfManager()
	{}
	~CPerfManager()
	{}

public:
	CPerfData&	getPerfData(std::string strName){return mPerfHash[strName];}

public:
	PERF_HASH&		getPerfHash(){return mPerfHash;}
	CSimpleLock&	getLock(){return mLock;}
private:
	PERF_HASH	mPerfHash;
	CSimpleLock	mLock;
};


// 使用Perf的功能,线上版本将__USE_PERF_FUNC__禁用
#ifdef __USE_PERF_FUNC__
#define PERF_FUNC(funcname, func)														\
	do																					\
	{																					\
		CSimpleLock& tPerfLock = CPerfManager::Inst()->getLock();						\
		tPerfLock.lock();																\
		CPerfData& rPerfData = CPerfManager::Inst()->getPerfData(funcname);				\
		CClockTime tTime;																\
		tTime.start();																	\
		func;																			\
		tTime.end();																	\
		rPerfData.addCall((int)tTime.getInterval());									\
		tPerfLock.unlock();																\
	}while (0);																			\
	

#define PERF_FUNC_RETURN(funcname, func, result)										\
	do																					\
	{																					\
		CSimpleLock& tPerfLock = CPerfManager::Inst()->getLock();						\
		tPerfLock.lock();																\
		CPerfData& rPerfData = CPerfManager::Inst()->getPerfData(funcname);				\
		CClockTime tTime;																\
		tTime.start();																	\
		result = func;																	\
		tTime.end();																	\
		rPerfData.addCall((int)tTime.getInterval());									\
		tPerfLock.unlock();																\
	} while (0);																		\


#define PERF_FUNC_ELAPSE(funcname, func, elapse)										\
	do																					\
	{																					\
		CSimpleLock& tPerfLock = CPerfManager::Inst()->getLock();						\
		tPerfLock.lock();																\
		CPerfData& rPerfData = CPerfManager::Inst()->getPerfData(funcname);				\
		CClockTime tTime;																\
		tTime.start();																	\
		func;																			\
		tTime.end();																	\
		rPerfData.addCall((int)tTime.getInterval());									\
		elapse = (int)tTime.getInterval();												\
		tPerfLock.unlock();																\
	}while (0);																			\

#else
	
#define PERF_FUNC(funcname, func)	func;
#define PERF_FUNC_RETURN(funcname, func, result) func;
#define PERF_FUNC_ELAPSE(funcname, func, elapse) func;

#endif

#endif