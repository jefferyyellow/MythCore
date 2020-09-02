#include "skillmodule.h"
#include "skillmodule.hxx.pb.h"
#include "locallogjob.h"
#include "skillunit.h"
#include "entityplayer.h"
#include "gameserver.h"
#include "timemanager.h"
CSkillModule::CSkillModule()
{

}

CSkillModule::~CSkillModule()
{
	
}

/// ����������
void CSkillModule::onLaunchServer()
{

}

/// ������ɼ��
bool CSkillModule::onCheckLaunch()
{
	return true;
}

/// �����������ɹ�
void CSkillModule::onLaunchComplete()
{

}

/// �˳�������
void CSkillModule::onExitServer()
{

}

/// ��һ�쵽��
void CSkillModule::onNewDayCome()
{

}

/// ��һ�ܵ���
void CSkillModule::onNewWeekCome()
{

}

/// ����ʵ��
void CSkillModule::onCreatePlayer(CEntityPlayer* pPlayer)
{

}

/// ����ʵ��
void CSkillModule::onDestroyPlayer(CEntityPlayer* pPlayer)
{

}

void CSkillModule::onTimer(unsigned int nTickOffset)
{

}

/// ���������ļ�
void CSkillModule::onLoadConfig()
{

}


void CSkillModule::onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage)
{
	if (NULL == pPlayer)
	{
		return;
	}

	switch(nMessageID)
	{
		case ID_C2S_REQUEST_USE_SKILL:
		{
			onUseSkillRequest(pPlayer, pMessage);
			break;
		}
	}
}

void CSkillModule::onUseSkillRequest(CEntityPlayer* pPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL != pMessage && NULL != pPlayer, return);
	CUseSkillRequest* pUseSkillRequest = static_cast<CUseSkillRequest*>(pMessage);
	MYTH_ASSERT(NULL != pUseSkillRequest, return);

	int nSkillIndex = pUseSkillRequest->skillindex();
	CPlayerSkill* pPlayerSkill = NULL;//pPlayer->getSkillUnit().getPlayerSkill(nSkillIndex);
	if (NULL == pPlayerSkill)
	{
		return;
	}

	CEntityCharacter* pSkillTarget[MAX_SKILL_TARGET_NUM] = {NULL};
	int nTargetCount = 0;
	for (int i = 0; i < pUseSkillRequest->target_size(); ++ i)
	{
		CEntityCharacter* pTarget = static_cast<CEntityCharacter*>(CObjPool::Inst()->getObj(pUseSkillRequest->target(i)));
		if (NULL == pTarget)
		{
			continue;
		}
		if (pTarget->getCurHP() <= 0)
		{
			continue;
		}
		pSkillTarget[nTargetCount++] = pTarget;
	}

	
	CTplSkill* pTplSkill = static_cast<CTplSkill*>(CStaticData::searchTpl(pPlayerSkill->getSkillId()));
	if (NULL == pTplSkill)
	{
		return;
	}

	time_t tTimeNow = CTimeManager::Inst()->getCurrTime();

	// CD��û��
	if(tTimeNow - pPlayerSkill->getSkillCD() < pTplSkill->mSkillCD)
	{
		return;
	}


	for (int i = 0; i < nTargetCount; ++ i)
	{
		int nSkillDamage = getSkillDamage(pPlayer, pTplSkill, pPlayerSkill->getSkillLevel(), pSkillTarget[i]);
		pSkillTarget[i]->reduceHp(pPlayer, nSkillDamage);
	}
}


int CSkillModule::getSkillDamage(CEntityCharacter* pSrcEntity, CTplSkill* pTplSkill, int nSkillLevel, CEntityCharacter* pTargetEntity)
{
	if (NULL == pSrcEntity || NULL == pTplSkill || NULL == pTargetEntity)
	{
		return 0;
	}

	return pTplSkill->mAddDamage;
}
