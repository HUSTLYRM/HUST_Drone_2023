/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.
*/


#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/* 针对MDK编译器的配置 */
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
	#include <stdint.h>
	extern uint32_t SystemCoreClock;
#endif


/* FreeRTOS 基础配置 */

/* 1->RTOS使用抢占式调度器; 0->RTOS使用协作式调度器（时间片）
 * FreeRTOS 现已不再更新对协作式调度的支持 */
#define configUSE_PREEMPTION											( 1 )

/* 1->使能时间片调度(默认) */
#define configUSE_TIME_SLICING										( 1 )		

/* 运行FreeRTOS的MCU可选择软硬件两种方式调度下一个要执行的任务
 * 1->硬件方式 ; 0->软件方式
 * 软件方式效率较低 但是可以支持所有MCU 且优先级数目无限制
 * 硬件方式效率高 但一般强制限定最大可用优先级数目为32
 * MCU(如STM32)有硬件计算前导零指令 如果所使用的MCU没有这些硬件指令的话此宏应该设置为0！*/
#define configUSE_PORT_OPTIMISED_TASK_SELECTION	 	( 1 )                       

/* 1->使能低功耗tickless模式; 0->保持系统节拍（tick）中断一直运行
 * 开启低功耗模式可能会导致下载出现问题 因为程序在睡眠中*/
#define configUSE_TICKLESS_IDLE										( 0 )   

/* 写入实际的CPU内核时钟频率 也就是CPU指令执行频率 (通常称为Fclk) Fclk为供给CPU内核的时钟信号 
 * CPU主频为 XX MHz 就是指的这个时钟信号 相应的1/Fclk即为CPU时钟周期*/
#define configCPU_CLOCK_HZ						 			( SystemCoreClock )

/* RTOS系统节拍中断的频率(即一秒中断的次数) 每次中断RTOS都会进行任务调度 */
#define configTICK_RATE_HZ					     		(( TickType_t )1000)

/* 可使用的最大优先级 */
#define configMAX_PRIORITIES					  					( 32 )

/* 空闲任务使用的堆栈大小 */
#define configMINIMAL_STACK_SIZE				 		((unsigned short)128)
  
/* 任务名字字符串最大长度 */
#define configMAX_TASK_NAME_LEN										( 16 )

/* 系统节拍计数器变量数据类型(和MCU位数对应)
 * 1->16位无符号整形; 0->32位无符号整形 */
#define configUSE_16_BIT_TICKS										( 0 )                     

/* 空闲任务放弃CPU使用权给其他同优先级的用户任务使能 */
#define configIDLE_SHOULD_YIELD										( 1 )           

/* 启用队列 */
#define configUSE_QUEUE_SETS											( 0 )   

/* 开启任务通知功能(默认开启) */
#define configUSE_TASK_NOTIFICATIONS 				    	( 1 )

/* 使用互斥信号量 */
#define configUSE_MUTEXES						    					( 0 )    

/* 使用递归互斥信号量 */ 
#define configUSE_RECURSIVE_MUTEXES								( 0 )   

/* 使用计数信号量 */
#define configUSE_COUNTING_SEMAPHORES							( 0 )

/* 可以注册的信号量和消息队列个数 */
#define configQUEUE_REGISTRY_SIZE									( 10 )                    

/* 1->编译以下2个函数:
 * configUSE_APPLICATION_TASK_FAGF()
 * xTaskCallApplicationTaskHook() */
#define configUSE_APPLICATION_TASK_TAG		  			( 0 )                      


/* FreeRTOS 内存申请相关配置 */

/* 支持动态内存申请 */
#define configSUPPORT_DYNAMIC_ALLOCATION  	     	( 1 )    
/* 支持静态内存 */
#define configSUPPORT_STATIC_ALLOCATION						( 0	)
/* 系统堆的大小 申请内存函数即从堆(数组)中申请内存 */
#define configTOTAL_HEAP_SIZE					 			((size_t)(10*1024))


/* FreeRTOS HOOK函数相关配置 */

