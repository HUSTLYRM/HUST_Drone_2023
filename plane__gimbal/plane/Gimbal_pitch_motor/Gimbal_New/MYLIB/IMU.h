#ifndef __IMU_H__
#define __IMU_H__

#include "config.h"

struct IMU
{
	float yawAngle;
	float pitchAngle;
	float yawSpeed;
	float pitchSpeed;
	uint32_t sendId;
	uint32_t receiveId;
  uint32_t updateTime;
	uint8_t Ever_Lost;	
};


#endif // __DBUS_H__


/* end */















