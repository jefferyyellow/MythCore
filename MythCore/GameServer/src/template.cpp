#include "template.h"
#include <stddef.h>
CTemplate* CDataStatic::spTemplate[ MAX_TEMPLATE_NUM ]							= { NULL };

CTemplate* CDataStatic::SearchTpl(unsigned int nTempID)
{
	if (nTempID <= 0 || nTempID >= MAX_TEMPLATE_NUM)
	{
#ifdef LINUX
		//LOG_ERROR( "default", "can't find obj TempID: %d", vTempID );
#endif
		return NULL;
	}
}