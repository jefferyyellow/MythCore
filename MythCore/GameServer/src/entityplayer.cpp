#include "entityplayer.h"
#include "mapmodule.hxx.pb.h"
/// ˢ�»�������
void CEntityPlayer::refreshBaseProperty()
{
	for (int i = 0; i < emPropertyTypeMax; ++ i)
	{
		if (mBaseProperty[i].getDirty())
		{
			/// ˢ�¶�Ӧ������
			//mBaseProperty[i] = 
			// mFightProperty[i] = 
		}
	}
}

/// ˢ��ս������
void CEntityPlayer::refreshFightProperty()
{
	for (int i = 0; i < emPropertyTypeMax; ++i)
	{
		
	}
}

/// ���л�������Ϣ��PB��
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
