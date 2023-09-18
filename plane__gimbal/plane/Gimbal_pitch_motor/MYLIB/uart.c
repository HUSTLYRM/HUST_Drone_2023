/* include */ 
#include "config.h"

/* define */ 
unsigned char ReceiveFromRC_Buff[18] = {0};
unsigned char SendToPC_Buff[USART4_PC_DMA_send_buffersize] = {0};
unsigned char ReceiveFromPC_Buff[USART4_PC_DMA_receive_buffersize] = {0};
unsigned char SendToAno_Buff[USART2_ANO_DMA_send_buffersize] = {0};
unsigned char SendToSentey_Buff[USART1_JUD_DMA_send_buffersize] = {0};
unsigned char ReceiveFromJUD_Buff[USART1_JUD_DMA_receive_buffersize] = {0};
unsigned char ReceiveFromApp_Buff[USART5_APP_DMA_receive_buffersize] = {0};
unsigned char SendToApp_Buff[USART5_APP_DMA_send_buffersize] = {0};


/* declare*/
//uart1 for judgement
//uart2 for debug(ano)
//uart3 for rc
//uart4 for pc
//uart5 for debug(printf)



//裁判系统图传
/* UART1 for judgement */
void vUart1Config(void)
{
	USART_InitTypeDef usart;
	GPIO_InitTypeDef  gpio;
	NVIC_InitTypeDef  nvic;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); 

	gpio.GPIO_Pin = GPIO_Pin_9;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&gpio);

	
	gpio.GPIO_Pin = GPIO_Pin_10;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&gpio);
	
	usart.USART_BaudRate = 115200;
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1,&usart);

	//USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
	
	USART_Cmd(USART1,ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	
	nvic.NVIC_IRQChannel = DMA2_Stream2_IRQn;
  nvic.NVIC_IRQChannelPreemptionPriority = 3;
  nvic.NVIC_IRQChannelSubPriority = 1;
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);

	{ /* receive dma */
		DMA_InitTypeDef 	dma;
		DMA_DeInit(DMA2_Stream2);
		dma.DMA_Channel= DMA_Channel_4;
		dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
		dma.DMA_Memory0BaseAddr = (uint32_t)ReceiveFromJUD_Buff;
		dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
		dma.DMA_BufferSize = USART1_JUD_DMA_receive_buffersize;
		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		dma.DMA_Mode = DMA_Mode_Circular;
		dma.DMA_Priority = DMA_Priority_High;
		dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
		dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
		dma.DMA_MemoryBurst = DMA_Mode_Normal;
		dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(DMA2_Stream2,&dma);
		DMA_ITConfig(DMA2_Stream2,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA2_Stream2,ENABLE);
	}
		nvic.NVIC_IRQChannel = DMA2_Stream7_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 3;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
	{ /* send dma */
		DMA_InitTypeDef 	dma;
		DMA_DeInit(DMA2_Stream7);
		dma.DMA_Channel= DMA_Channel_4;
		dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
		dma.DMA_Memory0BaseAddr = (uint32_t)SendToSentey_Buff;
		dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;
		dma.DMA_BufferSize = USART1_JUD_DMA_send_buffersize;
		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		dma.DMA_Mode = DMA_Mode_Normal;
		dma.DMA_Priority = DMA_Priority_High;
		dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
		dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
		dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(DMA2_Stream7,&dma);
		DMA_ITConfig(DMA2_Stream7,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA2_Stream7,DISABLE);
	}
}
void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
    (void)USART1->SR;
		(void)USART1->DR;
  }

}

/* USART1 dma receive for judgement */
void DMA2_Stream2_IRQHandler(void)
{	
	if(DMA_GetFlagStatus(DMA2_Stream2,DMA_IT_TCIF2) == SET)
	{
		DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
    JudgeReceive(ReceiveFromJUD_Buff );
	}
}

