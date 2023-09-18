/* include */
#include "config.h"

/* CAN1 config */
void vCan1Config(void)
{
	CAN_InitTypeDef        can;
	CAN_FilterInitTypeDef  can_filter;
	GPIO_InitTypeDef       gpio;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_CAN1);

	gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &gpio);
		
	CAN_DeInit(CAN1);
	CAN_StructInit(&can);
	
	can.CAN_TTCM = DISABLE;
	can.CAN_ABOM = ENABLE;
	can.CAN_AWUM = ENABLE;
	can.CAN_NART = ENABLE;
	can.CAN_RFLM = DISABLE;
	can.CAN_TXFP = ENABLE;
	can.CAN_Mode = CAN_Mode_Normal;//CAN_Mode_LoopBack

	/* CAN BaudRate = 42/(1+9+4)/3 = 1Mbps */
	can.CAN_SJW  = CAN_SJW_1tq;
	can.CAN_BS1 = CAN_BS1_9tq;
	can.CAN_BS2 = CAN_BS2_4tq;
	can.CAN_Prescaler = 3;   
	CAN_Init(CAN1, &can);
	
	/* fifo 0 point to filter 0 */
	/* config 4 16bits filter */
	can_filter.CAN_FilterNumber = 0;
	can_filter.CAN_FilterMode = CAN_FilterMode_IdList;
	can_filter.CAN_FilterScale = CAN_FilterScale_16bit;
	/* standard id 11bits and left aligning(×ó¶ÔÆë) */
	/* below 4 id can be receive by can1 fifo 0 */
	can_filter.CAN_FilterIdHigh = 0x205<<5;
	can_filter.CAN_FilterIdLow = 0x206<<5;
	can_filter.CAN_FilterMaskIdHigh = 0x207<<5;
	can_filter.CAN_FilterMaskIdLow = 0x208<<5;
	can_filter.CAN_FilterFIFOAssignment = 0;
	can_filter.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&can_filter);

	/* fifo 1 point to filter 1 */
	/* config 2 32bits filter */
	can_filter.CAN_FilterNumber = 1;
	can_filter.CAN_FilterMode = CAN_FilterMode_IdList;
	can_filter.CAN_FilterScale = CAN_FilterScale_32bit;
	/* standard id 11bits and left aligning */
	/* config as 32bits filter but id is still 11bits standard id */
	can_filter.CAN_FilterIdHigh = 0x201<<5;
	can_filter.CAN_FilterIdLow =  0 | CAN_ID_STD;
	can_filter.CAN_FilterMaskIdHigh = 0x202<<5;  
	can_filter.CAN_FilterMaskIdLow = 0 | CAN_ID_STD;
	can_filter.CAN_FilterFIFOAssignment = 1;
	can_filter.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&can_filter);
	
	/* fifo pending interrupt */
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
	CAN_ITConfig(CAN1,CAN_IT_FMP1,ENABLE);
	/* Transmit mailbox empty Interrupt */
	CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE); 
}

