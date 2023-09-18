#ifndef __GRAPHICS_SEND_TASK_H
#define __GRAPHICS_SEND_TASK_H

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "stdint.h"

#include "bsp_referee.h"
#include "Referee.h"

#include "crc.h"

void Refereetask(void *pvParameters);

#endif
