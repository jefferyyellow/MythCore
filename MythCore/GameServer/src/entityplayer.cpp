#include "entityplayer.h"
#include "mapmodule.hxx.pb.h"
/// 刷新基本属性
void CEntityPlayer::refreshBaseProperty()
{
	for (int i = emPropertyType_MaxHp; i < emPropertyTypeMax; ++ i)
	{
		if (mBaseProperty[i].getDirty())
		{
			/// 刷新对应的属性
			int nValue = mItemUnit.getEquipList().getProperty(i);
			mBaseProperty[i].setValue(nValue);
			refreshFightProperty(i);
		}
	}
}

/// 刷新战斗属性
void CEntityPlayer::refreshFightProperty()
{
	for (int i = emPropertyType_MaxHp; i < emPropertyTypeMax; ++i)
	{
		refreshFightProperty(i);
	}
}

void CEntityPlayer::refreshFightProperty(int nPropertyType)
{
	if (nPropertyType < emPropertyType_MaxHp || nPropertyType >= emPropertyTypeMax)
	{
		return;
	}

	int nPropertyValue = mBaseProperty[nPropertyType].getValue();

	mFightProperty[nPropertyType] = nPropertyValue;
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
