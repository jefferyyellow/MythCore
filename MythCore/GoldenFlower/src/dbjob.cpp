#include "dbjob.h"
#include "internalmsg.h"
#include "internalmsgpool.h"
#include "mysqlqueryresult.h"
#include "gameserver.h"
#include "entityplayer.h"
#include "objpool.h"
#include "dbmodule.hxx.pb.h"
#include "errcode.h"
CDBJob::CDBJob()
{

}

CDBJob::~CDBJob()
{

}

int CDBJob::init(char* pHost, char* pUserName, char* pPasswd, char* pDataBase, int nPort, char* pUnixSocket)
{
	int nResult = mDataBase.connectMysqlServer(pHost, pUserName, pPasswd, pDataBase, nPort, pUnixSocket);
	return nResult;

	return 0;
}

void CDBJob::doing(int uParam)
{
	//printf("CDBJob:: %d\n", uParam);
	//Sleep(3000);
	checkDBStream();
	while (true)
	{
		CInternalMsg* pIMMsg = mTaskManager.popTask();
		// 如果
		if (NULL == pIMMsg)
		{
			break;
		}
		onTask(pIMMsg);

		CInternalMsgPool::Inst()->freeMsg(pIMMsg);
	}
}

int CDBJob::setBuffer(int nBufferSize)
{
	mJobBuffer = new byte[nBufferSize];
	if (NULL == mJobBuffer)
	{
		return -1;
	}
	mJobStream.Initialize(mJobBuffer, nBufferSize);
	return 0;
}

/// 压入工作数据
void CDBJob::pushBackJobData(byte* pData, int nDataLength)
{
	if (NULL == pData || 0 == nDataLength)
	{
		return;
	}

	mJobStreamLock.lock();
	mJobStream.PushPacket(pData, nDataLength);
	mJobStreamLock.unlock();
}

/// 取出工作数据
void CDBJob::popUpJobData(byte* pData, int &rLength)
{
	if (NULL == pData)
	{
		return;
	}

	mJobStreamLock.lock();
	mJobStream.GetHeadPacket(pData, rLength);
	mJobStreamLock.unlock();
}

/// 处理DB流里的数据
void CDBJob::checkDBStream()
{
	for (int i = 0; i < 2000; ++ i)
	{
		int nLength = 0;
		popUpJobData((byte*)&mDBRequest, nLength);
		if (nLength <= 0)
		{
			return;
		}
		nLength -= sizeof(CDBRequestHeader);

		int nResultLength = sizeof(mDBResponse.mSqlBuffer) - 1;

		int nRowNum = 0;
		int nColNum = 0;
		int nResult = SUCCESS;
		switch (mDBRequest.mSessionType)
		{
			case emSessionType_SavePlayerBaseProperty:
			{
				nResult = onSavePlayerBaseProperty(nLength);
				break;
			}
			default:
			{
				break;
			}
		}
		if (SUCCESS == nResult)
		{
			nResult = mDataBase.query((char*)mDBRequest.mSqlBuffer, (byte*)mDBResponse.mSqlBuffer, nResultLength,
				nRowNum, nColNum);
			if (nResult != SUCCESS)
			{
				LOG_ERROR("mysql query error, errno: %d, %s, %s", mysql_errno(mDataBase.GetMysql()), mysql_error(mDataBase.GetMysql()),
					(char*)mDBRequest.mSqlBuffer);
			}
		}

		mDBResponse.mPlayerID = mDBRequest.mPlayerID;
		mDBResponse.mResult = nResult;
		mDBResponse.mParam1 = mDBRequest.mParam1;
		mDBResponse.mParam2 = mDBRequest.mParam2;
		mDBResponse.mSessionType = mDBRequest.mSessionType;
		mDBResponse.mRowNum = nRowNum;
		mDBResponse.mColNum = nColNum;
		mDBResponse.mSqlLenth = nResultLength;
		CSceneJob::Inst()->pushBackDBData((byte*)&mDBResponse, sizeof(CDBResponseHeader)+nResultLength);
	}
}

