/* include */
#include "config.h"

/* define */
#define IMU_GIMBAL_TASK_PRIO		5
#define IMU_GIMBAL_TASK_SIZE		128
TaskHandle_t ImugimbalTask_Handler;

#define SHOOT_TASK_PRIO		5
#define SHOOT_TASK_SIZE		128
TaskHandle_t ShootTask_Handler;

struct state droneState;

/* declare */
extern RC_Ctl_t RC_Ctl; 
extern PC_Recv_t PC_Recv;
extern struct GM6020 yaw;
extern struct GM6020 pitch;
extern struct M2006  pluck;
extern struct IMU 	 imu;

static void selfCheck_update(void);
static void led_update(void);
static void control(void );
static uint8_t dis_control(void);
static uint16_t checkComponent(uint32_t now,uint32_t last,uint32_t delay);
static void device_lost_reset(void);


/* 初始化时首先开机自检 然后300ms检查一次信号更新(运行自检) */
void SelfcheckTask( void * pvParameters )
{
	static uint32_t initTime;
	static short dis_control_flag = 1;
	initTime = xTaskGetTickCount();
	
	/* 红蓝LED常亮提示自检进行中 */
	led_selfCheck();
	IWDG_Config(64,1300);
	/* 自检300ms */
	while(xTaskGetTickCount()-initTime<800)
	{
		/* 更新自检数据 */
		selfCheck_update();
		IWDG_Feed();
		vTaskDelay(1);
	}
	
	/* 开机自检正常 则复位历史掉线记录 */
	if(droneState.drone.droneState == 0)
		device_lost_reset();
	
	/* 所有设备在线 或 系统配置放弃开机自检 */
	if(droneState.drone.droneState == 0||SELFCHECK == 0)
	{
		/* 正常控制 创建云台和射击任务 */
		control();
		
		dis_control_flag = 0;
	}

	/* 运行自检任务主体 */
	for(;;)
	{
		/* 运行中自检频率100ms */
		selfCheck_update();
		
		/* LED状态更新 */
		led_update();
		
		/* 飞行中自检正常后恢复云台控制 */
		/* (所有设备在线 或 上场状态) 且 当前未获得控制权 */ 
		if((droneState.drone.droneState == 0)&&(dis_control_flag == 1))
		{
			/* 当前自检正常 恢复正常控制 */		
			control();
			
			dis_control_flag = 0;
		}
		
		/* 飞行中设备掉线放弃云台控制 */
		/* 正常控制中有设备掉线(飞行状态) */
		else if (droneState.drone.droneState != 0 
					&& dis_control_flag == 0
					&& SELFCHECK == 1)
		{
			if(dis_control())
				dis_control_flag = 1;
		}
		IWDG_Feed();
		vTaskDelay(100);
	}
}

/* 创建正常控制任务 */
static void control(void)
{
	/* IMU自检通过 */
	if(droneState.drone.composeState.IMU_disconneced == 0||SELFCHECK == 0)
	{
		/* 创建IMU云台任务 */
		xTaskCreate((TaskFunction_t)ImugimbalTask,      	
								(const char *	)"Imu cotrol gimbal",     
								(uint16_t			)IMU_GIMBAL_TASK_SIZE,    
								(void *				)NULL,              
								(UBaseType_t		)IMU_GIMBAL_TASK_PRIO,    
								(TaskHandle_t *)&ImugimbalTask_Handler);
	}
	
	xTaskCreate((TaskFunction_t)ShootTask,      	
							(const char *	)"shoot",     
							(uint16_t			)SHOOT_TASK_SIZE,    
							(void *				)NULL,              
							(UBaseType_t		)SHOOT_TASK_PRIO,    
							(TaskHandle_t *)&ShootTask_Handler);
}

