#ifndef __MAPMODULE_H__
#define __MAPMODULE_H__
#include "singleton.h"
#include "logicmodule.h"
using namespace Myth;
class CMapModule : public CLogicModule, public CSingleton < CMapModule >
{
	friend class CSingleton < CMapModule > ;
private:
	CMapModule()
	{
	}
	~CMapModule()
	{
	}

	// ´«ËÍ
	// ¹ã²¥

};
#endif