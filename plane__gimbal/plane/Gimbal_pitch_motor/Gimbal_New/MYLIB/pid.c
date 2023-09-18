/* include */
#include "config.h"

/* limit */
static void abs_limit(float *a, float ABS_MAX)
{
  if (*a > ABS_MAX)
    *a = ABS_MAX;
  if (*a < -ABS_MAX)
    *a = -ABS_MAX;
}

/*  pid init */
static void pid_param_init(
	pid_t*   pid,
	uint32_t mode,
	uint32_t maxout,
	uint32_t intergral_limit,
	float    kp,
	float    ki,
	float    kd)
{
  pid->integral_limit = intergral_limit;
  pid->max_out        = maxout;
  pid->pid_mode       = mode;

  pid->p = kp;
  pid->i = ki;
  pid->d = kd;
}

/*  pid reset */
static void pid_reset(pid_t *pid, float kp, float ki, float kd)
{
  pid->p = kp;
  pid->i = ki;
  pid->d = kd;
  
  pid->pout = 0;
  pid->iout = 0;
  pid->dout = 0;
  pid->out  = 0;
}

/* pid calculate */
float pid_calc(pid_t *pid, float get, float set)
{
	pid->get = get;
	pid->set = set;
	pid->err[0] = set - get;
	pid->input_max_err += pid->err[0];
	
  abs_limit(&pid->input_max_err,pid->integral_limit);	
	
	pid->pout = pid->p * pid->err[0];
	pid->iout = pid->i * pid->input_max_err;
	pid->dout = pid->d * (pid->err[0]-pid->err[1]);
	pid->out = pid->pout+pid->iout+pid->dout;
	
	pid->err[1] = pid->err[0];

	abs_limit(&pid->out,pid->max_out);
	
	return pid->out;
}

/* pid init */
void PID_struct_init(
	pid_t*   pid,
	uint32_t mode,
	uint32_t maxout,
	uint32_t intergral_limit,
	float kp,
	float ki,
	float kd )
{
  pid->f_param_init = pid_param_init;
  pid->f_pid_reset  = pid_reset;

  pid->f_param_init(pid, mode, maxout, intergral_limit, kp, ki, kd);
  pid->f_pid_reset(pid, kp, ki, kd);
}


/* end */