/* USART1 dma send for judgement */
extern uint8_t DMAsendflag;
void DMA2_Stream7_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_Stream7, DMA_IT_TCIF7))
	{
		DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);
		DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);
		DMA_Cmd(DMA2_Stream7,DISABLE);
		DMAsendflag=0;
	}
}
void vUart2Config(void)
{
	USART_InitTypeDef usart2;
	GPIO_InitTypeDef  gpio;
	NVIC_InitTypeDef  nvic;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); 

	gpio.GPIO_Pin = GPIO_Pin_2;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&gpio);

	
	gpio.GPIO_Pin = GPIO_Pin_3;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&gpio);
	
	usart2.USART_BaudRate = 115200;
	usart2.USART_WordLength = USART_WordLength_8b;
	usart2.USART_StopBits = USART_StopBits_1;
	usart2.USART_Parity = USART_Parity_No;
	usart2.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	usart2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART2,&usart2);

	//USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
	
	USART_Cmd(USART2,ENABLE);
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
	
	nvic.NVIC_IRQChannel = DMA1_Stream5_IRQn;
  nvic.NVIC_IRQChannelPreemptionPriority = 3;
  nvic.NVIC_IRQChannelSubPriority = 1;
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);

	{ /* receive dma */
		DMA_InitTypeDef 	dma;
		DMA_DeInit(DMA1_Stream5);
		dma.DMA_Channel= DMA_Channel_4;
		dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
		//dma.DMA_Memory0BaseAddr = (uint32_t)ReceiveFromJUD_Buff;
		dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
		//dma.DMA_BufferSize = USART1_JUD_DMA_receive_buffersize;
		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		dma.DMA_Mode = DMA_Mode_Circular;
		dma.DMA_Priority = DMA_Priority_High;
		dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
		dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
		dma.DMA_MemoryBurst = DMA_Mode_Normal;
		dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(DMA1_Stream5,&dma);
		DMA_ITConfig(DMA1_Stream5,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA1_Stream5,ENABLE);
	}
		nvic.NVIC_IRQChannel = DMA1_Stream6_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 3;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
	{ /* send dma */
		DMA_InitTypeDef 	dma;
		DMA_DeInit(DMA1_Stream6);
		dma.DMA_Channel= DMA_Channel_4;
		dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
		//dma.DMA_Memory0BaseAddr = (uint32_t)SendToSentey_Buff;
		dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;
		//dma.DMA_BufferSize = USART1_JUD_DMA_send_buffersize;
		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		dma.DMA_Mode = DMA_Mode_Normal;
		dma.DMA_Priority = DMA_Priority_High;
		dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
		dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
		dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(DMA1_Stream6,&dma);
		DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA1_Stream6,DISABLE);
	}
}
//void USART2_IRQHandler(void)
//{
//	if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
//	{
//    (void)USART2->SR;
//		(void)USART2->DR;
//  }

//}

///* USART1 dma receive for judgement */
//void DMA1_Stream5_IRQHandler(void)
//{	
//	if(DMA_GetFlagStatus(DMA1_Stream5,DMA_IT_TCIF5) == SET)
//	{
//		DMA_ClearFlag(DMA1_Stream5, DMA_IT_TCIF5);
//    JudgeReceive(ReceiveFromJUD_Buff );
//	}
//}

///* USART1 dma send for judgement */
//extern uint8_t DMAsendflag;
//void DMA1_Stream6_IRQHandler(void)
//{
//	if(DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6))
//	{
//		DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);
//		DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
//		DMA_Cmd(DMA1_Stream6,DISABLE);
//		DMAsendflag=0;
//	}
//}

