#ifndef __SELFCHECK__
#define __SELFCHECK__

#include "config.h"


/* 位域用法: 为了节省内存空间 可以将一个数据类型拆分成位来使用 */
/* 冒号后面表示该位域所占的位数 如下:每个位域长度为2Bit */
/* 注意: 位域必须存储在同一个类型中 不能跨类型 同时也说明位域的长度不会超过所定义类型的长度
   如果一个定义类型单元里所剩空间无法存放下一个域 则下一个域应该从下一单元开始存放 */
struct compose {
	/* 注意最先定义的位域在数据的末尾(即从后向前填充) */
	unsigned int IMU_disconneced 			: 2; //错误代码:0x0001
	unsigned int RC_disconneced 			: 2; //错误代码:0x0040

	unsigned int yaw_disconneced 			: 2; 
	unsigned int pitch_disconneced 		: 2;
    
	unsigned int Pluck_disconneced 		: 2;
	
} __attribute__ ((__packed__));

typedef struct state
{
	/* union中的内容指向同一个地址 */
	union
	{
		unsigned short droneState;//代替composeState
		struct compose composeState;//状态标志位
	}drone;
	
	uint8_t Gimbal_poweroff;
	uint8_t AimAssist;
	uint8_t PC_Received;
	
}state;


void SelfcheckTask( void * pvParameters );


#endif


/* end */
















