#ifndef __CHATMODULE_H__
#define __CHATMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "messagefactory.h"
class CEntityPlayer;
using namespace Myth;

// ����Ƶ��
enum EmChatChannel
{
	emChatChannel_None		= 0,		// ��
	emChatChannel_World		= 1,		// ��������
	emChatChannel_Team		= 2,		// �������
	emChatChannel_Faction	= 3,		// ��������
	emChatChannelMax					// ע��ͬ��MAX_CHAT_CHANNEL_NUM
};

// ����֪ͨ����
enum EmWorldNewsType
{
	emWorldNewsType_None	= 0,		// ��
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
	/// ����������
	virtual void onLaunchServer();
	/// ������ɼ��
	virtual bool onCheckLaunch();
	/// �����������ɹ�
	virtual void onLaunchComplete();
	/// �˳�������
	virtual void onExitServer();
	/// ��һ�쵽��
	virtual void onNewDayCome();
	/// ��һ�ܵ���
	virtual void onNewWeekCome();
	/// ����ʵ��
	virtual void onCreatePlayer(CEntityPlayer* pPlayer);
	/// ����ʵ��
	virtual void onDestroyPlayer(CEntityPlayer* pPlayer);
	/// ʱ�亯��
	virtual	void onTimer(unsigned int nTickOffset);
	/// ���������ļ�
	virtual	void onLoadConfig();

public:
	void onClientMessage(CEntityPlayer& rPlayer, unsigned int nMessageID, Message* pMessage);

public:
	void onChatRequest(CEntityPlayer& rPlayer, Message* pMessage);

public:
	/// ������������
	void sendWorldNews(CEntityPlayer& rPlayer, EmWorldNewsType eNewsType, PBNewsParam& rNewParam);
	/// ������������
	void sendWorldNews(EmWorldNewsType eNewsType, PBNewsParam& rNewParam);
	/// �������紫�ŵ������Ϣ
	void createPBNewsRole(CEntityPlayer& rPlayer, PBNewsRole* pbNewRole);
};
#endif