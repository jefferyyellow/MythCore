#ifndef __RANKTYPE_H__
#define __RANKTYPE_H__
enum EmRankType
{
	emRankType_None				= -1,	// 无类型
	emRankType_Level			= 0,	// 等级排行榜
	emRankTypeMax
};

#define MAX_RANK_NUM			1000
#define MAX_RANK_SHOW_NUM		100		// 排行榜最大显示人数
#define MAX_RANK_SHOW_CACHE_NUM	200		// 排行榜最大显示缓存人数(为了防止掉出MAX_RANK_SHOW_NUM没人补充)

#endif