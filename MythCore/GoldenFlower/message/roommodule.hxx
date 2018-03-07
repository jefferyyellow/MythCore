syntax = "proto3";
// 消息ID范围（1-65535），共16位，前6位表示模块号，后10位表示模块内的序号，一个模块最多只有1024个消息

enum ROOM_MODULE_MSG_ID
{
	ID_ROOM_MODULE_ERROR					= 0x0;
	ID_C2S_REQUEST_CREATE_ROOM				= 0x0800;
	ID_C2S_REQUEST_ENTER_GAME				= 0x0801;
	ID_S2C_NOTIFY_ENTER_GAME				= 0x0802;
	ID_C2S_REQUEST_READY					= 0x0803;
	ID_S2C_RESPONSE_READY					= 0x0804;
	ID_S2C_NOTIFY_SYNC_PLAYER_DATA			= 0x0805;
	ID_C2S_REQUEST_LEAVE_ROOM				= 0x0806;
	ID_C2S_REQUEST_LEAVEROOM				= 0x0807;
	ID_S2C_NOTIFY_LEAVE_ROOM				= 0x0808;
	ID_C2S_REQUEST_START_GAME				= 0x0809;
	ID_S2C_NOTIFY_START_GAME				= 0x080A;
	ID_S2C_NOTIFY_GAME_OVER					= 0x080B;
	ID_C2S_REQUEST_GAME_BET					= 0x080C;
	ID_S2C_NOTIFY_ALL_GAME_BET				= 0x080D;

};

//创建房间 ID_C2S_REQUEST_CREATE_ROOM
message UCreateGame{
	uint32 type = 1;  // 游戏类型
	uint32 gametpye = 2;//游戏类型子类型
	sint32 gameTimes = 3;  //房间局数
	sint32 gamePerson = 4;// 需要多少人才能游戏
}

//进入房间 ID_C2S_REQUEST_ENTER_GAME
message UEnterGame{
	sint32 roomID = 1;//房间id
}

//自己进入房间反馈 ID_S2C_NOTIFY_ENTER_GAME
message DEnterGame{
	sint32 creatid = 1;//房主id
	sint32 gametimes = 2;//游戏次数
	sint32 roomID = 3;//房间id
	sint32 type = 4;//游戏类型
	sint32 gameType = 5;//游戏子类型
	sint32 curTime = 6;//当前第几局
	sint32 isStart = 7;//游戏是否开始
	sint32 count = 8;//赢三张奖池
	sint32 status = 9; //0:发确定,1:显示下注,2:抢庄阶段

}

message UReady{
}

message DReady{
	sint32 playerID = 1;
}

// 同步玩家数据
message DSyncPlayerData{
	string headImg = 1;//头像url
	string nickname = 2;//玩家名称
	sint32 money = 3;//玩家金币
	sint32 playerID = 4;//玩家Id
	sint32 isbank = 5;//是不是庄
	sint32 isReady = 6;//是不是准备的
	sint32 bet = 7;//下注
	sint32 alive = 8;//0:输了，1:没输
	sint32 index = 9;//player index
	sint32 isqiangzhuang = 10;//是否抢庄
}

//玩家离开房间
message ULeaveRoom{

}
//玩家离开房间反馈
message DLeaveRoom{
	sint32 playerID = 2;
}
//各种错误提示
message DErrorMessage{
	GameStatus errorCode = 1;  // 错误类型
}
//开始游戏
message UStartGame{

}
//开始游戏反馈
message DStartGame{
	GameStatus status = 1;
}
//客户端重连同步
message DReconnectData{
	sint32 roomID = 1; //正在进行的房间id
}
//游戏结束
message DGameOver{
	sint32 bankerId = 1;//庄家Id
	sint32 curTimes = 2;//当前第几局
	sint32 data = 3;//[id,score,id,score,...]
}
//游戏下注
message UGameBet{
	sint32 Diamoud = 1;
}
//游戏下注所有人反馈
message DAllGameBet{
	sint32 playerId = 1;
	sint32 Diamoud = 2;
}
//请求解散房间
message URequestDissposeRoom{

}
//询问是否解散房间
message DDissposeAsk{
}
//确认解散
message UDissposeConfirm{
	sint32 result = 1; // 0：不同意， 1：同意
}
//最终解散房间反馈
message DDissposeRoom{

}