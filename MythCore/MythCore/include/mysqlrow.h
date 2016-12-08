#ifndef __MYSQLFIELD_H__
#define __MYSQLFIELD_H__
#include "mysqlfield.h"
namespace Myth
{
	#define MAX_ROW_BUFF_NUM	8000		// innodb row limit
	#define MAX_ROW_FIELD_NUM	128

	class CMysqlRow
	{
		CMysqlRow()
		{
			mRowBuff[0] = '\0';
			mRowLen = 0;
		}

		CMysqlField*	getField(int nIndex)
		{
			if (nIndex < 0 || nIndex >= MAX_ROW_FIELD_NUM)
			{
				return NULL;
			}
			return &mField[nIndex];
		}
		void			setField(int nIndex, char* pValue, uint16 nValueLen)
		{
			if (nIndex < 0 || nIndex >= MAX_ROW_FIELD_NUM)
			{
				return;
			}
			mField[nIndex].mpValue
		}

		char*			getRowBuff()
		{
			return mRowBuff;
		}
		bool			appendBuff(char* pValue, uint16 nValueLen)
		{
			if (NULL == pValue)
			{
				return false;
			}
			if (mRowLen + nValueLen > MAX_ROW_BUFF_NUM)
			{
				return false;
			}

			strncat(mRowBuff + mRowLen, pValue, nValueLen);
			return true;
		}

		uint16			getValueLen(){ return mRowLen; }
		void			setValueLen(uint16 nRoleLen){ mRowLen = nRoleLen; }

	private:
		CMysqlField		mField[MAX_ROW_FIELD_NUM];
		char			mRowBuff[MAX_ROW_BUFF_NUM];
		uint16			mRowLen;
	};
}
#endif