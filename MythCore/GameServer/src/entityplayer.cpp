#include "entityplayer.h"
#include "mapmodule.hxx.pb.h"
#include "propertymodule.hxx.pb.h"
#include "timemanager.h"
/// ����
void CEntityPlayer::onTimer(unsigned int nTickOffset)
{

}

/// ˢ�»�������
void CEntityPlayer::refreshBaseProperty()
{
	for (int i = emProType_MaxHp; i < emProTypeMax; ++ i)
	{
		if (mBaseProperty[i].getDirty())
		{
			/// ˢ�¶�Ӧ������
			int nValue = mItemUnit.getEquip().getProperty(i);
			int nPer = mItemUnit.getEquip().getProperty(i + ENTITY_PRO_PER_START);
			nValue += (int)((double)nPer * MYTH_MILLIONTH * nValue);
			mBaseProperty[i].setValue(nValue);

			// �������
			mBaseProperty[i].setDirty(false);
			refreshFightProperty(i);
		}
	}
}

/// ˢ��ս������
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

//// �õ������������
void CEntityPlayer::onGetPlayerPropertyRequest(Message* pMessage)
{
	CGetPlayerPropertyResponse tResponse;
	for (int i = emProType_MaxHp ; i < emProTypeMax; ++ i)
	{
		tResponse.add_propertyvalue(mBaseProperty[i].getValue());
	}

	//CSceneJob::Inst()->send2Player(this, ID_S2C_RESPONSE_GET_PLAYER_PROPERTY, &tResponse);
}

/// ÿ��ˢ��
void CEntityPlayer::dailyRefresh(bool bLogin)
{
	// �ϴ�ˢ�µ�ʱ������ˢ�µ�ʱ���ǲ���ͬһ��
	time_t tNewDayTime = mTimeUnit.getNewDayTime();
	time_t tNowTime = CTimeManager::Inst()->getCurrTime();

	// �����ͬһ��
	if (CTimeManager::Inst()->checkSameDay(tNewDayTime, tNowTime))
	{
		return;
	}

	// ÿ��ˢ�µĻ�����ˢ�¿����ģ�飬��Ҫ�ȵ������ģ������ü�������Ժ���ˢ��
	if (bLogin)
	{
		mServerActUnit.dailyRefresh();
	}

	mTimeUnit.setNewDayTime(tNowTime);
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
	CTimerList& rTimeList = mTimeUnit.getTimerList();
	if (rTimeList.getTimeListSize() >= MAX_PLAYER_TIMER_NUM)
	{
		return INVALID_OBJ_ID;
	}
	return rTimeList.setTimer(nOwerObjID, nModule, nMilliSec, pParam, nParamNum, nCallTimes);
}

void CEntityPlayer::send2PlayerImp(unsigned short nMessageID, Message* pMessage)
{
	//if (rExchangeHead.mSocketIndex < 0)
	//{
	//	return;
	//}
	//char* pTemp = mBuffer;
	//memcpy(pTemp, &rExchangeHead, sizeof(rExchangeHead));
	//pTemp += sizeof(rExchangeHead);

	//unsigned short nMessageLen = pMessage->ByteSize() + sizeof(unsigned short) * 2;
	//memcpy(pTemp, &nMessageLen, sizeof(nMessageLen));

	//pTemp += sizeof(nMessageLen);

	//memcpy(pTemp, &nMessageID, sizeof(nMessageID));
	//pTemp += sizeof(nMessageID);

	//pMessage->SerializeToArray(pTemp, sizeof(mBuffer) - sizeof(rExchangeHead) - sizeof(unsigned short) * 2);
	////printf("PushPacket nMessageID: %d\n", nMessageID);
	//mServer2TcpMemory->PushPacket((byte*)mBuffer, pMessage->ByteSize() + sizeof(rExchangeHead) + sizeof(unsigned short) * 2);

}
void CEntityPlayer::send2Player(unsigned short nMessageID, Message* pMessage)
{
	const ::google::protobuf::Descriptor* pDescriptor = pMessage->GetDescriptor();
	LOG_DEBUG("default", "---- Send to client(Obj Id:%d|Role:%d|Name:%s) Msg[ %s ][id: 0x%04x/%d] ---",
		getObjID(), getRoleID(), getName(),
		pDescriptor->name().c_str(), nMessageID, nMessageID);
	LOG_DEBUG("default", "[%s]", pMessage->ShortDebugString().c_str());

	send2PlayerImp(nMessageID, pMessage);
}
/// ����ǰ����Ϣ(lua)
void CEntityPlayer::send2PlayerLua(unsigned short nMessageID, const char* pMsgBuff, int nBuffLen)
{
	//if (NULL == pMsgBuff)
	//{
	//	return;
	//}

	//CExchangeHead& rExchangeHead = rPlayer.getExchangeHead();
	//if (rExchangeHead.mSocketIndex < 0)
	//{
	//	return;
	//}
	//char* pTemp = mBuffer;
	//memcpy(pTemp, &rExchangeHead, sizeof(rExchangeHead));
	//pTemp += sizeof(rExchangeHead);

	//unsigned short nMessageLen = nBuffLen + sizeof(unsigned short) * 2;
	//memcpy(pTemp, &nMessageLen, sizeof(nMessageLen));

	//pTemp += sizeof(nMessageLen);

	//memcpy(pTemp, &nMessageID, sizeof(nMessageID));
	//pTemp += sizeof(nMessageID);

	//int nLeftLen = sizeof(mBuffer) - sizeof(rExchangeHead) - sizeof(unsigned short) * 2;
	//memcpy(pTemp, pMsgBuff, nBuffLen > nLeftLen ? nLeftLen : nBuffLen);
	////pMessage->SerializeToArray(pTemp, sizeof(mBuffer) - sizeof(rExchangeHead) - sizeof(unsigned short) * 2);
	////printf("PushPacket nMessageID: %d\n", nMessageID);
	//mServer2TcpMemory->PushPacket((byte*)mBuffer, nBuffLen + sizeof(rExchangeHead) + sizeof(unsigned short) * 2);
}