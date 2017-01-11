syntax = "proto3";

// 消息ID范围（1-65535），共16位，前6位表示模块号，后10位表示模块内的序号，一个模块最多只有1024个消息

enum MESSAGE_MODULE_ID
{
	MESSAGE_MODULE_LOGIN			= 0x0000;		// 二进制(0000 0000 0000 0000)
	MESSAGE_MODULE_PROPERTY			= 0x0400;		// 二进制(0000 0100 0000 0000)
}
