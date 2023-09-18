#ifndef _START_TASK_H_
#define _START_TASK_H_
  
#include "config.h"

#define TASK_NUM 8

void StartTask( void * pvParameters );
void vCreateStartTask( void );

enum TASK_LIST
{
    SELFCHECK_TASK,
    DEBUG_TASK,
    JUDGE_TASK,
    GRAPHIC_SEND_TASK,
    IMUGIMBAL_TASK,
    SHOOT_TASK,
    CPU_TASK,
    CONTROL_TASK,
	  INS_TASK
	
};


#endif


/* END */













