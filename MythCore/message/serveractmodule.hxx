syntax = "proto3";
// ��ϢID��Χ��1-65535������16λ��ǰ6λ��ʾģ��ţ���10λ��ʾģ���ڵ���ţ�һ��ģ�����ֻ��1024����Ϣ

enum SERVER_ACT_MODULE_MSG_ID
{
	ID_SERVER_ACT_MODULE_ERROR				= 0x0;
	ID_C2S_REQUEST_GET_SERVER_ACT			= 0x1C00;	// �õ������������
	ID_S2C_RESPONSE_GET_SERVER_ACT			= 0x1C01;	// �õ�������Ļ�Ӧ
};

// ����������� ID_C2S_REQUEST_GET_SERVER_ACT
message CGetServerActRequest
{
}

// ��������Ӧ ID_S2C_RESPONSE_GET_SERVER_ACT
message CGetServerActResponse
{

}
