#ifndef __pid_H__
#define __pid_H__

#include "config.h"


typedef  struct pid_t//普通pid
{
  float p;//比例常数
  float i;//积分常数
  float d;//微分常数

  float set;//设定目标值
  float get;//输出反馈值
  float err[3];//err数组,当前误差,前次误差

  float pout;//比例输出
  float iout;//积分输出
  float dout;//微分输出
  float out;

  float input_max_err;    //input max err;
  float output_deadband;  //output deadband; 
  
  uint32_t pid_mode;//pid模式标志位
  uint32_t max_out;//输出限幅
  uint32_t integral_limit;//积分限制

  void (*f_param_init)(struct pid_t *pid, 
                       uint32_t      pid_mode,
                       uint32_t      max_output,
                       uint32_t      inte_limit,
                       float         p,
                       float         i,
                       float         d);
  void (*f_pid_reset)(struct pid_t *pid, float p, float i, float d);
 
} pid_t;


float pid_calc(pid_t *pid, float fdb, float ref);
void PID_struct_init(pid_t* pid,uint32_t mode,uint32_t maxout,uint32_t intergral_limit,float kp,float ki,float kd);
		



/*
-----------------------------------------模糊pid部分
*/	
/*********模糊pid部分*/
typedef struct FuzzyPID
{
		float SetPoint;			//设定目标值
	
		float ActualValue;  //实际值

    float DeadZone;			//死区控制参数
		
		float LastError;		//前次误差
		float PreError;			//当前误差
		float SumError;			//积分误差
	
		float IMax;					//积分限制
		
		float POut;					//比例输出
		float IOut;					//积分输出
		float DOut;					//微分输出
	  float DOut_last;    //上一次微分输出
		float OutMax;       //限幅
	  float Out;          //总输出
		float Out_last;     //上一次输出
		
		float I_U;          //变速积分上限
		float I_L;          //变速积分下限
		
		float RC_DM;        //微分先行滤波系数
		float RC_DF;        //不完全微分滤波系数
	
	  float Kp0;          //PID初值
	  float Ki0;
  	float Kd0;
	
	  float dKp;          //PID变化量
	  float dKi;
  	float dKd;
	
    float stair ;	      //动态调整梯度   //0.25f
	  float Kp_stair;                      //0.015f
	  float Ki_stair;                      //0.0005f
	  float Kd_stair;                      //0.001f
		
		 void (*fuzzy_param_init)(struct FuzzyPID *   pid,
													uint32_t OutMax,uint32_t IMax,
													float    Kp0,float    Ki0,float    Kd0,
													float		 dKp,float		 dKi,float		 dKd,
													float 	 I_U,float    I_L,float    RC_DM,float 	 RC_DF,	
													float 	 stair,float 	 Kp_stair,float 	 Ki_stair,float 	 Kd_stair);
		void (*fuzzy_pid_reset)(struct FuzzyPID *pid);
	  
} FuzzyPID;
//float FuzzyPID_Calc(FuzzyPID *pid);
float FuzzyPID_Calc(FuzzyPID *P,float SetPoint ,float ActualValue );
void Fuzzy_PID_struct_init(
	FuzzyPID *   pid,
	uint32_t OutMax,
	uint32_t IMax,
	float    Kp0,
	float    Ki0,
	float    Kd0,
	float		 dKp,
	float		 dKi,
	float		 dKd,
	float 	 I_U,
	float    I_L,
	float    RC_DM,
	float 	 RC_DF,	
	float 	 stair,
	float 	 Kp_stair,
	float 	 Ki_stair,
	float 	 Kd_stair);
	
#endif
/* end */		

		
		
		
		
		
		
		
