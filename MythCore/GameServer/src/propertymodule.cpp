#include "propertymodule.h"
#include "propertyunit.h"
#include "entityplayer.h"
#include "propertymodule.hxx.pb.h"
#include "scenejob.h"
#include "locallogjob.h"
#include "dbmessage.h"
/// 时间函数
void CPropertyModule::OnTimer(unsigned int nTickOffset)
{

}

void CPropertyModule::onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage)
{
	switch (nMessageID)
	{
		case ID_C2S_REQUEST_GM_COMMAND:
		{
			onMessageGMCommandRequest(pPlayer, pMessage);
			break;
		}
		default:
			break;
	}
}




// GM命令请求
void CPropertyModule::onMessageGMCommandRequest(CEntityPlayer* pPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL == pPlayer || NULL == pMessage, return);

	CGMCommandRequest* pGMCommandRequest = reinterpret_cast<CGMCommandRequest*>(pMessage);
	if (NULL == pGMCommandRequest)
	{
		return;
	}

	const std::string& rCommondString = pGMCommandRequest->commandstring();
	StrTokens tTokens = strSplit(rCommondString, " ");
	MYTH_ASSERT(tTokens.size() >= 1, return);

	const char* pCommandName = tTokens[0].c_str();
	tTokens.erase(tTokens.begin());
	mGMCmdManager.excuteCommand(pCommandName, tTokens, pPlayer);
}

/// 加载玩家信息
void CPropertyModule::onLoadPlayerInfo(CDBResponse& rResponse)
{
	CEntityPlayer* pPlayer = (CEntityPlayer*)CObjPool::Inst()->getObj(rResponse.mParam1);
	if (NULL == pPlayer)
	{
		return;
	}
	// 	select role_name, level, exp, vip_level, vip_exp, money, diamond from PlayerRole WHERE role_id=RoleID;
	
	rResponse.getString(pPlayer->getName(), PLAYER_NAME_LENGTH - 1);
	pPlayer->GetPropertyUnit().setLevel((byte)rResponse.getShort());
	pPlayer->GetPropertyUnit().setRoleExp(rResponse.getInt64());
	pPlayer->GetPropertyUnit().setVIPLevel(rResponse.getByte());
	pPlayer->GetPropertyUnit().SetVIPExp(rResponse.getInt());


	pPlayer->GetItemUnit().setMoney(rResponse.getInt());
	pPlayer->GetItemUnit().setDiamond(rResponse.getInt());
}

/// 加载玩家基础属性
void CPropertyModule::onLoadPlayerBaseProperty(CDBResponse& rResponse)
{
	CEntityPlayer* pPlayer = (CEntityPlayer*)CObjPool::Inst()->getObj(rResponse.mParam1);
	if (NULL == pPlayer)
	{
		return;
	}

}