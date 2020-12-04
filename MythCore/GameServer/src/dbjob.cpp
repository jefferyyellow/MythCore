#include "dbjob.h"
#include "internalmsg.h"
#include "internalmsgpool.h"
#include "mysqlqueryresult.h"
#include "gameserver.h"
#include "entityplayer.h"
#include "dbmodule.hxx.pb.h"
#include "errcode.h"
#include "timemanager.h"
#include "jobmanager.h"
CDBJob::CDBJob()
{
	init();
	setJobType((int)emJobID_DB);
}

CDBJob::~CDBJob()
{
	clear();
}

void CDBJob::init()
{
    mSqlLength = 0;
}

int CDBJob::initDB(const char* pHost, const char* pUserName, const char* pPasswd, 
				   const char* pDataBase, int nPort, const char* pUnixSocket)
{
	int nResult = mDataBase.connectMysqlServer(pHost, pUserName, pPasswd, pDataBase, nPort, pUnixSocket);
	return nResult;
}

void CDBJob::clear()
{
}

void CDBJob::doing(int uParam)
{
	time_t tTmpTime = CTimeManager::Inst()->getCurrTime();
	if (tTmpTime - mLogTime > TIME_JOB_RUN_LOG)
	{
		mLogTime = tTmpTime;
		LOG_INFO("Job doing, Thread Num: %d, Job ID: %d", uParam, getJobType());
	}
	checkDBTask();

	//// 如果scene job已经退出完成了，表示需要保存的数据基本都完成了
	//if (CSceneJob::Inst()->getExited())
	//{
	//	// 检查请求队列里是否还有数据，如果没有数据了，表示可以退出了
	//	if ((!getExited()) && mTaskManager.empty()))
	//	{
	//		setExited(true);
	//	}
	//}
}

