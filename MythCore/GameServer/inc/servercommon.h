#ifndef __SERVERCOMMON_H__
#define __SERVERCOMMON_H__
#include "commontype.h"
#include <vector>
#include <map>
using namespace std;

#include "tinyxml2.h"
using namespace tinyxml2;

#define INVALID_OBJ_ID				0		// �Ƿ���OBJ ID
// ������rand()%x�ķ�ʽ�õ�0-(x-1)�����������ΪRAND_MAX��������x,���Իᵼ�²�����ȫ���
#define RAND(x) (int)( ( x )*(rand( )/(float)( RAND_MAX + 1.0 ) ))

#define MYTH_PERCENT_FLOAT		10000.0f					// ��ֱȣ���������
#define MYTH_PERCENT_INT		10000						// ��ֱȣ�������
#define MYTH_MILLIONTH			0.0001						// ���֮һ

#define TEAM_MEM_NUM				4						// ��������
#define PLAYER_NAME_LENGTH			32						// ������ֳ���

#define SECONDS_PER_DAY				86400					// ÿ�������
#define SECONDS_PER_MIN				60						// ÿ���ӵ�����

#define MAX_DB_JOB_BUFFER_SIZE		8 * 1024* 1024			// 8M
#define MAX_SCENE_DB_BUFFER_SIZE	32 * 1024 * 1024		// 32M

#define TIME_JOB_RUN_LOG			30						// job ������־

#define MAX_DEBUG_LOG_NAME			32						// ������־���ֳ���
#define PLAT_LOG_CONTENT_LEN		1024					// ƽ̨��־�����ݳ���
#define WEB_URL_LEN					128						// Web URL����
#define WEB_POST_DATA_LEN			256						// Web Post���ݵĳ���
#define RETURN_DATA_LENGTH			1024					// Web���󷵻����ݳ���


#define MAX_GATE_BUFF_SIZE			10*1024*1024			// 10M
#define SOCKET_CACHE_SIZE			1024*1024				// socket�Ļ���
#define PLAYER_SOCKET_BUFF_SIZE		8096					// ���socket����
#define MAX_PACKAGE_SIZE			4096					// ������С
struct CExchangeHead
{
	short	mDataLength;				// ���ݳ���
	time_t	mSocketTime;				// socket����ʱ��
	short	mSocketIndex;				// socket����
	short	mSocketError;				// socket����
};

class	CDBRequestHeader
{
public:
	int		mPlayerID;					// ���ID
	int		mParam1;					// ����1
	int		mParam2;					// ����2
	short	mSessionType;				// �Ự����
	short	mSqlLenth;					// ����������
	byte	mJobID;						// �����JobID
};

class CDBResponseHeader
{
public:
	int			mPlayerID;				// ���ID
	int			mSqlResult;				// SQLִ�н��
	int			mParam1;				// ����1
	int			mParam2;				// ����2
	short		mSessionType;			// �ỰID
	short		mRowNum;				// ����
	short		mColNum;				// ����
	short		mSqlLenth;				// ����������
};
#endif
