/**
 ******************************************************************************
 * @file    bsp_referee.c
 * @brief   裁判系统串口通信
 ******************************************************************************
 * @attention
 ******************************************************************************
 */

#include "config.h"

/*  数据定义  */
uint8_t Refereebuffer[REFEREE_RECVBUF_SIZE] = {0};
uint8_t SendToReferee_Buff[REFEREE_SENDBUF_SIZE] = {0};

/**
 * @brief 裁判系统通信
 * @param[in] void
 */
//void REFEREE_Configuration(void)
//{
//    USART_InitTypeDef usart;
//    GPIO_InitTypeDef gpio;
//    NVIC_InitTypeDef nvic;

//    /* 如果是通信频率不够，需要加入这句  */
//    // USART_OverSampling8Cmd(REFEREE_RCC_AxBxPeriph_UARTx, ENABLE);

//    REFEREE_GPIOx_CLOCK_CMD(REFEREE_RCC_AxBxPeriph_GPIOx, ENABLE);
//    REFEREE_UARTx_CLOCK_CMD(REFEREE_RCC_AxBxPeriph_UARTx, ENABLE);

//    GPIO_PinAFConfig(REFEREE_GPIOx, REFEREE_GPIO_PinSourcex1, REFEREE_GPIO_AF_USARTx);
//    GPIO_PinAFConfig(REFEREE_GPIOx, REFEREE_GPIO_PinSourcex2, REFEREE_GPIO_AF_USARTx);

//    gpio.GPIO_Pin = REFEREE_GPIO_Pin_x1 | REFEREE_GPIO_Pin_x2;
//    gpio.GPIO_Mode = GPIO_Mode_AF;
//    gpio.GPIO_OType = GPIO_OType_PP;
//    gpio.GPIO_Speed = GPIO_Speed_100MHz;
//    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    GPIO_Init(REFEREE_GPIOx, &gpio);

//    USART_DeInit(REFEREE_UARTx);
//    usart.USART_BaudRate = REFEREE_BaudRate;
//    usart.USART_WordLength = USART_WordLength_8b;
//    usart.USART_StopBits = USART_StopBits_1;
//    usart.USART_Parity = USART_Parity_No;
//    usart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
//    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    USART_Init(REFEREE_UARTx, &usart);
//    USART_Cmd(REFEREE_UARTx, ENABLE);

//    USART_DMACmd(REFEREE_UARTx, USART_DMAReq_Rx, ENABLE);
//    USART_DMACmd(REFEREE_UARTx, USART_DMAReq_Tx, ENABLE);

//    nvic.NVIC_IRQChannel = REFEREE_RECV_DMAx_Streamx_IRQn;
//    nvic.NVIC_IRQChannelPreemptionPriority = 0;
//    nvic.NVIC_IRQChannelSubPriority = 0;
//    nvic.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&nvic);

//    // RX
//    DMA_InitTypeDef dma;
//    REFEREE_RECV_RCC_AxBxPeriphClockCmd(REFEREE_RECV_RCC_AxBxPeriph_DMAx, ENABLE);
//    DMA_DeInit(REFEREE_RECV_DMAx_Streamx);
//    while (DMA_GetCmdStatus(REFEREE_RECV_DMAx_Streamx) != DISABLE)
//    {
//    };
//    dma.DMA_Channel = REFEREE_RECV_DMA_Channel_x;
//    dma.DMA_PeripheralBaseAddr = (uint32_t) & (REFEREE_UARTx->DR);
//    dma.DMA_Memory0BaseAddr = (uint32_t)Refereebuffer;
//    dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
//    dma.DMA_BufferSize = REFEREE_RECVBUF_SIZE;
//    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
//    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//    dma.DMA_Mode = DMA_Mode_Circular;
//    dma.DMA_Priority = DMA_Priority_High;
//    dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
//    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
//    dma.DMA_MemoryBurst = DMA_Mode_Normal;
//    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//    DMA_Init(REFEREE_RECV_DMAx_Streamx, &dma);
//    DMA_ITConfig(REFEREE_RECV_DMAx_Streamx, DMA_IT_TC, ENABLE);
//    DMA_Cmd(REFEREE_RECV_DMAx_Streamx, ENABLE);
//}

