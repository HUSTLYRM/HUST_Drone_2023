/* include */
#include "config.h"

/* define */

void DebugTask( void * pvParameters )
{
	static uint16_t printf_count = 0;
	/* for delayuntil previous be waked time */
	TickType_t PreviousWakeTime;
	/* ms to freertos ticks */
	const TickType_t TimeIncrement = pdMS_TO_TICKS(DebugData_Period);
	/* for save the runtime information */
	char RunTimeInfo[100];
	/* get now systick count */
	PreviousWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		/* send debug data to ANO earth station */
		ANOSend();
		
		/* enable cpu occupy monitor */
		if(CPU_OCCUPY)
		{
			/* printf task run time statistic information to pc */
			printf_count++;
			if(printf_count >= Runstatistics_Time/DebugData_Period)
			{
				printf_count = 0;
				/* get runtime information */
				vTaskGetRunTimeStats(RunTimeInfo);
				printf("task name\t\t\t run time\t timetime percent\r\n");
				printf("%s\r\n",RunTimeInfo);
			}
		}
		
		/* delay with certain period */
		vTaskDelayUntil(&PreviousWakeTime,TimeIncrement);
	}
}


/* end */













