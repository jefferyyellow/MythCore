#ifndef _PB_TOOL_COMMON_H_
#define _PB_TOOL_COMMON_H_

#define  XML_ASSERT(x)          { if( NULL == (x) ) return NULL; }

namespace PBToolCommon
{
	// 检查要创建文件的路径，若该路径不存在则创建一个新的，若存在则跳过
	int CheckDirection( const char* pDirName );

	// 根据表名获取主键名称
	const char* GetPrimaryName(const char* nFormName);

	// 转换字符串大小写
	const char* ToLower( char pSrc[], int nLen );

	// 拷贝文件，将新文件命名为“老名称.back”
	int CopyFile(const char* pFilePath);
}

#endif
