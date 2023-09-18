/* include */
#include "config.h"

/* define */
#define START_TASK_PRIO		4
#define START_TASK_SIZE		128
TaskHandle_t StartTask_Handler;

#define SELFCHECK_TASK_PRIO		4
#define SELFCHECK_TASK_SIZE		128
TaskHandle_t SelfcheckTask_Handler;

#define DEBUG_TASK_PRIO		1
#define DEBUG_TASK_SIZE		128
TaskHandle_t DebugTask_Handler;

#define JUDGE_TASK_PRIO		1
#define JUDGE_TASK_SIZE		128
TaskHandle_t JudgeTask_Handler;

void StartTask( void * pvParameters )
{
	/* create selfcheck task */
	xTaskCreate(( TaskFunction_t	)SelfcheckTask,      	
							( const char *		)"Selfcheck Task",     
							( uint16_t			)SELFCHECK_TASK_SIZE,    
							( void *			)NULL,              
							( UBaseType_t		)SELFCHECK_TASK_PRIO,    
							( TaskHandle_t *	)&SelfcheckTask_Handler);
							
	/* create debug task */
	xTaskCreate(( TaskFunction_t	)DebugTask,      	
							( const char *		)"Debug Task",     
							( uint16_t			)DEBUG_TASK_SIZE,    
							( void *			)NULL,              
							( UBaseType_t		)DEBUG_TASK_PRIO,    
							( TaskHandle_t *	)&DebugTask_Handler);
							
	/* create judge task */
	xTaskCreate(( TaskFunction_t	)JudgeTask,      	
							( const char *		)"Judge Task",     
							( uint16_t			)JUDGE_TASK_SIZE,    
							( void *			)NULL,              
							( UBaseType_t		)JUDGE_TASK_PRIO,    
							( TaskHandle_t *	)&JudgeTask_Handler);

	/* delete start task */
	vTaskDelete( StartTask_Handler );
}

/* create start task */
void vCreateStartTask( void )
{
	xTaskCreate(( TaskFunction_t	)StartTask,      	
							( const char *		)"SystemStart",     
							( uint16_t			)START_TASK_SIZE,    
							( void *			)NULL,              
							( UBaseType_t		)START_TASK_PRIO,    
							( TaskHandle_t *	)&StartTask_Handler);
}


/* end */














