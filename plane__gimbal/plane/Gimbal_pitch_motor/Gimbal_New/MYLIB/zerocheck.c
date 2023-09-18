/* include */
#include "config.h"

/* get zerocheck data */
float Get_ZeroCheck_Value_Interface(ZeroCheck_Typedef *zero,float value)
{
	return ZeroCheck_cal(zero,value);//调用datareceive的接口函数
}

/* zerocheck init */
void ZeroCheck_Init_Interface(ZeroCheck_Typedef *zeroCheck,float CountCycle,float NowValue)
{
	zeroCheck->ActualValue = NowValue;
	zeroCheck->CountCycle  = CountCycle;
	zeroCheck->LastValue   = 0;
	zeroCheck->PreError    = 0;
	zeroCheck->Circle = 0;
}

/* 将过零检测值置零 */
float ResetZeroCheck(ZeroCheck_Typedef *zeroCheck)
{
	float temp = zeroCheck->CountCycle;
	ZeroCheck_Init_Interface(zeroCheck,temp,0);
	return 1;
}

/* 位置式和速度式过零检测 */
/* 使用此函数前要申明对应检测量结构体的 Zero->CountCycle与Zero->LastValue */

/* paramter:	
		ZeroCheck_Typedef *Zero 	过零检测结构体
			Zero->ActualValue 	表示检测量当前值
			Zero->LastValue 		表示检测量上一次值
			Zero->CountCycle 		表示检测量过零时越变值(即计数周期)
			Zero->PreError 			表示检测量差值
		float value  							待检测量 */
float ZeroCheck_cal(ZeroCheck_Typedef *Zero,float value)
{
	Zero->ActualValue=value;
	
	Zero->PreError=Zero->ActualValue-Zero->LastValue;
	
	Zero->LastValue=Zero->ActualValue;
	
	if(Zero->PreError>0.7f*Zero->CountCycle)
	{
		Zero->PreError=Zero->PreError-Zero->CountCycle;
		Zero->Circle++;
	}
	
	if(Zero->PreError<-0.7f*Zero->CountCycle)
	{
		Zero->PreError=Zero->PreError+Zero->CountCycle;
		Zero->Circle--;
	}
	
	return Zero->ActualValue - Zero->Circle * Zero->CountCycle;
}


/* end */
















