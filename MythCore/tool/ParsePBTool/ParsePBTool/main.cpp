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


// Note : 拟做成“parsetool 【表名】【主键ID值】”，然后导出一个xml
// 以后考虑扩展成支持多个表名做参数，然后输出在一个文件里
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
		printf("参数错误！请使用 ./parsetool [数据库地址] [端口号] [用户名] [密码] [数据库名称] .\n");
		printf("更多命令请使用 --help 命令查看使用方法 .\n");
		return 0;
	}

	if ( strcmp( argv[1], "--help") == 0 )// 命令使用方法
	{
		printf("用法：\n");
		printf("[单条数据读写 : ]\n");
		printf("建立与某个数据库的连接：\n ./parsepbtool [数据库地址] [端口号] [用户名] [密码] [数据库名称] .\n");
		printf("切换数据库：switch");
		printf("输出某个表的某一条记录的一个字段：\n");
		printf("./parsepbtool [表名] [字段名] [记录的主键值] \n");
		printf("如：./parsepbtool UMS_GLOBAL_DATA global_status 1 \n");
		printf("\n");
		printf("输出某个表的某一条记录的所有字段：\n");
		printf("./parsetool all [表名] [记录的主键值] \n");
		printf("如：./parsepbtool all UMS_GLOBAL_DATA 1 \n");
		printf("\n");
		printf("[多条数据批量读写 : ]\n");
		printf("[把连接的数据库信息，以及要操作的Blob字段名，在 Config.xml 里配置好。把要操作的账号名，写在 primarykey.txt 里 ]\n");
		printf("批量解析 Blob 字段成 xml：./parsepbtool config parse\n");
		printf("批量把 xml 文件写成数据库 Blob 字段：./parsepbtool config write\n");
		printf("\n");
		printf("如有问题，请向 Jeffery@7k7k, Sunrise Meier@7k7k 报告错误.\n");
		return 0;
	}

	// 加载配置文件方式解析和写入
	if ( argv[1] != NULL && strcmp( argv[1], "config" ) == 0 )
	{
		if ( argv[2] == NULL )
		{
			printf("参数错误，请重新输入！\n");
			printf("config 参数后必须指定 parse 或 write！详见 ./parsepbtool --help 输出信息\n");
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
		printf("--------------------------------- 请输入新的指令 :  ---------------------------------\n");
		printf("switch : 切换要连接的数据库 \n");
		printf("parse : 把某个表的某个字段解析成一个xml文件\n");
		printf("write : 把某个xml文件写入到某个表的某个字段中去\n");
		printf("exit : 指令结束程序\n");
		printf("如果使用 Backspace 后出现“^H”，请使用“Ctrl + Backspace” \n");
		printf("\n");
		gets( tpEnterStr );
		if ( tpEnterStr == NULL )
		{
			printf("参数错误，请重新输入！\n");
			continue;
		}
		else if( strcmp( tpEnterStr, "exit") == 0 )
		{
			break;
		}
		else if ( strcmp( tpEnterStr, "switch") == 0 )// 切换到别的数据库
		{
			if ( feof(stdin) || ferror(stdin) )
				continue;
			printf("切换数据库：\n");
			char pDataBase[100] = { 0 };
			gets( pDataBase );
			//read(STDIN_FILENO, pDataBase, 100);
			if ( pDataBase == NULL )
			{
				printf("切换数据库失败！\n");
				continue;
			}
			bool bSwitch = tParseProtoBuf.SwitchOtherDataBase( pDataBase );
			if ( !bSwitch )
			{
				printf("切换数据库失败！\n");
				continue;
			}
		}
		else if ( strcmp( tpEnterStr, "parse") == 0 )
		{
			if ( feof(stdin) || ferror(stdin) )
				continue;

			printf("解析PB：\n");
			printf("命令格式 :  [表名] [字段名] [记录的主键值] \n");
			printf("或 : all [表名] [记录的主键值] \n");
			printf("\n");
			char pParseCmd[100] = { 0 };
			if ( gets( pParseCmd ) == NULL )
			{
				printf("无效的解析 PB 指令！\n");
				continue;
			}

			Tokens tpArgv = StrSplit( pParseCmd , " " );
			if ( strcmp( tpArgv[0].c_str(), "all") == 0 )// 一键生成
			{
				if ( tpArgv.size() < 3 )
				{
					printf("解析指定字段参数错误，请重新输入！\n");
					continue;
				}
				const char* tFormName = tpArgv[1].c_str();// 表名
				const char* tPrimaryKeyVal = tpArgv[2].c_str();// 主键值
				if ( tFormName == NULL || tPrimaryKeyVal == NULL )
				{
					printf("一键解析 all 参数错误，请重新输入！\n");
					continue;
				}
				// 解析单个表的所有字段
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
					printf("解析指定字段参数错误，请重新输入！\n");
					continue;
				}
				const char* tFormName = tpArgv[0].c_str();// 表名
				const char* tFieldName = tpArgv[1].c_str();// 字段名（以后考虑多个，或是用一个"all"输出全部？）
				const char* tPrimaryKeyVal = tpArgv[2].c_str();// 主键值

				if ( tFormName == NULL || tFieldName == NULL || tPrimaryKeyVal == NULL )
				{
					printf("解析指定字段参数错误，请重新输入！\n");
					continue;
				}

				// 解析单个表的单个字段
				int tResult = tParseProtoBuf.ParseCore( tFormName, tFieldName, tPrimaryKeyVal, false );
				if ( tResult != 0 )
				{
					printf("Parse UMS_FORM Failed !\n");
					continue;
				}
			}//end else if ( strcmp( tpArgv[0].c_str(), "all") == 0 )// 一键生成
		}//end else if ( strcmp( tpEnterStr, "parse") == 0 )
		else if ( strcmp( tpEnterStr, "write") == 0 )
		{
			if ( feof(stdin) || ferror(stdin) )
				continue;

			printf("写入PB：\n");
			printf("命令格式 :  [表名] [字段名] [记录的主键值] \n");
			printf("\n");
			char pParseCmd[100] = { 0 };
			if ( gets( pParseCmd ) == NULL )
			{
				printf("无效的解析 PB 指令！\n");
				continue;
			}

			Tokens tpArgv = StrSplit( pParseCmd , " " );
			if ( tpArgv.size() < 3 )
			{
				printf("解析指定字段参数错误，请重新输入！\n");
				continue;
			}
			const char* tFormName = tpArgv[0].c_str();// 表名
			const char* tFieldName = tpArgv[1].c_str();// 字段名（以后考虑多个，或是用一个"all"输出全部？）
			const char* tPrimaryKeyVal = tpArgv[2].c_str();// 主键值

			if ( tFormName == NULL || tFieldName == NULL || tPrimaryKeyVal == NULL )
			{
				printf("解析指定字段参数错误，请重新输入！\n");
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
