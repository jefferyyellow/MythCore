syntax = "proto3";
// 消息ID范围（1-65535），共16位，前6位表示模块号，后10位表示模块内的序号，一个模块最多只有1024个消息

enum DAILY_ACT_MODULE_MSG_ID
{
	ID_DAILY_ACT_MODULE_ERROR				= 0x0;
	ID_S2C_NOTIFY_ACTIVITY_START			= 0x2000;
};

// 活动开启通知 ID_S2C_NOTIFY_ACTIVITY_START
message CActivityStatusNotify
{
	uint32	ActivityID			= 1;
	uint32	Status				= 2;
}
