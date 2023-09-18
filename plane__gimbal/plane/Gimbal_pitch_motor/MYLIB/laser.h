#ifndef __LASER_H__
#define __LASER_H__

void vLaserGpioConfig(void);

#define LASER_OFF 	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
#define LASER_ON    GPIO_SetBits(GPIOC,GPIO_Pin_1);

#endif 


/* end */














