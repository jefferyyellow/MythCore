#include "entityplayer.h"
#include "mapmodule.hxx.pb.h"
/// 刷新最大血
int CEntityPlayer::RefreshMaxHP()
{

	return 0;
}

/// 刷新最大魔
int CEntityPlayer::RefreshMaxMP()
{

	return 0;
}

/// 刷新攻击力
int CEntityPlayer::RefreshAttack()
{

	return 0;
}

/// 刷新防御力
int CEntityPlayer::RefreshDefence()
{

	return 0;
}

/// 刷新最大血（战斗属性）
int CEntityPlayer::RefreshMaxHPFight()
{
	return 0;
}

/// 刷新最大魔（战斗属性）
int CEntityPlayer::RefreshMaxMPFight()
{
	return 0;
}

/// 刷新攻击力（战斗属性）
int CEntityPlayer::RefreshAttackFight()
{
	return 0;
}

/// 刷新防御力（战斗属性）
int CEntityPlayer::RefreshDefenceFight()
{
	return 0;
}

/// 序列化场景信息到PB·
void CEntityPlayer::serializeSceneInfoToPB(PBPlayerSceneInfo* pbPlayerInfo)
{
	if (NULL == pbPlayerInfo)
	{
		return;
	}

	pbPlayerInfo->set_entityid(getObjID());
	pbPlayerInfo->set_posx(mPos.mX);
	pbPlayerInfo->set_posy(mPos.mY);
}
