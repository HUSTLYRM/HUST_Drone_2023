#ifndef __LED_H
#define __LED_H

#include "config.h"

#define LED1_TOGGLE 	GPIO_ToggleBits(GPIOC,GPIO_Pin_14);
#define LED1_OFF      GPIO_SetBits(GPIOC,GPIO_Pin_14);
#define LED1_ON 			GPIO_ResetBits(GPIOC,GPIO_Pin_14);

#define LED2_TOGGLE   GPIO_ToggleBits(GPIOC,GPIO_Pin_13);
#define LED2_OFF      GPIO_SetBits(GPIOC,GPIO_Pin_13);
#define LED2_ON 			GPIO_ResetBits(GPIOC,GPIO_Pin_13);

void vLedGpioConfig(void);

void LED_OFF(void);
void LED_RED(void);
void LED_BLUE(void);
void LED_PURPLE(void);

void led_normal(void);
void led_error(void);
void led_selfCheck(void);
void led_IMU_Lost(void);
void led_YAW_Lost(void);
void led_Pitch_Lost(void);
void led_RC_Lost(void);
void led_Pluck_Lost(void);
void led_Gimbal_powerOff(void);
void led_AimAssist(void);

#endif


/* END */
