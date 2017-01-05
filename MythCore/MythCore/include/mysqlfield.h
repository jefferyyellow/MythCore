#ifndef __MYSQLFIELD_H__
#define __MYSQLFIELD_H__
#include "commontype.h"
namespace Myth
{
	class CMysqlField
	{
	public:
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
		~CMysqlField()
		{

		}

	public:
		char*				getValue(){ return mpValue; }
		void				setValue(char* pValue){ mpValue = pValue; }

		uint32				getValueLen(){ return mValueLen; }
		void				setValueLen(uint32 nValueLen){ mValueLen = nValueLen; }

	private:
		char*				mpValue;
		uint32				mValueLen;
	};
}

#endif