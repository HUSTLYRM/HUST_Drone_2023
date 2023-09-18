/**
 ******************************************************************************
 * @file    InsTask.c
 * @brief   位姿估计任务
 ******************************************************************************
 * @attention
 ******************************************************************************
 */
#include "config.h"

/* IMU */
IMU IMUReceive;
IMU_Data_t IMUData;
uint32_t InsTaskHighWater;
//extern TasksRunTime_Typedef TaskRunTime;

/**
 * @brief 姿态计算任务
 * @param[in] void
 */
void INS_Task(void *pvParameters)
{
    portTickType xLastWakeTime;
    const portTickType xFrequency = 1; // 1kHZ
	xLastWakeTime = xTaskGetTickCount();
    //适当延时
//    vTaskDelay(100);

    while (1)
    {
        INS_DtatHandle(&IMUReceive, &IMUData);

        /*  延时  */
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}
