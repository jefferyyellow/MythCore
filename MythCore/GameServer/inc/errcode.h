#ifndef __ERRCODE_H__
#define __ERRCODE_H__

// 通用错误码
#define SUCCESS			0

// linux的错误码0-255，所以我们从256开始
// windows的错误码特别多，先不管他了，如果使用GetLastError，转换成自己的错误码
#define ERROR_UNKNOWN					256				// 未知错误
#define ERROR_PARAMETER_INVALID			257				// 参数非法
#define ERROR_MONEY_NOT_ENOUGH			258				// 金币不足
#define ERROR_DIAMOND_NOT_ENOUGH		258				// 金币不足



// 300 - 399
enum ERROR_ITEM
{
	ERROR_ITEM_OBTAIN_MONEY_INVALID		= 300,		// 得到的金币非法
	ERROR_ITEM_CONSUME_MONEY_INVALID	= 301,		// 消费的金币非法
	ERROR_ITEM_OBTAIN_DIAMOND_INVALID	= 302,		// 得到的钻石非法
	ERROR_ITEM_CONSUME_DIAMOND_INVALID	= 303,		// 消费的钻石非法

};

// 400 - 499
enum ERROR_MAP
{
	ERROR_MAP_CREATE_MAP_MEMORY			= 400,		// 创建地图时内存分配失败
	ERROR_MAP_CREATE_MAP_UNIT_FAILURE	= 401,		// 创建地图单元失败
	ERROR_MAP_ADD_MAP_LIST_FAILURE		= 402,		// 插入地图列表失败
};

#endif