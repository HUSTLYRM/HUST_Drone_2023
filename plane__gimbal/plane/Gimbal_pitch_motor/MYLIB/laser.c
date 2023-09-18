/* include */
#include "config.h"

/* Laser GPIO config */
/* this block laser gpio always burn, so now laser derict connect to vcc and gnd */
void vLaserGpioConfig( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE ); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       
	GPIO_Init( GPIOC, &GPIO_InitStructure );  
	
	GPIO_SetBits(GPIOC,GPIO_Pin_1);
}


/* end */



















