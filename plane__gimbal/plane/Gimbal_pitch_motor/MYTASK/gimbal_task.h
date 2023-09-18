#ifndef __GIMBAL__
#define __GIMBAL__

#include "config.h"

#define yaw_Min  -90.0f	
#define yaw_Max  90.0f	

#define pitch_Min  -30.0f
#define pitch_Max  3.0f

/* 过零检测之后的值 */
#define YawMotorCenter 5150
//#define YawMotorLeft   -1400  
//#define YawMotorRight  1752 

#define PitchMotorCenter 4200
#define PitchMotorUp     8000  
#define PitchMotorDown   100 

#define Lost 0
#define Rc   1
#define Pc   2
#define Auto 3


void ImugimbalTask( void * pvParameters );
void ChangeMode(void);
void Auto_gimbal(void);
void PC_gimbal(void);
void RC_gimbal(void);
void Lost_gimbal(void);
void IMU_Control_Limit(float min,float max, struct GM6020 *Motor, float *set ,float imuAngle);
void limit_max_min(float *value,float max,float min);
#endif //__GIMBAL__












