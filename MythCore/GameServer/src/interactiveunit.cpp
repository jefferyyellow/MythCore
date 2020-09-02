#include "interactiveunit.h"
#include "propertymodule.hxx.pb.h"
#include "scenejob.h"
#include "locallogjob.h"
#include "itemunit.h"
#include "errcode.h"
#include "entityplayer.h"
#include "dbmodule.h"
#include "mailmodule.h"
void CInteractiveUnit::addMail(CMail& tMail)
{
	// 如果邮件列表已满,将第一封邮件删除
	if (mMailList.isfull())
	{
		mMailList.erase(mMailList.begin());
	}

	mMailList.push_back(tMail);
}

// 得到邮件
CInteractiveUnit::MAIL_LIST::iterator CInteractiveUnit::getMail(uint nMailID)
{
	MAIL_LIST::iterator end = mMailList.end();
	for (MAIL_LIST::iterator it = mMailList.begin(); it != end; ++ it)
	{
		if (it->getMailID() == nMailID)
		{
			return it;
		}
	}
	return mMailList.end();
}

/// 从数据库删除对应ID的邮件
void CInteractiveUnit::deleteMailFromDB(uint nMailID)
{
	CDBModule::Inst()->pushDBTask(0, emSessionType_DeleteMail, 0, 0, 
		"delete from Mail where mail_id=%d", nMailID);
}

/// 更新邮件的状态到数据库
void CInteractiveUnit::updateMailStatus2DB(uint nMailID, EmMailStatus eMailStatus)
{
	CDBModule::Inst()->pushDBTask(0, emSessionType_UpdateMailStatus, 0, 0, 
		"update Mail set mail_status=%d where mail_id=%d", eMailStatus, nMailID);
}


/// 获取邮件列表的请求
void CInteractiveUnit::onGetMailListRequest(Message* pMessage)
{
	CGetMailListResponse tResponse;
	MAIL_LIST::iterator end = mMailList.end();
	for (MAIL_LIST::iterator it = mMailList.begin(); it != end; ++it)
	{
		PBMail* pbMail = tResponse.add_mail();
		if (NULL == pbMail)
		{
			continue;
		}
		CMail& rMail = *it;
		pbMail->set_mail_id(rMail.getMailID());
		pbMail->set_mail_status(rMail.GetMailStatus());
		pbMail->set_mail_type(rMail.getMailType());
		pbMail->set_create_time((google::protobuf::uint32)rMail.getCreateTime());
		pbMail->set_mail_title(rMail.getMailTitle());
	}
	CSceneJob::Inst()->send2Player(mPlayer, ID_S2C_RESPONSE_GET_MAIL_LIST, &tResponse);
}

/// 查看邮件的请求
void CInteractiveUnit::onReadMailRequest(Message* pMessage)
{
	CReadMailRequest* pRequest = (CReadMailRequest*)pMessage;
	MYTH_ASSERT(NULL != pRequest, return);
	int nMailID = pRequest->mailid();

	MAIL_LIST::iterator it = getMail(nMailID);
	if (it == mMailList.end())
	{
		return;
	}
	CMail rMail = *it;
	EmMailStatus status = rMail.GetMailStatus();
	// 已经读过了
	if (status & emMailStatus_Read)
	{
		return;
	}

	rMail.SetMailStatus((EmMailStatus)(emMailStatus_Read | status));
	updateMailStatus2DB(nMailID, rMail.GetMailStatus());
	sendReadMailResponse(SUCCESS, rMail.GetMailStatus());
}

/// 发送查看邮件的回应
void CInteractiveUnit::sendReadMailResponse(int nResult, EmMailStatus eStatus)
{
	CReadMailResponse tResponse;
	tResponse.set_result(nResult);
	tResponse.set_status(eStatus);

	CSceneJob::Inst()->send2Player(mPlayer, ID_S2C_RESPONSE_READ_MAIL, &tResponse);
}

