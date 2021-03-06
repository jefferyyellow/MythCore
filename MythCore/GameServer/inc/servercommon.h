#ifndef __SERVERCOMMON_H__
#define __SERVERCOMMON_H__
#include "commontype.h"
#include <vector>
#include <map>
using namespace std;

#include "tinyxml2.h"
using namespace tinyxml2;

#define INVALID_OBJ_ID				0		// 非法的OBJ ID
// 不能用rand()%x的方式得到0-(x-1)的随机数，因为RAND_MAX不能整除x,所以会导致不能完全随机
#define RAND(x) (int)( ( x )*(rand( )/(float)( RAND_MAX + 1.0 ) ))

#define MYTH_PERCENT_FLOAT		10000.0f					// 万分比（浮点数）
#define MYTH_PERCENT_INT		10000						// 万分比（整数）
#define MYTH_MILLIONTH			0.0001						// 万分之一

#define TEAM_MEM_NUM				4						// 队伍人数
#define PLAYER_NAME_LENGTH			32						// 玩家名字长度

#define SECONDS_PER_DAY				86400					// 每天的秒数
#define SECONDS_PER_MIN				60						// 每分钟的秒数

#define MAX_DB_JOB_BUFFER_SIZE		8 * 1024* 1024			// 8M
#define MAX_SCENE_DB_BUFFER_SIZE	32 * 1024 * 1024		// 32M

#define TIME_JOB_RUN_LOG			30						// job 运行日志

#define MAX_DEBUG_LOG_NAME			32						// 调试日志名字长度
#define PLAT_LOG_CONTENT_LEN		1024					// 平台日志的内容长度
#define WEB_URL_LEN					128						// Web URL长度
#define WEB_POST_DATA_LEN			256						// Web Post数据的长度
#define RETURN_DATA_LENGTH			1024					// Web请求返回数据长度


#define MAX_GATE_BUFF_SIZE			10*1024*1024			// 10M
#define SOCKET_CACHE_SIZE			1024*1024				// socket的缓存
#define PLAYER_SOCKET_BUFF_SIZE		8096					// 玩家socket缓存
#define MAX_PACKAGE_SIZE			4096					// 最大包大小
struct CExchangeHead
{
	short	mDataLength;				// 数据长度
	time_t	mSocketTime;				// socket建立时间
	short	mSocketIndex;				// socket索引
	short	mSocketError;				// socket错误
};

class	CDBRequestHeader
{
public:
	int		mPlayerID;					// 玩家ID
	int		mParam1;					// 参数1
	int		mParam2;					// 参数2
	short	mSessionType;				// 会话类型
	short	mSqlLenth;					// 缓冲区长度
	byte	mJobID;						// 请求的JobID
};

class CDBResponseHeader
{
public:
	int			mPlayerID;				// 玩家ID
	int			mSqlResult;				// SQL执行结果
	int			mParam1;				// 参数1
	int			mParam2;				// 参数2
	short		mSessionType;			// 会话ID
	short		mRowNum;				// 行数
	short		mColNum;				// 列数
	short		mSqlLenth;				// 缓冲区长度
};
#endif
