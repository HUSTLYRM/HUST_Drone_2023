/* include */
#include "config.h"
/* define */
int pluckmotor_speed = 2500;	//single shoot mode pluck speed 750
int plunk_inc_speed;//连发拨盘转角  max 14000
short FrictionWheel_speed_stable1; //15000;// frictionwheel speed 
short FrictionWheel_speed_stable2;
uint16_t shoot_period = 1;	//单发射击延时,10ms单位 10
uint8_t pluck_stop = 0;//拨盘停转  ,0为正常转
int pluck_stop_position;
int shoot_flag = 0;
short quick_shoot_period=50; //快速连发延时(待测试)

short friction_speed_last[2]={0,0};
float K1=0.995;//低通滤波参数
float A = 0.5;//限幅参数
float new_value = 0; 
float old_value = 0; 
uint32_t shootinitTime;

int back_distance = 40000;
int back_decrease = 100000;

/* declare */
static void shoot_config(void);
extern struct M2006 pluck;
extern struct M2006  frimition[2];
extern RC_Ctl_t RC_Ctl;
extern struct state droneState;
extern short ContralMode;
extern int shoot_mode;

static void Plane_ID_Shoot_Config(void);
/*
 *    			 S2							        S1    
 *  1 --> 		无           1 --> 键鼠模式	
 * 	                             
 *  3 --> 允许射击         3 --> 辅瞄模式	
 *	                      								
 *  2 --> 掉电模式         2 --> 遥控模式
 *										
 */

void ShootTask( void * pvParameters )
{
	shoot_config();
	Plane_ID_Shoot_Config();
	
	shootinitTime = xTaskGetTickCount();
	//云台上电摆到最左,是因为板子先上电,电机没有数据传回,默认值为0,添加自检任务,电机上电有数据后再建立云台任务
	if(xTaskGetTickCount()-shootinitTime<2500)
	{
		pluck.setPoint = pluck.Angle_zeroCheck;
	}
	/* 掉电延时 */
	static short powerOff_delay = 0;
	RC_Ctl.rc.ch3=1024;
	for(;;)
	{
		/* 设备自检正常且状态不属于LOST掉电------拨盘 */
		//pluck_check();
		if(droneState.drone.composeState.Pluck_disconneced ==1)			
		{
			pluck.setPoint = pluck.Angle_zeroCheck;
			shoot_flag = 0;
		}
		if(RC_Ctl.rc.s2 == 3 || RC_Ctl.rc.s2 == 1)	//非掉电模式
		{
			//遥控模式*********************************************************************
			if(ContralMode==Rc||ContralMode==Auto)//遥控模式和辅瞄模式
			{
				static int RC_shoot_delay = 0;//计数器,单发射击间隔			
				if((RC_shoot_delay>shoot_period)&&(shoot_mode==single_shoot))//单发模式
				{
					RC_shoot_delay = 0;//计数器,单发射击间隔
					if(droneState.drone.composeState.Pluck_disconneced !=1)
					{
						pluck.setPoint += pluckmotor_speed;	
						shoot_flag = 1;
					}
				}
				else if(shoot_mode==cont_shoot)//连发模式
				{
					if(droneState.drone.composeState.Pluck_disconneced !=1)
					{
						pluck.setPoint += plunk_inc_speed;
						shoot_flag = 1;
					}
				}
				else if(shoot_mode==stop_shoot)//停止打弹
				{
					//pluck_stop=0;
					pluck.setPoint += 0;	
					shoot_flag = 0;
				}
				RC_shoot_delay ++;//单发模式下,当RC_shoot_delay>shoot_period时,打一发
				if((shoot_mode==cont_shoot)||(shoot_mode==single_shoot))
				{
						frimition[0].setPoint=-FrictionWheel_speed_stable1;
						frimition[1].setPoint=FrictionWheel_speed_stable2;
				}
				else 
				{
						frimition[0].setPoint=(K1*(friction_speed_last[0])+(1-K1)*0);//低通滤波减速,不然反电动势传回裁判系统会断电
						frimition[1].setPoint=(K1*(friction_speed_last[1])+(1-K1)*0);
				}
				/* control pluck motor */
				friction_speed_last[0]=frimition[0].setPoint;
				friction_speed_last[1]=frimition[1].setPoint;
			}
			
			//键鼠模式*********************************************************************
			if(ContralMode==Pc)
			{
				if(RC_Ctl.mouse.press_l==1)
				{
					if(pluck_stop==0)//连发会存在卡盘情况
					{
						if(droneState.drone.composeState.Pluck_disconneced !=1)
						{
							pluck.setPoint += plunk_inc_speed;//连发
							shoot_flag = 1;
						}
					}	
				}
				else 
				{
					pluck.setPoint += 0;	
					shoot_flag = 0;
				}
				/* friction wheel speed set */
				if((RC_Ctl.mouse.press_l==1)&&ContralMode==Pc)
				{
					frimition[0].setPoint=-FrictionWheel_speed_stable1;
					frimition[1].setPoint=FrictionWheel_speed_stable2;
				}
				else 
				{
					frimition[0].setPoint=(K1*(friction_speed_last[0])+(1-K1)*0);//低通滤波减速,不然反电动势传回裁判系统会断电
					frimition[1].setPoint=(K1*(friction_speed_last[1])+(1-K1)*0);
				}
			/* control pluck motor */
				friction_speed_last[0]=frimition[0].setPoint;
				friction_speed_last[1]=frimition[1].setPoint;			
			}
		}
		if(ContralMode == Lost )
		{
			pluck.setPoint = pluck.Angle_zeroCheck;	
			RC_Ctl.rc.ch0=1024;
			RC_Ctl.rc.ch1=1024;
			RC_Ctl.rc.ch2=1024;
			RC_Ctl.rc.ch3=1024;
			frimition[0].setPoint=(K1*(friction_speed_last[0])+(1-K1)*0);//低通滤波减速,不然反电动势传回裁判系统会断电
			frimition[1].setPoint=(K1*(friction_speed_last[1])+(1-K1)*0);
			friction_speed_last[0]=frimition[0].setPoint;
			friction_speed_last[1]=frimition[1].setPoint;
		}
		
		taskENTER_CRITICAL();
		M2006_control();//射击电机控制
		taskEXIT_CRITICAL();

		/* 发射机构掉电后重启 恢复正常PID参数 */
		if(powerOff_delay > 0)
		{
			PID_struct_init(&pluck.speedPID,1,15000,10000,10.0f,0.5f,0.0f);
			PID_struct_init(&pluck.positionPID,1,20000,20000,0.03f,0.0f,0.0f);
			
			powerOff_delay = 0;
		}

	vTaskDelay(10);	
	}
}

