syntax = "proto3";
// ��ϢID��Χ��1-65535������16λ��ǰ6λ��ʾģ��ţ���10λ��ʾģ���ڵ���ţ�һ��ģ�����ֻ��1024����Ϣ

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

//�������� ID_C2S_REQUEST_CREATE_ROOM
message UCreateGame{
	uint32 type = 1;  // ��Ϸ����
	uint32 gametpye = 2;//��Ϸ����������
	sint32 gameTimes = 3;  //�������
	sint32 gamePerson = 4;// ��Ҫ�����˲�����Ϸ
}

//���뷿�� ID_C2S_REQUEST_ENTER_GAME
message UEnterGame{
	sint32 roomID = 1;//����id
}

//�Լ����뷿�䷴�� ID_S2C_NOTIFY_ENTER_GAME
message DEnterGame{
	sint32 creatid = 1;//����id
	sint32 gametimes = 2;//��Ϸ����
	sint32 roomID = 3;//����id
	sint32 type = 4;//��Ϸ����
	sint32 gameType = 5;//��Ϸ������
	sint32 curTime = 6;//��ǰ�ڼ���
	sint32 isStart = 7;//��Ϸ�Ƿ�ʼ
	sint32 count = 8;//Ӯ���Ž���
	sint32 status = 9; //0:��ȷ��,1:��ʾ��ע,2:��ׯ�׶�

}

message UReady{
}

message DReady{
	sint32 playerID = 1;
}

// ͬ���������
message DSyncPlayerData{
	string headImg = 1;//ͷ��url
	string nickname = 2;//�������
	sint32 money = 3;//��ҽ��
	sint32 playerID = 4;//���Id
	sint32 isbank = 5;//�ǲ���ׯ
	sint32 isReady = 6;//�ǲ���׼����
	sint32 bet = 7;//��ע
	sint32 alive = 8;//0:���ˣ�1:û��
	sint32 index = 9;//player index
	sint32 isqiangzhuang = 10;//�Ƿ���ׯ
}

//����뿪����
message ULeaveRoom{

}
//����뿪���䷴��
message DLeaveRoom{
	sint32 playerID = 2;
}
//���ִ�����ʾ
message DErrorMessage{
	GameStatus errorCode = 1;  // ��������
}
//��ʼ��Ϸ
message UStartGame{

}
//��ʼ��Ϸ����
message DStartGame{
	GameStatus status = 1;
}
//�ͻ�������ͬ��
message DReconnectData{
	sint32 roomID = 1; //���ڽ��еķ���id
}
//��Ϸ����
message DGameOver{
	sint32 bankerId = 1;//ׯ��Id
	sint32 curTimes = 2;//��ǰ�ڼ���
	sint32 data = 3;//[id,score,id,score,...]
}
//��Ϸ��ע
message UGameBet{
	sint32 Diamoud = 1;
}
//��Ϸ��ע�����˷���
message DAllGameBet{
	sint32 playerId = 1;
	sint32 Diamoud = 2;
}
//�����ɢ����
message URequestDissposeRoom{

}
//ѯ���Ƿ��ɢ����
message DDissposeAsk{
}
//ȷ�Ͻ�ɢ
message UDissposeConfirm{
	sint32 result = 1; // 0����ͬ�⣬ 1��ͬ��
}
//���ս�ɢ���䷴��
message DDissposeRoom{

}