#ifndef __MYSQLFIELD_H__
#define __MYSQLFIELD_H__
#include "commontype.h"
namespace Myth
{
	enum TmMysqlDataType
	{
		emMysqlDataType_Unknow = 0,
		emMysqlDataType_String = 1,
		emMysqlDataType_Bool = 2,
		emMysqlDataType_Byte = 3,
		emMysqlDataType_Short = 4,
		emMysqlDataType_Int = 5,
		emMysqlDataType_Int64 = 6,
		emMysqlDataType_Float = 7,
		emMysqlDataType_Double = 8,
		emMysqlDataType_Blob = 9,
	};

	class CMysqlField
	{
		CMysqlField()
		:mpValue(NULL), mValueLen(0)
		{
		}
		CMysqlField(CMysqlField& rFiled)
		{
			if (&rFiled == this)
			{
				return;
			}
			mpValue = rFiled.mpValue;
			mValueLen = rFiled.mValueLen;
		}
		CMysqlField(char* pValue, uint16 nValueLen)
			:mpValue(pValue), mValueLen(nValueLen)
		{

		}
		~CMysqlField();

		char*				getValue(){ return mpValue; }
		void				setValue(char* pValue){ mpValue = pValue; }

		uint16				getValueLen(){ return mValueLen; }
		void				setValueLen(uint16 nValueLen){ mValueLen = nValueLen; }

	private:
		char*				mpValue;
		uint16				mValueLen;
	};
}

#endif