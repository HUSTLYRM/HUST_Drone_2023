#ifndef _CPU_TASK_H
#define _CPU_TASK_H

#define CPU_INFO_LIST_LENGTH 400

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"

#define DEBUG_MODE_FREERTOS
//#define DEBUG_MODE
void CPU_task(void *pvParameters);

#endif // !_CPU_TASK_H
