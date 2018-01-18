#ifndef __LOGMANAGER_H__
#define __LOGMANAGER_H__
#include "commontype.h"
#include "singleton.h"

#include <map>
#include <stdarg.h>
#include "log.h"
namespace Myth
{
#ifdef __DEBUG__
#define MAX_LOG_BUFFER_NUM	10240 
#else
#define MAX_LOG_BUFFER_NUM	1024 
#endif

//#	define LOG_INFO(fmt, ... )				CLogManager::Inst()->LogInfoMessage(fmt, ##__VA_ARGS__ )
//#	define LOG_WARN(fmt, ... )				CLogManager::Inst()->LogWarnMessage(fmt, ##__VA_ARGS__ )
//#	define LOG_ERROR(fmt, ... )				CLogManager::Inst()->LogErrorMessage(fmt, ##__VA_ARGS__ )
//#	define LOG_DEBUG(logname, fmt, ... )	CLogManager::Inst()->LogDebugMessage(logname, fmt, ##__VA_ARGS__ )

#include "log.h"
	class CLogManager : public CSingleton < CLogManager >
	{
		friend class CSingleton < CLogManager > ;
	public:
		typedef std::map<std::string, CLog*> DebugLogMap;
	private:
		CLogManager()
		{
			Init();
		}
		~CLogManager()
		{
			Finial();
		}

	public:
		int 	Init();
		void	Finial();

	public:
		CLog&		GetErrorLog(){ return mErrorLog; }

		CLog&		GetInfoLog(){ return mInfoLog; }

		CLog&		GetWarnLog(){ return mWarnLog; }

	public:
		void		LogErrorMessageFormat(const char* pFormat, ...);
		void		LogErrorMessage(const char* pLogContent);
		void		LogInfoMessageFormat(const char* pFormat, ...);
		void		LogInfoMessage(const char* pLogContent);
		void		LogWarnMessageFormat(const char* pFormat, ...);
		void		LogWarnMessage(const char* pLogContent);

	public:
		/// get exist debug log by name
		CLog*		GetDebugLog(const char* pName);
		/// add debug log with name
		void		AddDebugLog(CLog* pLog, const char* pName);
		/// log debug message
		void		LogDebugMessageFormat(const char* pLogName, const char* pFormat, ...);
		void		LogDebugMessage(const char* pLogName, const char* pLogContent);

	public:
		void		FormatDateString(char* pDateBuffer, int nBuffSize);

		void 		FormatLogMessage(char* pDataBuffer, int nBuffSize, const char* pLogTypeName, const char* pFormat, va_list vaList);
		void 		FormatLogMessage(char* pDataBuffer, int nBuffSize, const char* pLogTypeName, const char* pLogContent);

	public:
		tm&				getTmNow(){ return mTmNow; }
		void			setTmNow(time_t tTimeNow)
		{
#ifdef MYTH_OS_WINDOWS
			localtime_s(&mTmNow, &tTimeNow);
#else
			localtime_r(&tTimeNow, &mTmNow);
#endif // MYTH_OS_WINDOWS
		}

	private:
		/// error log
		CLog		mErrorLog;
		/// info log
		CLog 		mInfoLog;
		/// warning log
		CLog		mWarnLog;
		/// debug log
		DebugLogMap	mDebugLog;
		/// 当前的时间
		struct tm				mTmNow;
	};
}

#endif
