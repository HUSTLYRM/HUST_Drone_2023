#ifndef __USART1_H__
#define __USART1_H__

#include "config.h"

/* send data to ano earth station buffer */
#define USART1_JUD_DMA_send_buffersize			22
#define USART1_JUD_DMA_receive_buffersize		32

/* send data to ano earth station buffer */
#define USART2_ANO_DMA_send_buffersize			30

/* remote control buffer */
#define USART3_RC_DMA_receive_buffersize 		30

/* small pc buffer */
#define USART4_PC_DMA_send_buffersize 			8
#define USART4_PC_DMA_receive_buffersize 		8


void vUart1Config(void);
void vUart2Config(void);
void vUart3Config(void);
void vUart4Config(void);
void vUart5Config(void);

#endif

/* end */










