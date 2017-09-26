#include "PBToolCommon.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <string.h>

#ifdef LINUX
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#else
#ifdef WIN32
#include <io.h>
#endif
#endif

// ���Ҫ�����ļ���·��������·���������򴴽�һ���µģ�������������
int PBToolCommon::CheckDirection( const char* pDirName )
{
	if ( pDirName == NULL )
	{
		return -1;
	}
#ifdef LINUX
	DIR* pDir = opendir( pDirName );
	if ( pDir == NULL )// �ļ��в�����
	{
		int tStatus = mkdir( pDirName, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
		if ( tStatus != 0 )
		{
			printf("create direction failed !\n");
			return -1;
		}
	}
#endif
	return 0;
}

// ���ݱ�����ȡ��������
const char* PBToolCommon::GetPrimaryName(const char* nFormName)
{
	if (nFormName == NULL)
	{
		return NULL;
	}
	if ( strcmp( nFormName, "GAME_EVENT" ) == 0 )
	{
		return "event_id";
	}
	else if ( strcmp( nFormName, "UMS_APPRENTICE" ) == 0 ||
		strcmp( nFormName, "UMS_LEFT_PARTNER" ) == 0 || 
		strcmp( nFormName, "UMS_OFFLINE_REWARD" ) == 0 ||
		strcmp( nFormName, "UMS_ROLE_INFO" ) == 0 )
	{
		return "role_id";
	}
	else if ( strcmp( nFormName, "UMS_BATTLE_RESULT" ) == 0 )
	{
		return "battle_id";
	}
	else if ( strcmp( nFormName, "UMS_FACTION" ) == 0 )
	{
		return "faction_id";
	}
	else if ( strcmp( nFormName, "UMS_GLOBAL_DATA" ) == 0 )
	{
		return "global_id";
	}
	else if ( strcmp( nFormName, "UMS_SCHOOL" ) == 0 )
	{
		return "school_type";
	}
	else if ( strcmp( nFormName, "UMS_WORLDWAR" ) == 0 )
	{
		return "war_id";
	}
	else if ( strcmp( nFormName, "UMS_WORLDWAR_ROLE" ) == 0 )
	{
		return "war_role";
	}
	else
	{
		return NULL;
	}
}

// ת���ַ�����Сд
const char* PBToolCommon::ToLower( char pSrc[], int nLen )
{
	for (int i = 0; i < nLen ; ++i)
	{
		pSrc[i] = tolower( pSrc[i] );
	}
	return pSrc;
}

// �����ļ��������ļ�����Ϊ��������.back��
int PBToolCommon::CopyFile(const char* pFilePath)
{
	if ( pFilePath == NULL )
	{
		return -1;
	}

	const int tMaxPathSize = 2048;
	const char* tpExName = ".back";
	int tFilePathLen = strlen(pFilePath);
	if ( tFilePathLen + strlen(tpExName) >= tMaxPathSize - 1 )
	{
		printf("MaxPathSize : %d is Not Enougth !\n", tMaxPathSize);
		return -1;
	}

	char tOutFilePath[tMaxPathSize] = { 0 };
	strncpy( tOutFilePath, pFilePath, tFilePathLen + 1 );
	strncat( tOutFilePath, tpExName, tFilePathLen + 1 );

	FILE* fpInFile = fopen( pFilePath, "rb" );
	if ( fpInFile == NULL )
	{
		printf("The File : %s Open Failed !\n", pFilePath);
	}

	FILE* fpOutFile = fopen( tOutFilePath, "wb" );
	if ( fpInFile == NULL )
	{
		printf("The Copy File : %s Create Failed !\n", tOutFilePath);
	}

	int ch = getc( fpInFile );
	while ( !feof(fpInFile) )
	{
		putc( ch, fpOutFile );
		ch = getc( fpInFile );
	}

	fclose( fpInFile );
	fclose( fpOutFile );

	return 0;
}