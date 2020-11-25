#include "mailmodule.h"
#include "mail.h"
#include "entityplayer.h"
#include "dbmodule.hxx.pb.h"
#include "scenejob.h"
#include "dbmodule.h"
#include "errcode.h"
#include "timemanager.h"
#include "objpoolimp.h"
/// 启动服务器
void CMailModule::onLaunchServer()
{
	loadMaxMailID();
	loadMaxGlobalMailID();
	loadGlobalMail();
}

/// 启动完成检查
bool CMailModule::onCheckLaunch()
{
	return mGlobalLoadComplete && (mGlobalMailID > 0)&& (mPlayerMailID > 0);
}

/// 服务器启动成功
void CMailModule::onLaunchComplete()
{

}

/// 退出服务器
void CMailModule::onExitServer()
{
	
}

/// 新一天到来
void CMailModule::onNewDayCome()
{
	deleteExpireMail();
}

/// 新一周到来
void CMailModule::onNewWeekCome()
{

}

/// 建立实体
void CMailModule::onCreatePlayer(CEntityPlayer& rPlayer)
{

}

/// 销毁实体
void CMailModule::onDestroyPlayer(CEntityPlayer& rPlayer)
{

}

/// 时间函数
void CMailModule::onTimer(unsigned int nTickOffset)
{

}

/// 加载配置文件
void CMailModule::onLoadConfig()
{

}

void CMailModule::loadMaxMailID()
{
	CDBModule::Inst()->pushDBTask(0, emSessionType_LoadMaxMailID, 0, 0,
		"select AUTO_INCREMENT from information_schema.tables where table_schema=database() and table_name='Mail'");
}

/// 加载最大的邮件ID回调
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


/// 加载最大的全局邮件ID
void CMailModule::loadMaxGlobalMailID()
{
	CDBModule::Inst()->pushDBTask(0, emSessionType_LoadMaxGlobalMailID, 0, 0,
		"select AUTO_INCREMENT from information_schema.tables where table_schema=database() and table_name='GlobalMail'");

}
/// 加载最大的全局邮件ID回调
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


/// 删除过期的邮件
void CMailModule::deleteExpireMail()
{
	// 得到现在 - 邮件存在最长时间 = 过期时间，比过期时间早的都删除掉
	time_t nExpireTime = CTimeManager::Inst()->getCurrTime() - MAX_MAIL_DURATION_TIME;
	CDBModule::Inst()->pushDBTask(0, emSessionType_DeleteExpireMail, 0, 0,
		"delete from Mail where create_time <= %d", nExpireTime);
}


/// 删除过期的全局邮件
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

/// 全局邮件列表
void CMailModule::sendGlobalMail(CMail& rMail)
{
	// 创建邮件ID
	rMail.setMailID(mGlobalMailID++);

	if (mGlobalList.isfull())
	{
		deleteGlobalMail2DB(mGlobalList.back().getMailID());
		mGlobalList.pop_back();
	}
	mGlobalList.push_front(rMail);
	saveGlobalMail2DB(rMail);

	// 给线上的玩家发全局邮件
	giveAllPlayerGlobalMail(rMail);
}

/// 给玩家发送邮件
void CMailModule::sendPlayerMail(uint nRoleID, CMail& rMail)
{
	// 创建邮件ID
	rMail.setMailID(mPlayerMailID++);

	// 如果在线，加入玩家的邮件列表中
	CEntityPlayer* pPlayer = CSceneJob::Inst()->getPlayerByRoleID(nRoleID);
	if (NULL != pPlayer)
	{
		pPlayer->getInteractiveUnit().addMail(rMail);
	}
	saveMail2DB(nRoleID, rMail);
}

/// 给玩家发送邮件
void CMailModule::sendPlayerMail(CEntityPlayer& rPlayer, CMail& rMail)
{
	// 创建邮件ID
	rMail.setMailID(mPlayerMailID++);
	rPlayer.getInteractiveUnit().addMail(rMail);
	saveMail2DB(rPlayer.getRoleID(), rMail);

}

/// 保存邮件到数据库
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

/// 保存全局邮件到数据库
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


/// 加载邮件
void CMailModule::loadPlayerMail(int nRoleID)
{
	CDBModule::Inst()->pushDBTask(nRoleID, emSessionType_LoadPlayerMail, nRoleID, 0,
		"select mail_id, mail_status, mail_type, item_log, create_time, mail_title, mail_body, mail_item from Mail where role_id=%d",
		nRoleID);
}

/// 加载邮件数据库回调
void CMailModule::onLoadPlayerMail(CDBResponse& rResponse)
{
	int nPlayerID = rResponse.mPlayerID;
	CEntityPlayer* pPlayer = CSceneJob::Inst()->getPlayerByRoleID(nPlayerID);
	if (NULL == pPlayer)
	{
		return;
	}
	// 没有邮件
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

/// 加载全局邮件
void CMailModule::loadGlobalMail()
{
	CDBModule::Inst()->pushDBTask(0, emSessionType_LoadGlobalMail, 0, 0,
		"select mail_id, mail_type, create_time, mail_title, mail_body, mail_item from GlobalMail order by mail_id asc");
}

/// 加载全局邮件回调
void CMailModule::onLoadGlobalMail(CDBResponse& rResponse)
{
	if (SUCCESS != rResponse.mSqlResult)
	{
		return;
	}
	// 没有邮件
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

/// 加载邮件道具列表
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

/// 保存邮件道具列表
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

/// 删除全局邮件
void CMailModule::deleteGlobalMail2DB(uint nMailID)
{
	CDBModule::Inst()->pushDBTask(0, emSessionType_DeleteGlobalMail, 0, 0,
		"delete from GlobalMail where mail_id=%d", nMailID);
}

/// 给玩家发全局邮件
void CMailModule::giveAllGlobalMail(CEntityPlayer& rPlayer)
{
	if (mGlobalList.empty())
	{
		return;
	}
	uint nGlobalMailTime = rPlayer.getInteractiveUnit().getGlobalMailTime();
	// 如果玩家身上的最大ID
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

/// 给所以的玩家发全局邮件
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
	// 如果事件过多一次性加载不完的话，一次性加载500，平均每条event_id有130多个字节左右
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

	// 如果有一次性的加载条数，说明可能还有
	if (MAX_GAME_EVENT_LOAD == rResponse.mRowNum)
	{
		loadGameEvent(nRoleID, nMaxEventID);
	}
	else
	{
		// 加载完成
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