/// 处理保存玩家基本属性
int CDBJob::onSavePlayerBaseProperty(int nLength)
{
	PBSavePlayer tSavePlayer;
	if (!tSavePlayer.ParseFromArray(mDBRequest.mSqlBuffer, nLength))
	{
		LOG_ERROR("Player Base Property Parse From Array Error, PlayerID: %d", mDBRequest.mPlayerID);
		return -1;
	}
	//mSqlLength = 0;
	//int tLen = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
	//	"update PlayerBaseProperty set ");
	//mSqlLength += tLen;
	//int nResult = parsePBForSql(tSavePlayer);
	//if (SUCCESS != nResult)
	//{
	//	return nResult;
	//}

	//tLen = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
	//	" where role_id=%d", mDBRequest.mPlayerID);

	mSqlLength = 0;
	int tLen = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
		"call UpdatePlayerBaseProperty(%d,", mDBRequest.mPlayerID);
	mSqlLength += tLen;
	int nResult = parsePBForPrecedure(tSavePlayer);
	if (SUCCESS != nResult)
	{
		return nResult;
	}

	tLen = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
		")");
	return SUCCESS;
}

/// 分析PB结构，组成sql语句
int CDBJob::parsePBForSql(Message& rMessage)
{
	const ::google::protobuf::Descriptor* pDesc = rMessage.GetDescriptor();
	const ::google::protobuf::Reflection* pRef = rMessage.GetReflection();
	MYSQL *pMysql = mDataBase.GetMysql();


	int nLength = 0;
	char tBuffer[65000] = { 0 };
	int nCount = 0;
	for (int i = 0; i < pDesc->field_count(); ++ i)
	{
		const ::google::protobuf::FieldDescriptor* pFieldDescriptor = pDesc->field(i);
		// repeated不处理
		if (pFieldDescriptor->is_repeated())
		{
			continue;
		}
		// 消息里没有这个字段
		if (!pRef->HasField(rMessage, pFieldDescriptor))
		{
			continue;
		}
		if (nCount > 0)
		{
			nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
				"%s", ",");
			mSqlLength += nLength;
		}
		++ nCount;
		switch (pFieldDescriptor->cpp_type())
		{
			//case ::google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
			//case ::google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
			case ::google::protobuf::FieldDescriptor::CPPTYPE_INT32:
			{
				nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength, 
					"%s=%d", pFieldDescriptor->name().c_str(), pRef->GetInt32(rMessage, pFieldDescriptor));
				mSqlLength += nLength;
				break;
			}
			case ::google::protobuf::FieldDescriptor::CPPTYPE_INT64:
			{
				nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
					"%s=%lld", pFieldDescriptor->name().c_str(), pRef->GetInt64(rMessage, pFieldDescriptor));
				mSqlLength += nLength;
				break;
			}
			case ::google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
			{
				nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
					"%s=%u", pFieldDescriptor->name().c_str(), pRef->GetUInt32(rMessage, pFieldDescriptor));
				mSqlLength += nLength;
				break;
			}
			case ::google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
			{
				nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
					"%s=%llu", pFieldDescriptor->name().c_str(), pRef->GetUInt64(rMessage, pFieldDescriptor));
				mSqlLength += nLength;
				break;
			}
			case ::google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
			{
				nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
					"%s=%lf", pFieldDescriptor->name().c_str(), pRef->GetDouble(rMessage, pFieldDescriptor));
				mSqlLength += nLength;
				break;
			}
			case ::google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
			{
				nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
					"%s=%f", pFieldDescriptor->name().c_str(), pRef->GetFloat(rMessage, pFieldDescriptor));
				mSqlLength += nLength;
				break;
			}
			case ::google::protobuf::FieldDescriptor::CPPTYPE_STRING:
			{
				nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
					"%s=%s", pFieldDescriptor->name().c_str(), pRef->GetString(rMessage, pFieldDescriptor).c_str());
				mSqlLength += nLength;
				break;
			}
			case ::google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
			{
				const Message &rSubMsg = pRef->GetMessage(rMessage, pFieldDescriptor);
				nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
					"%s=", pFieldDescriptor->name().c_str());
				mSqlLength += nLength;
				if (rSubMsg.SerializeToArray(tBuffer, sizeof(tBuffer) - 1))
				{
					nLength = mysql_real_escape_string(pMysql, (char*)&(mDBRequest.mSqlBuffer[mSqlLength]), tBuffer, rSubMsg.ByteSize());
					if (nLength == 0)
					{
						nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
							"%s", "NULL");
					}
					else if (nLength < 0)
					{
						return -1;
					}
					mSqlLength += nLength;
				}
				else
				{
					return -1;
				}

				break;
			}
			default:
				break;
		}
	}
	return SUCCESS;
}

