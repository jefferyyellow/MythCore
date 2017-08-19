#include "template.h"
#include <stddef.h>
CTemplate* CStaticData::spTemplate[ MAX_TEMPLATE_NUM ]							= { NULL };

CTplPlayerLevelUpExpConfig* CTplPlayerLevelUpExpConfig::spConfig				= NULL;
CTplVIPConfig* CTplVIPConfig::spConfig											= NULL;

CTemplate* CStaticData::SearchTpl(unsigned int nTempID)
{
	// ע��0Ҳ�ǷǷ�ֵ
	if (nTempID <= 0 || nTempID >= MAX_TEMPLATE_NUM)
	{
#ifdef LINUX
		//LOG_ERROR( "default", "can't find obj TempID: %d", vTempID );
#endif
		return NULL;
	}

	return spTemplate[nTempID];
}