void vCan2Config(void)
{
	CAN_InitTypeDef 			can;
	CAN_FilterInitTypeDef can_filter;
	GPIO_InitTypeDef 			gpio;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);
	
	gpio.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_12 ;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &gpio);
		
	CAN_DeInit(CAN2);
	CAN_StructInit(&can);
	
	can.CAN_TTCM = DISABLE;
	can.CAN_ABOM = ENABLE; 
	can.CAN_AWUM = ENABLE; 
	can.CAN_NART = ENABLE;
	can.CAN_RFLM = DISABLE; 
	can.CAN_TXFP = ENABLE; 
	can.CAN_Mode = CAN_Mode_Normal;
	
	/* CAN BaudRate = 42/(1+9+4)/3 = 1Mbps */
	can.CAN_SJW = CAN_SJW_1tq;
	can.CAN_BS1 = CAN_BS1_9tq;
	can.CAN_BS2 = CAN_BS2_4tq;
	can.CAN_Prescaler = 3; 
	CAN_Init(CAN2, &can);
	
	/* fifo 0 point to filter 15 */
	/* can1 and can2 use these filter together */
	can_filter.CAN_FilterNumber = 15; 
	can_filter.CAN_FilterMode = CAN_FilterMode_IdList;
	can_filter.CAN_FilterScale = CAN_FilterScale_16bit;
	can_filter.CAN_FilterIdHigh = 0x201 << 5;
	can_filter.CAN_FilterIdLow = 0x202 << 5;
	can_filter.CAN_FilterMaskIdHigh = 0x203 << 5;
	can_filter.CAN_FilterMaskIdLow = 0x204 << 5;
	can_filter.CAN_FilterFIFOAssignment = 0;
	can_filter.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&can_filter);
	
	/* fifo 1 point to filter 16 */	
	/* for receive gyro_id */
	can_filter.CAN_FilterNumber = 16;
	can_filter.CAN_FilterMode = CAN_FilterMode_IdList;
	can_filter.CAN_FilterScale = CAN_FilterScale_32bit;
	/* standard id 11bits and left aligning */
	/* config as 32bits filter but id is still 11bits standard id */		
	can_filter.CAN_FilterIdHigh = 0x100 << 5;
	can_filter.CAN_FilterIdLow = 0 | CAN_ID_STD;
	can_filter.CAN_FilterMaskIdHigh = 0x101 << 5;
	can_filter.CAN_FilterMaskIdLow = 0 | CAN_ID_STD;
	can_filter.CAN_FilterFIFOAssignment = 1;
	can_filter.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&can_filter);
	
	/* fifo pending interrupt */	
	CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);
	CAN_ITConfig(CAN2,CAN_IT_FMP1,ENABLE);
	/* Transmit mailbox empty Interrupt */	
	CAN_ITConfig(CAN2,CAN_IT_TME,ENABLE);
}


/* can1 send interrupt */
void CAN1_TX_IRQHandler(void)
{
	if (CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET) 
	{
		CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
	}
}

/* can1 fifo 0 receive interrupt : gimbalmotor GM6020 */
/* ID : 0x205 0x206(pitch) 0x207(yaw) 0x208 */
void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg rx_message0;
	if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
		CAN_Receive(CAN1, CAN_FIFO0, &rx_message0);
		Can1Receive0(rx_message0);
	}
}

/* can1 fifo 1 receive interrupt : pluckmotor M2006 */
/* ID : 0x201(¡Ì) 0x202 */
void CAN1_RX1_IRQHandler(void)
{
	CanRxMsg rx_message1;
	if (CAN_GetITStatus(CAN1,CAN_IT_FMP1)!= RESET)
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP1);
		CAN_Receive(CAN1, CAN_FIFO1, &rx_message1);
		Can1Receive1(rx_message1);
	}
}

/* can2 fifo0 receive interrupt */
/* ID : 0x201 0x202 0x203 0x204 */
void CAN2_RX0_IRQHandler(void)
{
	CanRxMsg rx_message0;
	if (CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET)
	{
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
		CAN_Receive(CAN2, CAN_FIFO0, &rx_message0);
		Can2Receive0(rx_message0);
	}
}

/* can2 fifo1 receive interrupt : imu */
/* ID : 0x100(¡Ì) 0x101(¡Ì) */
void CAN2_RX1_IRQHandler(void)
{
	CanRxMsg rx_message1;
	if (CAN_GetITStatus(CAN2,CAN_IT_FMP1)!= RESET)
	{
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP1);
		CAN_Receive(CAN2, CAN_FIFO1, &rx_message1);
		Can2Receive1(rx_message1);
	}
}

/* send interrupt */
void CAN2_TX_IRQHandler(void)
{
	if (CAN_GetITStatus(CAN2,CAN_IT_TME)!= RESET) 
	{
		CAN_ClearITPendingBit(CAN2,CAN_IT_TME);
	}
}


/* end */










