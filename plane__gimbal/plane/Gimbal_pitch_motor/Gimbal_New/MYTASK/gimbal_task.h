#ifndef __GIMBAL__
#define __GIMBAL__


#define yaw_Min  -80.0f	
#define yaw_Max  60.0f	

#define pitch_Min  -28.0f
#define pitch_Max  40.0f

/* 过零检测之后的值 */
#define YawMotorCenter 2102
//#define YawMotorLeft   -1400  
//#define YawMotorRight  1752 

#define PitchMotorCenter 4046
//#define PitchMotorUp   200  
//#define PitchMotorDown  -400 

#define Lost 0
#define Rc   1
#define Pc   2
#define Auto 3

void ImugimbalTask( void * pvParameters );
void ChangeMode(void);
#endif //__GIMBAL__












