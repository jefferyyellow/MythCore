#ifndef __LOCALLOGJOB_H__
#define __LOCALLOGJOB_H__
#include "job.h"

enum EmLogType
{
	emLogType_Debug			= 0,
	emLogType_Warn			= 1,
	emLogType_Info			= 3,
	emLogType_Error			= 4,
};


#	define LOG_INFO(fmt, ... )				LogLocalLog(emLogType_Info, fmt, ##__VA_ARGS__ )
#	define LOG_WARN(fmt, ... )				LogLocalLog(emLogType_Warn, fmt, ##__VA_ARGS__ )
#	define LOG_ERROR(fmt, ... )				LogLocalLog(emLogType_Error, fmt, ##__VA_ARGS__ )
#	define LOG_DEBUG(logname, fmt, ... )	LogLocalDebugLog(logname, fmt, ##__VA_ARGS__ )

extern void		LogLocalLog(EmLogType eLogType, const char* pFormat, ...);
extern void		LogLocalDebugLog(const char* pLogName, const char* pFormat, ...);

using namespace Myth;
class CIMLocalLogRequest;
class CLocalLogJob : public CJob<100, 10>
{
public:
	CLocalLogJob(){}
	~CLocalLogJob(){}

public:
	virtual void doing(uint32 uParam);

private:
	void	OnIMLocalLogRequest(CIMLocalLogRequest* pLogRequest);
};
#endif