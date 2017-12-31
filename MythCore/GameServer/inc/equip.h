#ifndef __EQUIP_H__
#define __EQUIP_H__
#include "itemlist.h"
#include "itemtype.h"
class CEntityPlayer;
class CItemBox;
class CItemObject;
class CEquipList : public CItemList<MAX_EQUIP_LIST>
{
public:
	CEquipList()
	{
	}
	~CEquipList()
	{
	}

public:
	int		equip(CEntityPlayer& rPlayer, CItemBox& rBox, int nBoxIndex, int& rEquipPart);
	int		unequip(CEntityPlayer& rPlayer, int nEquipPart, CItemBox& rBox, int nBoxIndex);
	int		getProperty(int nPropertyType);
};
#endif