/// 获取邮件附件的请求
void CInteractiveUnit::onGetMailAttachmentRequest(Message* pMessage)
{
	MYTH_ASSERT(NULL != pMessage, return);
	CGetMailAttachmentRequest* pRequest = (CGetMailAttachmentRequest*)pMessage;
	int nMailID = pRequest->mailid();
	
	MAIL_LIST::iterator it = getMail(nMailID);
	if (it == mMailList.end())
	{
		return;
	}
	CMail rMail = *it;
	EmMailStatus status = rMail.GetMailStatus();
	// 已经获取过附件了
	if (status & emMailStatus_Attatch)
	{
		return;
	}

	CItemArrange tItemArrange;
	for (uint i = 0; i < MAX_MAIL_ITEM; ++ i)
	{
		CMailItem* pMailItem = rMail.getMailItem(i);
		if (NULL == pMailItem)
		{
			continue;
		}

		tItemArrange.addItem(pMailItem->getItemID(), pMailItem->getItemNum());
	}

	if (!mPlayer.getItemUnit().checkItemSpace(tItemArrange.getIDPtr(), tItemArrange.getNumPtr(), tItemArrange.GetCount()))
	{
		return;
	}

	mPlayer.getItemUnit().insertAllItem(tItemArrange.getIDPtr(), tItemArrange.getNumPtr(), tItemArrange.GetCount());
	rMail.SetMailStatus((EmMailStatus)(emMailStatus_Attatch | status));

	updateMailStatus2DB(nMailID, rMail.GetMailStatus());
	sendGetMailAttachmentResponse(SUCCESS, rMail.GetMailStatus());
}

/// 发送获取邮件附件的回应
void CInteractiveUnit::sendGetMailAttachmentResponse(int nResult, int nStatus)
{
	CGetMailAttachmentResponse tResponse;
	tResponse.set_result(nResult);
	tResponse.set_status(nStatus);

	CSceneJob::Inst()->send2Player(mPlayer, ID_S2C_RESPONSE_GET_MAIL_ATTACHMENT, &tResponse);
}

/// 删除邮件的请求
void CInteractiveUnit::onDeleteMailRequest(Message* pMessage)
{
	MYTH_ASSERT(NULL != pMessage, return);
	CDeleteMailRequest* pRequest = (CDeleteMailRequest*)pMessage;

	MAIL_LIST::iterator end = mMailList.end();
	for (int i = 0; i < pRequest->mailid_size(); ++ i)
	{
		MAIL_LIST::iterator it = getMail(pRequest->mailid(i));
		if (it == end)
		{
			continue;
		}
		// 从数据库删除
		deleteMailFromDB(it->getMailID());
		// 从邮件列表删除
		mMailList.erase(it);
	}

	sendDeleteMailResponse(SUCCESS);
}

// 发送删除邮件的回应
void CInteractiveUnit::sendDeleteMailResponse(int nResult)
{
	CDeleteMailResponse tResponse;
	tResponse.set_result(nResult);

	CSceneJob::Inst()->send2Player(mPlayer, ID_S2C_RESPONSE_DELETE_MAIL, &tResponse);
}

/// 得到邮件的详细信息请求
void CInteractiveUnit::onGetMailDetailRequest(Message* pMessage)
{
	MYTH_ASSERT(NULL != pMessage, return);
	CGetMailDetailRequest* pRequest = (CGetMailDetailRequest*)pMessage;
	int nMailID = pRequest->mailid();

	MAIL_LIST::iterator it = getMail(nMailID);
	if (it == mMailList.end())
	{
		return;
	}
	CMail rMail = *it;
	sendGetMailDetailResponse(SUCCESS, rMail);
}

/// 发送邮件详细信息的回应
void CInteractiveUnit::sendGetMailDetailResponse(int nResult, CMail& rMail)
{
	CGetMailDetailResponse tResponse;
	tResponse.set_result(nResult);
	PBMail* pbMail = tResponse.mutable_mail();
	if (NULL != pbMail)
	{
		pbMail->set_mail_id(rMail.getMailID());
		pbMail->set_mail_status(rMail.GetMailStatus());
		pbMail->set_mail_type(rMail.getMailType());
		pbMail->set_create_time((google::protobuf::uint32)rMail.getCreateTime());
		pbMail->set_mail_title(rMail.getMailTitle());
		pbMail->set_mail_body(rMail.getMailBody());
		PBMailItemList* pbItemList = pbMail->mutable_mail_item();
		CMailModule::Inst()->saveMailItemList(rMail, pbItemList);
	}

	CSceneJob::Inst()->send2Player(mPlayer, ID_S2C_RESPONSE_GET_MAIL_DETAIL, &tResponse);
}
