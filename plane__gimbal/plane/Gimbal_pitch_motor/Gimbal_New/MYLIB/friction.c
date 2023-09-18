/* include */
#include "config.h"

/* define */
uint32_t SRC_Buffer[]={12,23,12,23,12,23,12,23,12,23,12,23,12,23,12,23,12,23,12,23,12,23,12,23,12,23,12,23,12,23,12,23};

/* FrictionWheel: PC7 PC8(TIM8 CH2 CH3) */
void vFrictionConfig(void)             
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
//	DMA_InitTypeDef DMA_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM8);

	TIM_TimeBaseInitStruct.TIM_Prescaler=168-1;//1Mhz
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=20000;//600k 1.67us  12 23
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseInitStruct);

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 1000;
	TIM_OC3Init(TIM8,&TIM_OCInitStruct);
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OC2Init(TIM8,&TIM_OCInitStruct);

	TIM_Cmd(TIM8,ENABLE);
	TIM_CtrlPWMOutputs(TIM8,ENABLE);
}

void DMA2_Stream1_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA2_Stream1,DMA_IT_TCIF1)==SET)
	{
		TIM_DMACmd(TIM8, TIM_DMA_Update, DISABLE);
		DMA_ITConfig(DMA2_Stream1,DMA_IT_TC,DISABLE);
		DMA_Cmd(DMA2_Stream1, DISABLE);
		TIM_SetCompare2(TIM8,0);		
		TIM_SetCompare3(TIM8,0);
		TIM_Cmd(TIM4, ENABLE);
		TIM_ClearFlag(TIM4, TIM_FLAG_Update);
		TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
		DMA_ClearITPendingBit(DMA2_Stream1,DMA_IT_TCIF1);
	}
}

extern uint16_t accelerator1;
extern uint16_t accelerator2;
/* set friction wheel speed */
void FrictionWheel_Set(short speed1,short speed2)
{
	speed1=LIMIT_MAX_MIN(speed1,2000,0);//snail
	speed2=LIMIT_MAX_MIN(speed2,2000,0);//snail
	accelerator1=speed1;
	accelerator2=speed2;
}


/* end */












