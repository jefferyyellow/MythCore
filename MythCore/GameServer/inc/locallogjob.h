#ifndef __LOCALLOGJOB_H__
#define __LOCALLOGJOB_H__
#include "job.h"



#define MYTH_ASSERT(a, fail_handle)  \
	if (!(a))   \
	{         \
		LogLocalLog(emLogType_Error, __FILE__, __LINE__, __FUNCTION__, "ASSERT:%s ", #a); \
		fail_handle; \
	}

#define MYTH_ASSERT_INFO( a, fail_handle, fmt, ... )														\
	if(!(a))																									\
	{																										\
		char log_str[1024] = {0};																			\
		snprintf(log_str, sizeof(log_str) - 1, fmt, ##__VA_ARGS__ );										\
		LogLocalLog(emLogType_Error, __FILE__, __LINE__, __FUNCTION__, "ASSERT:%s %s", #a, log_str);		\
		fail_handle;																						\
	}

enum EmLogType
{
	emLogType_Debug			= 0,
	emLogType_Warn			= 1,
	emLogType_Info			= 3,
	emLogType_Error			= 4,
};

// 默认的info没有加位置参数，如果需要使用位置参数，请使用LOG_INFO_L
#define LOG_INFO(fmt, ... )				LogNoLocation(emLogType_Info, fmt, ##__VA_ARGS__ )
#define LOG_WARN(fmt, ... )				LogLocalLog(emLogType_Warn, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__ )
#define LOG_ERROR(fmt, ... )			LogLocalLog(emLogType_Error, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__ )
#define LOG_DEBUG(logname, fmt, ... )	LogLocalDebugLog(logname, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__ )

// L:location 定位/位置
#define LOG_INFO_L(fmt, ... )			LogLocalLog(emLogType_Info, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__ )


// 下面这个名字是随便起的，目的是和别的变量名不会重复，原来是这样
// #define LOG_FILE(pFile)	( ( strrchr( pFile, '\\' ) == NULL ) ? pFile :  strrchr( pFile, '\\' ) + 1 )
// 为了避免多次调用strchar，所以改成先用一个全局变量承载

#ifdef MYTH_OS_WINDOWS
#define LOG_FILE(pFile)	( ( (pFileName = strrchr( pFile, '\\' )) == NULL ) ? pFile : pFileName + 1)
#else
#define LOG_FILE(pFile)	( ((pFileName = strrchr( pFile, '/' )) == NULL ) ? pFile : pFileName + 1)
#endif // MYTH_OS_WINDOWS


extern void		LogLocalLog(EmLogType eLogType, const char* pFile, int nLine, const char* pFunction, const char* pFormat, ...);
extern void		LogLocalDebugLog(const char* pLogName, const char* pFile, int nLine, const char* pFunction, const char* pFormat, ...);
extern void		LogNoLocation(EmLogType eLogType, const char* pFormat, ...);

class CIMLocalLogRequest;
class CLocalLogJob : public CJob
{
public:
	CLocalLogJob()
	{
		init();
		setJobType(emJobID_Log);
	}
	~CLocalLogJob(){}
	void init();
	void initAll();
public:
	virtual void doing(int uParam);

private:
	void	OnIMLocalLogRequest(CIMLocalLogRequest* pLogRequest);
	/// 上次刷新的时间
	uint64	mLastTime;
};
#endif