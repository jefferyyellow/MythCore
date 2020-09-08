#include "template.h"
#include <stddef.h>
#include "template.hxx.pb.h"
#ifndef TEMPEDIT
#include "locallogjob.h"
#else
#include "servercommon.h"
#include "logmanager.h"
using namespace Myth;
#endif
#include <iostream>
#include <fstream>

CTemplate* CStaticData::spTemplate[ MAX_TEMPLATE_NUM ]							= { NULL };
int	CStaticData::mVersion														= 0;

CTplLevelExpConfig* CTplLevelExpConfig::spConfig								= NULL;
CTplVIPConfig* CTplVIPConfig::spConfig											= NULL;
CTplNewPlayerConfig* CTplNewPlayerConfig::spConfig								= NULL;

CTemplate* CStaticData::searchTpl(unsigned int nTempID)
{
	// 注意0也是非法值
	if (nTempID <= 0 || nTempID >= MAX_TEMPLATE_NUM)
	{
#ifdef LINUX
		//LOG_ERROR( "default", "can't find obj TempID: %d", vTempID );
#endif
		return NULL;
	}

	return spTemplate[nTempID];
}

/// 从文件中加载
bool CStaticData::loadFromFile(const char* pFilePath)
{
	PBTplTemplate* pTplTemplate = new PBTplTemplate();
	if (pTplTemplate == NULL)
	{
		//LOG_ERROR("New PBTplTemplate failed");
		return false;
	}

	char* pcRetBuffer = (char*)malloc(TEMPLATE_CACHE_SIZE);
	if (pcRetBuffer == NULL)
	{
		//LOG_ERROR("malloc template cache failed");
		return false;
	}

	std::fstream fstm;
	fstm.open(pFilePath, std::ios::in | std::ios::binary);
	if (fstm.is_open() == false)
	{
		//LOG_ERROR("open fsteam failed.\n");
		free(pcRetBuffer);
		pcRetBuffer = NULL;
		return false;
	}

	fstm.seekg(0, std::ios::end);
	int nFileSize = (int)fstm.tellg();
	fstm.seekg(0, std::ios::beg);

	fstm.read((char*)&mVersion, sizeof(int));

	char* tpTempBuffer = pcRetBuffer;

	int tLen = 0;

	while (!fstm.eof() && tLen < TEMPLATE_CACHE_SIZE)
	{
		fstm.read(tpTempBuffer, nFileSize);
		tpTempBuffer += fstm.gcount();
		tLen += (int)fstm.gcount();
	}

	if (pTplTemplate->ParseFromArray(pcRetBuffer, tLen) == false)
	{
		//LOG_ERROR("parse failed");
		free(pcRetBuffer);
		pcRetBuffer = NULL;
		return false;
	}

	fstm.close();
#ifdef __DEBUG__
	std::ofstream* fLog = new std::ofstream("template.txt");
	*fLog << pTplTemplate->DebugString() << std::endl;
	delete fLog;
#endif
	free(pcRetBuffer);
	pcRetBuffer = NULL;

	createFromPB(pTplTemplate);
	delete (pTplTemplate);
	return true;
}

/// 从PB中创建
void CStaticData::createFromPB(PBTplTemplate* pTplTemplate)
{
	if (NULL == pTplTemplate)
	{
		return;
	}

	PBTplItemSet*	pbTplItemSet = pTplTemplate->mutable_itemset();
	PBTplConfigSet* pbTplConfigSet = pTplTemplate->mutable_configset();
	PBTplSkillSet*	pbTplSkillSet = pTplTemplate->mutable_skillset();
	PBTplNPCSet*	pbTplNPCSet = pTplTemplate->mutable_npcset();

	TEMPLATE_SET_FROM_PB(CTplLevelExpConfig, pbTplConfigSet, levelexpconfig);
	TEMPLATE_SET_FROM_PB(CTplVIPConfig, pbTplConfigSet, vipconfig);
	TEMPLATE_SET_FROM_PB(CTplFuncNPC, pbTplNPCSet, funcnpc);
	TEMPLATE_SET_FROM_PB(CTplOgre, pbTplNPCSet, ogre);
	TEMPLATE_SET_FROM_PB(CTplEquip, pbTplItemSet, equip);
	TEMPLATE_SET_FROM_PB(CTplSkill, pbTplSkillSet, skill);
	TEMPLATE_SET_FROM_PB(CTplDropTable, pbTplConfigSet, droptable);
	TEMPLATE_SET_FROM_PB(CTplNewPlayerConfig, pbTplConfigSet, newplayerconfig);
}

void CStaticData::clearTemplate()
{
	for (int i = 0; i < MAX_TEMPLATE_NUM; ++ i)
	{
		if (NULL != spTemplate[i])
		{
			free(spTemplate[i]);
			spTemplate[i] = NULL;
		}
	}
}