/* UART3 for rc */
/* Type A board uart1 for remote control */
/* Type B board uart3 for remote control */
void vUart3Config(void)
{
	USART_InitTypeDef usart3;
	GPIO_InitTypeDef  gpio;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); 

	gpio.GPIO_Pin = GPIO_Pin_11;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&gpio);

	usart3.USART_BaudRate = 100000;
	usart3.USART_WordLength = USART_WordLength_8b;
	usart3.USART_StopBits = USART_StopBits_1;
	usart3.USART_Parity = USART_Parity_Even;
	usart3.USART_Mode = USART_Mode_Rx;
	usart3.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART3,&usart3);

	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);
	USART_Cmd(USART3,ENABLE);
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);	
	
	{	/* receive dma */
		DMA_InitTypeDef	dma;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
		DMA_DeInit(DMA1_Stream1);
		dma.DMA_Channel= DMA_Channel_4;
		dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
		dma.DMA_Memory0BaseAddr = (uint32_t)ReceiveFromRC_Buff;
		dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
		dma.DMA_BufferSize = USART3_RC_DMA_receive_buffersize;
		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		dma.DMA_Mode = DMA_Mode_Circular;
		dma.DMA_Priority = DMA_Priority_VeryHigh;
		dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
		dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
		dma.DMA_MemoryBurst = DMA_Mode_Normal;
		dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(DMA1_Stream1,&dma);
		DMA_ITConfig(DMA1_Stream1,DMA_IT_TC,ENABLE);//????????????
		DMA_Cmd(DMA1_Stream1,ENABLE);
	}
}
//遥控器接收发送
/* UART3 IDLE Interrupt for remote control */
void USART3_IRQHandler(void)
{
	static int DATA_LENGTH=0;
	
	if (USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
    (void)USART3->SR;
		(void)USART3->DR;	
		
	  DMA_Cmd(DMA1_Stream1,DISABLE);
	  DATA_LENGTH = USART3_RC_DMA_receive_buffersize-DMA_GetCurrDataCounter(DMA1_Stream1);
		
		if(DATA_LENGTH==18)
		{
			RemoteReceive(ReceiveFromRC_Buff);
		}
		
		DMA_SetCurrDataCounter(DMA1_Stream1,USART3_RC_DMA_receive_buffersize);	
		DMA_Cmd(DMA1_Stream1,ENABLE);
  }
}

//辅瞄接收发送
/* UART4 for PC */
void vUart4Config(void)
{
	USART_InitTypeDef uart4;
	GPIO_InitTypeDef  gpio;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);

	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4); 

	/* all AF_PP? */
	gpio.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC,&gpio);

	uart4.USART_BaudRate = 115200;
	uart4.USART_WordLength = USART_WordLength_8b;
	uart4.USART_StopBits = USART_StopBits_1;
	uart4.USART_Parity = USART_Parity_No;
	uart4.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
	uart4.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(UART4,&uart4);
	
	USART_ITConfig(UART4,USART_IT_IDLE,ENABLE);
	
	USART_Cmd(UART4,ENABLE);
	USART_DMACmd(UART4,USART_DMAReq_Rx,ENABLE);	
	USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);
	
	{ /* receive dma */
		DMA_InitTypeDef dma;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
		DMA_DeInit(DMA1_Stream2);
		dma.DMA_Channel= DMA_Channel_4;
		dma.DMA_PeripheralBaseAddr = (uint32_t)&(UART4->DR);
		dma.DMA_Memory0BaseAddr = (uint32_t)ReceiveFromPC_Buff;
		dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
		dma.DMA_BufferSize = USART4_PC_DMA_receive_buffersize;
		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		dma.DMA_Mode = DMA_Mode_Circular;
		dma.DMA_Priority = DMA_Priority_VeryHigh;
		dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
		dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
		dma.DMA_MemoryBurst = DMA_Mode_Normal;
		dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(DMA1_Stream2,&dma);
		DMA_ITConfig(DMA1_Stream2,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA1_Stream2,ENABLE);
	}
	
	{ /* send dma */
		DMA_InitTypeDef dma;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
		DMA_DeInit(DMA1_Stream4);
		dma.DMA_Channel= DMA_Channel_4;
		dma.DMA_PeripheralBaseAddr = (uint32_t)&(UART4->DR);
		dma.DMA_Memory0BaseAddr = (uint32_t)SendToPC_Buff;
		dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;
		dma.DMA_BufferSize = USART4_PC_DMA_send_buffersize;
		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		dma.DMA_Mode = DMA_Mode_Circular;
		dma.DMA_Priority = DMA_Priority_VeryHigh;
		dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
		dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
		dma.DMA_MemoryBurst = DMA_Mode_Normal;
		dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(DMA1_Stream4,&dma);
		DMA_ITConfig(DMA1_Stream4,DMA_IT_TC,ENABLE);		
		DMA_Cmd(DMA1_Stream4,DISABLE);
	}
}

