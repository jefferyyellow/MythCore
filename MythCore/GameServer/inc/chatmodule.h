#ifndef __CHATMODULE_H__
#define __CHATMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
using namespace Myth;
class CChatModule : public CLogicModule, public CSingleton<CChatModule>
{
	friend class CSingleton<CChatModule>;
public:
	CChatModule();
	~CChatModule();
};
#endif