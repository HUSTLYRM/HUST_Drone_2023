#ifndef __MX64_H__
#define __MX64_H__

#include "config.h"

struct MX64
{
	union
	{
		uint8_t  raw[8];
		short setPoint;
	}MX64_sendData;
};

#endif // __MX64_H__













