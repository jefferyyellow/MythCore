syntax = "proto3";

// 消息ID范围（1-65535），共16位，前6位表示模块号，后10位表示模块内的序号，一个模块最多只有1024个消息

enum MESSAGE_MODULE_ID
{
	MESSAGE_MODULE_LOGIN			= 0x0000;		// 二进制(0000 00	00 0000 0000)	登录模块
	MESSAGE_MODULE_PROPERTY			= 0x0400;		// 二进制(0000 01	00 0000 0000)	人物属性模块
	MESSAGE_MODULE_ITEM				= 0x0800;		// 二进制(0000 10	00 0000 0000)	道具模块
	MESSAGE_MODULE_MAP				= 0x0C00;		// 二进制(0000 11	00 0000 0000)	地图模块
	MESSAGE_MODULE_TASK				= 0x1000;		// 二进制(0001 00	00 0000 0000)	任务模块
	MESSAGE_MODULE_SKILL			= 0x1400;		// 二进制(0001 01	00 0000 0000)	任务模块
	MESSAGE_MODULE_CHAT				= 0x1800;		// 二进制(0001 10	00 0000 0000)	聊天模块
	MESSAGE_MODULE_SERVER_ACT		= 0x1C00;		// 二进制(0001 11	00 0000 0000)	开服活动模块
	MESSAGE_MODULE_DAILY_ACT		= 0x2000;		// 二进制(0010 00	00 0000 0000)	日常活动模块
	MESSAGE_MODULE_RANK				= 0x2400;		// 二进制(0010 01	00 0000 0000)	排行榜模块
	MESSAGE_MODULE_INSTANCE			= 0x2800;		// 二进制(0010 10	00 0000 0000)	副本模块
};