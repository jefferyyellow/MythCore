#include "entityplayer.h"
#include "mapmodule.hxx.pb.h"
/// 刷新基本属性
void CEntityPlayer::RefreshBaseProperty()
{
	for (int i = 0; i < emPropertyTypeMax; ++ i)
	{
		if (mBaseProperty[i].getDirty())
		{
			/// 刷新对应的属性
			//mBaseProperty[i] = 
			// mFightProperty[i] = 
		}
	}
}

/// 刷新战斗属性
void CEntityPlayer::RefreshFightProperty()
{
	for (int i = 0; i < emPropertyTypeMax; ++i)
	{
		
	}
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
