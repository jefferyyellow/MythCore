#include "mailmodule.h"
#include "mail.h"
#include "entityplayer.h"
#include "dbmodule.hxx.pb.h"
#include "scenejob.h"
#include "dbmodule.h"
#include "errcode.h"
#include "timemanager.h"
#include "objpoolimp.h"
/// ����������
void CMailModule::onLaunchServer()
{
	loadMaxMailID();
	loadMaxGlobalMailID();
	loadGlobalMail();
}

/// ������ɼ��
bool CMailModule::onCheckLaunch()
{
	return mGlobalLoadComplete && (mGlobalMailID > 0)&& (mPlayerMailID > 0);
}

/// �����������ɹ�
void CMailModule::onLaunchComplete()
{

}

/// �˳�������
void CMailModule::onExitServer()
{
	
}

/// ��һ�쵽��
void CMailModule::onNewDayCome()
{
	deleteExpireMail();
}

/// ��һ�ܵ���
void CMailModule::onNewWeekCome()
{

}

/// ����ʵ��
void CMailModule::onCreatePlayer(CEntityPlayer& rPlayer)
{

}

/// ����ʵ��
void CMailModule::onDestroyPlayer(CEntityPlayer& rPlayer)
{

}

/// ʱ�亯��
void CMailModule::onTimer(unsigned int nTickOffset)
{

}

/// ���������ļ�
void CMailModule::onLoadConfig()
{

}

void CMailModule::loadMaxMailID()
{
	CDBModule::Inst()->pushDBTask(0, emSessionType_LoadMaxMailID, 0, 0,
		"select AUTO_INCREMENT from information_schema.tables where table_schema=database() and table_name='Mail'");
}

/// ���������ʼ�ID�ص�
void CMailModule::onLoadMaxMailID(CDBResponse& rResponse)
{
	if (SUCCESS != rResponse.mSqlResult)
	{
		return;
	}

	if (rResponse.mRowNum <= 0)
	{
		return;
	}

	mPlayerMailID = rResponse.getUInt();
}


/// ��������ȫ���ʼ�ID
void CMailModule::loadMaxGlobalMailID()
{
	CDBModule::Inst()->pushDBTask(0, emSessionType_LoadMaxGlobalMailID, 0, 0,
		"select AUTO_INCREMENT from information_schema.tables where table_schema=database() and table_name='GlobalMail'");

}
/// ��������ȫ���ʼ�ID�ص�
void CMailModule::onLoadMaxGlobalMailID(CDBResponse& rResponse)
{
	if (SUCCESS != rResponse.mSqlResult)
	{
		return;
	}

	if (rResponse.mRowNum <= 0)
	{
		return;
	}

	mGlobalMailID = rResponse.getUInt();
}


/// ɾ�����ڵ��ʼ�
void CMailModule::deleteExpireMail()
{
	// �õ����� - �ʼ������ʱ�� = ����ʱ�䣬�ȹ���ʱ����Ķ�ɾ����
	time_t nExpireTime = CTimeManager::Inst()->getCurrTime() - MAX_MAIL_DURATION_TIME;
	CDBModule::Inst()->pushDBTask(0, emSessionType_DeleteExpireMail, 0, 0,
		"delete from Mail where create_time <= %d", nExpireTime);
}


/// ɾ�����ڵ�ȫ���ʼ�
void CMailModule::deleteExpireGlobalMail()
{
	time_t nExpireTime = CTimeManager::Inst()->getCurrTime() - MAX_MAIL_DURATION_TIME;
	MAIL_LIST::iterator end = mGlobalList.end();
	for (MAIL_LIST::iterator it = mGlobalList.begin(); it != end; )
	{
		if (nExpireTime >= it->getCreateTime())
		{
			deleteGlobalMail2DB(it->getMailID());
			it = mGlobalList.erase(it);
			continue;
		}
		else
		{
			++ it;
		}
	}
}

