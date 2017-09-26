#include <dirent.h>
#include <sys/stat.h>

#include <string.h>
#include <stdio.h>
#include "sk_hashmap.h"
#include "DatabaseMysql.h"
#include "QueryResultMysql.h"
#include "ParseProtobuf.h"

#include "tinyxml.h"
#include "tinystr.h"

#include "db_msg_pb.hxx.pb.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/descriptor.pb.h"

#include "property_pb.hxx.pb.h"
#include "servermessage_pb.hxx.pb.h"
#include "arenamessage_pb.hxx.pb.h"
#include "activitymessag_pb.hxx.pb.h"
//#include "servermessage_pb.hxx"

typedef ::google::protobuf::Message Message;
typedef ::google::protobuf::Descriptor Descriptor;
typedef ::google::protobuf::Reflection Reflection;
typedef ::google::protobuf::FieldDescriptor FieldDescriptor;
typedef ::google::protobuf::EnumValueDescriptor EnumValueDescriptor;


// Note : �����ɡ�parsetool ��������������IDֵ������Ȼ�󵼳�һ��xml
// �Ժ�����չ��֧�ֶ��������������Ȼ�������һ���ļ���
int main(int argc, char** argv)
{
	if (argc > 1)
	{
#ifdef LINUX
		if (!strcasecmp(argv[1], "-v"))
#else
		if ( !_stricmp(argv[1], "-v") )
#endif
		{
#ifdef _DEBUG_
			printf("ParsePBTool %s debug build at %s %s\n", __DATE__, __TIME__);
#else
			printf("ParsePBTool %s release build at %s %s\n", __DATE__, __TIME__);
#endif
			return 0;
		}
	}

	if ( argv[1] == NULL )
	{
		printf("����������ʹ�� ./parsetool [���ݿ��ַ] [�˿ں�] [�û���] [����] [���ݿ�����] .\n");
		printf("����������ʹ�� --help ����鿴ʹ�÷��� .\n");
		return 0;
	}

	if ( strcmp( argv[1], "--help") == 0 )// ����ʹ�÷���
	{
		printf("�÷���\n");
		printf("[�������ݶ�д : ]\n");
		printf("������ĳ�����ݿ�����ӣ�\n ./parsepbtool [���ݿ��ַ] [�˿ں�] [�û���] [����] [���ݿ�����] .\n");
		printf("�л����ݿ⣺switch");
		printf("���ĳ�����ĳһ����¼��һ���ֶΣ�\n");
		printf("./parsepbtool [����] [�ֶ���] [��¼������ֵ] \n");
		printf("�磺./parsepbtool UMS_GLOBAL_DATA global_status 1 \n");
		printf("\n");
		printf("���ĳ�����ĳһ����¼�������ֶΣ�\n");
		printf("./parsetool all [����] [��¼������ֵ] \n");
		printf("�磺./parsepbtool all UMS_GLOBAL_DATA 1 \n");
		printf("\n");
		printf("[��������������д : ]\n");
		printf("[�����ӵ����ݿ���Ϣ���Լ�Ҫ������Blob�ֶ������� Config.xml �����úá���Ҫ�������˺�����д�� primarykey.txt �� ]\n");
		printf("�������� Blob �ֶγ� xml��./parsepbtool config parse\n");
		printf("������ xml �ļ�д�����ݿ� Blob �ֶΣ�./parsepbtool config write\n");
		printf("\n");
		printf("�������⣬���� Jeffery@7k7k, Sunrise Meier@7k7k �������.\n");
		return 0;
	}

	// ���������ļ���ʽ������д��
	if ( argv[1] != NULL && strcmp( argv[1], "config" ) == 0 )
	{
		if ( argv[2] == NULL )
		{
			printf("�����������������룡\n");
			printf("config ���������ָ�� parse �� write����� ./parsepbtool --help �����Ϣ\n");
		}
		else if ( strcmp( argv[2], "parse" ) == 0 )
		{
			CParseProtobuf tParseProtoBuf;
			tParseProtoBuf.ParseByConfig();
		}
		else if ( strcmp( argv[2], "write" ) == 0 )
		{
			CParseProtobuf tParseProtoBuf;
			tParseProtoBuf.WriteByConfig();
		}
		return 0;
	}

	const char* tpIP = argv[1];
	const char* tpPort = argv[2];
	const char* tpUser = argv[3];
	const char* tpPasswd = argv[4];
	const char* tpDBName = argv[5];
	if ( tpIP == NULL || tpPort == NULL || tpUser == NULL || tpPasswd == NULL || tpDBName == NULL )
	{
		return -1;
	}

	CParseProtobuf tParseProtoBuf;
	bool bResult = tParseProtoBuf.InitializeMysql( tpIP, atoi(tpPort), tpUser, tpPasswd, tpDBName );
	if ( !bResult )
	{
		return -1;
	}
	
	char tpEnterStr[100] = { 0 };
	while (true)
	{
		printf("\n");
		printf("--------------------------------- �������µ�ָ�� :  ---------------------------------\n");
		printf("switch : �л�Ҫ���ӵ����ݿ� \n");
		printf("parse : ��ĳ�����ĳ���ֶν�����һ��xml�ļ�\n");
		printf("write : ��ĳ��xml�ļ�д�뵽ĳ�����ĳ���ֶ���ȥ\n");
		printf("exit : ָ���������\n");
		printf("���ʹ�� Backspace ����֡�^H������ʹ�á�Ctrl + Backspace�� \n");
		printf("\n");
		gets( tpEnterStr );
		if ( tpEnterStr == NULL )
		{
			printf("�����������������룡\n");
			continue;
		}
		else if( strcmp( tpEnterStr, "exit") == 0 )
		{
			break;
		}
		else if ( strcmp( tpEnterStr, "switch") == 0 )// �л���������ݿ�
		{
			if ( feof(stdin) || ferror(stdin) )
				continue;
			printf("�л����ݿ⣺\n");
			char pDataBase[100] = { 0 };
			gets( pDataBase );
			//read(STDIN_FILENO, pDataBase, 100);
			if ( pDataBase == NULL )
			{
				printf("�л����ݿ�ʧ�ܣ�\n");
				continue;
			}
			bool bSwitch = tParseProtoBuf.SwitchOtherDataBase( pDataBase );
			if ( !bSwitch )
			{
				printf("�л����ݿ�ʧ�ܣ�\n");
				continue;
			}
		}
		else if ( strcmp( tpEnterStr, "parse") == 0 )
		{
			if ( feof(stdin) || ferror(stdin) )
				continue;

			printf("����PB��\n");
			printf("�����ʽ :  [����] [�ֶ���] [��¼������ֵ] \n");
			printf("�� : all [����] [��¼������ֵ] \n");
			printf("\n");
			char pParseCmd[100] = { 0 };
			if ( gets( pParseCmd ) == NULL )
			{
				printf("��Ч�Ľ��� PB ָ�\n");
				continue;
			}

			Tokens tpArgv = StrSplit( pParseCmd , " " );
			if ( strcmp( tpArgv[0].c_str(), "all") == 0 )// һ������
			{
				if ( tpArgv.size() < 3 )
				{
					printf("����ָ���ֶβ����������������룡\n");
					continue;
				}
				const char* tFormName = tpArgv[1].c_str();// ����
				const char* tPrimaryKeyVal = tpArgv[2].c_str();// ����ֵ
				if ( tFormName == NULL || tPrimaryKeyVal == NULL )
				{
					printf("һ������ all �����������������룡\n");
					continue;
				}
				// ����������������ֶ�
				int tResult = tParseProtoBuf.ParseCore( tFormName, "", tPrimaryKeyVal, true );
				if ( tResult != 0 )
				{
					printf("Parse UMS_FORM Failed !\n");
					continue;
				}
			}
			else
			{
				if ( tpArgv.size() < 3 )
				{
					printf("����ָ���ֶβ����������������룡\n");
					continue;
				}
				const char* tFormName = tpArgv[0].c_str();// ����
				const char* tFieldName = tpArgv[1].c_str();// �ֶ������Ժ��Ƕ����������һ��"all"���ȫ������
				const char* tPrimaryKeyVal = tpArgv[2].c_str();// ����ֵ

				if ( tFormName == NULL || tFieldName == NULL || tPrimaryKeyVal == NULL )
				{
					printf("����ָ���ֶβ����������������룡\n");
					continue;
				}

				// ����������ĵ����ֶ�
				int tResult = tParseProtoBuf.ParseCore( tFormName, tFieldName, tPrimaryKeyVal, false );
				if ( tResult != 0 )
				{
					printf("Parse UMS_FORM Failed !\n");
					continue;
				}
			}//end else if ( strcmp( tpArgv[0].c_str(), "all") == 0 )// һ������
		}//end else if ( strcmp( tpEnterStr, "parse") == 0 )
		else if ( strcmp( tpEnterStr, "write") == 0 )
		{
			if ( feof(stdin) || ferror(stdin) )
				continue;

			printf("д��PB��\n");
			printf("�����ʽ :  [����] [�ֶ���] [��¼������ֵ] \n");
			printf("\n");
			char pParseCmd[100] = { 0 };
			if ( gets( pParseCmd ) == NULL )
			{
				printf("��Ч�Ľ��� PB ָ�\n");
				continue;
			}

			Tokens tpArgv = StrSplit( pParseCmd , " " );
			if ( tpArgv.size() < 3 )
			{
				printf("����ָ���ֶβ����������������룡\n");
				continue;
			}
			const char* tFormName = tpArgv[0].c_str();// ����
			const char* tFieldName = tpArgv[1].c_str();// �ֶ������Ժ��Ƕ����������һ��"all"���ȫ������
			const char* tPrimaryKeyVal = tpArgv[2].c_str();// ����ֵ

			if ( tFormName == NULL || tFieldName == NULL || tPrimaryKeyVal == NULL )
			{
				printf("����ָ���ֶβ����������������룡\n");
				continue;
			}

			int tResult = tParseProtoBuf.CreatePBCore( tFormName, tFieldName, tPrimaryKeyVal );
			if ( tResult != 0 )
			{
				printf("XML Write PB Error !\n");
				continue;
			}
		}
		else
		{
			printf("Error Enter Args : %s\n", tpEnterStr);
			continue;
		}
	}//end while(true)
	
	return 0;
}
