#ifndef __MAPMODULE_H__
#define __MAPMODULE_H__
#include "singleton.h"
#include "logicmodule.h"
#include "messagefactory.h"

class CEntity;
class CEntityCharacter;
class CMythPoint;
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

	/// �㲥�������Ŀɼ����
	void		broadCastVisiblePlayer(CEntity* pEntity, unsigned short nMessageID, Message* pMessage);
	/// ʵ���ƶ�
	void		onEntityMove(CEntityCharacter* pEntity, CMythPoint& rDesPos);
};
#endif