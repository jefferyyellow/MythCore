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
	/// �Ƿ����������
	bool			mHeartBeatCheck;
	/// ����ʱ��
	int				mHeartBeatTime;
	/// ���������������ʱ��
	int				mHeartBeatCheckTime;
	/// ���������ڼ��������ٴ���(���������ʾʹ�ü������)
	int				mHeartBeatInvalidMax;
};
#endif