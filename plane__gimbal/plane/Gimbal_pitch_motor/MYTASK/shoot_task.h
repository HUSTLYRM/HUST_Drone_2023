#ifndef __SHOOT__
#define __SHOOT__

unsigned char shoot_ProTerminate(void);
void ShootTask( void * pvParameters );
void shoot_2006(void const * argument);
void pluck_check(void);
void shoot_mode_check(void);
float imu_filter(float imu_in);

#define single_shoot 1
#define cont_shoot 3
#define stop_shoot 2

#endif //__SHOOT__


/* end */
















