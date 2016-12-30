#include "dbjob.h"
#include "internalmsg.h"
#include "dbmanager.h"

CDBJob::CDBJob()
{

}

CDBJob::~CDBJob()
{

}

void CDBJob::doing(uint32 uParam)
{
	while (true)
	{
		CInternalMsg* pIMMsg = CDBManager::Inst()->popTask();
		// Èç¹û
		if (NULL == pIMMsg)
		{
			break;
		}
		onTask(pIMMsg);
	}
}


void CDBJob::onTask(CInternalMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return;
	}
	switch (pMsg->getMsgID())
	{
		case IM_REQUEST_PLAYER_LOGIN:
		{
			CIMPlayerLoginRequest* pIMLoginRequest = reinterpret_cast<CIMPlayerLoginRequest*>(pMsg);
			int nChannelID = pIMLoginRequest->mChannelID;
			break;
		}
		default:
			break;
	}
}