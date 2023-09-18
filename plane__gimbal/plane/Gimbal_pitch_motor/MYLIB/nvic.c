/* include */
#include "config.h"

/* NIVC config */
void vNvicConfig( void )
{
	NVIC_InitTypeDef  nvic;

	/* UART */
	
	/* usart1 for judgement control (receive) */
	nvic.NVIC_IRQChannel = USART1_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 2;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	/* uart1 for judgement (receive dma) */	
	nvic.NVIC_IRQChannel = DMA2_Stream2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 3;
	nvic.NVIC_IRQChannelSubPriority = 2;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	/* uart1 for judgement (send dma) */
	nvic.NVIC_IRQChannel = DMA2_Stream7_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 3;
	nvic.NVIC_IRQChannelSubPriority = 3;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

//	/* uart2 for ANO debug (send dma) */
//	nvic.NVIC_IRQChannel = DMA1_Stream6_IRQn;
//	nvic.NVIC_IRQChannelPreemptionPriority = 1;
//	nvic.NVIC_IRQChannelSubPriority = 1;
//	nvic.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&nvic);

	/* uart2 for APP (send dma) */
	nvic.NVIC_IRQChannel = DMA1_Stream6_IRQn;//初始化具体哪一个中断
	nvic.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级
	nvic.NVIC_IRQChannelSubPriority = 2;//响应优先级
	nvic.NVIC_IRQChannelCmd = ENABLE;//中断通道使能
	NVIC_Init(&nvic);
	
	/* uart2 for APP (receive dma) */
	nvic.NVIC_IRQChannel = DMA1_Stream5_IRQn;//初始化具体哪一个中断
	nvic.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级
	nvic.NVIC_IRQChannelSubPriority = 1;//响应优先级
	nvic.NVIC_IRQChannelCmd = ENABLE;//中断通道使能
	NVIC_Init(&nvic);
	
	/* uart3 for reomote control (receive) */
	nvic.NVIC_IRQChannel = USART3_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 2;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	/* uart4 for PC IDLE interrupt */
	nvic.NVIC_IRQChannel = UART4_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 2;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	/* uart4 for small PC (receive dma) */
	nvic.NVIC_IRQChannel = DMA1_Stream2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	/* uart4 for small PC (send dma) */
	nvic.NVIC_IRQChannel = DMA1_Stream4_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	/* TIM */
	
	/* tim3 update : runtime statistics */
	nvic.NVIC_IRQChannel = TIM3_IRQn;	  
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 0;	
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	/* tim8 dma : friction wheel dshoot produce */
	nvic.NVIC_IRQChannel = DMA2_Stream1_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 2;
	nvic.NVIC_IRQChannelSubPriority = 2;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	/* tim4 update : friction wheel send */
	nvic.NVIC_IRQChannel = TIM4_IRQn;	  
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 3;	
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	/* CAN */

	/* can1 : receive fifo 0 */
	nvic.NVIC_IRQChannel = CAN1_RX0_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 2;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	/* can1 : receive fifo 1 */
	nvic.NVIC_IRQChannel = CAN1_RX1_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	/* can1 : send */
	nvic.NVIC_IRQChannel = CAN1_TX_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 2;
	nvic.NVIC_IRQChannelSubPriority = 2;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	/* can2 : receive fifo 0 */
	nvic.NVIC_IRQChannel = CAN2_RX0_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 3;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	/* can2 : receive fifo 1 */	
	nvic.NVIC_IRQChannel = CAN2_RX1_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 2;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	/* can2 : send */	
	nvic.NVIC_IRQChannel = CAN2_TX_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic); 
}


/* END */








