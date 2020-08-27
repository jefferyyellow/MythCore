#include "interactiveunit.h"
#include "propertymodule.hxx.pb.h"
#include "scenejob.h"
#include "locallogjob.h"
#include "itemunit.h"
#include "errcode.h"
void CInteractiveUnit::addMail(CMail& tMail)
{
	// ����ʼ��б�����,����һ���ʼ�ɾ��
	if (mMailList.size() >= mMailList.capacity())
	{
		mMailList.erase(0);
	}

	mMailList.push_back(tMail);
}

void CInteractiveUnit::deleteMail(int nIndex)
{
	mMailList.erase(nIndex);
}

// �õ��ʼ�
CMail* CInteractiveUnit::getMail(int nMailID)
{
	for (uint i = 0; i < mMailList.size(); ++i)
	{
		if (mMailList[i].getMailID() == nMailID)
		{
			return &mMailList[i];
		}
	}
	return NULL;
}


/// ��ȡ�ʼ��б������
void CInteractiveUnit::onGetMailListRequest(Message* pMessage)
{
	CGetMailListResponse tResponse;
	for (uint i = 0; i < mMailList.size(); ++ i)
	{
		PBMail* pbMail = tResponse.add_mail();
		if (NULL == pbMail)
		{
			continue;
		}
		CMail& rMail = mMailList[i];
		pbMail->set_mail_status(rMail.GetMailStatus());
		pbMail->set_mail_type(rMail.getMailType());
		pbMail->set_create_time((google::protobuf::uint32)rMail.getCreateTime());
		pbMail->set_mail_title(rMail.getMailTitle());
	}
	CSceneJob::Inst()->send2Player(&mPlayer, ID_S2C_RESPONSE_GET_MAIL_LIST, &tResponse);
}

/// �鿴�ʼ�������
void CInteractiveUnit::onReadMailRequest(Message* pMessage)
{
	CReadMailRequest* pRequest = (CReadMailRequest*)pMessage;
	MYTH_ASSERT(NULL != pRequest, return);
	int nMailID = pRequest->mailid();

	CMail* pMail = getMail(nMailID);
	if (NULL == pMail)
	{
		return;
	}
	EmMailStatus status = pMail->GetMailStatus();
	// �Ѿ�������
	if (status & emMailStatus_Read)
	{
		return;
	}

	pMail->SetMailStatus((EmMailStatus)(emMailStatus_Read | status));
	sendReadMailResponse(SUCCESS, pMail->GetMailStatus());
}

/// ���Ͳ鿴�ʼ��Ļ�Ӧ
void CInteractiveUnit::sendReadMailResponse(int nResult, EmMailStatus eStatus)
{
	CReadMailResponse tResponse;
	tResponse.set_result(nResult);
	tResponse.set_status(eStatus);

	CSceneJob::Inst()->send2Player(&mPlayer, ID_S2C_RESPONSE_READ_MAIL, &tResponse);
}

/// ��ȡ�ʼ�����������
void CInteractiveUnit::onGetMailAttachmentRequest(Message* pMessage)
{
	MYTH_ASSERT(NULL != pMessage, return);
	CGetMailAttachmentRequest* pRequest = (CGetMailAttachmentRequest*)pMessage;
	int nMailID = pRequest->mailid();
	
	CMail* pMail = getMail(nMailID);
	if (NULL == pMail)
	{
		return;
	}
	EmMailStatus status = pMail->GetMailStatus();
	// �Ѿ���ȡ��������
	if (status & emMailStatus_Attatch)
	{
		return;
	}

	CItemArrange tItemArrange;
	for (uint i = 0; i < MAX_MAIL_ITEM; ++ i)
	{
		CMailItem* pMailItem = pMail->getMailItem(i);
		if (NULL == pMailItem)
		{
			continue;
		}

		tItemArrange.addItem(pMailItem->getItemID(), pMailItem->getItemNum());
	}


	pMail->SetMailStatus((EmMailStatus)(emMailStatus_Attatch | status));
}

/// ɾ���ʼ�������
void CInteractiveUnit::onDeleteMailRequest(Message* pMessage)
{

}