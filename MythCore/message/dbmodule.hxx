syntax = "proto3";
import "common.hxx";
// 注意：存盘的消息不要用repeated，parseMessage不会处理repeated
message PBSavePlayer
{
	PBItemList		bag				= 1;	// 包裹
	PBItemList		equip			= 2;	// 装备
	PBTaskList		task			= 3;	// 任务
}