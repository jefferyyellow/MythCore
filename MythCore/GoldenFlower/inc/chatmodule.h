#ifndef __CHATMODULE_H__
#define __CHATMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "messagefactory.h"
class CEntityPlayer;
using namespace Myth;

#define MAX_CHAT_CONTENT_LENG	96		// 32 * 3,utf8中文是3个字符
enum EmChatChannel
{
	emChatChannel_None		= 0,		// 无
	emChatChannel_World		= 1,		// 世界聊天
	emChatChannel_Team		= 2,		// 组队聊天
	emChatChannel_Faction	= 3,		// 工会聊天
};

class CChatModule : public CLogicModule, public CSingleton<CChatModule>
{
	friend class CSingleton<CChatModule>;
private:
	CChatModule();
	~CChatModule();

public:
	/// 启动服务器
	virtual void onLaunchServer();
	/// 启动完成检查
	virtual bool onCheckLaunch();
	/// 服务器启动成功
	virtual void onLaunchComplete();
	/// 退出服务器
	virtual void onExitServer();
	/// 新一天到来
	virtual void onNewDayCome();
	/// 新一周到来
	virtual void onNewWeekCome();
	/// 建立实体
	virtual void onCreatePlayer(CEntityPlayer* pPlayer);
	/// 销毁实体
	virtual void onDestroyPlayer(CEntityPlayer* pPlayer);
	/// 时间函数
	virtual	void onTimer(unsigned int nTickOffset);

public:
	void onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage);

public:
	void onChatRequest(CEntityPlayer* pPlayer, Message* pMessage);
};
#endif