/// ȫ���ʼ��б�
void CMailModule::sendGlobalMail(CMail& rMail)
{
	// �����ʼ�ID
	rMail.setMailID(mGlobalMailID++);

	if (mGlobalList.isfull())
	{
		deleteGlobalMail2DB(mGlobalList.back().getMailID());
		mGlobalList.pop_back();
	}
	mGlobalList.push_front(rMail);
	saveGlobalMail2DB(rMail);

	// �����ϵ���ҷ�ȫ���ʼ�
	giveAllPlayerGlobalMail(rMail);
}

/// ����ҷ����ʼ�
void CMailModule::sendPlayerMail(uint nRoleID, CMail& rMail)
{
	// �����ʼ�ID
	rMail.setMailID(mPlayerMailID++);

	// ������ߣ�������ҵ��ʼ��б���
	CEntityPlayer* pPlayer = CSceneJob::Inst()->getPlayerByRoleID(nRoleID);
	if (NULL != pPlayer)
	{
		pPlayer->getInteractiveUnit().addMail(rMail);
	}
	saveMail2DB(nRoleID, rMail);
}

/// ����ҷ����ʼ�
void CMailModule::sendPlayerMail(CEntityPlayer& rPlayer, CMail& rMail)
{
	// �����ʼ�ID
	rMail.setMailID(mPlayerMailID++);
	rPlayer.getInteractiveUnit().addMail(rMail);
	saveMail2DB(rPlayer.getRoleID(), rMail);

}

/// �����ʼ������ݿ�
void CMailModule::saveMail2DB(uint nRoleID, CMail& rMail)
{
	PBMail pbMail;

	pbMail.set_mail_id(rMail.getMailID());
	pbMail.set_mail_status(rMail.GetMailStatus());
	pbMail.set_role_id(nRoleID);
	pbMail.set_mail_type(rMail.getMailType());
	pbMail.set_item_log(rMail.getItemLog());
	pbMail.set_create_time(google::protobuf::uint32(rMail.getCreateTime()));
	pbMail.set_mail_title(rMail.getMailTitle());
	pbMail.set_mail_body(rMail.getMailBody());

	PBMailItemList* pbMailItemList = pbMail.mutable_mail_item();
	saveMailItemList(rMail, pbMailItemList);

	CDBModule::Inst()->pushDBTask(nRoleID, emSessionType_SavePlayerMail, nRoleID, 0, &pbMail);
}

/// ����ȫ���ʼ������ݿ�
void CMailModule::saveGlobalMail2DB(CMail& rMail)
{
	PBGlobalMail pbMail;

	pbMail.set_mail_id(rMail.getMailID());
	pbMail.set_mail_type(rMail.getMailType());
	pbMail.set_create_time(google::protobuf::uint32(rMail.getCreateTime()));
	pbMail.set_mail_title(rMail.getMailTitle());
	pbMail.set_mail_body(rMail.getMailBody());

	PBMailItemList* pbMailItemList = pbMail.mutable_mail_item();
	saveMailItemList(rMail, pbMailItemList);

	CDBModule::Inst()->pushDBTask(0, emSessionType_SaveGlobalMail, 0, 0, &pbMail);
}


/// �����ʼ�
void CMailModule::loadPlayerMail(int nRoleID)
{
	CDBModule::Inst()->pushDBTask(nRoleID, emSessionType_LoadPlayerMail, nRoleID, 0,
		"select mail_id, mail_status, mail_type, item_log, create_time, mail_title, mail_body, mail_item from Mail where role_id=%d",
		nRoleID);
}

/// �����ʼ����ݿ�ص�
void CMailModule::onLoadPlayerMail(CDBResponse& rResponse)
{
	int nPlayerID = rResponse.mPlayerID;
	CEntityPlayer* pPlayer = CSceneJob::Inst()->getPlayerByRoleID(nPlayerID);
	if (NULL == pPlayer)
	{
		return;
	}
	// û���ʼ�
	if (rResponse.mRowNum <= 0)
	{
		return;
	}

	CMail tMail;
	for (int i = 0; i < rResponse.mRowNum; ++i)
	{
		tMail.setMailID(rResponse.getUInt());
		tMail.SetMailStatus((EmMailStatus)rResponse.getByte());
		tMail.setMailType((EmMailType)rResponse.getByte());
		tMail.setItemLog((EmItemLog)rResponse.getShort());
		tMail.setCreateTime(rResponse.getUInt());
		rResponse.getString(tMail.getMailTitle(), MAX_MAIL_TITLE);
		rResponse.getString(tMail.getMailBody(), MAX_MAIL_BODY);

		PBMailItemList pbMailItemList;
		pbMailItemList.ParseFromArray(rResponse.getValue(), rResponse.getLength());
		rResponse.next();

		loadMailItemList(tMail, pbMailItemList);

		pPlayer->getInteractiveUnit().addMail(tMail);
	}

	giveAllGlobalMail(*pPlayer);
}