void UART4_IRQHandler(void)
{
	if (USART_GetITStatus(UART4, USART_IT_IDLE) != RESET)
	{
    (void)UART4->SR;
		(void)UART4->DR;	
  }
}

/* tempPC need to be golbal,otherwise crc will fall */
unsigned char tempPC[USART4_PC_DMA_receive_buffersize];
/* UART4 receive interrupt for PC */
/* PC_RECVBUF_SIZE = 8 
		[0]:!
		[1]:Data ID
		[2]~[5]:Valid Data
		[6]:CRC 
		[7]:								*/
void DMA1_Stream2_IRQHandler(void)
 {
	/* stack: buffer(缓冲数组) */
  static unsigned char temptemp[2*USART4_PC_DMA_receive_buffersize];//temp[18] 0-17
	short PackPoint,n;
	
	if(DMA_GetITStatus(DMA1_Stream2, DMA_IT_TCIF2))
	{
		/* stack: save at back buffer */
		memcpy(temptemp+USART4_PC_DMA_receive_buffersize,ReceiveFromPC_Buff,USART4_PC_DMA_receive_buffersize);
	  //temp[9]
		
		/* avoid the dislocation */
		for(PackPoint = 0; PackPoint < USART4_PC_DMA_receive_buffersize; PackPoint++)
		{
			/* look for the buffer head */
			if(temptemp[PackPoint] == '!')
			{
				/* copy valid data to tempPC[] */
				for(n=0;n<USART4_PC_DMA_receive_buffersize;n++)
				{
					tempPC[n] = temptemp[(n+PackPoint)];
				}
				if(Verify_CRC8_Check_Sum(tempPC,USART4_PC_DMA_receive_buffersize))
				{
					PCReceive(tempPC);
				}
				 
				break;/* jump out from "for" loop */
			}
	  }
		
		DMA_ClearFlag(DMA1_Stream2, DMA_FLAG_TCIF2);
		DMA_ClearITPendingBit(DMA1_Stream2, DMA_IT_TCIF2);
		
		/* stack: push */
		memcpy(temptemp,temptemp+USART4_PC_DMA_receive_buffersize,USART4_PC_DMA_receive_buffersize);
  }
}

/* UART4 send interrupt for PC */
void DMA1_Stream4_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_Stream4, DMA_IT_TCIF4) == SET)
	{
		DMA_Cmd(DMA1_Stream4, DISABLE);
 		DMA_SetCurrDataCounter(DMA1_Stream4, USART4_PC_DMA_send_buffersize);
		DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);
		DMA_ClearITPendingBit(DMA1_Stream4, DMA_IT_TCIF4);
	}
}

/* usart2 for debug (Ano) */
//void vUart2Config(void)
//{
//	USART_InitTypeDef usart2;
//	GPIO_InitTypeDef  gpio;

//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);

//	gpio.GPIO_Pin = GPIO_Pin_2;
//	gpio.GPIO_Mode = GPIO_Mode_AF;
//	gpio.GPIO_OType = GPIO_OType_PP;
//	gpio.GPIO_Speed = GPIO_Speed_100MHz;
//	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOA,&gpio);

//	gpio.GPIO_Pin = GPIO_Pin_3;
//	gpio.GPIO_Mode = GPIO_Mode_IN;
//	gpio.GPIO_Speed = GPIO_Speed_100MHz;
//	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOA,&gpio);