void CTplNewPlayerConfig::setFromPB(PBTplNewPlayerConfig* pbData)
{
	mTempID = pbData->tempid();
	for (int i = 0; i < MAX_NEW_PLAYER_ITEM && i < pbData->itemid_size(); ++i)
	{
		mItemID[i] = pbData->itemid(i);
	}
	for (int i = 0; i < MAX_NEW_PLAYER_ITEM && i < pbData->itemnum_size(); ++i)
	{
		mItemNum[i] = pbData->itemnum(i);
	}
	mLevel = pbData->level();
	mVipLevel = pbData->viplevel();
}
void CTplNewPlayerConfig::createToPB(PBTplNewPlayerConfig* pbData)
{
	pbData->set_tempid(mTempID);
	for (int i = 0; i < MAX_NEW_PLAYER_ITEM; ++i)
	{
		pbData->add_itemid(mItemID[i]);
	}
	for (int i = 0; i < MAX_NEW_PLAYER_ITEM; ++i)
	{
		pbData->add_itemnum(mItemNum[i]);
	}
	pbData->set_level(mLevel);
	pbData->set_viplevel(mVipLevel);
}

void CTplItem::setFromPB(PBItem* pbItem)
{
	if (NULL == pbItem)
	{
		return;
	}
	mTempID = pbItem->tempid();
	strncpy(mName, pbItem->name().c_str(), sizeof(mName) - 1);
	strncpy(mDescribe, pbItem->describe().c_str(), sizeof(mDescribe) - 1);
	mItemType = pbItem->itemtype();
	mPileLimit = pbItem->pilelimit();
}

void CTplItem::createToPB(PBItem* pbItem)
{
	if (NULL == pbItem)
	{
		return;
	}
	pbItem->set_tempid(mTempID);
	pbItem->set_name(mName);
	pbItem->set_describe(mDescribe);
	pbItem->set_itemtype(mItemType);
	pbItem->set_pilelimit(mPileLimit);
}
void CTplEquip::setFromPB(PBTplEquip* pbData)
{
	CTplItem::setFromPB(pbData->mutable_super());
	strncpy(mEquipModal, pbData->equipmodal().c_str(), sizeof(mEquipModal)-1);
	mEquipPart = pbData->equippart();
	mLevelRequire = pbData->levelrequire();
	for (int i = 0; i < EQUIP_PROPERTY_NUM && i < pbData->property_size(); ++i)
	{
		mProperty[i].setFromPB(pbData->mutable_property(i));
	}
}
void CTplEquip::createToPB(PBTplEquip* pbData)
{
	CTplItem::createToPB(pbData->mutable_super());
	pbData->set_equipmodal(mEquipModal);
	pbData->set_equippart(mEquipPart);
	pbData->set_levelrequire(mLevelRequire);
	for (int i = 0; i < EQUIP_PROPERTY_NUM; ++i)
	{
		mProperty[i].createToPB(pbData->add_property());
	}
}


void CTplEquip::CProperty::setFromPB(PBTplProperty* pbData)
{
	mType = pbData->type();
	mValue = pbData->value();
}
void CTplEquip::CProperty::createToPB(PBTplProperty* pbData)
{
	pbData->set_type(mType);
	pbData->set_value(mValue);
}

void CTplLevelExpConfig::setFromPB(PBTplLevelExpConfig* pbConfig)
{
	if (NULL == pbConfig)
	{
		return;
	}

	mTempID = pbConfig->tempid();
	for (int i = 0; i < MAX_LEVEL && i < pbConfig->levelupexp_size(); ++ i)
	{
		mLevelUpExp[i] = pbConfig->levelupexp(i);
	}
}

void CTplLevelExpConfig::createToPB(PBTplLevelExpConfig* pbConfig)
{
	if (NULL == pbConfig)
	{
		return;
	}

	pbConfig->set_tempid(mTempID);
	for (int i = 0; i < MAX_LEVEL; ++ i)
	{
		pbConfig->add_levelupexp(mLevelUpExp[i]);
	}
}

void CTplVIPConfig::setFromPB(PBTplVIPConfig* pbConfig)
{
	if (NULL == pbConfig)
	{
		return;
	}
	mTempID = pbConfig->tempid();
	for (int i = 0; i < MAX_VIP_LEVEL && i < pbConfig->vipexp_size(); ++ i)
	{
		mVIPExp[i] = pbConfig->vipexp(i);
	}
}

void CTplVIPConfig::createToPB(PBTplVIPConfig* pbConfig)
{
	if (NULL == pbConfig)
	{
		return;
	}
	pbConfig->set_tempid(mTempID);
	for (int i = 0; i < MAX_VIP_LEVEL; ++ i)
	{
		pbConfig->add_vipexp(mVIPExp[i]);
	}
}

void CTplNPC::setFromPB(PBTplNPC* pbNpc)
{
	if (NULL == pbNpc)
	{
		return;
	}

	mTempID = pbNpc->tempid();
	strncpy(mName, pbNpc->name().c_str(), sizeof(mName) - 1);

}

