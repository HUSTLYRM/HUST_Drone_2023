/* include */ 
#include "config.h"

/* define */ 
unsigned char ReceiveFromRC_Buff[18] = {0};
unsigned char SendToPC_Buff[USART4_PC_DMA_send_buffersize] = {0};
unsigned char ReceiveFromPC_Buff[USART4_PC_DMA_receive_buffersize] = {0};
unsigned char SendToAno_Buff[USART2_ANO_DMA_send_buffersize] = {0};
unsigned char SendToJUD_Buff[USART1_JUD_DMA_send_buffersize] = {0};
unsigned char ReceiveFromJUD_Buff[USART1_JUD_DMA_receive_buffersize] = {0};

/* declare*/

/* UART1 for judgement */
void vUart1Config(void)
{
	USART_InitTypeDef usart;
	GPIO_InitTypeDef  gpio;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); 

	gpio.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
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
	
	USART_Cmd(USART1,ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);

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
		dma.DMA_Priority = DMA_Priority_VeryHigh;
		dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
		dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
		dma.DMA_MemoryBurst = DMA_Mode_Normal;
		dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(DMA2_Stream2,&dma);
		DMA_ITConfig(DMA2_Stream2,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA2_Stream2,ENABLE);
	}
	
	{ /* send dma */
		DMA_InitTypeDef 	dma;
		DMA_DeInit(DMA2_Stream7);
		dma.DMA_Channel= DMA_Channel_4;
		dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
		dma.DMA_Memory0BaseAddr = (uint32_t)SendToJUD_Buff;
		dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;
		dma.DMA_BufferSize = USART1_JUD_DMA_send_buffersize;
		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		dma.DMA_Mode = DMA_Mode_Circular;
		dma.DMA_Priority = DMA_Priority_VeryHigh;
		dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
		dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
		dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(DMA2_Stream7,&dma);
		DMA_ITConfig(DMA2_Stream7,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA2_Stream7,DISABLE);
	}
}

/* USART1 dma receive for judgement */
void DMA2_Stream2_IRQHandler(void)
{	
	if(DMA_GetFlagStatus(DMA2_Stream2,DMA_IT_TCIF2) == SET)
	{
		DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
    JudgeReceive(ReceiveFromJUD_Buff);
	}
}

/* USART1 dma send for judgement */
void DMA2_Stream7_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_Stream7, DMA_IT_TCIF7)!=RESET)
	{
		DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);
		DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);
		DMA_Cmd(DMA2_Stream7,DISABLE);
	}
}

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
		DMA_Cmd(DMA1_Stream1,ENABLE);
	}
}

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
	/* stack: buffer(»º³åÊý×é) */
  static unsigned char temptemp[2*USART4_PC_DMA_receive_buffersize];
	short PackPoint,n;
	
	if(DMA_GetITStatus(DMA1_Stream2, DMA_IT_TCIF2))
	{
		/* stack: save at back buffer */
		memcpy(temptemp+USART4_PC_DMA_receive_buffersize,ReceiveFromPC_Buff,USART4_PC_DMA_receive_buffersize);
	  
		/* avoid the dislocation */
		for(PackPoint = 0; PackPoint < USART4_PC_DMA_receive_buffersize; PackPoint++)
		{
			/* look for the buffer head */
			if(temptemp[PackPoint] == '!')
			{
				/* copy valid data to tempPC[] */
				for(n=0;n<USART4_PC_DMA_receive_buffersize;n++)
					tempPC[n] = temptemp[(n+PackPoint)];

				if(Verify_CRC8_Check_Sum(tempPC+1,USART4_PC_DMA_receive_buffersize-2))
				  PCReceive(tempPC);

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
void vUart2Config(void)
{
	USART_InitTypeDef usart2;
	GPIO_InitTypeDef  gpio;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);

	gpio.GPIO_Pin = GPIO_Pin_2;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&gpio);

	gpio.GPIO_Pin = GPIO_Pin_3;
	gpio.GPIO_Mode = GPIO_Mode_IN;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&gpio);

	USART_DeInit(USART2);
	/* zigbee digital transmiter support up to 115200 */	
	usart2.USART_BaudRate = 115200;
	usart2.USART_WordLength = USART_WordLength_8b;
	usart2.USART_StopBits = USART_StopBits_1;
	usart2.USART_Parity = USART_Parity_No;
	usart2.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	usart2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART2,&usart2);
	
	USART_Cmd(USART2,ENABLE);
	
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE); 
	
	/* send dma */
	{
		DMA_InitTypeDef dma;
		
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
		DMA_DeInit(DMA1_Stream6);
		dma.DMA_Channel= DMA_Channel_4;
		dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
		dma.DMA_Memory0BaseAddr = (uint32_t)SendToAno_Buff;
		dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;
		dma.DMA_BufferSize = USART2_ANO_DMA_send_buffersize;
		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		dma.DMA_Mode = DMA_Mode_Normal;
		dma.DMA_Priority = DMA_Priority_VeryHigh;
		dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
		dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
		dma.DMA_MemoryBurst = DMA_Mode_Normal;
		dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(DMA1_Stream6,&dma);
		
		DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);		
		/* enable dma when data preparing finish */
		DMA_Cmd(DMA1_Stream6,DISABLE);
	}
}

/* uart2 DMA send finish interrupt : send debug date (Ano) */
void DMA1_Stream6_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_Stream6, DMA_IT_TCIF6) == SET)
	{
		DMA_Cmd(DMA1_Stream6, DISABLE);
		DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);
		DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
	}
}

/* repoint printf */
int fputc(int ch, FILE *f)
{
	while((UART5->SR&0X40)==0);
    UART5->DR = (u8) ch;      
	return ch;
}

/* UART5 for Debug (printf)  */
void vUart5Config(void)
{
	USART_InitTypeDef usart5;
	GPIO_InitTypeDef  gpio;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5);

	gpio.GPIO_Pin = GPIO_Pin_12;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC,&gpio);

	gpio.GPIO_Pin = GPIO_Pin_2;
	gpio.GPIO_Mode = GPIO_Mode_IN;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD,&gpio);

	usart5.USART_BaudRate = 115200;
	usart5.USART_WordLength = USART_WordLength_8b;
	usart5.USART_StopBits = USART_StopBits_1;
	usart5.USART_Parity = USART_Parity_No;
	usart5.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	usart5.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(UART5,&usart5);

	USART_Cmd(UART5,ENABLE);
}


/* end */