//	USART_DeInit(USART2);
//	/* zigbee digital transmiter support up to 115200 */	
//	usart2.USART_BaudRate = 115200;
//	usart2.USART_WordLength = USART_WordLength_8b;
//	usart2.USART_StopBits = USART_StopBits_1;
//	usart2.USART_Parity = USART_Parity_No;
//	usart2.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
//	usart2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_Init(USART2,&usart2);
//	
//	USART_Cmd(USART2,ENABLE);
//	
//	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE); 
//	
//	/* send dma */
//	{
//		DMA_InitTypeDef dma;
//		
//		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
//		DMA_DeInit(DMA1_Stream6);
//		dma.DMA_Channel= DMA_Channel_4;
//		dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
//		dma.DMA_Memory0BaseAddr = (uint32_t)SendToAno_Buff;
//		dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;
//		dma.DMA_BufferSize = USART2_ANO_DMA_send_buffersize;
//		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
//		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//		dma.DMA_Mode = DMA_Mode_Normal;
//		dma.DMA_Priority = DMA_Priority_VeryHigh;
//		dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
//		dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
//		dma.DMA_MemoryBurst = DMA_Mode_Normal;
//		dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//		DMA_Init(DMA1_Stream6,&dma);
//		
//		DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);		
//		/* enable dma when data preparing finish */
//		DMA_Cmd(DMA1_Stream6,DISABLE);
//	}
//}
/* uart2 DMA send finish interrupt : send debug date (Ano) */


/* repoint printf */
int fputc(int ch, FILE *f)
{
	while((UART5->SR&0X40)==0);
    UART5->DR = (u8) ch;      
	return ch;
}

/* UART5 for Debug (printf)  */
//void vUart5Config(void)
//{
//	USART_InitTypeDef usart5;
//	GPIO_InitTypeDef  gpio;

//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
//	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5);
//	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5);

//	gpio.GPIO_Pin = GPIO_Pin_12;
//	gpio.GPIO_Mode = GPIO_Mode_AF;
//	gpio.GPIO_OType = GPIO_OType_PP;
//	gpio.GPIO_Speed = GPIO_Speed_100MHz;
//	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOC,&gpio);

//	gpio.GPIO_Pin = GPIO_Pin_2;
//	gpio.GPIO_Mode = GPIO_Mode_IN;
//	gpio.GPIO_Speed = GPIO_Speed_100MHz;
//	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOD,&gpio);

//	usart5.USART_BaudRate = 115200;
//	usart5.USART_WordLength = USART_WordLength_8b;
//	usart5.USART_StopBits = USART_StopBits_1;
//	usart5.USART_Parity = USART_Parity_No;
//	usart5.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
//	usart5.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_Init(UART5,&usart5);

//	USART_Cmd(UART5,ENABLE);
//}


