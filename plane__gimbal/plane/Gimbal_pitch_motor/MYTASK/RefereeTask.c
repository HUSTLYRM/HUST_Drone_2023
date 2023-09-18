/**
 ******************************************************************************
 * @file    RefereeTask.c
 * @brief   给裁判系统发送图形化界面信息,接收裁判系统数据
 ******************************************************************************
 * @attention
 ******************************************************************************
 */
#include "config.h"

/**
 * @brief 图形界面数据发送(包括字符和界面)
 * @param[in] void
 */
void Refereetask(void *pvParameters)
{
	portTickType xLastWakeTime;
	const portTickType xFrequency = 1; // 1kHZ

	while (1)
	{
		Referee_UnpackFifoData();

		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
