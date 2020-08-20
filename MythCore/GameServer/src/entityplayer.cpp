#include "entityplayer.h"
#include "mapmodule.hxx.pb.h"
#include "propertymodule.hxx.pb.h"
#include "scenejob.h"
#include "timemanager.h"
/// 刷新基本属性
void CEntityPlayer::refreshBaseProperty()
{
	for (int i = emProType_MaxHp; i < emProTypeMax; ++ i)
	{
		if (mBaseProperty[i].getDirty())
		{
			/// 刷新对应的属性
			int nValue = mItemUnit.getEquip().getProperty(i);
			int nPer = mItemUnit.getEquip().getProperty(i + ENTITY_PRO_PER_START);
			nValue += (int)((double)nPer * MYTH_MILLIONTH * nValue);
			mBaseProperty[i].setValue(nValue);

			// 清空脏标记
			mBaseProperty[i].setDirty(false);
			refreshFightProperty(i);
		}
	}
}

/// 刷新战斗属性
void CEntityPlayer::refreshFightProperty()
{
	for (int i = emProType_MaxHp; i < emProTypeMax; ++i)
	{
		refreshFightProperty(i);
	}
}

void CEntityPlayer::refreshFightProperty(int nPropertyType)
{
	if (nPropertyType < emProType_MaxHp || nPropertyType >= emProTypeMax)
	{
		return;
	}

	int nPropertyValue = mBaseProperty[nPropertyType].getValue();
	mFightProperty[nPropertyType] = nPropertyValue;
}

//// 得到玩家属性请求
void CEntityPlayer::onGetPlayerPropertyRequest(Message* pMessage)
{
	CGetPlayerPropertyResponse tResponse;
	for (int i = emProType_MaxHp ; i < emProTypeMax; ++ i)
	{
		tResponse.add_propertyvalue(mBaseProperty[i].getValue());
	}

	CSceneJob::Inst()->send2Player(this, ID_S2C_RESPONSE_GET_PLAYER_PROPERTY, &tResponse);
}

/// 每日刷新
void CEntityPlayer::dailyRefresh(bool bLogin)
{
	// 上传刷新的时候和这次刷新的时间是不是同一天
	time_t tNewDayTime = mTimeUnit.getNewDayTime();
	time_t tNowTime = CTimeManager::Inst()->getCurrTime();

	// 如果是同一天
	if (CTimeManager::Inst()->checkSameDay(tNewDayTime, tNowTime))
	{
		return;
	}

	// 每日刷新的话，不刷新开服活动模块，需要等到开服活动模块的配置加载完成以后再刷新
	if (bLogin)
	{
		mServerActUnit.dailyRefresh();
	}

	mTimeUnit.setNewDayTime(tNowTime);
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

/// 设置毫秒级计时器(不能超过24天)
int CEntityPlayer::setTimer(int nOwerObjID, int nModule, int nMilliSec, const int* pParam, int nParamNum, int nCallTimes)
{
	CTimerList& rTimeList = mTimeUnit.getTimerList();
	if (rTimeList.getTimeListSize() >= MAX_PLAYER_TIMER_NUM)
	{
		return INVALID_OBJ_ID;
	}
	return rTimeList.setTimer(nOwerObjID, nModule, nMilliSec, pParam, nParamNum, nCallTimes);
}
