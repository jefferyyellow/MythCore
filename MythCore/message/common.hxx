syntax = "proto3";
message PBItemList
{
	repeated	PBItemObject	ItemObject	= 1;
				uint32			Size		= 2;
}

message PBItemObject
{
	uint32			ItemID			= 1;		// ID
	uint32			Number			= 2;		// 数量
	uint32			Index			= 3;		// 包裹中的索引

	PBItemEquip		ItemEquip		= 4;		// 装备
}

message PBItemEquip
{
	uint32			Level		= 4;			// 等级
}

message PBTask
{
				uint32				TaskID			= 1;		// 任务ID
	repeated	uint32				Param			= 2;		// 任务参数
};

message PBTaskList
{
				uint32				MaxCompleteTaskID=1;		// 最大的已经完成的任务ID
	repeated	fixed32				CompleteTasks	= 2;		// 功能开启
	repeated	PBTask				TaskList		= 3;		// 任务列表
};

message PBMailItem
{
	uint32			ItemID			= 1;	// 道具ID
	uint32			ItemNum			= 2;	// 道具数目
}

message PBMailItemList
{
	repeated PBMailItem	MailItem	= 1;	// 邮件道具
}

message	PBMail
{
	uint32			mail_id			= 1;	// 邮件ID
	uint32			mail_status		= 2;	// 邮件状态
	uint32			role_id			= 3;	// 角色ID
	uint32			mail_type		= 4;	// 邮件类型
	uint32			item_log		= 5;	// 道具日志类型
	uint32			create_time		= 6;	// 邮件创建时间
	string			mail_title		= 7;	// 邮件标题
	string			mail_body		= 8;	// 邮件正文
	PBMailItemList	mail_item		= 9;	// 邮件道具
}
