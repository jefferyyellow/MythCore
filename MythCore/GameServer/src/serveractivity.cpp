#include "serveractivity.h"

/// 加载配置文件
int CPhaseActivity::loadActivity(XMLElement* pActivityElem)
{
	mCondDataList.clear();
	mActivityPrizeList.clear();


}

/// 得到配置文件的名字
const char* CPhaseActivity::getConfigFileName()
{
	return "PhaseActivity.xml";
}

/// 活动开启
void CPhaseActivity::start()
{

}

/// 活动结束
void CPhaseActivity::end()
{

}

/// 刷新玩家数据
void CPhaseActivity::refreshPlayerData(CEntityPlayer* pPlayer)
{
	if (NULL == pPlayer)
	{
		return;
	}


}

/// 清空玩家数据
void CPhaseActivity::clearPlayerData(CEntityPlayer* pPlayer)
{
	if (NULL == pPlayer)
	{
		return;
	}

	for (int i = 0; i < )
	{
	}
}
