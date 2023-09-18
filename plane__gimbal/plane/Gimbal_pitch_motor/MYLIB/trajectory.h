#ifndef __TRAJECTORY__
#define __TRAJECTORY__

#include "config.h"

//#define gravity		9.8
#define real_target_heightdiff	0.5

/* trajectory estimation */
struct trajectory_t
{
	float realdistance;
	float realspeed;
	float realheight;
	float targetheight;
	float glideheight;
	float glidetime;
	float pcpitch;	
	float setpitch;
	uint8_t finish_flag;
};

void trajectory_equation(void);

#endif //__GIMBAL__











