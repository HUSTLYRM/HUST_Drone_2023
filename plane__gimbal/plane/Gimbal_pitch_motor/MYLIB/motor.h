#ifndef __MOTOR_H__
#define __MOTOR_H__


#include "config.h"

enum motorMode
{
	openLoop,
	speedLoop,
	positionLoop,
	doubleLoop,
	imuPitch,
	imuYaw,
	tripleloop,
	pitch_mode,
	yaw_mode
};
void GM6020_control(void);
void M2006_control(void);

void M2006_Send_Can1(void);
void GM6020_Send_can1(void);


#endif // __MOTOR_H__


/* end */









