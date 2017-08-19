#include "propertymodule.h"
#include "propertyunit.h"
#include "entityplayer.h"
#include "propertymodule.hxx.pb.h"
#include "scenejob.h"

void CPropertyModule::obtainExp(CEntityPlayer* pPlayer, int nExp)
{
	if (NULL == pPlayer || nExp <= 0)
	{
		return;
	}

	CPropertyUnit& rPropertyUnity = pPlayer->GetPropertyUnit();
	int nOldLeve = rPropertyUnity.getLevel();
	// 得到经验
	rPropertyUnity.obtainExp(nExp);
	// 升级了
	if (rPropertyUnity.getLevel() > nOldLeve)
	{
		onPlayerLevelUp(pPlayer, rPropertyUnity.getLevel());
	}
}

void CPropertyModule::onPlayerLevelUp(CEntityPlayer* pPlayer, int nLevel)
{

	CMessagePlayerLevelUpNotify tPlayerLevelUpNotify;
	tPlayerLevelUpNotify.set_level(nLevel);
	CSceneJob::Inst()->sendClientMessage(pPlayer, ID_S2C_NOTIYF_PLAYER_LEVEL_UP, &tPlayerLevelUpNotify);
}


