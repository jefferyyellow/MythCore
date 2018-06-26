#include "platlog.h"
#include "json.h"
#include "timemanager.h"
#include "internalmsgpool.h"
#include "gameserver.h"
using namespace Myth;

void LogPlatLog(const char* pLogName, Json::Value& tLogValue)
{
	tLogValue["log_name"] = pLogName;
	CIMPlatLogRequest* pRequest = static_cast<CIMPlatLogRequest*>(CInternalMsgPool::Inst()->allocMsg(IM_REQUEST_PLAT_LOG));
	if (NULL == pRequest)
	{
		return;
	}
	Json::StreamWriterBuilder wbuilder;
	strncpy(pRequest->mContent, Json::writeString(wbuilder, tLogValue).c_str(), sizeof(pRequest->mContent) - 1);
	CGameServer::Inst()->pushTask(emTaskType_Plat, pRequest);
}

void LogServerStartComplete()
{
	Json::Value tPlatLog;
	tPlatLog["time"] = CTimeManager::Inst()->getCurrTime();

	LogPlatLog("log_server_start", tPlatLog);
}