/// 分析PB结构，组成调用precedure语句
int CDBJob::parsePBForPrecedure(Message& rMessage)
{
	const ::google::protobuf::Descriptor* pDesc = rMessage.GetDescriptor();
	const ::google::protobuf::Reflection* pRef = rMessage.GetReflection();
	MYSQL *pMysql = mDataBase.GetMysql();


	int nLength = 0;
	char tBuffer[65000] = { 0 };
	int nCount = 0;
	for (int i = 0; i < pDesc->field_count(); ++i)
	{
		const ::google::protobuf::FieldDescriptor* pFieldDescriptor = pDesc->field(i);
		// repeated不处理
		if (pFieldDescriptor->is_repeated())
		{
			continue;
		}
		// 消息里没有这个字段
		if (!pRef->HasField(rMessage, pFieldDescriptor))
		{
			continue;
		}
		if (nCount > 0)
		{
			nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
				"%s", ",");
			mSqlLength += nLength;
		}
		++nCount;
		switch (pFieldDescriptor->cpp_type())
		{
			//case ::google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
			//case ::google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
			case ::google::protobuf::FieldDescriptor::CPPTYPE_INT32:
			{
				nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
					"%d", pFieldDescriptor->name().c_str(), pRef->GetInt32(rMessage, pFieldDescriptor));
				mSqlLength += nLength;
				break;
			}
			case ::google::protobuf::FieldDescriptor::CPPTYPE_INT64:
			{
				nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
					"%lld", pFieldDescriptor->name().c_str(), pRef->GetInt64(rMessage, pFieldDescriptor));
				mSqlLength += nLength;
				break;
			}
			case ::google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
			{
				nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
					"%u", pFieldDescriptor->name().c_str(), pRef->GetUInt32(rMessage, pFieldDescriptor));
				mSqlLength += nLength;
				break;
			}
			case ::google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
			{
				nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
					"%llu", pFieldDescriptor->name().c_str(), pRef->GetUInt64(rMessage, pFieldDescriptor));
				mSqlLength += nLength;
				break;
			}
			case ::google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
			{
				nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
					"%lf", pFieldDescriptor->name().c_str(), pRef->GetDouble(rMessage, pFieldDescriptor));
				mSqlLength += nLength;
				break;
			}
			case ::google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
			{
				nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
					"%f", pFieldDescriptor->name().c_str(), pRef->GetFloat(rMessage, pFieldDescriptor));
				mSqlLength += nLength;
				break;
			}
			case ::google::protobuf::FieldDescriptor::CPPTYPE_STRING:
			{
				nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
					"%s", pFieldDescriptor->name().c_str(), pRef->GetString(rMessage, pFieldDescriptor).c_str());
				mSqlLength += nLength;
				break;
			}
			case ::google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
			{
				const Message &rSubMsg = pRef->GetMessage(rMessage, pFieldDescriptor);
				if (rSubMsg.SerializeToArray(tBuffer, sizeof(tBuffer) - 1))
				{
					nLength = mysql_real_escape_string(pMysql, (char*)&(mDBRequest.mSqlBuffer[mSqlLength]), tBuffer, rSubMsg.ByteSize());
					if (nLength == 0)
					{
						nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
							"%s", "NULL");
					}
					else if (nLength < 0)
					{
						return -1;
					}
					mSqlLength += nLength;
				}
				else
				{
					return -1;
				}

				break;
			}
			default:
				break;
		}
	}
	return SUCCESS;
}

void CDBJob::onTask(CInternalMsg* pMsg)
{
}