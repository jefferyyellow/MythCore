#ifndef __LOGINTYPE_H__
#define __LOGINTYPE_H__
#include "servercommon.h"
enum EmTcpError
{
	emTcpError_None				= 0,	// 未知
	emTcpError_OffLineClose		= 1,	// 下线关闭
};

struct CExchangeHead
{
	time_t	mSocketTime;				// socket建立时间
	short	mSocketIndex;				// socket索引
	short	mSocketError;				// socket错误
};


#define MAX_PLAYER_NAME_LEN		32		// 玩家名字最大长度
#define PLAYER_NAME_RESERVE		6		// 玩家名字合服预留

#define MIN_PLAYER_NAME_CHAR	2		// 玩家名字最小字符
#define MAX_PLAYER_NAME_CHAR	10		// 玩家名字最大字符

enum EmPlayerStatus
{
	emPlayerStatus_None			= 0,	// 无状态
	emPlayerStatus_Loading		= 1,	// 加载状态
	emPlayerStatus_Gameing		= 2,	// 游戏状态
	emPlayerStatus_Exiting		= 3,	// 退出状态
};

enum EmPlayerSaveStatus
{
	emSaveStatus_None			= 0x0000,	// 玩家没有保存成功任何一部分数据
	emSaveStatus_Info			= 0x0001,	// 已经保存成功主要信息部分
	emSaveStatus_BaseProperty	= 0x0002,	// 已经保存成功基础属性部分
	emSaveStatusAll				= 0x0003,	// 所有的都已经存盘完成
};

enum EmPlayerLoadStatus
{
	emLoadStatus_None			= 0x0000,	// 玩家没有加载成功任何一部分数据
	emLoadStatus_Info			= 0x0001,	// 已经加载成功主要信息部分
	emLoadStatus_BaseProperty	= 0x0002,	// 已经加载成功基础属性部分
	emLoadStatusAll				= 0x0003,	// 所有的都已经加载完成
};

#define	MAX_SERVER_ID			10000			// 最大的服务器ID
#define MAX_ROLE_PER_SERVER		400000			// 每个服务器最多40万角色
#define BAT_LOAD_ROLE_ID_NUM	2000			// 批量加载分配角色ID数量

enum EmPlayerSex
{
	EmPlayerSex_Male			= 0,		// 男性
	EmPlayerSex_Female			= 1,		// 女性
	EmPlayerSex_None			= 2,		// 无性别
};

enum EmLeaveReason
{
	EmLeaveReason_Disconnection	= 0,		// Sokect断开连接
	EmLeaveReason_ServerShutDown= 1,		// 服务器关机
	EmLeaveReason_LoadTimeOut	= 2,		// 加载超时
	EmLeaveReason_PlayerRequest	= 3,		// 玩家请求
	EmLeaveReason_HeartBeatQuick= 4,		// 心跳过速
};

enum EmAccountStatus
{
	emAccountStatus_None		= 0,		// 无状态
	emAccountStatus_Disable		= 1,		// 账号禁用状态
	emAccountStatus_Mute		= 2,		// 禁言状态
	emAccountStatus		
};
#endif