short temp_disconnetTimes = 0; 
/* 放弃控制权 */
static uint8_t dis_control(void)
{
	/* 发射机构掉电前处理 */
	if(shoot_ProTerminate())
	{
		/* 放弃控制权 */
		temp_disconnetTimes++;
		if(droneState.drone.composeState.IMU_disconneced == 0)
			vTaskDelete(ImugimbalTask);
		
		vTaskDelete(ShootTask);
		return 1;
	}
	else 
		return 0;
}

/* LED状态更新 */
static void led_update(void)
{
	switch (droneState.drone.droneState)
	{
		/* 无掉线 */
		case 0x0000:
		{
			/* 正常状态: 蓝灯慢闪 */
			if((droneState.Gimbal_poweroff == 0)&&(droneState.AimAssist == 0))
				led_normal();
			/* 辅瞄状态: 蓝灯双闪 */
			else if((droneState.Gimbal_poweroff == 0)&&(droneState.AimAssist == 1))
				led_AimAssist();
			/* 云台掉电: 蓝灯常亮 */
			else if(droneState.Gimbal_poweroff == 1)
				led_Gimbal_powerOff();
			break;
		}
		/* IMU掉线 */
		case 0x0001:led_IMU_Lost();
			break;
		/* 遥控信号丢失 */
		case 0x0004:led_RC_Lost(); 
			break;
		/* YAW轴电机掉线 */
		case 0x0010:led_YAW_Lost();
			break;
		/* Pitch轴电机掉线 */
		case 0x0040:led_Pitch_Lost();
			break;
		/* 拨弹电机掉线 */
		case 0x0100:led_Pluck_Lost();
			break;
		/* 严重错误: 红灯常亮*/
		default :led_error();
			break;
	}
}

/* 自检数据更新 */
static void selfCheck_update(void)
{
	/* 更新时间 */
	uint32_t nowTime = xTaskGetTickCount();
	/* 更新IMU状态 */
	droneState.drone.composeState.IMU_disconneced = checkComponent(nowTime,imu.updateTime,100);
	/* 更新YAW轴电机状态 */
	droneState.drone.composeState.yaw_disconneced = checkComponent(nowTime,yaw.updateTime,100);
	/* 更新PITCH轴电机状态 */
	droneState.drone.composeState.pitch_disconneced = checkComponent(nowTime,pitch.updateTime,100);
	/* 更新RC状态 */
	droneState.drone.composeState.RC_disconneced = checkComponent(nowTime,RC_Ctl.updateTime,1000);
	/* 更新拨弹电机状态 */
	droneState.drone.composeState.Pluck_disconneced = checkComponent(nowTime,pluck.updateTime,100);
	/* 更新云台上电状态 */
	if(RC_Ctl.rc.s1 == 2)
		droneState.Gimbal_poweroff = 1;
	else
		droneState.Gimbal_poweroff = 0;
	/* 更新辅瞄开启状态 */
	
	/* 更新PC数据接收状态 */
	droneState.PC_Received = checkComponent(nowTime,PC_Recv.updateTime,200);
	
	/* 历史掉线记录 */
	if(droneState.drone.composeState.IMU_disconneced == 1)
		imu.Ever_Lost = 1;
	if(droneState.drone.composeState.yaw_disconneced == 1)
		yaw.Ever_Lost = 1;
	if(droneState.drone.composeState.pitch_disconneced == 1)
		pitch.Ever_Lost = 1;
	if(droneState.drone.composeState.RC_disconneced == 1)
		RC_Ctl.Ever_Lost = 1;
	if(droneState.drone.composeState.Pluck_disconneced == 1)
		pluck.Ever_Lost = 1;
}

/* 模块自检 */
/* 自检超时 -> 返回1 */
/* 自检正常 -> 返回0 */
static uint16_t checkComponent(uint32_t now,uint32_t last,uint32_t delay)
{
	if((now-last)>delay)
	return 1;
	else 
	return 0;
}

static void device_lost_reset(void)
{
	imu.Ever_Lost = 1;
	yaw.Ever_Lost = 1;
	pitch.Ever_Lost = 1;
	RC_Ctl.Ever_Lost = 1;
	pluck.Ever_Lost = 1;
}

/* end */

