#include "platlog.h"
#include "include/json.h"
#include "internalmsgpool.h"
#include "gameserver.h"
#include "jobmanager.h"
#include "timemanager.h"
using namespace Myth;

void LogPlatLog(const char* pLogName, Json::Value& tLogValue)
{
	tLogValue["log_name"] = pLogName;
	CIMPlatLogRequest* pRequest = new CIMPlatLogRequest;
	if (NULL == pRequest)
	{
		return;
	}
	Json::StreamWriterBuilder wbuilder;
	strncpy(pRequest->mContent, Json::writeString(wbuilder, tLogValue).c_str(), sizeof(pRequest->mContent) - 1);
	CJobManager::Inst()->pushTaskByType(emJobTaskType_Plat, pRequest);
}

void LogServerStartComplete()
{
	Json::Value tPlatLog(Json::nullValue);
	tPlatLog["time"] = (Json::Value::UInt)(CTimeManager::Inst()->getCurrTime());

	LogPlatLog("log_server_start", tPlatLog);
}