/* 发射机构掉电的前处理过程(延时) */
unsigned char shoot_ProTerminate(void)
{
	static short delay = 0;
	if(delay<20)
	{
		delay ++;
		return 0;
	}
	else 
	{
		delay = 0;
		return 1;
	}
}

/* 射击任务参数配置 */
static void shoot_config(void)
{
	//拨盘
	pluck.zero.CountCycle = 8191;  //pluck电机一圈为8192
	pluck.mode = doubleLoop;//模式为双环PID
	pluck.receiveId = 0x201;//PLUCK拨盘电机电调ID 201
	ZeroCheck_Init_Interface(&pluck.zero,8191,pluck.receiveAngle);
	pluck.setPoint = pluck.Angle_zeroCheck;//设定初始化位置角度值

	PID_struct_init(&pluck.speedPID,1,15000,10000,10.0f,0.5f,0.0f);//初始化速度PID
	PID_struct_init(&pluck.positionPID,1,20000,20000,0.03f,0.0f,0.0f);//初始化位置PID
	
	frimition[0].mode=speedLoop;//左摩擦轮 速度PID
	frimition[0].receiveId=0x202;//ID
	frimition[0].setPoint=0;//初始化值 = 0
	PID_struct_init(&frimition[0].speedPID,1,10000,700,10.0f,1.0f,2.0f);//摩擦轮1 速度PID初始化
	frimition[1].mode=speedLoop;//右摩擦轮 速度PID
	frimition[1].receiveId=0x203;//ID
	frimition[1].setPoint=0;//初始化值 = 0
	PID_struct_init(&frimition[1].speedPID,1,10000,700,10.0f,1.0f,2.0f);//摩擦轮2 速度PID初始化
}

void pluck_check(void)//几乎不用
{
	if(pluck_stop==0&&pluck.setPoint-pluck.Angle_zeroCheck>=back_distance)//电机落后5圈视为堵转
	{			
		pluck_stop_position=pluck.Angle_zeroCheck;
		pluck.setPoint=pluck.Angle_zeroCheck-back_decrease;
		if(shoot_mode==1)//连发卡盘
		{
			pluck_stop=1;
		}
	}			
	else	if(pluck_stop==1&&shoot_mode==cont_shoot&&pluck_stop_position-pluck.Angle_zeroCheck<=back_distance)
	{
	
		pluck_stop=0;//连发卡盘恢复
	}
}

float imu_filter(float imu_in)
{ 
	new_value = imu_in;  
	if ((new_value - old_value > A) || (old_value - new_value > A))  
	{
		return old_value;  
	}
	return new_value; 
}

static void Plane_ID_Shoot_Config(void)
{
#if Plane_ID == 1
	FrictionWheel_speed_stable1 = -7000;
	FrictionWheel_speed_stable2 = -7000;
	plunk_inc_speed = 10000;//拨盘转速
#elif Plane_ID == 2 
	FrictionWheel_speed_stable1 = -10000; //15000;// frictionwheel speed 
	FrictionWheel_speed_stable2 =-10000;
	plunk_inc_speed = 15000;//拨盘转速
#endif

}
/* end */