/* end */
//UART5未使用
void vUart5Config()
{
	USART_InitTypeDef usart5;
	GPIO_InitTypeDef gpio;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);//使能GPIOC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);//使能GPIOA
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);//使能串口5
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5);//PD2复用串口5
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5);//PC12复用串口5
	
	gpio.GPIO_Pin = GPIO_Pin_12;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC,&gpio);//定义引入PC12的模式

	gpio.GPIO_Pin = GPIO_Pin_2;
	gpio.GPIO_Mode = GPIO_Mode_IN;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD,&gpio);//定义引入PD2的模式
	
	usart5.USART_BaudRate = 115200;//波特率
	usart5.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	usart5.USART_StopBits = USART_StopBits_1;//一个停止位
	usart5.USART_Parity = USART_Parity_No;//无奇偶校验位
	usart5.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;//收发模式
	usart5.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制

	USART_Init(UART5,&usart5);//初始化串口5
	USART_ITConfig(UART5, USART_IT_IDLE, ENABLE);//开启串口接受中断
	USART_Cmd(UART5,ENABLE);//使能串口5
	
		{ /* receive dma */
		DMA_InitTypeDef dma;
			
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//使能DMA1时钟
		DMA_DeInit(DMA1_Stream0);//将DMA1的数据流0设为缺省值
		dma.DMA_Channel= DMA_Channel_4;//设置通道为4
		dma.DMA_PeripheralBaseAddr = (uint32_t)&(UART5->DR);//启动传输前装入实际RAM地址
		dma.DMA_Memory0BaseAddr = (uint32_t)ReceiveFromApp_Buff;//设置接收缓冲区首地址
		dma.DMA_DIR = DMA_DIR_PeripheralToMemory;//数据传输方向，从外设读取到内存
		dma.DMA_BufferSize = USART5_APP_DMA_receive_buffersize;//DMA通道的DMA缓存的大小
		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址寄存器不变
		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址寄存器递增
		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据数据宽度为8位
		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//内存数据数据宽度为8位
		dma.DMA_Mode = DMA_Mode_Circular;//DMA循环模式
		dma.DMA_Priority = DMA_Priority_VeryHigh;//拥有最高优先级
		dma.DMA_FIFOMode = DMA_FIFOMode_Disable;//禁用FIFO模式
		dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
		dma.DMA_MemoryBurst = DMA_Mode_Normal;
		dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设单次模式
			
		DMA_Init(DMA1_Stream0,&dma);//对DMA通道0进行初始化
		DMA_ITConfig(DMA1_Stream0,DMA_IT_TC,ENABLE);//开UART5 Rx DMA中断
		DMA_Cmd(DMA1_Stream0,ENABLE);//开启串口DMA接收
	}
	
	{ /* send dma */
		DMA_InitTypeDef dma;
		
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
		DMA_DeInit(DMA1_Stream7);
		dma.DMA_Channel= DMA_Channel_4;
		dma.DMA_PeripheralBaseAddr = (uint32_t)&(UART5->DR);
		dma.DMA_Memory0BaseAddr = (uint32_t)SendToApp_Buff;
		dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;
		dma.DMA_BufferSize = USART5_APP_DMA_send_buffersize;
		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		dma.DMA_Mode = DMA_Mode_Circular;
		dma.DMA_Priority = DMA_Priority_VeryHigh;
		dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
		dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
		dma.DMA_MemoryBurst = DMA_Mode_Normal;
		dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		
		DMA_Init(DMA1_Stream7,&dma);
		DMA_ITConfig(DMA1_Stream7,DMA_IT_TC,ENABLE);		
		DMA_Cmd(DMA1_Stream7,DISABLE);
	}
}

void UART5_IRQHandler(void)
{
	if (USART_GetITStatus(UART5, USART_IT_IDLE) != RESET)
	{
    (void)UART5->SR;
		(void)UART5->DR;	
  }
}

void DMA1_Stream0_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream0, DMA_IT_TCIF0)!=RESET)//检查DMA传输完成中断
	{
		AppReceive(ReceiveFromApp_Buff);
		DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);//清除IT flag
		DMA_ClearITPendingBit(DMA1_Stream0,DMA_FLAG_TCIF0);//清中断
		DMA_Cmd(DMA1_Stream0,DISABLE);
	}
}

void DMA1_Stream7_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream7, DMA_IT_TCIF7)!=RESET)
	{
		DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);
		DMA_ClearITPendingBit(DMA1_Stream7,DMA_FLAG_TCIF7);
		DMA_Cmd(DMA1_Stream0,DISABLE);
	}
}

/*USART2用于APP通信*/
//void vUart2Config(void)
//{
//	USART_InitTypeDef usart2;
//	GPIO_InitTypeDef  gpio;

//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);//PA2 TX
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);//PA3 RX

//	gpio.GPIO_Pin = GPIO_Pin_2;
//	gpio.GPIO_Mode = GPIO_Mode_AF;
//	gpio.GPIO_OType = GPIO_OType_PP;
//	gpio.GPIO_Speed = GPIO_Speed_100MHz;
//	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOA,&gpio);

