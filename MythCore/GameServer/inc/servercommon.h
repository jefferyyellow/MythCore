// 不能用rand()%x的方式得到0-(x-1)的随机数，因为RAND_MAX不能整除x,所以会导致不能完全随机
#include "commontype.h"
#include <vector>
#include <map>
using namespace std;

#include "tinyxml2.h"
using namespace tinyxml2;

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

