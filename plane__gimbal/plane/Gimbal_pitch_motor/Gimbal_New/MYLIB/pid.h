#ifndef __pid_H__
#define __pid_H__

#include "config.h"


typedef  struct pid_t
{
  float p;
  float i;
  float d;

  float set;
  float get;
  float err[3];

  float pout;
  float iout;
  float dout;
  float out;

  float input_max_err;    //input max err;
  float output_deadband;  //output deadband; 
  
  uint32_t pid_mode;
  uint32_t max_out;
  uint32_t integral_limit;

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
		

#endif

		
/* end */		

		
		
		
		
		
		
		
