#ifndef __MYSQLFIELD_H__
#define __MYSQLFIELD_H__
#include "commontype.h"
namespace Myth
{
	class CMysqlField
	{
	public:
		CMysqlField()
		:mpValue(nullptr), mValueLen(0)
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
		CMysqlField(char* pValue, int nValueLen)
			:mpValue(pValue), mValueLen(nValueLen)
		{

		}
		~CMysqlField()
		{

		}

	public:
		char*				getValue(){ return mpValue; }
		void				setValue(char* pValue){ mpValue = pValue; }

		int					getValueLen(){ return mValueLen; }
		void				setValueLen(int nValueLen){ mValueLen = nValueLen; }

	private:
		char*				mpValue;
		int					mValueLen;
	};
}

#endif