/// 处理DB流里的数据
void CDBJob::checkDBTask()
{
	for (int i = 0; i < 2000; ++ i)
	{
		CInternalMsg* pIMMsg = mTaskManager.popTask();
		// 如果
		if (NULL == pIMMsg)
		{
			break;
		}
		if (pIMMsg->getMsgID() != IM_REQUEST_DB_SQL && pIMMsg->getMsgID() !=  IM_REQUEST_DB_MSG)
		{
			continue;
		}
		CIMDBMsgResponse* pResponse = new CIMDBMsgResponse;
		if (NULL == pResponse)
		{
			break;
		}

		CIMDBMsgBase* pMDBMsgBase = (CIMDBMsgBase*)pIMMsg;
		CDBRequestHeader& rHeader = pMDBMsgBase->mHeader;
		
		pResponse->mHeader.mParam1 = rHeader.mParam1;
		pResponse->mHeader.mParam2 = rHeader.mParam2;
		pResponse->mHeader.mPlayerID = rHeader.mPlayerID;
		pResponse->mHeader.mSessionType = rHeader.mSessionType;
		int nTargetJobID = rHeader.mJobID;

		int nRowNum = 0;
		int nColNum = 0;
		int nResult = SUCCESS;
		int nResultLength = sizeof(mDBResponse.mSqlBuffer);
		if (pIMMsg->getMsgID() == IM_REQUEST_DB_SQL)
		{
			CIMDBSqlRequest* pRequest = (CIMDBSqlRequest*)pIMMsg;
			nResult = mDataBase.query((char*)pRequest->mSql, (byte*)mDBResponse.mSqlBuffer, nResultLength,
				nRowNum, nColNum);
			if (nResult != SUCCESS)
			{
				LOG_ERROR("mysql query error, errno: %d, %s, %s",
					mysql_errno(mDataBase.GetMysql()),
					mysql_error(mDataBase.GetMysql()),
					(char*)pRequest->mSql);
			}
		}
		else
		{
			CIMDBMsgRequest* pRequest = (CIMDBMsgRequest*)pIMMsg;
			switch (rHeader.mSessionType)
			{
				case emSessionType_SavePlayerBaseProperty:
				{
					nResult = onSavePlayerBaseProperty(pRequest->mSqlMsg);
					break;
				}
				case emSessionType_SavePlayerMail:
				{
					nResult = onSaveMail(pRequest->mSqlMsg);
					break;
				}
				case emSessionType_SaveGlobalMail:
				{
					nResult = onSaveGlobalMail(pRequest->mSqlMsg);
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
					LOG_ERROR("mysql query error, errno: %d, %s, %s",
						mysql_errno(mDataBase.GetMysql()),
						mysql_error(mDataBase.GetMysql()),
						(char*)mDBRequest.mSqlBuffer);
				}
			}
			delete pRequest->mSqlMsg;
		}
		if (SUCCESS == nResult)
		{
			pResponse->mHeader.mRowNum = nRowNum;
			pResponse->mHeader.mColNum = nColNum;
			pResponse->mMsgBuffer = new byte[nResultLength];
			pResponse->mHeader.mSqlLenth = nResultLength;
			memcpy(pResponse->mMsgBuffer, mDBResponse.mSqlBuffer, nResultLength);
		}
		pResponse->mHeader.mSqlResult = nResult;

		CJobManager::Inst()->pushTaskByID(nTargetJobID, pResponse);
		delete pIMMsg;
	}
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

		// 消息里没有这个字段，只有消息类型才能分清，数值类型和字符串如果是默认值的话，HasField也会返回false，无法区分是设置了值还是本身就是默认值
		if (::google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE == pFieldDescriptor->cpp_type()
			&& !pRef->HasField(rMessage, pFieldDescriptor))
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
		// 消息里没有这个字段，只有消息类型才能分清，数值类型和字符串如果是默认值的话，HasField也会返回false，无法区分是设置了值还是本身就是默认值
		if (::google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE == pFieldDescriptor->cpp_type()
			&& !pRef->HasField(rMessage, pFieldDescriptor))
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
					"%d", pRef->GetInt32(rMessage, pFieldDescriptor));
				mSqlLength += nLength;
				break;
			}
			case ::google::protobuf::FieldDescriptor::CPPTYPE_INT64:
			{
				nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
					"%lld", pRef->GetInt64(rMessage, pFieldDescriptor));
				mSqlLength += nLength;
				break;
			}
			case ::google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
			{
				nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
					"%u", pRef->GetUInt32(rMessage, pFieldDescriptor));
				mSqlLength += nLength;
				break;
			}
			case ::google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
			{
				nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
					"%llu", pRef->GetUInt64(rMessage, pFieldDescriptor));
				mSqlLength += nLength;
				break;
			}
			case ::google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
			{
				nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
					"%lf", pRef->GetDouble(rMessage, pFieldDescriptor));
				mSqlLength += nLength;
				break;
			}
			case ::google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
			{
				nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
					"%lf", pRef->GetFloat(rMessage, pFieldDescriptor));
				mSqlLength += nLength;
				break;
			}
			case ::google::protobuf::FieldDescriptor::CPPTYPE_STRING:
			{
				nLength = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
					"'%s'", pRef->GetString(rMessage, pFieldDescriptor).c_str());
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

/// 处理保存玩家基本属性
int CDBJob::onSavePlayerBaseProperty(const Message* pMessage)
{
	PBSavePlayer* pbSavePlayer = (PBSavePlayer*)pMessage;

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
	int nResult = parsePBForPrecedure(*pbSavePlayer);
	if (SUCCESS != nResult)
	{
		return nResult;
	}

	snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
		")");
	return SUCCESS;
}

/// 处理保存玩家邮件
int CDBJob::onSaveMail(const Message* pMessage)
{
	PBMail* pbMail = (PBMail*)pMessage;
	if (NULL == pbMail)
	{
		return -1;
	}

	mSqlLength = 0;
	int tLen = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer),
			"call SaveMail(");

	mSqlLength += tLen;
	int nResult = parsePBForPrecedure(*pbMail);
	if (SUCCESS != nResult)
	{
		return nResult;
	}

	snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
		")");
	return SUCCESS;
}

/// 处理保存全局邮件
int CDBJob::onSaveGlobalMail(const Message* pMessage)
{
	PBGlobalMail* pbGlobalMail = (PBGlobalMail*)pMessage;
	if (NULL == pbGlobalMail)
	{
		return -1;
	}

	mSqlLength = 0;
	int tLen = snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer),
		"call SaveGlobalMail(");

	mSqlLength += tLen;
	int nResult = parsePBForPrecedure(*pbGlobalMail);
	if (SUCCESS != nResult)
	{
		return nResult;
	}

	snprintf((char*)&(mDBRequest.mSqlBuffer[mSqlLength]), sizeof(mDBRequest.mSqlBuffer) - mSqlLength,
		")");
	return SUCCESS;
}