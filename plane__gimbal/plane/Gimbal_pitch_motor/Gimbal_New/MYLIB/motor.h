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
	imuYaw
};


void GM6020_control(void);
void send_MX64_action(void);
void send_MX64_position(unsigned short Control_Angle);
void set_MX64_baudrate(void);
void M2006_control(void);


#endif // __MOTOR_H__


/* end */









