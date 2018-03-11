#include "messagefactory.h"
//char CMessageFactory::sMsgBuff[MAX_MSG_BUFF_LEN] = { 0 };
#include "loginmessage.hxx.pb.h"
#include "propertymodule.hxx.pb.h"
#include "chatmodule.hxx.pb.h"
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
		/// login module
		CASE_NEW_MSG(CLoginRequest, ID_C2S_REQUEST_LOGIN)
		CASE_NEW_MSG(CLoginResponse, ID_S2C_RESPONSE_LOGIN)
		CASE_NEW_MSG(CCreateRoleRequest, ID_C2S_REQUEST_CREATE_ROLE)
		CASE_NEW_MSG(CCreateRoleResponse, ID_S2C_RESPONSE_CREATE_ROLE)
		CASE_NEW_MSG(CEnterSceneRequest, ID_C2S_REQUEST_ENTER_SCENE)
		CASE_NEW_MSG(CEnterSceneResponse, ID_S2C_RESPONSE_ENTER_SCENE)

		// chat module
		CASE_NEW_MSG(CChatRequest, ID_C2S_REQUEST_CHAT)
		CASE_NEW_MSG(CChatResponse, ID_S2C_RESPONSE_CHAT)
		CASE_NEW_MSG(CChatNotify, ID_S2C_NOTIFY_CHAT)
		default:
			break;
	}

	return pMessage;
}

Message* CMessageFactory::createServerMessage(unsigned short nMessageID)
{
	//switch (nMessageID)
	//{

	//	default:
	//		break;
	//}

	return NULL;
}

void CMessageFactory::destroyMessage(Message* pMessage)
{

}
