syntax = "proto3";
import "common.hxx";
// 注意：存盘的消息不要用repeated，parseMessage不会处理repeated
message PBSavePlayer
{
	PBItemList		bag				= 1;	// 包裹
	PBItemList		equip			= 2;	// 装备
	PBTaskList		task			= 3;	// 任务
}


// 由于proto3对普通字段没有hasfield的判断，
// 所以只能普通邮件和全局邮件使用两个PB结构
message PBGlobalMail
{
	uint32			mail_id			= 1;	// 邮件ID
	uint32			mail_type		= 2;	// 邮件类型
	uint32			create_time		= 3;	// 邮件创建时间
	string			mail_title		= 4;	// 邮件标题
	string			mail_body		= 5;	// 邮件正文
	PBMailItemList	mail_item		= 6;	// 邮件道具
}

message PBGameEventParam
{
	repeated uint32	Param			= 1;	// int参数
	repeated string	StrParam		= 2;	// 字符串参数
}

message PBGameEvent
{
	uint32			event_id		= 1;	// 事件ID
	uint32			event_type		= 2;	// 事件类型
	uint32			role_id			= 3;	// 角色ID
	uint32			create_time		= 4;	// 创建时间
	PBGameEventParam event_param	= 5;	// 事件参数
}