/* include */

#include "config.h"
/* declare */
extern struct state droneState;
extern struct GM6020 yaw;
extern struct GM6020 pitch;
extern struct IMU 	 imu;
extern struct trajectory_t trajectory;

extern RC_Ctl_t RC_Ctl;
extern PC_Recv_t PC_Recv;
extern short ContralMode;
extern float new_value; 
extern float old_value; 
extern short UI_Change_Mode;

uint32_t gimbalinitTime;
short LastContralMode=Lost;
int Grativity  = 0;
float set_pitch;
float set_yaw;
float set_yaw_Motor;
float set_pitch_Motor;

float INCAngle_Low,INCAngle_High;

int RC_IMU_control = 1;//默认陀螺仪反馈
int RC_Motor_control = 0;

int yaw_left_limit;
int yaw_right_limit;
int pitch_high_limit_imu; 
int pitch_low_limit_imu;
int pitch_high_limit_motor;
int pitch_low_limit_motor;

float yaw_RC_sensitivity 									=	0.0008f;	//遥控模式yaw灵敏度_imu反馈
float pitch_RC_sensitivity								=	0.0004f;	//遥控模式pitch灵敏度 imu反馈
float yaw_RC_sensitivity_motor 						=	0.016f;		//遥控模式yaw灵敏度_电机反馈
float pitch_RC_sensitivity_motor					=	0.008f;		//遥控模式pitch灵敏度 电机反馈
float yaw_PC_Mouse_sensitivity						=	0.03f;		//键鼠模式yaw鼠标灵敏度 imu反馈
float pitch_PC_Mouse_sensitivity 					= 0.04f;		//键鼠模式pitch鼠标灵敏度 imu反馈
float yaw_PC_Mouse_sensitivity_motor			=	0.03f;		//键鼠模式yaw鼠标灵敏度 电机反馈
float pitch_PC_Mouse_sensitivity_motor 		= 0.04f;		//键鼠模式pitch鼠标灵敏度 电机反馈
float yaw_PC_Keyboard_sensitivity 				= 0.80f;		//键鼠模式yaw键盘灵敏度 imu反馈
float pitch_PC_Keyboard_sensitivity 			= 0.25f;		//键鼠模式pitch键盘灵敏度 imu反馈
float yaw_PC_Keyboard_sensitivity_motor 	= 0.10f;		//键鼠模式yaw键盘灵敏度 电机反馈
float pitch_PC_Keyboard_sensitivity_motor = 0.15f;		//键鼠模式pitch键盘灵敏度 电机反馈

//static void limit_max_min(float *value,float max,float min);
static void imugimbal_config(void);
static void Plane_ID_Init(void );
int a,b;
/*
 *    			S2							    					S1    
 *  1 --> 允许云台和射击(电机)    1 --> 键鼠模式	
 * 	                             
 *  3 --> 允许云台和射击(IMU)     3 --> 辅瞄模式	
 *	                     								
 *  2 --> 掉电模式     						2 --> 遥控模式
 *					
 * (开关辅瞄: S1 turn from rc mode to key mode and turn back in 1.5 seconds) 
 */
 //IMU左到右变大 YAW左到右变大
 //IMU上到下大到小,上正下负,PITCH上到下变小


/* IMU云台控制任务 */
void ImugimbalTask( void * pvParameters )
{
	/* IMU云台参数配置 */
	imugimbal_config();
	Plane_ID_Init();
	//vTaskDelay(5000);
	gimbalinitTime = xTaskGetTickCount();

	//云台上电摆到最左,是因为板子先上电,电机没有数据传回,默认值为0,添加自检任务,电机上电有数据后再建立云台任务
	if(xTaskGetTickCount()-gimbalinitTime<2000)
	{
		//电机角控制
		if(RC_Motor_control ==1)
		{
			set_yaw_Motor = yaw.receiveAngle;			
			set_pitch_Motor = pitch.receiveAngle;
			yaw.setPoint = set_yaw_Motor;
			pitch.setPoint = set_pitch_Motor;
		}
		//陀螺仪控制
		if(RC_IMU_control ==1) 
		{
			new_value = imu.pitchAngle;
			old_value = imu.pitchAngle;
			set_yaw = imu.yawAngle;
			//IMU_Control_Limit(3800,4700,&pitch,&set_pitch,imu.pitchAngle);
			set_pitch = imu.pitchAngle;
			IMU_Control_Limit(yaw_left_limit,yaw_right_limit,&yaw,&set_yaw,imu.yawAngle);//电机角控制陀螺仪限位
			yaw.setPoint = set_yaw;
			pitch.setPoint = set_pitch;
		}
	}
	for(;;)
	{
		yaw.zero.Circle=0;
		pitch.zero.Circle=0;
		/* gimbal running */		
		//if(ContralMode!=Lost)
		if((droneState.drone.droneState&0x55) == 0 && ContralMode != Lost)
		{
			//键鼠模式
			PC_gimbal();
			//辅瞄模式
			Auto_gimbal();
			//遥控模式
			RC_gimbal();
		}
		else
		{	//掉电模式
			Lost_gimbal();
		}
		taskENTER_CRITICAL();
		GM6020_control();
   	taskEXIT_CRITICAL();
		/* send data to TX2 */
		PCSend();
		vTaskDelay(10);
	}
}

