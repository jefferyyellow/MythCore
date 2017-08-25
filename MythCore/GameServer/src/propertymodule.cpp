#include "propertymodule.h"
#include "propertyunit.h"
#include "entityplayer.h"
#include "propertymodule.hxx.pb.h"
#include "scenejob.h"
#include "locallogjob.h"

/// ʱ�亯��
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

// ��һ�þ���
void CPropertyModule::obtainExp(CEntityPlayer* pPlayer, int nExp)
{
	if (NULL == pPlayer || nExp <= 0)
	{
		return;
	}

	CPropertyUnit& rPropertyUnity = pPlayer->GetPropertyUnit();
	int nOldLeve = rPropertyUnity.getLevel();
	// �õ�����
	rPropertyUnity.obtainExp(nExp);
	// ������
	if (rPropertyUnity.getLevel() > nOldLeve)
	{
		onPlayerLevelUp(pPlayer, rPropertyUnity.getLevel());
	}
}

// ��ҵȼ�����
void CPropertyModule::onPlayerLevelUp(CEntityPlayer* pPlayer, int nLevel)
{
	CMessagePlayerLevelUpNotify tPlayerLevelUpNotify;
	tPlayerLevelUpNotify.set_level(nLevel);
	CSceneJob::Inst()->sendClientMessage(pPlayer, ID_S2C_NOTIYF_PLAYER_LEVEL_UP, &tPlayerLevelUpNotify);
}

// GM��������
void CPropertyModule::onMessageGMCommandRequest(CEntityPlayer* pPlayer, Message* pMessage)
{
	MYTH_ASSERT(NULL == pPlayer || NULL == pMessage, return);

	CMessageGMCommandRequest* pGMCommandRequest = reinterpret_cast<CMessageGMCommandRequest*>(pMessage);
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