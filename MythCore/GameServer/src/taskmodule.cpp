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
			pPlayer->getTaskUnit().onAcceptTaskRequest(pMessage);
			break;
		}
		case ID_C2S_REQUEST_SUBMIT_TASK:
		{
			pPlayer->getTaskUnit().onSubmitTaskRequest(pMessage);
			break;
		}
		case ID_C2S_REQUEST_ABORT_TASK:
		{
			pPlayer->getTaskUnit().onAbortTaskRequest(pMessage);
			break;
		}
		default:
			break;
	}
}


// 加载配置
void CTaskModule::loadAllTaskConfig(const char* pPath)
{
	//MYTH_ASSERT(pPath == NULL, return);
	//CDir tDir(pPath);
	//const char *tpName = tDir.file();
	//while (tpName != NULL)
	//{
	//	char tBuff[128] = { 0 };
	//	snprintf(tBuff, sizeof(tBuff), "%s/%s", pPath, tpName);
	//	LoadTaskConfig(tBuff);
	//	tpName = tDir.file();
	//}
}

// 加载单个任务配置
void CTaskModule::loadTaskConfig(const char* pFileName)
{
	//if (NULL == pFileName)
	//{
	//	return;
	//}

	//SK_ASSERT(pFileName != NULL, return);
	//TiXmlDocument tDoc;
	//SK_ASSERT_STR(tDoc.LoadFile(pFileName) == true, return, "Load task config error: %s", pFileName);
	//TiXmlElement *tpRoot = tDoc.FirstChildElement();
	//SK_ASSERT_STR(tpRoot != NULL, return, "Load task config error, has no root node: %s", pFileName);

	//TiXmlElement* pIDEle = tpRoot->FirstChildElement("id");
	//int nTaskID = 0;
	//if (NULL != pIDEle)
	//{
	//	const char* pID = pIDEle->GetText();
	//	if (NULL != pID)
	//	{
	//		nTaskID = atoi(pID);
	//	}
	//}

	//if (nTaskID <= 0 || nTaskID >= MAX_TASK_ID)
	//{
	//	LOG_ERROR("pro", "[ %s : %d ] [ %s ] Task id is invalid: TaskID: %d", __SK_FILE__, __LINE__, __FUNCTION__, nTaskID);
	//	return;
	//}
	//CTaskConfig* pNewTaskConfig = new CTaskConfig;
	//if (NULL != mTaskConfig[nTaskID])
	//{
	//	delete mTaskConfig[nTaskID];
	//}
	//mTaskConfig[nTaskID] = pNewTaskConfig;

	//int nGiveNPCID = 0;
	//TiXmlElement* pGiveNPCEle = tpRoot->FirstChildElement("give_npc");
	//if (NULL != pGiveNPCEle)
	//{
	//	const char* pNPCID = pGiveNPCEle->GetText();
	//	if (NULL != pNPCID)
	//	{
	//		nGiveNPCID = atoi(pNPCID);
	//	}
	//}

	//TiXmlElement* pGiveStoryIDEle = tpRoot->FirstChildElement("give_story_id");
	//if (NULL != pGiveStoryIDEle)
	//{
	//	const char* pGiveStoryID = pGiveStoryIDEle->GetText();
	//	if (NULL != pGiveStoryID)
	//	{
	//		pNewTaskConfig->mAcceptStoryID = atoi(pGiveStoryID);
	//	}
	//}

	//TiXmlElement* pCompleteStoryIDEle = tpRoot->FirstChildElement("complete_story_id");
	//if (NULL != pCompleteStoryIDEle)
	//{
	//	const char* pCompleteStoryID = tpRoot->GetText();
	//	if (NULL != pCompleteStoryID)
	//	{
	//		pNewTaskConfig->mCompleteStoryID = atoi(pCompleteStoryID);
	//	}
	//}

	//TiXmlElement* pRecvStoryIDEle = tpRoot->FirstChildElement("recv_story_id");
	//if (NULL != pRecvStoryIDEle)
	//{
	//	const char* pRecvStoryID = pRecvStoryIDEle->GetText();
	//	if (NULL != pRecvStoryID)
	//	{
	//		pNewTaskConfig->mCommitStoryID = atoi(pRecvStoryID);
	//	}
	//}

	//TiXmlElement* pAcceptConditionEle = tpRoot->FirstChildElement("accept_cond");
	//if (NULL != pAcceptConditionEle)
	//{
	//	TiXmlElement* pLevelEle = pAcceptConditionEle->FirstChildElement("level");
	//	if (NULL != pLevelEle)
	//	{
	//		const char* pLevel = pLevelEle->Attribute("accept_level");
	//		if (NULL != pLevel)
	//		{
	//			CTaskCondition tCondition;
	//			tCondition.mType = emAccept_Level;
	//			tCondition.mParam[0] = atoi(pLevel);
	//			pNewTaskConfig->mAcceptCondition.push_back(tCondition);
	//		}
	//	}

	//	TiXmlElement* pSchoolEle = pAcceptConditionEle->FirstChildElement("school");
	//	if (NULL != pSchoolEle)
	//	{
	//		const char* pValue = pSchoolEle->Attribute("value");
	//		if (NULL != pValue)
	//		{
	//			CTaskCondition tCondition;
	//			tCondition.mType = emAccept_School;
	//			tCondition.mParam[0] = atoi(pValue);
	//			pNewTaskConfig->mAcceptCondition.push_back(tCondition);
	//		}
	//	}

	//	TiXmlElement* pPreTaskEle = pAcceptConditionEle->FirstChildElement("pre_task");
	//	if (NULL != pPreTaskEle)
	//	{
	//		const char* pTaskID = pPreTaskEle->Attribute("id");
	//		if (NULL != pTaskID)
	//		{
	//			CTaskCondition tCondition;
	//			tCondition.mType = emAccept_PreTask;
	//			tCondition.mParam[0] = atoi(pTaskID);
	//			pNewTaskConfig->mAcceptCondition.push_back(tCondition);
	//		}
	//	}

	//	TiXmlElement* pFriendlyEle = pAcceptConditionEle->FirstChildElement("friendly");
	//	if (NULL != pFriendlyEle)
	//	{
	//		const char* pFriendValue = pFriendlyEle->Attribute("value");
	//		if (NULL != pFriendValue)
	//		{
	//			CTaskCondition tCondition;
	//			tCondition.mType = emAccept_PartnerFavorability;
	//			tCondition.mParam[0] = nGiveNPCID;
	//			tCondition.mParam[1] = atoi(pFriendValue);
	//			pNewTaskConfig->mAcceptCondition.push_back(tCondition);
	//		}
	//	}

	//	TiXmlElement* pPrestigeEle = pAcceptConditionEle->FirstChildElement("prestige");
	//	if (NULL != pPrestigeEle)
	//	{
	//		const char* pPrestigeValue = pPrestigeEle->Attribute("accept_value");
	//		if (NULL != pPrestigeValue)
	//		{
	//			CTaskCondition tCondition;
	//			tCondition.mType = emAccept_Prestige;
	//			tCondition.mParam[0] = atoi(pPrestigeValue);
	//			pNewTaskConfig->mAcceptCondition.push_back(tCondition);
	//		}
	//	}

	//	TiXmlElement* pPartnerEle = pAcceptConditionEle->FirstChildElement("partner");
	//	if (NULL != pPartnerEle)
	//	{
	//		const char* pPartnerID = pPartnerEle->Attribute("id");
	//		if (NULL != pPartnerID)
	//		{
	//			CTaskCondition tCondition;
	//			tCondition.mType = emAccept_ArrayPartner;
	//			tCondition.mParam[0] = atoi(pPartnerID);
	//			pNewTaskConfig->mAcceptCondition.push_back(tCondition);
	//		}
	//	}

	//	TiXmlElement* pPartnerNumEle = pAcceptConditionEle->FirstChildElement("partner_num");
	//	if (NULL != pPartnerNumEle)
	//	{
	//		const char* pPartnerNum = pPartnerNumEle->Attribute("num");
	//		if (NULL != pPartnerNum)
	//		{
	//			CTaskCondition tCondition;
	//			tCondition.mType = emAccept_ArrayPartnerNum;
	//			tCondition.mParam[0] = atoi(pPartnerNum);
	//			pNewTaskConfig->mAcceptCondition.push_back(tCondition);
	//		}
	//	}
	//}

	//TiXmlElement* pCompleteCondEle = tpRoot->FirstChildElement("complete_cond");
	//if (NULL != pCompleteCondEle)
	//{
	//	TiXmlElement* pLevelEle = pCompleteCondEle->FirstChildElement("level");
	//	if (NULL != pLevelEle)
	//	{
	//		const char* pLevel = pLevelEle->Attribute("level");
	//		if (NULL != pLevel)
	//		{
	//			CTaskCondition tCondition;
	//			tCondition.mType = emComplete_Level;
	//			tCondition.mParam[0] = atoi(pLevel);
	//			pNewTaskConfig->mCompleteCondition.push_back(tCondition);
	//		}
	//	}

	//	TiXmlElement* pLeadFightEle = pCompleteCondEle->FirstChildElement("lead_fight");
	//	if (NULL != pLeadFightEle)
	//	{
	//		const char* pFightValue = pLeadFightEle->Attribute("lead_fight");
	//		if (NULL != pFightValue)
	//		{
	//			CTaskCondition tCondition;
	//			tCondition.mType = emComplete_CombatPower;
	//			tCondition.mParam[0] = atoi(pFightValue);
	//			pNewTaskConfig->mCompleteCondition.push_back(tCondition);
	//		}
	//	}

	//	TiXmlElement* pAllFightEle = pCompleteCondEle->FirstChildElement("all_fight");
	//	if (NULL != pAllFightEle)
	//	{
	//		const char* pAllFightValue = pAllFightEle->Attribute("all_fight");
	//		if (NULL != pAllFightValue)
	//		{
	//			CTaskCondition tCondition;
	//			tCondition.mType = emComplete_AllCombatPower;
	//			tCondition.mParam[0] = atoi(pAllFightValue);
	//			pNewTaskConfig->mCompleteCondition.push_back(tCondition);
	//		}
	//	}

	//	TiXmlElement* pPartnerEle = pCompleteCondEle->FirstChildElement("partner");
	//	if (NULL != pPartnerEle)
	//	{
	//		const char* pPartnerID = pPartnerEle->Attribute("partner_id");
	//		if (NULL != pPartnerID)
	//		{
	//			CTaskCondition tCondition;
	//			tCondition.mType = emComplete_Partner;
	//			tCondition.mParam[0] = atoi(pPartnerID);
	//			pNewTaskConfig->mCompleteCondition.push_back(tCondition);
	//		}
	//	}

	//	TiXmlElement* pMapEle = pCompleteCondEle->FirstChildElement("map");
	//	if (NULL != pMapEle)
	//	{
	//		int nMapID = 0;
	//		const char* pMapID = pMapEle->Attribute("map_id");
	//		if (NULL != pMapID)
	//		{
	//			nMapID = atoi(pMapID);
	//		}

	//		int nInstanceIndex = 0;
	//		const char* pInstanceIndex = pMapEle->Attribute("instance_index");
	//		if (NULL != pInstanceIndex)
	//		{
	//			nInstanceIndex = atoi(pInstanceIndex);
	//		}

	//		int nInstanceType = 0;
	//		const char* pInstanceType = pMapEle->Attribute("instance_type");
	//		if (NULL != pInstanceType)
	//		{
	//			nInstanceType = atoi(pInstanceType);
	//		}
	//		CTaskCondition tCondition;
	//		tCondition.mType = emComplete_PassFB;
	//		tCondition.mParam[0] = nMapID;
	//		tCondition.mParam[1] = nInstanceType;
	//		tCondition.mParam[2] = nInstanceIndex;
	//		tCondition.mParam[3] = 1;
	//		pNewTaskConfig->mCompleteCondition.push_back(tCondition);
	//	}

	//	TiXmlElement* pChallengeNPCEle = pCompleteCondEle->FirstChildElement("challenge_npc");
	//	if (NULL != pChallengeNPCEle)
	//	{
	//		const char* pNpcID = pChallengeNPCEle->Attribute("npc_id");
	//		if (NULL != pNpcID)
	//		{
	//			CTaskCondition tCondition;
	//			tCondition.mType = emComplete_ChallengeNPC;
	//			tCondition.mParam[0] = atoi(pNpcID);
	//			tCondition.mParam[1] = 1;
	//			pNewTaskConfig->mCompleteCondition.push_back(tCondition);
	//		}
	//	}

	//	TiXmlElement* pOgreEle = pCompleteCondEle->FirstChildElement("ogre");
	//	if (NULL != pOgreEle)
	//	{
	//		int nOgreID = 0;
	//		const char* pOgreID = pOgreEle->Attribute("id");
	//		if (NULL != pOgreID)
	//		{
	//			nOgreID = atoi(pOgreID);
	//		}

	//		int nOgreNum = 0;
	//		const char* pOgreNum = pOgreEle->Attribute("num");
	//		if (NULL != pOgreNum)
	//		{
	//			nOgreNum = atoi(pOgreNum);
	//		}
	//		CTaskCondition tCondition;
	//		tCondition.mType = emComplete_KillOgre;
	//		tCondition.mParam[0] = nOgreID;
	//		tCondition.mParam[1] = nOgreNum;
	//		pNewTaskConfig->mCompleteCondition.push_back(tCondition);
	//	}


	//	TiXmlElement* pCollectEle = pCompleteCondEle->FirstChildElement("collect");
	//	if (NULL != pCollectEle)
	//	{
	//		int nItemID = 0;
	//		const char* pItemID = pCollectEle->Attribute("id");
	//		if (NULL != pItemID)
	//		{
	//			nItemID = atoi(pItemID);
	//		}

	//		int nItemNum = 0;
	//		const char* pItemNum = pCollectEle->Attribute("num");
	//		if (NULL != pItemNum)
	//		{
	//			nItemNum = atoi(pItemNum);
	//		}
	//		CTaskCondition tCondition;
	//		tCondition.mType = emComplete_HoldItem;
	//		tCondition.mParam[0] = nItemID;
	//		tCondition.mParam[1] = nItemNum;
	//		pNewTaskConfig->mCompleteCondition.push_back(tCondition);
	//	}

	//	TiXmlElement* pDigEle = pCompleteCondEle->FirstChildElement("dig");
	//	if (NULL != pDigEle)
	//	{
	//		int nItemID = 0;
	//		const char* pItemID = pDigEle->Attribute("id");
	//		if (NULL != pItemID)
	//		{
	//			nItemID = atoi(pItemID);
	//		}

	//		int nItemNum = 0;
	//		const char* pItemNum = pDigEle->Attribute("num");
	//		if (NULL != pItemNum)
	//		{
	//			nItemNum = atoi(pItemNum);
	//		}
	//		CTaskCondition tCondition;
	//		tCondition.mType = emComplete_Collect;
	//		tCondition.mParam[0] = nItemID;
	//		tCondition.mParam[1] = nItemNum;
	//		pNewTaskConfig->mCompleteCondition.push_back(tCondition);
	//	}


	//	TiXmlElement* pImproveEle = pCompleteCondEle->FirstChildElement("improve");
	//	if (NULL != pImproveEle)
	//	{
	//		int nEquipPart = 0;
	//		const char* pEquipPart = pImproveEle->Attribute("part");
	//		if (NULL != pEquipPart)
	//		{
	//			nEquipPart = atoi(pEquipPart);
	//		}

	//		int nEquipLevel = 0;
	//		const char* pEquipLevel = pImproveEle->Attribute("level");
	//		if (NULL != pEquipLevel)
	//		{
	//			nEquipLevel = atoi(pEquipLevel);
	//		}
	//		CTaskCondition tCondition;
	//		tCondition.mType = emComplete_EquipLevel;
	//		tCondition.mParam[0] = nEquipPart;
	//		tCondition.mParam[1] = nEquipLevel;
	//		pNewTaskConfig->mCompleteCondition.push_back(tCondition);
	//	}
	//}


	//TiXmlElement* pAwardEle = tpRoot->FirstChildElement("awards");
	//if (NULL != pAwardEle)
	//{
	//	TiXmlElement* pPrestigeEle = pAwardEle->FirstChildElement("award_prestige");
	//	if (NULL != pPrestigeEle)
	//	{
	//		const char* pPrestige = pPrestigeEle->Attribute("value");
	//		if (NULL != pPrestige)
	//		{
	//			CTaskReward tTaskReward;
	//			tTaskReward.mType = emTaskPrize_Prestige;
	//			tTaskReward.mParam[0] = atoi(pPrestige);
	//			pNewTaskConfig->mPrizeList.push_back(tTaskReward);
	//		}
	//	}

	//	TiXmlElement* pMoneyEle = pAwardEle->FirstChildElement("award_money");
	//	if (NULL != pMoneyEle)
	//	{
	//		const char* pMoney = pMoneyEle->Attribute("value");
	//		if (NULL != pMoney)
	//		{
	//			CTaskReward tTaskReward;
	//			tTaskReward.mType = emTaskPrize_Money;
	//			tTaskReward.mParam[0] = atoi(pMoney);
	//			pNewTaskConfig->mPrizeList.push_back(tTaskReward);
	//		}
	//	}

	//	TiXmlElement* pTalentEle = pAwardEle->FirstChildElement("award_talent");
	//	if (NULL != pTalentEle)
	//	{
	//		const char* pTalent = pTalentEle->Attribute("award_talent");
	//		if (NULL != pTalent)
	//		{
	//			CTaskReward tTaskReward;
	//			tTaskReward.mType = emTaskPrize_Talent;
	//			tTaskReward.mParam[0] = atoi(pTalent);
	//			pNewTaskConfig->mPrizeList.push_back(tTaskReward);
	//		}
	//	}

	//	TiXmlElement* pExpEle = pAwardEle->FirstChildElement("award_expr");
	//	if (NULL != pExpEle)
	//	{
	//		const char* pExp = pExpEle->Attribute("value");
	//		if (NULL != pExpEle)
	//		{
	//			CTaskReward tTaskReward;
	//			tTaskReward.mType = emTaskPrize_Exp;
	//			tTaskReward.mParam[0] = atoi(pExp);
	//			pNewTaskConfig->mPrizeList.push_back(tTaskReward);
	//		}
	//	}

	//	TiXmlElement* pYuanBaoEle = pAwardEle->FirstChildElement("award_yuanbao");
	//	if (NULL != pYuanBaoEle)
	//	{
	//		const char* pYuanBao = pYuanBaoEle->Attribute("value");
	//		if (NULL != pYuanBao)
	//		{
	//			CTaskReward tTaskReward;
	//			tTaskReward.mType = emTaskPrize_YuanBao;
	//			tTaskReward.mParam[0] = atoi(pYuanBao);
	//			pNewTaskConfig->mPrizeList.push_back(tTaskReward);
	//		}
	//	}

	//	TiXmlElement* pItemEle = pAwardEle->FirstChildElement("award_item");
	//	int nCount = 0;
	//	for (; NULL != pItemEle; pItemEle = pItemEle->NextSiblingElement("award_item"))
	//	{
	//		CTaskReward tTaskReward;
	//		tTaskReward.mType = emTaskPrize_Item;
	//		const char* pItemID = pItemEle->Attribute("id");
	//		if (NULL != pItemID)
	//		{
	//			tTaskReward.mParam[0] = atoi(pItemID);
	//		}

	//		const char* pItemNum = pItemEle->Attribute("num");
	//		if (NULL != pItemNum)
	//		{
	//			tTaskReward.mParam[1] = atoi(pItemNum);
	//		}
	//		pNewTaskConfig->mPrizeList.push_back(tTaskReward);
	//	}

	//	TiXmlElement* pFunctionEle = pAwardEle->FirstChildElement("award_function");
	//	for (; NULL != pFunctionEle; pFunctionEle = pFunctionEle->NextSiblingElement("award_function"))
	//	{
	//		CTaskReward tTaskReward;
	//		tTaskReward.mType = emTaskPrize_Func;
	//		const char* pFuncType = pFunctionEle->Attribute("id");
	//		if (NULL != pFuncType)
	//		{
	//			tTaskReward.mParam[0] = atoi(pFuncType);
	//		}

	//		const char* pFuncParam1 = pFunctionEle->Attribute("param1");
	//		if (NULL != pFuncParam1)
	//		{
	//			tTaskReward.mParam[1] = atoi(pFuncParam1);
	//		}

	//		const char* pFuncParam2 = pFunctionEle->Attribute("param2");
	//		if (NULL != pFuncParam2)
	//		{
	//			tTaskReward.mParam[2] = atoi(pFuncParam2);
	//		}

	//		const char* pFuncParam3 = pFunctionEle->Attribute("param3");
	//		if (NULL != pFuncParam3)
	//		{
	//			tTaskReward.mParam[3] = atoi(pFuncParam3);
	//		}
	//		pNewTaskConfig->mPrizeList.push_back(tTaskReward);
	//	}

	//	TiXmlElement* pSceneEle = pAwardEle->FirstChildElement("award_scene");
	//	if (NULL != pSceneEle)
	//	{
	//		CTaskReward tTaskReward;
	//		tTaskReward.mType = emTaskPrize_BigMap;
	//		const char* pSceneID = pSceneEle->Attribute("id");
	//		if (NULL != pSceneID)
	//		{
	//			tTaskReward.mParam[0] = atoi(pSceneID);
	//		}
	//		pNewTaskConfig->mPrizeList.push_back(tTaskReward);
	//	}

	//	TiXmlElement* pPartnerEle = pAwardEle->FirstChildElement("award_partner");
	//	if (NULL != pPartnerEle)
	//	{
	//		CTaskReward tTaskReward;
	//		tTaskReward.mType = emTaskPrize_Partner;
	//		const char* pPartnerID = pPartnerEle->Attribute("id");
	//		if (NULL != pPartnerID)
	//		{
	//			tTaskReward.mParam[0] = atoi(pPartnerID);
	//		}
	//		pNewTaskConfig->mPrizeList.push_back(tTaskReward);
	//	}

	//	TiXmlElement* pFriendlyEle = pAwardEle->FirstChildElement("friendly");
	//	if (NULL != pFriendlyEle)
	//	{
	//		CTaskReward tTaskReward;
	//		tTaskReward.mType = emTaskPrize_Favorability;
	//		const char* pPartnerID = pFriendlyEle->Attribute("id");
	//		if (NULL != pPartnerID)
	//		{
	//			tTaskReward.mParam[0] = atoi(pPartnerID);
	//		}

	//		const char* pFrieldValue = pFriendlyEle->Attribute("value");
	//		if (NULL != pFrieldValue)
	//		{
	//			tTaskReward.mParam[1] = atoi(pFrieldValue);
	//		}
	//		pNewTaskConfig->mPrizeList.push_back(tTaskReward);
	//	}
	//}

	//TiXmlElement* pAcceptWardEle = tpRoot->FirstChildElement("accept_functionaward");
	//if (NULL != pAcceptWardEle)
	//{
	//	TiXmlElement* pAwardSceneEle = pAcceptWardEle->FirstChildElement("award_scene");
	//	if (NULL != pAwardSceneEle)
	//	{
	//		const char* pSceneID = pAwardSceneEle->Attribute("id");
	//		if (NULL != pSceneID)
	//		{
	//			CTaskReward tTaskReward;
	//			tTaskReward.mType = emTaskPrize_BigMap;
	//			tTaskReward.mParam[0] = atoi(pSceneID);
	//			pNewTaskConfig->mAcceptPrizeList.push_back(tTaskReward);
	//		}
	//	}
	//}

	//TiXmlElement* pCloseCondEle = tpRoot->FirstChildElement("close_cond");
	//if (NULL != pCloseCondEle)
	//{
	//	TiXmlElement* pPrestigeEle = pCloseCondEle->FirstChildElement("prestige");
	//	if (NULL != pPrestigeEle)
	//	{
	//		const char* pPrestigeValue = pPrestigeEle->Attribute("value");
	//		if (NULL != pPrestigeValue)
	//		{
	//			CTaskCondition tCloseCondition;
	//			tCloseCondition.mType = emClose_Prestige;
	//			tCloseCondition.mParam[0] = atoi(pPrestigeValue);
	//			pNewTaskConfig->mCloseCondition.push_back(tCloseCondition);
	//		}
	//	}

	//	TiXmlElement* pLevelEle = pCloseCondEle->FirstChildElement("level");
	//	if (NULL != pLevelEle)
	//	{
	//		const char* pLevel = pLevelEle->Attribute("level");
	//		if (NULL != pLevel)
	//		{
	//			CTaskCondition tCloseCondition;
	//			tCloseCondition.mType = emClose_Level;
	//			tCloseCondition.mParam[0] = atoi(pLevel);
	//			pNewTaskConfig->mCloseCondition.push_back(tCloseCondition);
	//		}
	//	}

	//	TiXmlElement* pPreTaskEle = pCloseCondEle->FirstChildElement("pre_task");
	//	if (NULL != pPreTaskEle)
	//	{
	//		const char* pPreTask = pPreTaskEle->Attribute("id");
	//		if (NULL != pPreTask)
	//		{
	//			CTaskCondition tCloseCondition;
	//			tCloseCondition.mType = emClose_PreTask;
	//			tCloseCondition.mParam[0] = atoi(pPreTask);
	//			pNewTaskConfig->mCloseCondition.push_back(tCloseCondition);
	//		}
	//	}
	//}
}