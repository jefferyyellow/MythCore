syntax = "proto3";
import "common.hxx";
// ע�⣺���̵���Ϣ��Ҫ��repeated��parseMessage���ᴦ��repeated
message PBSavePlayer
{
	PBItemList		bag				= 1;	// ����
	PBItemList		equip			= 2;	// װ��
	PBTaskList		task			= 3;	// ����
}