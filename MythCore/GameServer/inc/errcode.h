#ifndef __ERRCODE_H__
#define __ERRCODE_H__

// 通用错误码
#define SUCCESS			0

// linux的错误码0-255，所以我们从256开始
// windows的错误码特别多，先不管他了，如果使用GetLastError，转换成自己的错误码
enum ERR_COMMON
{
	ERR_UNKNOWN							= 256,				// 未知错误
	ERR_PARAMETER_INVALID				= 257,				// 参数非法
	ERR_MONEY_NOT_ENOUGH				= 258,				// 金币不足
	ERR_DIAMOND_NOT_ENOUGH				= 259,				// 金币不足
	ERR_TEMPLATE_INVALID				= 260,				// 模板非法
};




// 300 - 399
enum ERR_ITEM
{
	ERR_OBTAIN_MONEY_INVALID			= 300,		// 得到的金币非法
	ERR_CONSUME_MONEY_INVALID			= 301,		// 消费的金币非法
	ERR_OBTAIN_DIAMOND_INVALID			= 302,		// 得到的钻石非法
	ERR_CONSUME_DIAMOND_INVALID			= 303,		// 消费的钻石非法
	ERR_ITEM_INDEX_OBJ_ID_INVALID		= 304,		// 对应索引的道具ID非法
	ERR_ITEM_INDEX_OBJ_DATA_NULL		= 305,		// 对应索引的道具数据为空
	ERR_ITEM_INDEX_NUM_NOT_ENOUGH		= 306,		// 对应索引的道具数量不足
	ERR_ITEM_INDEX_INVALID				= 307,		// 道具索引非法
	ERR_TEMPLATE_IS_NOT_ITEM			= 308,		// 对应的模板不是道具
};

// 400 - 499
enum ERR_MAP
{
	ERR_MAP_CREATE_MAP_UNIT_FAILURE		= 400,		// 创建地图单元失败
	ERR_MAP_TELEPORT_MAP_FAILURE		= 401,		// 传送地图失败
};

// 500 - 599
enum ERR_TASK
{
	ERR_TASK_ID_INVALID					= 500,		// 任务ID非法
	ERR_TASK_CONFIG_IS_NULL				= 501,		// 任务配置为空
	ERR_TASK_ALREADY_COMPLETE			= 502,		// 任务已经完成
	ERR_TASK_PRE_TASK_NOT_COMPLETE		= 503,		// 接任务时前置任务没有完成
	ERR_TASK_PLAYER_LEVEL_INVALID		= 504,		// 接任务时玩家等级不符合条件
	ERR_TASK_TASK_LIST_FULL				= 505,		// 任务列表已满
	ERR_TASK_PLAYER_HAS_NO_TASK			= 506,		// 玩家身上没有该任务
	ERR_TASK_TASK_NOT_COMPLETE			= 507,		// 
};
#endif