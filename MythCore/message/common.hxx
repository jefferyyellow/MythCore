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