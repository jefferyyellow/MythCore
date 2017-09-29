#include "template.h"
#include <stddef.h>
#include "template.hxx.pb.h"
#ifndef TEMPEDIT
#include "locallogjob.h"
#else
#include "commontype.h"
#include "logmanager.h"
using namespace Myth;
#endif
#include <iostream>
#include <fstream>

CTemplate* CStaticData::spTemplate[ MAX_TEMPLATE_NUM ]							= { NULL };
int	CStaticData::mVersion														= 0;

CTplPlayerLevelExpConfig* CTplPlayerLevelExpConfig::spConfig				= NULL;
CTplVIPConfig* CTplVIPConfig::spConfig											= NULL;

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

	TEMPLATE_SET_FROM_PB(CTplPlayerLevelExpConfig, pbTplConfigSet, levelexpconfig);
	TEMPLATE_SET_FROM_PB(CTplVIPConfig, pbTplConfigSet, vipconfig);
	TEMPLATE_SET_FROM_PB(CTplFuncNPC, pbTplNPCSet, funcnpc);
	TEMPLATE_SET_FROM_PB(CTplOgre, pbTplNPCSet, ogre);
}


void CTplItem::setFromPB(PBTplItem* pbItem)
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

void CTplItem::createToPB(PBTplItem* pbItem)
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


void CTplPlayerLevelExpConfig::setFromPB(PBTplPlayerLevelExpConfig* pbConfig)
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

void CTplPlayerLevelExpConfig::createToPB(PBTplPlayerLevelExpConfig* pbConfig)
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

void CTplOgre::setFromPB(PBTplOgre* pbOgre)
{
	if (NULL == pbOgre)
	{
		return;
	}

	CTplNPC::setFromPB(pbOgre->mutable_baseinfo());
}

void CTplOgre::createToPB(PBTplOgre* pbOgre)
{
	if (NULL == pbOgre)
	{
		return;
	}

	CTplNPC::createToPB(pbOgre->mutable_baseinfo());
}