#ifndef __MAPCONFIGMANAGER_H__
#define __MAPCONFIGMANAGER_H__
#include "singleton.h"
using namespace Myth;
class CMapConfigManager : public CSingleton < CMapConfigManager >
{
	friend class CSingleton< CMapConfigManager >;

private:
	CMapConfigManager()
	{
	}
	~CMapConfigManager()
	{
	}


};
#endif