#include "messagefactory.h"
//char CMessageFactory::sMsgBuff[MAX_MSG_BUFF_LEN] = { 0 };
#include "loginmessage.hxx.pb.h"
#include "propertymodule.hxx.pb.h"
#include "itemmodule.hxx.pb.h"
#include "mapmodule.hxx.pb.h"
#include "taskmodule.hxx.pb.h"
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


		/// property module
		CASE_NEW_MSG(CLevelUpNotify, ID_S2C_NOTIYF_LEVEL_UP)
		CASE_NEW_MSG(CGMCommandRequest, ID_C2S_REQUEST_GM_COMMAND)
		CASE_NEW_MSG(CGMCommandResponse, ID_S2C_RESPONSE_GM_COMMAND)
		CASE_NEW_MSG(CObtainExpNotify, ID_S2C_NOTIFY_OBTAIN_EXP)
		CASE_NEW_MSG(CObtainVIPExpNotify, ID_S2C_NOTIFY_OBTAIN_VIP_EXP)
		CASE_NEW_MSG(CLeaveGameRequest, ID_C2S_REQUEST_LEAVE_GAME)
		CASE_NEW_MSG(CLeaveGameResponse, ID_S2C_RESPONSE_LEAVE_GAME)
		CASE_NEW_MSG(CPlayerBaseInfoNotify, ID_S2C_NOTIFY_PLAYER_BASE_INFO)
		CASE_NEW_MSG(CPlayerItemInfoNotify, ID_S2C_NOTIFY_PLAYER_ITEM_INFO)
		CASE_NEW_MSG(CPlayerTaskInfoNotify, ID_S2C_NOTIFY_PLAYER_TASK_INFO)
		CASE_NEW_MSG(CPlayerSkillInfoNotify, ID_S2C_NOTIFY_PLAYER_SKILL_INFO)
		CASE_NEW_MSG(CGetPlayerPropertyRequest, ID_C2S_REQUEST_GET_PLAYER_PROPERTY)
		CASE_NEW_MSG(CGetPlayerPropertyResponse, ID_S2C_RESPONSE_GET_PLAYER_PROPERTY)
		CASE_NEW_MSG(CHeartBeatRequest, ID_C2S_REQUEST_HEART_BEAT)
		CASE_NEW_MSG(CHeartBeatResponse, ID_S2C_RESPONSE_HEART_BEAT)


		/// item module
		CASE_NEW_MSG(CCurrencyUpdateNotify, ID_S2C_NOTIYF_CURRENCY_UPDATE)
		CASE_NEW_MSG(CInsertItemNotify, ID_S2C_NOTIYF_INSERT_ITEM)
		CASE_NEW_MSG(CInsertItemObjNotify, ID_S2C_NOTIYF_INSERT_ITEM_OBJ)
		CASE_NEW_MSG(CRemoveItemNotify, ID_S2C_NOTIYF_REMOVE_ITEM)
		CASE_NEW_MSG(CUseItemRequest, ID_C2S_REQUEST_USE_ITEM)
		CASE_NEW_MSG(CUseItemResponse, ID_S2C_RESPONSE_USE_ITEM)
		CASE_NEW_MSG(CSellItemRequest, ID_C2S_REQUEST_SELL_ITEM)
		CASE_NEW_MSG(CSellItemResponse, ID_S2C_RESPONSE_SELL_ITEM)
		CASE_NEW_MSG(CPurchaseItemRequest, ID_C2S_REQUEST_PURCHASE_ITEM)
		CASE_NEW_MSG(CPurchaseItemResponse, ID_S2C_RESPONSE_PURCHASE_ITEM)
		CASE_NEW_MSG(CEquipItemRequest, ID_C2S_REQUEST_EQUIP_ITEM)
		CASE_NEW_MSG(CEquipItemResponse, ID_S2C_RESPONSE_EQUIP_ITEM)
		CASE_NEW_MSG(CUnEquipItemRequest, ID_C2S_REQUEST_UNEQUIP_ITEM)
		CASE_NEW_MSG(CUnEquipItemResponse, ID_S2C_RESPONSE_UNEQUIP_ITEM)
		CASE_NEW_MSG(CEquipChangeNotify, ID_S2C_NOTIYF_EQUIP_CHANGE)
		CASE_NEW_MSG(CGetShopInfoRequest, ID_C2S_REQUEST_GET_SHOP_INFO)
		CASE_NEW_MSG(CGetShopInfoResponse, ID_S2C_RESPONSE_GET_SHOP_INFO)


		/// map module
		CASE_NEW_MSG(CEntityMoveNotify, ID_S2C_NOTIYF_ENTITY_MOVE)
		CASE_NEW_MSG(CCreatePlayerListNotify, ID_S2C_NOTIYF_CREATE_PLAYER_LIST)
		CASE_NEW_MSG(CCreateNPCListNotify, ID_S2C_NOTIYF_CREATE_NPC_LIST)
		CASE_NEW_MSG(CDestroyEntityNotify, ID_S2C_NOTIYF_DESTROY_ENTITY)
		CASE_NEW_MSG(CPlayerMoveRequest, ID_C2S_REQUEST_PLAYER_MOVE)
		CASE_NEW_MSG(CPlayerMoveResponse, ID_S2C_RESPONSE_PLAYER_MOVE)
		CASE_NEW_MSG(CPlayerTeleportRequest, ID_C2S_REQUEST_PLAYER_TELEPORT)
		CASE_NEW_MSG(CPlayerTeleportResponse, ID_S2C_RESPONSE_PLAYER_TELEPORT)


		// task module
		CASE_NEW_MSG(CAcceptTaskRequest, ID_C2S_REQUEST_ACCEPT_TASK)
		CASE_NEW_MSG(CAcceptTaskResponse, ID_S2C_RESPONSE_ACCEPT_TASK)
		CASE_NEW_MSG(CSubmitTaskRequest, ID_C2S_REQUEST_SUBMIT_TASK)
		CASE_NEW_MSG(CSubmitTaskResponse, ID_S2C_RESPONSE_SUBMIT_TASK)
		CASE_NEW_MSG(CAbortTaskRequest, ID_C2S_REQUEST_ABORT_TASK)
		CASE_NEW_MSG(CAbortTaskResponse, ID_S2C_RESPONSE_ABORT_TASK)
		CASE_NEW_MSG(CUpdateTaskProcessNotify, ID_S2C_NOTIFY_UPDATE_TASK_PROCESS)

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
