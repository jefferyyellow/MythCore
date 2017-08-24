#include "propertymodule.h"
#include "propertyunit.h"
#include "entityplayer.h"
#include "propertymodule.hxx.pb.h"
#include "scenejob.h"

/// ʱ�亯��
void CPropertyModule::OnTimer(unsigned int nTickOffset)
{

}

void CPropertyModule::onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage)
{

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