//	gpio.GPIO_Pin = GPIO_Pin_3;
//	gpio.GPIO_Mode = GPIO_Mode_IN;
//	gpio.GPIO_Speed = GPIO_Speed_100MHz;
//	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOA,&gpio);

//	USART_DeInit(USART2);
//	/* zigbee digital transmiter support up to 115200 */	
//	usart2.USART_BaudRate = 115200;
//	usart2.USART_WordLength = USART_WordLength_8b;
//	usart2.USART_StopBits = USART_StopBits_1;
//	usart2.USART_Parity = USART_Parity_No;
//	usart2.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
//	usart2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_Init(USART2,&usart2);
//	
//	USART_Cmd(USART2,ENABLE);
//	
//	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE); 
//			
//	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);	
//	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);	
//	
//	{ /* receive dma */
//		DMA_InitTypeDef dma;
//			
//		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//使能DMA1时钟
//		DMA_DeInit(DMA1_Stream5);//将DMA1的数据流5设为缺省值
//		dma.DMA_Channel= DMA_Channel_4;//设置通道为4
//		dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);//启动传输前装入实际RAM地址
//		dma.DMA_Memory0BaseAddr = (uint32_t)ReceiveFromApp_Buff;//设置接收缓冲区首地址
//		dma.DMA_DIR = DMA_DIR_PeripheralToMemory;//数据传输方向，从外设读取到内存
//		dma.DMA_BufferSize = USART5_APP_DMA_receive_buffersize;//DMA通道的DMA缓存的大小
//		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址寄存器不变
//		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址寄存器递增
//		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据数据宽度为8位
//		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//内存数据数据宽度为8位
//		dma.DMA_Mode = DMA_Mode_Circular;//DMA循环模式
//		dma.DMA_Priority = DMA_Priority_VeryHigh;//拥有最高优先级
//		dma.DMA_FIFOMode = DMA_FIFOMode_Disable;//禁用FIFO模式
//		dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
//		dma.DMA_MemoryBurst = DMA_Mode_Normal;
//		dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设单次模式
//			
//		DMA_Init(DMA1_Stream5,&dma);//对DMA通道0进行初始化
//		DMA_ITConfig(DMA1_Stream5,DMA_IT_TC,ENABLE);//开UART5 Rx DMA中断
//		DMA_Cmd(DMA1_Stream5,ENABLE);//开启串口DMA接收
//	}
//	/* send dma */
//	{
//		DMA_InitTypeDef dma;
//		
//		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
//		DMA_DeInit(DMA1_Stream6);
//		dma.DMA_Channel= DMA_Channel_4;
//		dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
//		dma.DMA_Memory0BaseAddr = (uint32_t)SendToApp_Buff;
//		dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;
//		dma.DMA_BufferSize = USART5_APP_DMA_send_buffersize;
//		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
//		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//		dma.DMA_Mode = DMA_Mode_Normal;
//		dma.DMA_Priority = DMA_Priority_VeryHigh;
//		dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
//		dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
//		dma.DMA_MemoryBurst = DMA_Mode_Normal;
//		dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//		DMA_Init(DMA1_Stream6,&dma);
//		
//		DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);
//		
//		/* enable dma when data preparing finish */
//		DMA_Cmd(DMA1_Stream6,DISABLE);

//	}
//}

//void DMA1_Stream6_IRQHandler(void)
//{
//	if(DMA_GetFlagStatus(DMA1_Stream6, DMA_IT_TCIF6) == SET)
//	{
//		DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);
//		DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
//		DMA_Cmd(DMA1_Stream6, DISABLE);
//	}
//}

//void DMA1_Stream5_IRQHandler(void)
//{
//	if(DMA_GetFlagStatus(DMA1_Stream5, DMA_IT_TCIF5) == SET)
//	{
//		AppReceive(ReceiveFromApp_Buff);
//		DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);
//		DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);
//		DMA_Cmd(DMA1_Stream5, DISABLE);
//	}
//}