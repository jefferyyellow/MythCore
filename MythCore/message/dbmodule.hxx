syntax = "proto3";
import "common.hxx";
// ע�⣺���̵���Ϣ��Ҫ��repeated��parseMessage���ᴦ��repeated
message PBSavePlayer
{
	PBBag			bag				= 1;	// ����
	PBTask			task			= 2;	// ����
}