/* Define to prevent recursive inclusion */
#ifndef __STM32F4_CONF_H
#define __STM32F4_CONF_H

/* define */
#define ABS(x) ((x)>0? (x):(-(x))) 
#define LIMIT_MAX_MIN(x, max, min)	(((x) <= (min)) ? (min):(((x) >= (max)) ? (max) : (x)))  

typedef union
{
	float fdata;
	unsigned long idata;
}FloatlongType;

/* MODE CHOOSE */
#define ANO_TEST			1	//ANO上位机测试模式
#define CPU_OCCUPY		1	//CPU占用统计模式
#define TRAJECTORY 		0	//弹道方程修正模式
#define SELFCHECK		 	1 //云台设备自检开启

/* Includes */
/* Standard Lib */
#include "math.h"
#include "stdio.h"
#include "stm32f4xx.h"	 
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_tim.h"

/* FreeRTOS */
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

/* MY LIB */
/* hardware */
#include "nvic.h"
#include "led.h"
#include "laser.h"
#include "uart.h"
#include "can.h"
#include "tim.h"
#include "motor.h"
#include "iwdg.h"

/* software */
#include "pid.h"
#include "crc.h"
#include "friction.h"
#include "zerocheck.h"
#include "communicate.h"
#include "trajectory.h"

/* MY DEVICE */
/* motor */
#include "GM6020.h"
#include "M2006.h"
#include "MX64.h"
/* imu */
#include "IMU.h"

/* MY TASK */
#include "start_task.h"
#include "selfcheck_task.h"
#include "gimbal_task.h"
#include "shoot_task.h"
#include "debug_task.h"
#include "judge_task.h"

#endif


/* END */