/* limit */
void limit_max_min(float *value,float max,float min)//限位函数
{
	if(*value >max)
	{
		*value = max;
	}
	else if(*value<min)
	{
		*value = min;
	}
}

void Auto_gimbal(void)
{
	if(ContralMode == Auto)//辅瞄模式
	{
			set_yaw_Motor = yaw.receiveAngle;			
			set_pitch_Motor = pitch.receiveAngle;
		/**************不开辅瞄接收值为0,将setpoint设为imu当前值   */
		if(PC_Recv.PCTargetYaw!=0)
		{
			set_yaw = PC_Recv.PCTargetYaw;
			IMU_Control_Limit(yaw_left_limit,yaw_right_limit,&yaw,&set_yaw,imu.yawAngle);//电机角控制陀螺仪限位

		}
		else 
		{
			set_yaw = imu.yawAngle;
			IMU_Control_Limit(yaw_left_limit,yaw_right_limit,&yaw,&set_yaw,imu.yawAngle);//电机角控制陀螺仪限位
		}
		
		if(PC_Recv.PCTargetPitch!=0)
		{
			set_pitch = PC_Recv.PCTargetPitch;
			limit_max_min(&set_pitch,pitch_high_limit_imu,pitch_low_limit_imu); 
		}
		else
		{
			set_pitch = imu.pitchAngle;
			limit_max_min(&set_pitch,pitch_high_limit_imu,pitch_low_limit_imu); 

		}
		yaw.setPoint = (float)(set_yaw);	
		pitch.setPoint = (float)(set_pitch);
	}
}
void PC_gimbal(void)
{
	if(ContralMode == Pc)//键鼠模式
	{
		if(RC_IMU_control == 1)//IMU反馈控制
		{
			set_yaw_Motor = yaw.receiveAngle;			
			set_pitch_Motor = pitch.receiveAngle;
			
			set_yaw += (((float)RC_Ctl.rc.ch0 - 1024) * yaw_RC_sensitivity);
			set_pitch +=  ((float)RC_Ctl.rc.ch1 - 1024) * pitch_RC_sensitivity;		
			
			set_pitch -= (float)RC_Ctl.mouse.y * pitch_PC_Mouse_sensitivity;
			set_yaw += (float)RC_Ctl.mouse.x * yaw_PC_Mouse_sensitivity;
				
			/* keyboard for fine tuning */
			if(UI_Change_Mode == 0)
			{
				set_pitch = set_pitch + ((float)RC_Ctl.key.w - (float)RC_Ctl.key.s) * pitch_PC_Keyboard_sensitivity;
				set_yaw = set_yaw + ((float)RC_Ctl.key.d - (float)RC_Ctl.key.a)*yaw_PC_Keyboard_sensitivity;
			}
			
			IMU_Control_Limit(yaw_left_limit,yaw_right_limit,&yaw,&set_yaw,imu.yawAngle);//电机角控制陀螺仪限位
			yaw.setPoint = set_yaw;
			yaw.ffout = FeedForward_Cal(&yaw.AngelFF , imu.yawAngle);//yaw前馈
			limit_max_min(&set_pitch,pitch_high_limit_imu,pitch_low_limit_imu);//陀螺仪角度
			//IMU_Control_Limit(3800,4600,&pitch,&set_pitch,imu.pitchAngle);
			pitch.setPoint = set_pitch;
			pitch.ffout = FeedForward_Cal(&pitch.AngelFF ,imu.pitchAngle);//pitch前馈
			
//		yaw.ffout = FeedForward_Cal(&yaw.AngelFF , yaw.Angle_zeroCheck);//前馈
//		limit_max_min(&set_pitch,4700,3800);//电机角
//		pitch.setPoint = (int)set_pitch;
//		pitch.ffout = FeedForward_Cal(&pitch.AngelFF ,pitch.Angle_zeroCheck);//前馈	
		}
		if(RC_Motor_control == 1)	//电机反馈控制
		{
			set_yaw = imu.yawAngle;
			//IMU_Control_Limit(3800,4700,&pitch,&set_pitch,imu.pitchAngle);
			set_pitch = imu.pitchAngle;
			IMU_Control_Limit(yaw_left_limit,yaw_right_limit,&yaw,&set_yaw,imu.yawAngle);//电机角控制陀螺仪限位
				
			set_yaw_Motor += (((float)RC_Ctl.rc.ch0 - 1024) * yaw_RC_sensitivity_motor);
			set_pitch_Motor -=  ((float)RC_Ctl.rc.ch1 - 1024) * pitch_RC_sensitivity_motor;		
			
			set_yaw_Motor += (float)RC_Ctl.mouse.x * yaw_PC_Mouse_sensitivity_motor;
			set_pitch_Motor -=  (float)RC_Ctl.mouse.y * pitch_PC_Mouse_sensitivity_motor;		
			
			if(UI_Change_Mode == 0)
			{
				set_pitch_Motor = set_pitch_Motor + ((float)RC_Ctl.key.w - (float)RC_Ctl.key.s) * pitch_PC_Keyboard_sensitivity_motor;
				set_yaw_Motor = set_yaw_Motor + ((float)RC_Ctl.key.d - (float)RC_Ctl.key.a) * yaw_PC_Keyboard_sensitivity_motor;
			}
			
			limit_max_min(&set_yaw_Motor,yaw_right_limit,yaw_left_limit);//电机角
			yaw.setPoint = set_yaw_Motor;
			yaw.ffout = FeedForward_Cal(&yaw.AngelFF , yaw.Angle_zeroCheck);//前馈
			
			limit_max_min(&set_pitch_Motor,pitch_high_limit_motor,pitch_low_limit_motor);//电机角
			pitch.setPoint = set_pitch_Motor;
			pitch.ffout = FeedForward_Cal(&pitch.AngelFF ,pitch.Angle_zeroCheck);//前馈				
		}
	}	
}
void RC_gimbal(void)
{
	if(ContralMode ==Rc)//遥控模式
	{
		if(RC_IMU_control == 1)//IMU反馈控制
		{
			set_yaw_Motor = yaw.receiveAngle;			
			set_pitch_Motor = pitch.receiveAngle;
			if(((RC_Ctl.rc.ch0-1024<10)&&(RC_Ctl.rc.ch0-1024>-10))) 
			{
				RC_Ctl.rc.ch0=1024;
			}
			if(((RC_Ctl.rc.ch1-1024<10)&&(RC_Ctl.rc.ch1-1024>-10)))
			{	
				RC_Ctl.rc.ch1=1024;
			}
			set_yaw += (((float)RC_Ctl.rc.ch0 - 1024) * yaw_RC_sensitivity);//0.008f
			set_pitch +=  ((float)RC_Ctl.rc.ch1 - 1024) * pitch_RC_sensitivity;		//0.015f
			
			IMU_Control_Limit(yaw_left_limit,yaw_right_limit,&yaw,&set_yaw,imu.yawAngle);//电机角控制陀螺仪限位
			yaw.setPoint = set_yaw;
			
			limit_max_min(&set_pitch,pitch_high_limit_imu,pitch_low_limit_imu);//陀螺仪角度
			//IMU_Control_Limit(3800,4600,&pitch,&set_pitch,imu.pitchAngle);
			pitch.setPoint = set_pitch;
			pitch.ffout = FeedForward_Cal(&pitch.AngelFF ,imu.pitchAngle);//前馈
		}
			
		if(RC_Motor_control == 1)	//电机反馈控制
		{
			set_yaw = imu.yawAngle;
			//IMU_Control_Limit(3800,4700,&pitch,&set_pitch,imu.pitchAngle);
			set_pitch = imu.pitchAngle;
			IMU_Control_Limit(yaw_left_limit,yaw_right_limit,&yaw,&set_yaw,imu.yawAngle);//电机角控制陀螺仪限位
			//电机角
			if(ContralMode==Lost||((RC_Ctl.rc.ch0-1024<10)&&(RC_Ctl.rc.ch0-1024>-10))) 
			{
				RC_Ctl.rc.ch0=1024;
			}
			if(ContralMode==Lost||((RC_Ctl.rc.ch1-1024<10)&&(RC_Ctl.rc.ch1-1024>-10)))
			{	
				RC_Ctl.rc.ch1=1024;
			}
				
			set_yaw_Motor += (((float)RC_Ctl.rc.ch0 - 1024) * yaw_RC_sensitivity_motor);//0.008f
			set_pitch_Motor -=  ((float)RC_Ctl.rc.ch1 - 1024) * pitch_RC_sensitivity_motor;		//0.015f

			limit_max_min(&set_yaw_Motor,yaw_right_limit,yaw_left_limit);//电机角
			yaw.setPoint = set_yaw_Motor;
			yaw.ffout = FeedForward_Cal(&yaw.AngelFF , yaw.Angle_zeroCheck);//前馈
			
			limit_max_min(&set_pitch_Motor,pitch_high_limit_motor,pitch_low_limit_motor);//电机角
			pitch.setPoint = set_pitch_Motor;
			pitch.ffout = FeedForward_Cal(&pitch.AngelFF ,pitch.Angle_zeroCheck);//前馈			
		}
	}
}