void CTplNPC::createToPB(PBTplNPC* pbNpc)
{
	if (NULL == pbNpc)
	{
		return;
	}
	pbNpc->set_tempid(mTempID);
	pbNpc->set_name(mName);
}

void CTplFuncNPC::setFromPB(PBTplFuncNPC* pbFuncNpc)
{
	if (NULL == pbFuncNpc)
	{
		return;
	}

	CTplNPC::setFromPB(pbFuncNpc->mutable_baseinfo());
}

void CTplFuncNPC::createToPB(PBTplFuncNPC* pbFuncNpc)
{
	if (NULL == pbFuncNpc)
	{
		return;
	}

	CTplNPC::createToPB(pbFuncNpc->mutable_baseinfo());
}

void CTplOgre::setFromPB(PBTplOgre* pbData)
{
	CTplNPC::setFromPB(pbData->mutable_super());
	mExp = pbData->exp();
	for (int i = 0; i < MAX_OGRE_DROP && i < pbData->droptable_size(); ++i)
	{
		mDropTable[i] = pbData->droptable(i);
	}
}

void CTplOgre::createToPB(PBTplOgre* pbData)
{
	CTplNPC::createToPB(pbData->mutable_super());
	pbData->set_exp(mExp);
	for (int i = 0; i < MAX_OGRE_DROP; ++i)
	{
		pbData->add_droptable(mDropTable[i]);
	}
}

void CTplSkill::setFromPB(PBTplSkill* pbData)
{
	mTempID = pbData->tempid();
	strncpy(mName, pbData->name().c_str(), sizeof(mName)-1);
	strncpy(mDescription, pbData->description().c_str(), sizeof(mDescription)-1);
	mSkillCD = pbData->skillcd();
	mAddDamage = pbData->adddamage();
}
void CTplSkill::createToPB(PBTplSkill* pbData)
{
	pbData->set_tempid(mTempID);
	pbData->set_name(mName);
	pbData->set_description(mDescription);
	pbData->set_skillcd(mSkillCD);
	pbData->set_adddamage(mAddDamage);
}


void CTplDropTable::setFromPB(PBTplDropTable* pbData)
{
	mTempID = pbData->tempid();
	for (int i = 0; i < MAX_DROP_ITEM && i < pbData->dropitem_size(); ++i)
	{
		mDropItem[i].setFromPB(pbData->mutable_dropitem(i));
	}

	for (int i = 1; i < MAX_DROP_ITEM; i++)
	{
		mDropItem[i].mProbability = mDropItem[i].mProbability + mDropItem[i - 1].mProbability;
	}


	// 加载后的处理
	if (mDropItem[MAX_DROP_ITEM - 1].mProbability > 10000)
	{
		printf("drop table probability is invalid %d", mTempID);
		return ;
	}
}
void CTplDropTable::createToPB(PBTplDropTable* pbData)
{
	pbData->set_tempid(mTempID);
	for (int i = 0; i < MAX_DROP_ITEM; ++i)
	{
		mDropItem[i].createToPB(pbData->add_dropitem());
	}
}


void CTplDropTable::CDropItem::setFromPB(PBTplDropItem* pbData)
{
	mItemID = pbData->itemid();
	mItemNum = pbData->itemnum();
	mProbability = pbData->probability();
}
void CTplDropTable::CDropItem::createToPB(PBTplDropItem* pbData)
{
	pbData->set_itemid(mItemID);
	pbData->set_itemnum(mItemNum);
	pbData->set_probability(mProbability);
}

CTplChatConfig* CTplChatConfig::spChatConfig = NULL;
void CTplChatConfig::CChatChannelLimit::setFromPB(PBChatChannelLimit* pbLimit)
{
	mWordNum = pbLimit->wordnum();
	mPlayerLevel = pbLimit->playerlevel();
	mInterval = pbLimit->interval();
}


void CTplChatConfig::CChatChannelLimit::createToPB(PBChatChannelLimit* pbLimit)
{
	pbLimit->set_wordnum(mWordNum);
	pbLimit->set_playerlevel(mPlayerLevel);
	pbLimit->set_interval(mInterval);
}

void CTplChatConfig::setFromPB(PBTplChatConfig* pbConfig)
{
	mWorldLimit.setFromPB(pbConfig->mutable_worldlimit());
	mTeamLimit.setFromPB(pbConfig->mutable_teamlimit());
	mFactionLimit.setFromPB(pbConfig->mutable_factionlimit());
}

void CTplChatConfig::createToPB(PBTplChatConfig* pbConfig)
{
	mWorldLimit.createToPB(pbConfig->mutable_worldlimit());
	mTeamLimit.createToPB(pbConfig->mutable_teamlimit());
	mFactionLimit.createToPB(pbConfig->mutable_factionlimit());
}