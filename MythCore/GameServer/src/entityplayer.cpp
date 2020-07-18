#include "entityplayer.h"
#include "mapmodule.hxx.pb.h"
#include "propertymodule.hxx.pb.h"
#include "scenejob.h"
/// ˢ�»�������
void CEntityPlayer::refreshBaseProperty()
{
	for (int i = emPropertyType_MaxHp; i < emPropertyTypeMax; ++ i)
	{
		if (mBaseProperty[i].getDirty())
		{
			/// ˢ�¶�Ӧ������
			int nValue = mItemUnit.getEquip().getProperty(i);
			mBaseProperty[i].setValue(nValue);
			refreshFightProperty(i);
		}
	}
}

/// ˢ��ս������
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

//// �õ������������
void CEntityPlayer::onGetPlayerPropertyRequest(Message* pMessage)
{
	CGetPlayerPropertyResponse tResponse;
	for (int i = emPropertyType_None ; i < emPropertyTypeMax; ++ i)
	{
		tResponse.add_propertyvalue(mBaseProperty[i].getValue());
	}

	CSceneJob::Inst()->send2Player(this, ID_S2C_RESPONSE_GET_PLAYER_PROPERTY, &tResponse);
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

/// ���ú��뼶��ʱ��(���ܳ���24��)
int CEntityPlayer::setTimer(int nOwerObjID, int nModule, int nMilliSec, const int* pParam, int nParamNum, int nCallTimes)
{
	if (mTimerList.getTimeListSize() >= MAX_PLAYER_TIMER_NUM)
	{
		return INVALID_OBJ_ID;
	}
	return mTimerList.setTimer(nOwerObjID, nModule, nMilliSec, pParam, nParamNum, nCallTimes);
}