/* 1->使用空闲HOOK（Idle Hook类似于回调函数）; 0->忽略空闲HOOK
 * 空闲HOOK是一个函数 由用户来实现 函数名:void vApplicationIdleHook( void )
 * 这个函数在每个空闲任务周期都会被调用
 * 对于已经删除的RTOS任务 空闲任务可以释放分配给它们的堆栈内存 因此必须保证空闲任务可以被CPU执行
 * 使用空闲HOOK设置CPU进入省电模式是很常见的
 * 不可以调用会引起空闲任务阻塞的API函数*/
#define configUSE_IDLE_HOOK												( 0 )      

/* 1->使用时间片HOOK（Tick Hook）; 0->忽略时间片HOOK
 * 时间片HOOK是一个函数 由用户来实现 函数名:void vApplicationTickHook( void )
 * 时间片中断可以周期性的调用
 * 函数必须非常短小 不能大量使用堆栈
 * 不能调用以”FromISR" 或 "FROM_ISR”结尾的API函数
 * xTaskIncrementTick函数是在xPortSysTickHandler中断函数中被调用的
 * 因此vApplicationTickHook()函数执行的时间必须很短才行*/
#define configUSE_TICK_HOOK												( 0 )	           

/* 使用内存申请失败HOOK函数 */
#define configUSE_MALLOC_FAILED_HOOK							( 0 ) 

/*
 * 使用堆栈溢出检测功能(一般仅调试使用)
 * 如果使用此功能 用户必须提供一个栈溢出HOOK函数
 * 如果使用的话此值可以为1或者2 (有两种栈溢出检测方法) */
#define configCHECK_FOR_STACK_OVERFLOW						( 0 )


/* FreeRTOS 运行时间和任务状态收集相关配置 */

/* 启用运行时间统计功能(调试使用) */
extern uint32_t FreeRTOSRunTimeTicks;
#define configGENERATE_RUN_TIME_STATS		 	       	( 1 )                     
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()	ConfigureTimeForRunTimeStatus()
#define portGET_RUN_TIME_COUNTER_VALUE()					FreeRTOSRunTimeTicks

/* 启用可视化跟踪调试 */
#define configUSE_TRACE_FACILITY				      		( 1 ) 

/* 与宏configUSE_TRACE_FACILITY同时为1时会编译下面3个函数
 * prvWriteNameToBuffer()
 * vTaskList()
 * vTaskGetRunTimeStats()*/
#define configUSE_STATS_FORMATTING_FUNCTIONS			( 1 )                      


/* FreeRTOS 协程相关配置 */

/* 启用协程 启用协程以后必须添加文件croutine.c */
#define configUSE_CO_ROUTINES 			          		( 0 )

/* 协程的有效优先级数目 */
#define configMAX_CO_ROUTINE_PRIORITIES       		( 2 )                   


/* FreeRTOS 软件定时器相关配置 */

/* 启用软件定时器 */
#define configUSE_TIMERS				            			( 0 )                              

/* 软件定时器优先级 */
#define configTIMER_TASK_PRIORITY		     ( configMAX_PRIORITIES-1 )        

/* 软件定时器队列长度 */
#define configTIMER_QUEUE_LENGTH		        			( 10 )                              

/* 软件定时器任务堆栈大小 */
#define configTIMER_TASK_STACK_DEPTH	 ( configMINIMAL_STACK_SIZE*2 )    


/* 可选择编译的函数配置(1:编译 0:不编译)
下划线后配置目标函数的函数名 */
#define INCLUDE_vTaskPrioritySet				1
#define INCLUDE_uxTaskPriorityGet				1
#define INCLUDE_vTaskDelete							1
#define INCLUDE_vTaskCleanUpResources		1
#define INCLUDE_vTaskSuspend						1
#define INCLUDE_vTaskDelayUntil					1
#define INCLUDE_vTaskDelay							1
#define INCLUDE_eTaskGetState		    		1
#define INCLUDE_xTimerPendFunctionCall  0


/* FreeRTOS 中断相关配置 */

#ifdef __NVIC_PRIO_BITS
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS   4  
#endif

/* 中断最低优先级 */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			15

/* 系统可管理的最高中断优先级 */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5

#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
	
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }
	
#define vPortSVCHandler 				SVC_Handler
#define xPortPendSVHandler 				PendSV_Handler
#define xPortSysTickHandler 			SysTick_Handler


#endif /* FREERTOS_CONFIG_H */

