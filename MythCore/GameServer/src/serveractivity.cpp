#include "serveractivity.h"

/// ���������ļ�
int CPhaseActivity::loadActivity(XMLElement* pActivityElem)
{
	mCondDataList.clear();
	mActivityPrizeList.clear();


}

/// �õ������ļ�������
const char* CPhaseActivity::getConfigFileName()
{
	return "PhaseActivity.xml";
}

/// �����
void CPhaseActivity::start()
{

}

/// �����
void CPhaseActivity::end()
{

}

/// ˢ���������
void CPhaseActivity::refreshPlayerData(CEntityPlayer* pPlayer)
{
	if (NULL == pPlayer)
	{
		return;
	}


}

/// ����������
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