//void REFEREE_SendBytes(uint8_t len)
//{
//    static DMA_InitTypeDef DMA_InitStructure;
//    static NVIC_InitTypeDef NVIC_InitStructure;
//    static uint8_t firstLoad = 1; //首次加载时加载配置缺省值
//    if (firstLoad == 1)
//    {
//        firstLoad = 0;
//        DMA_InitStructure.DMA_Channel = REFEREE_SEND_DMA_Channel_x;
//        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (REFEREE_UARTx->DR); //外设基地址
//        DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                      //数据传输方向：内存到外设
//        // dma缓存大小
//        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        //外设地址不变
//        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 //内置地址寄存器递增
//        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //数据宽度为八位
//        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //数据宽度为八位
//        DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           //工作模式为环形
//        DMA_InitStructure.DMA_Priority = DMA_Priority_High;                     // dma通道拥有高优先级

//        NVIC_InitStructure.NVIC_IRQChannel = REFEREE_SEND_DMAx_Streamx_IRQn;
//        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//        NVIC_Init(&NVIC_InitStructure);

//        DMA_ITConfig(REFEREE_RECV_DMAx_Streamx, DMA_IT_TC, DISABLE);
//    }
//    DMA_Cmd(REFEREE_RECV_DMAx_Streamx, DISABLE);
//    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)SendToReferee_Buff; //内存基地址
//    DMA_ClearFlag(REFEREE_RECV_DMAx_Streamx, REFEREE_SEND_DMA_IT_TCIFx);  //|DMA_FLAG_FEIF7|DMA_FLAG_HTIF7);
//    DMA_InitStructure.DMA_BufferSize = len;                               // dma缓存大小
//    DMA_Init(REFEREE_RECV_DMAx_Streamx, &DMA_InitStructure);
//    DMA_Cmd(REFEREE_RECV_DMAx_Streamx, ENABLE);
//}

// /**
//  * @brief RS485的接收中断
//  * @param[in] void
//  */
// void REFEREE_RECV_USARTx_IRQHandler(void)
// {
//     if (USART_GetITStatus(REFEREE_UARTx, USART_IT_IDLE) != RESET)
//     {
//         (void)REFEREE_UARTx->SR;
//         (void)REFEREE_UARTx->DR;
//         DMA_Cmd(REFEREE_RECV_DMAx_Streamx, DISABLE);
//         DMA_ClearFlag(REFEREE_RECV_DMAx_Streamx, REFEREE_RECV_DMA_FLAG_TCIFx);
//         DMA_ClearITPendingBit(REFEREE_RECV_DMAx_Streamx, REFEREE_RECV_DMA_IT_TCIFx);

//         RefereeBuffReceive(Refereebuffer, REFEREE_RECVBUF_SIZE);

//         DMA_SetCurrDataCounter(REFEREE_RECV_DMAx_Streamx, REFEREE_RECVBUF_SIZE);
//         DMA_Cmd(REFEREE_RECV_DMAx_Streamx, ENABLE);
//     }
// }
//void REFEREE_RECV_DMAx_Streamx_IRQHandler(void)
//{
//    if (DMA_GetITStatus(REFEREE_RECV_DMAx_Streamx, REFEREE_RECV_DMA_IT_TCIFx))
//    {
//        //数据过圈
//        referee_data.decoder.judgementFullCount++;
//        DMA_ClearFlag(REFEREE_RECV_DMAx_Streamx, REFEREE_RECV_DMA_FLAG_TCIFx);
//        DMA_ClearITPendingBit(REFEREE_RECV_DMAx_Streamx, REFEREE_RECV_DMA_IT_TCIFx);
//    }
//}

///**
// * @brief RS485的发送中断
// * @param[in] void
// */
//void REFEREE_SEND_DMAx_Streamx_IRQHandler(void)
//{
//    if (DMA_GetITStatus(REFEREE_SEND_DMAx_Streamx, REFEREE_SEND_DMA_IT_TCIFx))
//    {
//        DMA_ClearFlag(REFEREE_SEND_DMAx_Streamx, REFEREE_SEND_DMA_FLAG_TCIFx);
//        DMA_ClearITPendingBit(REFEREE_SEND_DMAx_Streamx, REFEREE_SEND_DMA_IT_TCIFx);
//        while (USART_GetFlagStatus(REFEREE_UARTx, USART_FLAG_TC) == RESET)
//            ;
//        USART_ClearFlag(REFEREE_UARTx, USART_FLAG_TC);
//    }
//}
