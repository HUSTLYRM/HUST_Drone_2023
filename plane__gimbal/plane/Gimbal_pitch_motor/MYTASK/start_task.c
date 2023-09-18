/* include */
#include "config.h"

/* define */


#define START_TASK_PRIO		4
#define START_TASK_SIZE		128
TaskHandle_t StartTask_Handler;

#define SELFCHECK_TASK_PRIO		4
#define SELFCHECK_TASK_SIZE		128
//TaskHandle_t SelfcheckTask_Handler;

#define DEBUG_TASK_PRIO		1
#define DEBUG_TASK_SIZE		128
//TaskHandle_t DebugTask_Handler;

#define JUDGE_TASK_PRIO		1
#define JUDGE_TASK_SIZE		128
//TaskHandle_t JudgeTask_Handler;

#define GRAPHICS_SEND_TASK_PRIO		1
#define GRAPHICS_SEND_TASK_SIZE		128
//TaskHandle_t GraphicSendtask_Handler;

/* define */
#define IMU_GIMBAL_TASK_PRIO		5
#define IMU_GIMBAL_TASK_SIZE		128
//TaskHandle_t ImugimbalTask_Handler;

#define SHOOT_TASK_PRIO		5
#define SHOOT_TASK_SIZE		128
//TaskHandle_t ShootTask_Handler;

// CPU占用情况估计任务
#define CPU_TASK_PRIO 2       //任务优先级
#define CPU_TASK_STK_SIZE 128 // 任务堆栈
//TaskHandle_t CPU_TASK_Handler;

#define CONTROL_TASK_PRIO 4  //任务优先级
#define CONTROL_STK_SIZE 256 //任务堆栈
//TaskHandle_t CONTROLTask_Handler; //任务句柄
#define INS_TASK_PRIO 14  //任务优先级
#define INS_TASK_STK_SIZE 128 //任务堆栈

TaskHandle_t User_Tasks[TASK_NUM];

void StartTask( void * pvParameters )
{
	taskENTER_CRITICAL();
	/* create selfcheck task */
	xTaskCreate(( TaskFunction_t	)SelfcheckTask,      	
							( const char *		)"Selfcheck Task",     
							( uint16_t			)SELFCHECK_TASK_SIZE,    
							( void *			)NULL,              
							( UBaseType_t		)SELFCHECK_TASK_PRIO,    
							( TaskHandle_t *	)&User_Tasks[SELFCHECK_TASK]);  //自检任务
							
	/* create debug task */
//	xTaskCreate(( TaskFunction_t	)DebugTask,      	
//							( const char *		)"Debug Task",     
//							( uint16_t			)DEBUG_TASK_SIZE,    
//							( void *			)NULL,              
//							( UBaseType_t		)DEBUG_TASK_PRIO,    
//							( TaskHandle_t *	)&User_Tasks[DEBUG_TASK]);   //测试任务(不用)
//							
	/* create judge task */
	xTaskCreate(( TaskFunction_t	)JudgeTask,      	
							( const char *		)"Judge Task",     
							( uint16_t			)JUDGE_TASK_SIZE,    
							( void *			)NULL,              
							( UBaseType_t		)JUDGE_TASK_PRIO,    
							( TaskHandle_t *	)&User_Tasks[JUDGE_TASK]);    //裁判任务
	
	/* create graphics task*/
		xTaskCreate(( TaskFunction_t	)GraphicSendtask,      	
							( const char *		)"Graphic Sendtask",     
							( uint16_t			)JUDGE_TASK_SIZE,    
							( void *			)NULL,              
							( UBaseType_t		)JUDGE_TASK_PRIO,    
							( TaskHandle_t *	)&User_Tasks[GRAPHIC_SEND_TASK]);			 //图传任务			
							
		/* 创建IMU云台任务 */
		xTaskCreate((TaskFunction_t)ImugimbalTask,      	
								(const char *	)"Imu cotrol gimbal",     
								(uint16_t			)IMU_GIMBAL_TASK_SIZE,    
								(void *				)NULL,              
								(UBaseType_t		)IMU_GIMBAL_TASK_PRIO,    
								(TaskHandle_t *)&User_Tasks[IMUGIMBAL_TASK]);         //云台任务
		/* 创建射击任务 */								
		xTaskCreate((TaskFunction_t)ShootTask,      	
							(const char *	)"shoot",     
							(uint16_t			)SHOOT_TASK_SIZE,    
							(void *				)NULL,              
							(UBaseType_t		)SHOOT_TASK_PRIO,    
							(TaskHandle_t *)&User_Tasks[SHOOT_TASK]);							//射击任务
		/* 创建控制模式任务 */								
    xTaskCreate((TaskFunction_t)ActionControl_task,         
                (const char *)"ActionControl_task",          
                (uint16_t)CONTROL_STK_SIZE,            
                (void *)NULL,                        
                (UBaseType_t)CONTROL_TASK_PRIO,        
                (TaskHandle_t *)&User_Tasks[CONTROL_TASK]); 			//控制任务
								
//	xTaskCreate((TaskFunction_t)INS_Task,          //任务函数
//                (const char *)"INS_Task",          //任务名称
//                (uint16_t)INS_TASK_STK_SIZE,            //任务堆栈大小
//                (void *)NULL,                        //传递给任务函数的参数
//                (UBaseType_t)INS_TASK_PRIO,        //任务优先级
// 								(TaskHandle_t *)&User_Tasks[INS_TASK]); //任务句柄		
//#ifdef DEBUG_MODE_FREERTOS
//    xTaskCreate((TaskFunction_t)CPU_task,               //任务函数
//                (const char *)"CPU_task",               //任务名称
//                (uint16_t)CPU_TASK_STK_SIZE,            //任务堆栈大小
//                (void *)NULL,                           //传递给任务函数的参数
//                (UBaseType_t)CPU_TASK_PRIO,             //任务优先级
//                (TaskHandle_t *)&User_Tasks[CPU_TASK]); //任务句柄
//#endif // DEBUG_MODE_FREERTOS								
	/* delete start task */
	vTaskDelete( StartTask_Handler );
	taskEXIT_CRITICAL();            //退出临界区
}

/* create start task */
void vCreateStartTask( void )
{
	//开始任务
	xTaskCreate(( TaskFunction_t	)StartTask,      	 			//任务函数
							( const char *		)"SystemStart",     		//任务名称
							( uint16_t			)START_TASK_SIZE,    			//任务堆栈大小
							( void *			)NULL,              				//传递给任务函数的参数
							( UBaseType_t		)START_TASK_PRIO,   			//任务优先级
							( TaskHandle_t *	)&StartTask_Handler);		//任务句柄	
}


/* end */














