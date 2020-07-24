#ifndef __GAMELOGICCONFIG_H__
#define __GAMELOGICCONFIG_H__
#include "servercommon.h"
#include "singleton.h"
using namespace Myth;
class CGameLogicConfig : public CSingleton<CGameLogicConfig>
{
	friend class CSingleton<CGameLogicConfig>;
private:
	CGameLogicConfig()
	{
		init();
	}

	~CGameLogicConfig(){}

	void init()
	{
		mHeartBeatCheck = false;
		mHeartBeatTime = 0;
		mHeartBeatCheckTime = 0;
		mHeartBeatInvalidMax = 0;
	}

public:
	bool			loadGameLogicConfig(const char* pFilePath);

public:
	/// 是否开启心跳检测
	bool			mHeartBeatCheck;
	/// 心跳时间
	int				mHeartBeatTime;
	/// 心跳计数检测周期时间
	int				mHeartBeatCheckTime;
	/// 心跳计数期间心跳过速次数(超过这个表示使用加速外挂)
	int				mHeartBeatInvalidMax;
};
#endif