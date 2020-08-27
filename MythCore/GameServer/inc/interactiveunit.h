#ifndef __INTERACTIVEUNIT_H__
#define __INTERACTIVEUNIT_H__
#include "playersubunit.h"
#include "mail.h"
#include "indexarray.h"
#include "messagefactory.h"
using namespace Myth;
class CEntityPlayer;
/// 交互单元,包括邮件，聊天，好友相关
class CInteractiveUnit : public CPlayerSubUnit
{
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
		
	}

public:
	// 得到一封邮件
	void		addMail(CMail& tMail);
	// 删除一封邮件
	void		deleteMail(int nIndex);
	// 得到邮件
	CMail*		getMail(int nMailID);
public:
	/// 获取邮件列表的请求
	void		onGetMailListRequest(Message* pMessage);
	/// 查看邮件的请求
	void		onReadMailRequest(Message* pMessage);
	/// 发送查看邮件的回应
	void		sendReadMailResponse(int nResult, EmMailStatus eStatus);
	/// 获取邮件附件的请求
	void		onGetMailAttachmentRequest(Message* pMessage);
	/// 删除邮件的请求
	void		onDeleteMailRequest(Message* pMessage);

private:
	CIndexArray<CMail, MAX_PLAYER_MAIL>	mMailList;
};
#endif