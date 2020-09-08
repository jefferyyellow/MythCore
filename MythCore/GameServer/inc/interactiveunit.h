#ifndef __INTERACTIVEUNIT_H__
#define __INTERACTIVEUNIT_H__
#include "playersubunit.h"
#include "mail.h"
#include "messagefactory.h"
#include "mythlist.h"
#include "chatmodule.h"
using namespace Myth;
class CEntityPlayer;
/// 交互单元,包括邮件，聊天，好友相关
class CInteractiveUnit : public CPlayerSubUnit
{
public:
	typedef CMythList<CMail, MAX_PLAYER_MAIL>	MAIL_LIST;

public:
	CInteractiveUnit(CEntityPlayer& rPlayer)
		:CPlayerSubUnit(rPlayer)
	{
		init();
	}
	~CInteractiveUnit()
	{

	}
	void init()
	{
		mGlobalMailTime = 0;
		memset(mChatTime, 0, sizeof(mChatTime));
	}

public:
	//// 获得一封邮件
	void		addMail(CMail& tMail);
	// 得到邮件ID对应的迭代器
	MAIL_LIST::iterator	getMail(uint nMailID);
	/// 从数据库删除对应ID的邮件
	void		deleteMailFromDB(uint nMailID);
	/// 更新邮件的状态到数据库
	void		updateMailStatus2DB(uint nMailID, EmMailStatus eMailStatus);

public:
	/// 获取邮件列表的请求
	void		onGetMailListRequest(Message* pMessage);
	/// 查看邮件的请求
	void		onReadMailRequest(Message* pMessage);
	/// 发送查看邮件的回应
	void		sendReadMailResponse(int nResult, EmMailStatus eStatus);
	/// 获取邮件附件的请求
	void		onGetMailAttachmentRequest(Message* pMessage);
	/// 发送获取邮件附件的回应
	void		sendGetMailAttachmentResponse(int nResult, int nStatus);
	/// 删除邮件的请求
	void		onDeleteMailRequest(Message* pMessage);
	/// 发送删除邮件的回应
	void		sendDeleteMailResponse(int nResult);
	/// 得到邮件的详细信息请求
	void		onGetMailDetailRequest(Message* pMessage);
	/// 发送邮件详细信息的回应
	void		sendGetMailDetailResponse(int nResult, CMail& rMail);
public:
	/// 获取过的最大全局邮件ID
	uint		getGlobalMailTime() const { return mGlobalMailTime; }
	void		setGlobalMailTime(uint nValue) { mGlobalMailTime = nValue; }

	/// 邮件列表
	MAIL_LIST&	getMailList(){return mMailList;}

	time_t getChatTime(uint nChannelID) const 
	{ 
		if (nChannelID >= emChatChannelMax)
		{
			return 0;
		}
		return mChatTime[nChannelID]; 
	}
	void setChatTime(uint nChannelID, time_t nValue) 
	{ 
		if (nChannelID >= emChatChannelMax)
		{
			return;
		}
		mChatTime[nChannelID] = nValue; 
	}

private:
	/// 邮件列表
	MAIL_LIST	mMailList;
	/// 收过的最新的全局邮件时间，注意不能用全局邮件ID，因为涉及到合服
	uint		mGlobalMailTime;
	/// 世界聊天最后的时间
	time_t		mChatTime[emChatChannelMax];
};
#endif