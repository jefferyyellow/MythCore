#include "messagefactory.h"
//char CMessageFactory::sMsgBuff[MAX_MSG_BUFF_LEN] = { 0 };
#include "loginmessage.hxx.pb.h"
#include "propertymodule.hxx.pb.h"
#include "itemmodule.hxx.pb.h"
#include "mapmodule.hxx.pb.h"
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


		/// property module
		CASE_NEW_MSG(CLevelUpNotify, ID_S2C_NOTIYF_LEVEL_UP)
		CASE_NEW_MSG(CGMCommandRequest, ID_C2S_REQUEST_GM_COMMAND)
		CASE_NEW_MSG(CGMCommandResponse, ID_S2C_RESPONSE_GM_COMMAND)


		/// item module
		CASE_NEW_MSG(CMoneyUpdateNotify, ID_S2C_NOTIYF_MONEY_UPDATE)
		CASE_NEW_MSG(CDiamondUpdateNotify, ID_S2C_NOTIYF_DIAMOND_UPDATE)


		/// map module
		CASE_NEW_MSG(CEntityMoveNotify, ID_S2C_NOTIYF_ENTITY_MOVE)
		CASE_NEW_MSG(CCreatePlayerListNotify, ID_S2C_NOTIYF_CREATE_PLAYER_LIST)
		CASE_NEW_MSG(CCreateNPCListNotify, ID_S2C_NOTIYF_CREATE_NPC_LIST)
		CASE_NEW_MSG(CDestroyEntityNotify, ID_S2C_NOTIYF_DESTROY_ENTITY)
		CASE_NEW_MSG(CPlayerMoveRequest, ID_C2S_REQUEST_PLAYER_MOVE)
		CASE_NEW_MSG(CPlayerMoveResponse, ID_S2C_RESPONSE_PLAYER_MOVE)
		CASE_NEW_MSG(CPlayerTeleportRequest, ID_C2S_REQUEST_PLAYER_TELEPORT)
		CASE_NEW_MSG(CPlayerTeleportResponse, ID_S2C_RESPONSE_PLAYER_TELEPORT)

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
