#include "dbjob.h"
#include "internalmsg.h"
#include "internalmsgpool.h"
#include "mysqlqueryresult.h"
#include "gameserver.h"
#include "entityplayer.h"
#include "dbmodule.hxx.pb.h"
#include "errcode.h"
#include "scenejob.h"
#include "timemanager.h"
CDBJob::CDBJob()
{
	init();
	setJobID((int)emJobID_DB);
}

CDBJob::~CDBJob()
{
	clear();
}

void CDBJob::init()
{
    mJobBuffer = NULL;
    mSqlLength = 0;
}

int CDBJob::initDB(const char* pHost, const char* pUserName, const char* pPasswd, const char* pDataBase, int nPort, const char* pUnixSocket)
{
	int nResult = mDataBase.connectMysqlServer(pHost, pUserName, pPasswd, pDataBase, nPort, pUnixSocket);
	return nResult;
}

void CDBJob::clear()
{
	if (NULL != mJobBuffer)
	{
		delete []mJobBuffer;
		mJobBuffer = NULL;
	}
}

void CDBJob::doing(int uParam)
{
	time_t tTmpTime = CTimeManager::Inst()->getCurrTime();
	if (tTmpTime - mLogTime > TIME_JOB_RUN_LOG)
	{
		mLogTime = tTmpTime;
		LOG_INFO("Job doing, Thread Num: %d, Job ID: %d", uParam, getJobID());
	}
	checkDBStream();

	// 如果scene job已经退出完成了，表示需要保存的数据基本都完成了
	if (CSceneJob::Inst()->getExited())
	{
		// 检查请求队列里是否还有数据，如果没有数据了，表示可以退出了
		if ((!getExited()) && checkJobDataEmpty())
		{
			setExited(true);
		}
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
void CDBJob::pushBackJobData(const byte* pData, int nDataLength)
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

/// 得到工作数据量大小
int CDBJob::checkJobDataEmpty()
{
	mJobStreamLock.lock();
	bool bEmpty = mJobStream.IsEmpty();
	mJobStreamLock.unlock();
	return bEmpty;
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
		nLength -= (int)(sizeof(CDBRequestHeader));

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
		mDBResponse.mSqlResult = nResult;
		mDBResponse.mParam1 = mDBRequest.mParam1;
		mDBResponse.mParam2 = mDBRequest.mParam2;
		mDBResponse.mSessionType = mDBRequest.mSessionType;
		mDBResponse.mRowNum = (short)nRowNum;
		mDBResponse.mColNum = (short)nColNum;
		mDBResponse.mSqlLenth = (short)nResultLength;
		CSceneJob::Inst()->pushBackDBData((byte*)&mDBResponse, (int)(sizeof(CDBResponseHeader)+nResultLength));
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
	int tLen = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer),
		"call UpdatePlayerBaseProperty(%d,", mDBRequest.mPlayerID);
	mSqlLength += tLen;
	int nResult = parsePBForPrecedure(tSavePlayer);
	if (SUCCESS != nResult)
	{
		return nResult;
	}

	snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
		")");
	return SUCCESS;
}

/// 分析PB结构，组成sql语句
int CDBJob::parsePBForSql(const Message& rMessage)
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
					"%s='%s'", pFieldDescriptor->name().c_str(), pRef->GetString(rMessage, pFieldDescriptor).c_str());
				mSqlLength += nLength;
				break;
			}
			case ::google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
			{
				const Message &rSubMsg = pRef->GetMessage(rMessage, pFieldDescriptor);
				nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
					"%s='", pFieldDescriptor->name().c_str());
				mSqlLength += nLength;
				if (rSubMsg.SerializeToArray(tBuffer, sizeof(tBuffer) - 1))
				{
					nLength = (int)mysql_real_escape_string(pMysql, (char*)&(mDBRequest.mSqlBuffer[mSqlLength]), tBuffer, (unsigned long)rSubMsg.ByteSize());
					if (nLength > 0)
					{
						mSqlLength += nLength;
						nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
							"%s", "'");
					}
					else if(nLength == 0)
					{
						nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
							"%s", "NULL'");
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
			case ::google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
			case ::google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
			default:
				break;
		}
	}
	return SUCCESS;
}

/// 分析PB结构，组成调用precedure语句
int CDBJob::parsePBForPrecedure(const Message& rMessage)
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
					"'%s'", pFieldDescriptor->name().c_str(), pRef->GetString(rMessage, pFieldDescriptor).c_str());
				mSqlLength += nLength;
				break;
			}
			case ::google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
			{
				const Message &rSubMsg = pRef->GetMessage(rMessage, pFieldDescriptor);
				if (rSubMsg.SerializeToArray(tBuffer, sizeof(tBuffer) - 1))
				{
					nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
						"%s", "'");
					mSqlLength += nLength;

					nLength = (int)mysql_real_escape_string(pMysql, (char*)&(mDBRequest.mSqlBuffer[mSqlLength]), tBuffer, (unsigned long)rSubMsg.ByteSize());
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
					nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
						"%s", "'");
					mSqlLength += nLength;
				}
				else
				{
					return -1;
				}

				break;
			}
			case ::google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
			case ::google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
			default:
				break;
		}
	}
	return SUCCESS;
}