/// ����ȫ���ʼ�
void CMailModule::loadGlobalMail()
{
	CDBModule::Inst()->pushDBTask(0, emSessionType_LoadGlobalMail, 0, 0,
		"select mail_id, mail_type, create_time, mail_title, mail_body, mail_item from GlobalMail order by mail_id asc");
}

/// ����ȫ���ʼ��ص�
void CMailModule::onLoadGlobalMail(CDBResponse& rResponse)
{
	if (SUCCESS != rResponse.mSqlResult)
	{
		return;
	}
	// û���ʼ�
	if (rResponse.mRowNum <= 0)
	{
		mGlobalLoadComplete = true;
		return;
	}
	CMail tMail;
	for (int i = 0; i < rResponse.mRowNum; ++i)
	{
		tMail.setMailID(rResponse.getUInt());
		tMail.setMailType((EmMailType)rResponse.getByte());
		tMail.setCreateTime(rResponse.getUInt());
		rResponse.getString(tMail.getMailTitle(), MAX_MAIL_TITLE);
		rResponse.getString(tMail.getMailBody(), MAX_MAIL_BODY);

		PBMailItemList pbMailItemList;
		pbMailItemList.ParseFromArray(rResponse.getValue(), rResponse.getLength());
		rResponse.next();
		loadMailItemList(tMail, pbMailItemList);

		mGlobalList.push_front(tMail);
	}

	mGlobalLoadComplete = true;
}

/// �����ʼ������б�
void CMailModule::loadMailItemList(CMail& rMail, PBMailItemList& pbMailItemList)
{
	rMail.clearMailItem();
	for (int i = 0; i < pbMailItemList.mailitem_size(); ++i)
	{
		PBMailItem* pbMailItem = pbMailItemList.mutable_mailitem(i);
		if (NULL == pbMailItem)
		{
			continue;
		}
		CMailItem* pMailItem = rMail.getMailItem(i);
		if (NULL == pMailItem)
		{
			continue;
		}
		pMailItem->setItemID(pbMailItem->itemid());
		pMailItem->setItemNum(pbMailItem->itemnum());
	}
}

/// �����ʼ������б�
void CMailModule::saveMailItemList(CMail& rMail, PBMailItemList* pbMailItemList)
{
	if (NULL == pbMailItemList)
	{
		return;
	}
	for (int i = 0; i < MAX_MAIL_ITEM; ++i)
	{
		CMailItem* pMailItem = rMail.getMailItem(i);
		if (NULL == pMailItem)
		{
			continue;
		}
		if (0 == pMailItem->getItemID())
		{
			break;
		}

		PBMailItem* pbMailItem = pbMailItemList->add_mailitem();
		if (NULL == pbMailItem)
		{
			continue;
		}
		pbMailItem->set_itemid(pMailItem->getItemID());
		pbMailItem->set_itemnum(pMailItem->getItemNum());
	}
}

/// ɾ��ȫ���ʼ�
void CMailModule::deleteGlobalMail2DB(uint nMailID)
{
	CDBModule::Inst()->pushDBTask(0, emSessionType_DeleteGlobalMail, 0, 0,
		"delete from GlobalMail where mail_id=%d", nMailID);
}

