/* include */
#include "config.h"

/* define */
uint32_t FreeRTOSRunTimeTicks = 0;

/* Tim3 for runtime statistics (50us interrupt) */
void vTim3Config(void)
{
	TIM_TimeBaseInitTypeDef tim;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	tim.TIM_Prescaler = 84-1;//1MHz
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_Period = 50-1;//50us
	tim.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3,&tim);
		
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3 , TIM_IT_Update) != RESET)
	{
		FreeRTOSRunTimeTicks++;
	}
}
	
/* Tim4 for dshot (period) */
void vTim4Config(void)
{
	TIM_TimeBaseInitTypeDef tim;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	tim.TIM_Prescaler = 4-1;//21MHz
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_Period = 42-1;//2us
	tim.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM4,&tim);
		
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
}

extern RC_Ctl_t RC_Ctl;
extern uint32_t SRC_Buffer[];
extern short Shoot_Flag;
uint16_t accelerator1;
uint16_t accelerator2;//friction wheel throttle
uint8_t BIT[16];
int _i;
/* tim4 update interrupt for friction wheel */
void TIM4_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM4 , TIM_IT_Update) != RESET ) 
	{
		/* disable tim4 */
		TIM_Cmd(TIM4, DISABLE);
		TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);
		
		/* 0~2048: prevent the speed too fast */
		accelerator1=LIMIT_MAX_MIN(accelerator1,2000,0);
		accelerator2=LIMIT_MAX_MIN(accelerator2,2000,0);//0~2048  速度太快需限幅

		/* dshot */
		BIT[0]   = (accelerator1&0x400 ? 1 : 0);
		BIT[1]   = (accelerator1&0x200 ? 1 : 0);
		BIT[2]   = (accelerator1&0x100 ? 1 : 0);
		BIT[3]   = (accelerator1&0x80  ? 1 : 0);
		BIT[4]   = (accelerator1&0x40  ? 1 : 0);
		BIT[5]   = (accelerator1&0x20  ? 1 : 0);
		BIT[6]   = (accelerator1&0x10  ? 1 : 0);
		BIT[7]   = (accelerator1&0x8   ? 1 : 0);
		BIT[8]   = (accelerator1&0x4   ? 1 : 0);
		BIT[9]   = (accelerator1&0x2   ? 1 : 0);
		BIT[10]  = (accelerator1&0x1   ? 1 : 0);
		BIT[11] =0;
		
		/* getCRC */
		BIT[12] = BIT[0]^BIT[4]^BIT[8];
		BIT[13] = BIT[1]^BIT[5]^BIT[9];
		BIT[14] = BIT[2]^BIT[6]^BIT[10];
		BIT[15] = BIT[3]^BIT[7]^BIT[11];
		
		for(_i=0;_i<16;_i++)
		{
			SRC_Buffer[2*_i]=(BIT[_i]==1)?23:12;
//			SRC_Buffer[2*_i+1]=(BIT[_i]==1)?23:12;
		}
		
		BIT[0]   = (accelerator2&0x400 ? 1 : 0);
		BIT[1]   = (accelerator2&0x200 ? 1 : 0);
		BIT[2]   = (accelerator2&0x100 ? 1 : 0);
		BIT[3]   = (accelerator2&0x80  ? 1 : 0);
		BIT[4]   = (accelerator2&0x40  ? 1 : 0);
		BIT[5]   = (accelerator2&0x20  ? 1 : 0);
		BIT[6]   = (accelerator2&0x10  ? 1 : 0);
		BIT[7]   = (accelerator2&0x8   ? 1 : 0);
		BIT[8]   = (accelerator2&0x4   ? 1 : 0);
		BIT[9]   = (accelerator2&0x2   ? 1 : 0);
		BIT[10]  = (accelerator2&0x1   ? 1 : 0);
		BIT[11] =0;
		
		//getCRC
		BIT[12] = BIT[0]^BIT[4]^BIT[8];
		BIT[13] = BIT[1]^BIT[5]^BIT[9];
		BIT[14] = BIT[2]^BIT[6]^BIT[10];
		BIT[15] = BIT[3]^BIT[7]^BIT[11];
		
		for(_i=0;_i<16;_i++)
		{
			//SRC_Buffer[2*_i]=(BIT[_i]==1)?23:12;
			SRC_Buffer[2*_i+1]=(BIT[_i]==1)?23:12;
		}		

		DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1);
		TIM_DMACmd(TIM8, TIM_DMA_Update, ENABLE);
		DMA_ITConfig(DMA2_Stream1,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA2_Stream1, ENABLE);
		TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update);    
	}
}

/* end */













