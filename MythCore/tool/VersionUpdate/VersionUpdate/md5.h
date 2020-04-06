#ifndef __MD5_H__
#define __MD5_H__
#include "common.h"
class CMD5
{
private:
	typedef struct
	{
		uint32 h[4];
	}MD5_Core_State;

	struct MD5Context
	{
		MD5_Core_State core;
		uint8 block[64];
		int32 bskused;
		uint32 lenhi, lenlo;
	};

public:
	CMD5();
	virtual ~CMD5();

	static int compute(const void *pInput, int32 nInputLength, void *pDigest);

private:
	static void MD5_Block(MD5_Core_State * s, uint32 * block);
	static void MD5_Core_Init(MD5_Core_State * s);
	static void MD5Final(uint8 output[16], struct MD5Context *s);
	static void MD5Update(struct MD5Context *s, const uint8 *p, uint32 len);
	static void MD5Init(struct MD5Context *s);
};
#endif