/// ����ҷ�ȫ���ʼ�
void CMailModule::giveAllGlobalMail(CEntityPlayer& rPlayer)
{
	if (mGlobalList.empty())
	{
		return;
	}
	uint nGlobalMailTime = rPlayer.getInteractiveUnit().getGlobalMailTime();
	// ���������ϵ����ID
	if (nGlobalMailTime >= mGlobalList.begin()->getCreateTime())
	{
		return;
	}
	MAIL_LIST::iterator end = mGlobalList.end();
	
	for (MAIL_LIST::iterator it = mGlobalList.begin(); it != end; ++it)
	{
		if (it->getCreateTime() <= nGlobalMailTime)
		{
			break;
		}
		
		CMail& rMail = *it;
		sendPlayerMail(rPlayer, rMail);
	}

	rPlayer.getInteractiveUnit().setGlobalMailTime((uint)mGlobalList.begin()->getCreateTime());
}

/// �����Ե���ҷ�ȫ���ʼ�
void CMailModule::giveAllPlayerGlobalMail(CMail& rMail)
{
	CShareObjPoolImp::PlayerEntityPool& rPlayerEntityPool = CObjPool::Inst()->getShareObjPoolImp()->mPlayerEntityPool;
	CEntityPlayer* pPlayer = rPlayerEntityPool.begin();
	for (; pPlayer != NULL; pPlayer = rPlayerEntityPool.next(pPlayer))
	{
		if (emPlayerStatus_Gameing != pPlayer->getPlayerStauts())
		{
			continue;
		}
		sendPlayerMail(*pPlayer, rMail);
		pPlayer->getInteractiveUnit().setGlobalMailTime((uint)rMail.getCreateTime());
	}
}

void CMailModule::gameEvent(uint nRoleID, EmGameEventType eType, PBGameEvent& rGameEvent)
{
	rGameEvent.set_role_id(nRoleID);
	rGameEvent.set_event_type(eType);
	rGameEvent.set_create_time((google::protobuf::uint32)CTimeManager::Inst()->getCurrTime());

	CDBModule::Inst()->pushDBTask(nRoleID, emSessionType_SaveGameEvent, nRoleID, 0, &rGameEvent);
}

void CMailModule::loadGameEvent(uint nRoleID, uint nMaxEventID)
{
	// ����¼�����һ���Լ��ز���Ļ���һ���Լ���500��ƽ��ÿ��event_id��130����ֽ�����
	char* pSql = "select event_id, event_type, event_param from GameEvent where role_id=%d and event_id > %d ordery by event_id asc limit %d";
	CDBModule::Inst()->pushDBTask(nRoleID, emSessionType_LoadGameEvent, nRoleID, 0, pSql, nRoleID, nMaxEventID, MAX_GAME_EVENT_LOAD);
}

void CMailModule::onLoadGameEvent(CDBResponse& rResponse)
{
	if (SUCCESS != rResponse.mSqlResult)
	{
		return;
	}

	uint nMaxEventID = 0;
	uint nEventID = 0;
	uint nRoleID = 0;
	EmGameEventType eType = emGameEvent_None;
	PBGameEventParam tParam;
	for (int i = 0; i < rResponse.mRowNum; ++ i)
	{
		nEventID = rResponse.getUInt();
		nRoleID = rResponse.getUInt();
		eType = (EmGameEventType)rResponse.getByte();

		tParam.ParseFromArray(rResponse.getValue(), rResponse.getLength());
		rResponse.next();
		processGameEvent(nEventID, nRoleID, eType, tParam);
		if (nMaxEventID < nEventID)
		{
			nMaxEventID = nEventID;
		}
	}

	// �����һ���Եļ���������˵�����ܻ���
	if (MAX_GAME_EVENT_LOAD == rResponse.mRowNum)
	{
		loadGameEvent(nRoleID, nMaxEventID);
	}
	else
	{
		// �������
	}
}

void CMailModule::processGameEvent(uint nEventID, uint nRoleID, EmGameEventType eType, PBGameEventParam& rParam)
{
	switch (eType)
	{
		case emGameEvent_None:
		{
			deleteGameEvent(nEventID, nRoleID);
			break;
		}
	}
}

void CMailModule::deleteGameEvent(uint nEventID, uint nRoleID)
{
	char* pSql = "delete from GameEvent where event_id=%d";
	CDBModule::Inst()->pushDBTask(nRoleID, emSessionType_DeleteGameEvent, 0, 0, pSql, nEventID);

}