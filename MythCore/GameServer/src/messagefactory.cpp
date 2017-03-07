#include "messagefactory.h"
char CMessageFactory::sMsgBuff[MAX_MSG_BUFF_LEN] = { 0 };
#include "loginmessage.hxx.pb.h"

#define CASE_NEW_MSG(msg, msgid)						\
		case msgid:										\
		{												\
			pMessage = new(sMsgBuff) msg;		\
			break;										\
		}

Message* CMessageFactory::createClientMessage(unsigned short nMessageID)
{
	Message* pMessage = NULL;
	switch (nMessageID)
	{
		CASE_NEW_MSG(CMessageLoginRequest, ID_C2S_REQUEST_LOGIN)
		CASE_NEW_MSG(CMessageLoginResponse, ID_S2C_RESPONSE_LOGIN)
		CASE_NEW_MSG(CMessageCreateRoleRequest, ID_C2S_REQUEST_CREATE_ROLE)
		CASE_NEW_MSG(CMessageCreateRoleResponse, ID_S2C_RESPONSE_CREATE_ROLE)
		CASE_NEW_MSG(CMessageEnterSceneRequest, ID_C2S_REQUEST_ENTER_SCENE)
		CASE_NEW_MSG(CMessageEnterSceneResponse, ID_S2C_RESPONSE_ENTER_SCENE)

		default:
			break;
	}

	return pMessage;
}

Message* CMessageFactory::createServerMessage(unsigned short nMessageID)
{
	switch (nMessageID)
	{

		default:
			break;
	}

	return NULL;
}

void CMessageFactory::destroyMessage(Message* pMessage)
{

}