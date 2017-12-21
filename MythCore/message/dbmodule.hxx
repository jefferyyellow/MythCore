syntax = "proto3";
import "common.hxx";
// 注意：存盘的消息不要用repeated，parseMessage不会处理repeated
message PBSavePlayer
{
	PBBag			bag				= 1;	// 包裹
	PBTask			task			= 2;	// 任务
}