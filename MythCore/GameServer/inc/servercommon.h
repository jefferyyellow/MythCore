// ������rand()%x�ķ�ʽ�õ�0-(x-1)�����������ΪRAND_MAX��������x,���Իᵼ�²�����ȫ���
#include "commontype.h"
#include <vector>
#include <map>
using namespace std;

#include "tinyxml2.h"
using namespace tinyxml2;

#define RAND(x) (int)( ( x )*(rand( )/(float)( RAND_MAX + 1.0 ) ))

#define MYTH_PERCENT_FLOAT	10000.0f
#define MYTH_PERCENT_INT	10000
#define MYTH_MILLIONTH		0.0001

#define TEAM_MEM_NUM			4
#define PLAYER_NAME_LENGTH		32

#define SECONDS_PER_DAY			86400	 // ÿ�������
#define SECONDS_PER_MIN			60			// ÿ���ӵ�����

#define MAX_DB_JOB_BUFFER_SIZE		8 * 1024* 1024		// 8M
#define MAX_SCENE_DB_BUFFER_SIZE	32 * 1024 * 1024	// 32M

#define TIME_JOB_RUN_LOG		30		// job ������־