#ifndef __TASKMODULE_H__
#define __TASKMODULE_H__
#include "singleton.h"
#include "logicmodule.h"
#include "messagefactory.h"
using namespace Myth;
class CEntityPlayer;
class CTaskModule : public CLogicModule, public CSingleton < CTaskModule >
{
	friend class CSingleton <CTaskModule> ;
private:
	CTaskModule()
	{
	}
	~CTaskModule()
	{
	}

	/// Ê±¼äº¯Êý
	virtual		void OnTimer(unsigned int nTickOffset);

public:
	void onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage);
};
#endif