#ifndef __CHATMODULE_H__
#define __CHATMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "messagefactory.h"
class CEntityPlayer;
using namespace Myth;

// 聊天频道
enum EmChatChannel
{
	emChatChannel_None		= 0,		// 无
	emChatChannel_World		= 1,		// 世界聊天
	emChatChannel_Team		= 2,		// 组队聊天
	emChatChannel_Faction	= 3,		// 工会聊天
	emChatChannelMax					// 注意同步MAX_CHAT_CHANNEL_NUM
};

// 世界通知类型
enum EmWorldNewsType
{
	emWorldNewsType_None	= 0,		// 无
};

class PBNewsParam;
class PBNewsRole;
class CChatModule : public CLogicModule, public CSingleton<CChatModule>
{
	friend class CSingleton<CChatModule>;
private:
	CChatModule();
	~CChatModule();

public:
	void	init();

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
	/// 加载配置文件
	virtual	void onLoadConfig();

public:
	void onClientMessage(CEntityPlayer& rPlayer, unsigned int nMessageID, Message* pMessage);

public:
	void onChatRequest(CEntityPlayer& rPlayer, Message* pMessage);

public:
	/// 发送世界新闻
	void sendWorldNews(CEntityPlayer& rPlayer, EmWorldNewsType eNewsType, PBNewsParam& rNewParam);
	/// 发送世界新闻
	void sendWorldNews(EmWorldNewsType eNewsType, PBNewsParam& rNewParam);
	/// 创建世界传闻的玩家信息
	void createPBNewsRole(CEntityPlayer& rPlayer, PBNewsRole* pbNewRole);
};
#endif