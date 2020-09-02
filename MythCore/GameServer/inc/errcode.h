#ifndef __ERRCODE_H__
#define __ERRCODE_H__

// 通用错误码
#define SUCCESS			0

// linux的错误码0-255，所以我们从300开始
// windows的错误码特别多，先不管他了，如果使用GetLastError，转换成自己的错误码
// 300 - 399
enum ERR_COMMON
{
	ERR_UNKNOWN							= 300,				// 未知错误
	ERR_PARAMETER_INVALID				= 301,				// 参数非法
	ERR_CURRENCY_NOT_ENOUGH				= 302,				// 货币不足
	ERR_TEMPLATE_INVALID				= 303,				// 模板非法
	ERR_BAGGAGE_IS_FULL					= 304,				// 包裹已满
};

// 400 - 499
enum ERR_ITEM
{
	ERR_OBTAIN_MONEY_INVALID			= 400,		// 得到的金币非法
	ERR_CONSUME_MONEY_INVALID			= 401,		// 消费的金币非法
	ERR_OBTAIN_DIAMOND_INVALID			= 402,		// 得到的钻石非法
	ERR_CONSUME_DIAMOND_INVALID			= 403,		// 消费的钻石非法
	ERR_ITEM_INDEX_OBJ_ID_INVALID		= 404,		// 对应索引的道具ID非法
	ERR_ITEM_INDEX_OBJ_DATA_NULL		= 405,		// 对应索引的道具数据为空
	ERR_ITEM_INDEX_NUM_NOT_ENOUGH		= 406,		// 对应索引的道具数量不足
	ERR_ITEM_INDEX_INVALID				= 407,		// 道具索引非法
	ERR_ITEM_TEMPLATE_IS_NOT_ITEM		= 408,		// 对应的模板不是道具
	ERR_ITEM_MOVE_SAME_INDEX			= 409,		// 道具移动到相同的索引
	ERR_ITEM_MOVE_NUM_INVALID			= 410,		// 道具移动的数目不对
	ERR_ITEM_CREATE_ITEM_OBJ_FAILURE	= 411,		// 创建道具obj失败
	ERR_ITEM_MOVE_SRC_NUM_INVALID		= 412,		// 道具移动的源道具数目不正确
};

// 500 - 599
enum ERR_MAP
{
	ERR_MAP_CREATE_MAP_UNIT_FAILURE		= 500,		// 创建地图单元失败
	ERR_MAP_TELEPORT_MAP_FAILURE		= 501,		// 传送地图失败
};

// 600 - 699
enum ERR_TASK
{
	ERROR_TASK_CONFIG_IS_NULL			= 600,			// 任务配置为空
	ERROR_TASK_PLAYER_ALREADY_COMPLETE	= 601,			// 玩家已经完成该任务
	ERROR_TASK_PLAYER_ALREADY_ACCEPT	= 602,			// 玩家已经接受该任务
	ERROR_TASK_PRE_TASK_NOT_COMPLETE	= 603,			// 前置任务没有完成
	ERROR_TASK_PLAYER_LEVEL_NOT_ENOUGH	= 604,			// 玩家等级不够
	ERROR_TASK_SCHOOL_NOT_MATCH			= 605,			// 玩家门票不匹配
	ERROR_TASK_PRESTIGE_NOT_ENOUGH		= 606,			// 声望不足
	ERROR_TASK_FAVORABILITY_NOT_ENOUGH	= 607,			// 伙伴友好度不足
	ERROR_TASK_PARTNER_NOT_IN_ARRAY		= 608,			// 对应的伙伴不在阵上
	ERROR_TASK_PARTNER_NUM_NOT_ENOUGH	= 609,			// 伙伴数目不足
	ERROR_TASK_HAS_NO_TASK				= 610,			// 没有对应的任务
	ERROR_TASK_KILL_OGRE_NOT_ENOUGH		= 611,			// 杀怪计数不够
	ERROR_TASK_ITEM_NUM_NOT_ENOUGH		= 612,			// 道具数目不够
	ERROR_TASK_ITEM_DATA_IS_NULL		= 613,			// 道具数目为空
	ERROR_TASK_ITEM_INDEX_NOT_MATCH		= 614,			// 发过来的道具不是任务道具
	ERROR_TASK_COLLECT_NUM_NOT_MATCH	= 615,			// 采集的数目不够
	ERROR_TASK_CHALLENGE_NOT_ENOUGH		= 616,			// 挑战NPC的次数不够
	ERROR_TASK_COMBAT_POWER_NOT_ENOUGH	= 617,			// 玩家战力不够
	ERROR_TASK_ALL_COMBAT_POWER_NOT_ENOUGH = 618,		// 所有上阵伙伴的战力不够
	ERROR_TASK_ALL_EQUIP_LEVEL_NOT_ENOUGH = 619,		// 玩家所有的装备等级不足
	ERROR_TASK_EQUIP_DATA_NULL			= 620,			// 玩家装备数据为空
	ERROR_TASK_EQUIP_LEVEL_NOT_ENOUGH	= 621,			// 玩家装备等级不足
	ERROR_TASK_INSTANCE_TIMES_NOT_ENOUGH= 622,			// 玩家通过副本次数不足
	ERROR_TASK_TELEPORT_FAIL			= 623,			// 传送失败
	ERROR_TASK_PARTNER_LIST_FULL		= 624,			// 伙伴列表已满
	ERROR_TASK_PLAYER_TASK_LIST_FULL	= 625,			// 玩家任务列表已满
	ERROR_TASK_ALREADY_END				= 626,			// 玩家任务已经完成过了
	ERROR_TASK_CLOSE_PRESTIGE			= 627,			// 任务由于声望太高关闭
	ERROR_TASK_CLOSE_LEVEL				= 628,			// 任务由于等级太高关闭
	ERROR_TASK_CLOSE_PRE_TASK_COMPLETE	= 629,			// 任务由于前置任务完成关闭
	ERROR_TASK_HAVENOT_POLT				= 630,			// 任务没有剧情
	ERROR_TASK_NPC_IS_VALID				= 631,			// 任务NPC非法
	ERROR_TASK_FAR_FROM_NPC				= 632,			// 任务NPC和玩家离得太远
};

// 700 - 799
enum ERR_EQUIP
{	
	ERR_EQUIP_SRC_ITEM_DATA_NULL		= 700,		// 装备的道具数据为空
	ERR_EQUIP_ITEM_IS_NOT_EQUIP			= 701,		// 道具不是装备
	ERR_EQUIP_PART_IS_INVALID			= 702,		// 装备部位非法
	ERR_EQUIP_DES_INDEX_DATA_NOT_NULL	= 703,		// 目标索引数据不为空
	ERR_EQUIP_PLAYER_LEVEL_NOT_ENOUGH	= 704,		// 装备等级不足
	ERR_EQUIP_ITEM_CANNOT_EQUIP			= 705,		// 道具不能装备
};

// 800 - 1099
enum ERR_SERVER_ACT
{
	
};
#endif