void Lost_gimbal(void)//掉电模式
{
	//if(ContralMode==Lost)
	//if((droneState.drone.droneState&0x55) != 0 ||ContralMode == Lost)
	{
		if(RC_Motor_control == 1)//电机角反馈控制_掉电
		{
			set_yaw_Motor = yaw.Angle_zeroCheck;
			set_pitch_Motor = pitch.Angle_zeroCheck;
			yaw.setPoint = set_yaw_Motor;
			pitch.setPoint = set_pitch_Motor;
			RC_Ctl.rc.ch0=1024;
			RC_Ctl.rc.ch1=1024;
			RC_Ctl.rc.ch2=1024;
			RC_Ctl.rc.ch3=1024;
		}
		if(RC_IMU_control == 1)//陀螺仪反馈控制_掉电
		{
			new_value = imu.pitchAngle;
			old_value = imu.pitchAngle;
			
			set_pitch = imu.pitchAngle;
			//IMU_Control_Limit(3800,4600,&pitch,&set_pitch,imu.pitchAngle);
			set_yaw = imu.yawAngle;
			IMU_Control_Limit(yaw_left_limit,yaw_right_limit,&yaw,&set_yaw,imu.yawAngle);//电机角控制陀螺仪限位
			yaw.setPoint = set_yaw;
			pitch.setPoint = set_pitch;
			RC_Ctl.rc.ch0=1024;
			RC_Ctl.rc.ch1=1024;
			RC_Ctl.rc.ch2=1024;
			RC_Ctl.rc.ch3=1024;
		}
	}
}

