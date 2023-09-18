/* include */
#include "config.h"

/* define */

void JudgeTask( void * pvParameters )
{
	/* for delayuntil previous be waked time */
	TickType_t PreviousWakeTime;
	/* ms to freertos ticks */
	const TickType_t TimeIncrement = pdMS_TO_TICKS(JudgeData_Period);
	/* get now systick count */
	PreviousWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		/* send judge osd data to judgement */
		JudgeOSD_NumberSend(ENERGY_POINT);
		
		/* delay with certain period */
		vTaskDelayUntil(&PreviousWakeTime,TimeIncrement);
	}
}


/* end */













