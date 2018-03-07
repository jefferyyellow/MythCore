#ifndef __CHATMODULE_H__
#define __CHATMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "messagefactory.h"
class CEntityPlayer;
using namespace Myth;

#define MAX_CHAT_CONTENT_LENG	96		// 32 * 3,utf8������3���ַ�
enum EmChatChannel
{
	emChatChannel_None		= 0,		// ��
	emChatChannel_World		= 1,		// ��������
	emChatChannel_Team		= 2,		// �������
	emChatChannel_Faction	= 3,		// ��������
};

class CChatModule : public CLogicModule, public CSingleton<CChatModule>
{
	friend class CSingleton<CChatModule>;
private:
	CChatModule();
	~CChatModule();

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

public:
	void onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage);

public:
	void onChatRequest(CEntityPlayer* pPlayer, Message* pMessage);
};
#endif