void IMU_Control_Limit(float min,float max, struct GM6020 *Motor, float *set, float imuAngle)
{
	INCAngle_Low =(min - Motor->Angle_zeroCheck)/8192*360;
	
	INCAngle_High = (max - Motor->Angle_zeroCheck)/8192*360;
	limit_max_min(set, (imuAngle + INCAngle_High) , (imuAngle + INCAngle_Low) );
	a = (imuAngle + INCAngle_High);
	b = (imuAngle + INCAngle_Low);
}

/* imu gimbal config */
static void imugimbal_config(void)			//imu云台初始化函数
{
#if Plane_ID == 1
	FeedForwardInit();
	/* yaw轴初始化 */
	yaw.id = 0;
	yaw.receiveId = 0x207;
	ZeroCheck_Init_Interface(&yaw.zero,8191,yaw.receiveAngle);
	yaw.mode = imuYaw;	
	//								PID结构体	 mode 输出限幅	积分限幅	 P				 I			D		
	//普通PID---RC	
	PID_struct_init(&yaw.speedPID,		1,	30000.0f,	100.0f,	45.0f,0.5f, -0.0f);	//-1700.0f,-0.00f, -2500.0f   75.0f 0 0
	PID_struct_init(&yaw.positionPID,	1,	30000.0f,	100.0f,	2.5f,		0.0f,	 2.0f	 );	//1.405f,		0.0051f,	 .9f		2.5F 0.005 1
	//纯电机控制---RC
	PID_struct_init(&yaw.Motor_SpeedPID ,		1,	30000.0f,	100.0f,	-350.0f, -0.01f, -0.0f);	
	PID_struct_init(&yaw.Motor_PositionPID,	1,	30000.0f,	100.0f,	0.5f,		0.001f,	 0.0f	 );	
	
	//电机控制  speed 1,	30000.0f,	100.0f,	45.0f, 0.0001f, -0.0f
	//   				position 1,	30000.0f,	100.0f,	1.5f,		0.0005f,	 1.0f	 
	//模糊PID---RC
	Fuzzy_PID_struct_init(&yaw.speedPID_fuzzy, 30000.0f, 100.0f,15.0f,0.3f,-0.3f,0.0f,0.0f,0.0f,0.4f,0.2f,0.0f,0.5f,5.0f,0.5f,0.15f,0.1f);
	Fuzzy_PID_struct_init(&yaw.positionPID_fuzzy, 30000.0f, 100.0f,8.0f,0.0f,-0.0f,0.0f,0.0f,0.0f,0.4f,0.2f,0.0f,0.5f,3.0f,0.5f,0.05f,0.1f);
	//辅瞄PID---Auto
	PID_struct_init(&yaw.AutospeedPID,		1,	30000.0f,	35.0f,	-5200.0f,	-10.0f, -600.0f);  //同Rc
	//PID_struct_init(&yaw.AutopositionPID,	1,	30000.0f,	35.0f,	 0.35f,		0.02f,	 0.0f);	//0.2 0.004 0.3
	PID_struct_init(&yaw.AutopositionPID,	1,	30000.0f,	35.0f,	 0.35f,		0.02f,	 0.f);	//0.2 0.004 0.3
	
	/* pitch轴初始化 */
	pitch.id = 0;
	pitch.receiveId = 0x206;
	ZeroCheck_Init_Interface(&pitch.zero,8191,pitch.receiveAngle);
	pitch.mode = imuPitch;	
	
	//									PID结构体	  mode 输出限幅	 积分限幅	 P			I			  D		
	//普通PID---RC	
	PID_struct_init(&pitch.speedPID,		1,	30000.0f,	100.0f,	700.0f,	 -20.0f, -0.0f);//6000f,	0.00f, 8000f   -46 0.001
	PID_struct_init(&pitch.positionPID,	1,	30000.0f,	200.0f,	0.2f,		0.0f,	0.0f	 );//0.15f,		0.00f,	0.15f	   0.1   0.0001
	//纯电机控制---RC
	PID_struct_init(&pitch.Motor_SpeedPID,		1,	30000.0f,	100.0f,	-35.0f,	 0.0f, 0.0f); //-50 0 0
	PID_struct_init(&pitch.Motor_PositionPID,	1,	30000.0f,	200.0f,	1.0f,		0.01f,	1.5f	 );//1.5 0.01 1.5
	//模糊PID---RC
	Fuzzy_PID_struct_init(&pitch.speedPID_fuzzy, 30000.0f, 100.0f,15.0f,0.2f,-0.3f,0.0f,0.0f,0.0f,0.4f,0.2f,0.0f,0.5f,5.0f,0.5f,0.15f,0.1f);
	Fuzzy_PID_struct_init(&pitch.positionPID_fuzzy, 30000.0f, 100.0f,8.0f,0.0f,-0.0f,0.0f,0.0f,0.0f,0.4f,0.2f,0.0f,0.5f,3.0f,0.5f,0.05f,0.1f);
	//辅瞄PID---Auto
	PID_struct_init(&pitch.AutospeedPID,		1,	30000.0f,	20000.0f,	4000.0f,	3.0f, 4500.0f); //同Rc
	PID_struct_init(&pitch.AutopositionPID,	1,	30000.0f,	100.0f,	 0.25f, 0.005f,	 0.1f	 );	//0.4 0.32
#elif Plane_ID ==2
	FeedForwardInit();
	/* yaw轴初始化 */
	yaw.id = 0;
	yaw.receiveId = 0x207;
	ZeroCheck_Init_Interface(&yaw.zero,8191,yaw.receiveAngle);
	yaw.mode = imuYaw;	
	//								PID结构体	 mode 输出限幅	积分限幅	 P				 I			D		
	//普通PID---RC	
	PID_struct_init(&yaw.speedPID,		1,	30000.0f,	100.0f,	45.0f,0.5f, -0.0f);	//-1700.0f,-0.00f, -2500.0f   75.0f 0 0
	PID_struct_init(&yaw.positionPID,	1,	30000.0f,	100.0f,	2.5f,		0.0f,	 2.0f	 );	//1.405f,		0.0051f,	 .9f		2.5F 0.005 1
	//纯电机控制---RC
	PID_struct_init(&yaw.Motor_SpeedPID ,		1,	30000.0f,	100.0f,	-350.0f, -0.01f, -0.0f);	
	PID_struct_init(&yaw.Motor_PositionPID,	1,	30000.0f,	100.0f,	0.5f,		0.0001f,	 0.0f	 );	
	
	//电机控制  speed 1,	30000.0f,	100.0f,	45.0f, 0.0001f, -0.0f
	//   				position 1,	30000.0f,	100.0f,	1.5f,		0.0005f,	 1.0f	 
	//模糊PID---RC
	Fuzzy_PID_struct_init(&yaw.speedPID_fuzzy, 30000.0f, 100.0f,15.0f,0.3f,-0.3f,0.0f,0.0f,0.0f,0.4f,0.2f,0.0f,0.5f,5.0f,0.5f,0.15f,0.1f);
	Fuzzy_PID_struct_init(&yaw.positionPID_fuzzy, 30000.0f, 100.0f,8.0f,0.0f,-0.0f,0.0f,0.0f,0.0f,0.4f,0.2f,0.0f,0.5f,3.0f,0.5f,0.05f,0.1f);
	//辅瞄PID---Auto
	PID_struct_init(&yaw.AutospeedPID,		1,	30000.0f,	35.0f,	-4500.0f,	-10.0f, -600.0f);  //同Rc
	PID_struct_init(&yaw.AutopositionPID,	1,	30000.0f,	35.0f,	 0.35f,		0.02f,	 0.0f);	//0.2 0.004 0.3
	
	/* pitch轴初始化 */
	pitch.id = 0;
	pitch.receiveId = 0x206;
	ZeroCheck_Init_Interface(&pitch.zero,8191,pitch.receiveAngle);
	pitch.mode = imuPitch;	
	
	//									PID结构体	  mode 输出限幅	 积分限幅	 P			I			  D		
	//普通PID---RC	
	PID_struct_init(&pitch.speedPID,		1,	30000.0f,	100.0f,	700.0f,	 -20.0f, -0.0f);//6000f,	0.00f, 8000f   -46 0.001
	PID_struct_init(&pitch.positionPID,	1,	30000.0f,	200.0f,	0.2f,		0.0f,	0.0f	 );//0.15f,		0.00f,	0.15f	   0.1   0.0001
	//纯电机控制---RC
	PID_struct_init(&pitch.Motor_SpeedPID,		1,	30000.0f,	100.0f,	-50.0f,	 0.0f, 0.0f); //-50 0 0
	PID_struct_init(&pitch.Motor_PositionPID,	1,	30000.0f,	200.0f,	2.0f,		0.001f,	1.5f	 );//1.5 0.01 1.5
	//模糊PID---RC
	Fuzzy_PID_struct_init(&pitch.speedPID_fuzzy, 30000.0f, 100.0f,15.0f,0.2f,-0.3f,0.0f,0.0f,0.0f,0.4f,0.2f,0.0f,0.5f,5.0f,0.5f,0.15f,0.1f);
	Fuzzy_PID_struct_init(&pitch.positionPID_fuzzy, 30000.0f, 100.0f,8.0f,0.0f,-0.0f,0.0f,0.0f,0.0f,0.4f,0.2f,0.0f,0.5f,3.0f,0.5f,0.05f,0.1f);
	//辅瞄PID---Auto
	PID_struct_init(&pitch.AutospeedPID,		1,	30000.0f,	20000.0f,	4100.0f,	3.0f, 4000.0f); //同Rc
	PID_struct_init(&pitch.AutopositionPID,	1,	30000.0f,	100.0f,	 0.25f, 0.015f,	 0.0f	 );	//0.4 0.32
#endif
}

static void Plane_ID_Init(void )
{
#if Plane_ID == 1
//*************************************************		
		yaw_left_limit  = 1695;//900
		yaw_right_limit =  4000;//6700
    pitch_high_limit_imu  = 10; 
		pitch_low_limit_imu = -30;//-40
		pitch_high_limit_motor = 4000;
		pitch_low_limit_motor = 150;
	
#elif Plane_ID == 2
//*************************************************	
		yaw_left_limit  = 5200;
		yaw_right_limit =  8100;
    pitch_high_limit_imu  = 10; 
		pitch_low_limit_imu = -30;4
		pitch_high_limit_motor = 7500;
		pitch_low_limit_motor = 6500;
	
#endif

}
/* end */

