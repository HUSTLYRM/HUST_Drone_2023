/* include */
#include "config.h"
extern RC_Ctl_t 							RC_Ctl;
ext_student_interactive_header_data_t   send_message;
/* define */
int judge_send_cnt=0;
int judge_send_fire=0;
void JudgeTask( void * pvParameters )
{
	
	/* for delayuntil previous be waked time */
	TickType_t PreviousWakeTime;
	/* ms to freertos ticks */
	const TickType_t TimeIncrement = pdMS_TO_TICKS(JudgeData_Period);
	/* get now systick count */
	PreviousWakeTime = xTaskGetTickCount();
	static short Lastv=0;
	static short Lastb=0;
	for(;;)
	{
		/* send judge osd data to judgement */
//		if (RC_Ctl.key.v == 1 && Lastv == 0)
//		{
//		
//		if(judge_send_cnt==0){
//		  JudgeSentey(0x10);
//			judge_send_cnt=1;
//			}else if(judge_send_cnt==1){
//			JudgeSentey(0x20);
//			judge_send_cnt=0;
//			}
//				//JudgeSentey(0x000);
//		}
//		Lastv=RC_Ctl.key.v;
//				/* send judge osd data to judgement */

//		
//		if (RC_Ctl.key.b == 1 && Lastb == 0)
//		{
//		if(judge_send_fire==0){
//			JudgeSentey(0x02);
//			judge_send_fire=1;
//		}else if(judge_send_fire==1){
//			judge_send_fire=0;
//		  JudgeSentey(0x00);
//			}
//			
//			//	JudgeSentey(0x10);
//		}
//		Lastb=RC_Ctl.key.b;
		
		
		
		
		
		/* delay with certain period */
		vTaskDelay(100);
	}
}
	

/* end */













