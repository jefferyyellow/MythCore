#include "taskmodule.h"
#include "taskmodule.hxx.pb.h"
#include "entityplayer.h"
void CTaskModule::OnTimer(unsigned int nTickOffset)
{

}

void CTaskModule::onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage)
{
	if (NULL == pPlayer)
	{
		return;
	}
	switch (nMessageID)
	{
		case ID_C2S_REQUEST_ACCEPT_TASK:
		{
			pPlayer->GetTaskUnit().onAcceptTaskRequest(pMessage);
			break;
		}
		default:
			break;
	}
}