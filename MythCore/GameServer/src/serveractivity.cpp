#include "serveractivity.h"

/// ���������ļ�
int CPhaseActivity::loadActivity(XMLElement* pActivityElem)
{
	mCondList.clear();
	mPrizeList.clear();
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
void CPhaseActivity::refreshPlayerData(CEntityPlayer* pPlayer, int nParam)
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
