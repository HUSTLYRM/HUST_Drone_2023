/* include */
#include "config.h"

/* define */

/* declare */
//static float motor_PID_cal(	pid_t *speedPID,pid_t *positionPID,FuzzyPID *speedPID_fuzzy, FuzzyPID *positionPID_fuzzy,float setPoint,float speedFeedback,float positionFeedback,uint8_t mode );
static float motor_PID_cal(	pid_t *speedPID,pid_t *positionPID,pid_t *qorquePID, float angleFF, float speedFF,float setPoint,float speedFeedback,float positionFeedback,float qorquefeedback,uint8_t mode );
extern struct GM6020 yaw;
extern struct GM6020 pitch;
extern struct M2006  pluck;
extern struct M2006 frimition[2];
extern struct IMU 	 imu;
extern struct state droneState;
extern IMU IMUReceive;
extern IMU_Data_t IMUData;
extern short ContralMode;
extern float new_value; 
extern float old_value; 
extern int RC_IMU_control;
extern int RC_Motor_control;
extern int Grativity;
extern float yaw_ffout,pitch_ffout;
/* GM6020 send data (pitch and yaw) */
void GM6020_control(void)
{
	/* PID */
	if(ContralMode==Rc||ContralMode==Pc)
	{
		if(RC_IMU_control == 1)//陀螺仪反馈控制
		{
			float yawsend;
			yawsend = (-1*motor_PID_cal(&yaw.AutospeedPID,&yaw.AutopositionPID,&yaw.qorquePID,yaw.ffout,0,
																					yaw.setPoint,yaw.receiveSpeed,
																					yaw.Angle_zeroCheck,yaw.qorque,yaw.mode)); //imuYaw
			yaw.sendData = yawsend;
			yaw.sendData = LIMIT_MAX_MIN(yawsend,30000,-30000);
			//Grativity = 250.3*imu.pitchAngle - 7.3829;
			Grativity =0;
			float pitchsend;
			pitchsend = (-1*motor_PID_cal(&pitch.AutospeedPID,&pitch.AutopositionPID,&pitch.qorquePID,pitch.ffout,0,
																					pitch.setPoint,pitch.receiveSpeed,
																					pitch.Angle_zeroCheck,pitch.qorque,pitch.mode)) + Grativity;//imuPitch
			pitch.sendData = pitchsend;
			pitch.sendData = LIMIT_MAX_MIN(pitchsend,30000,-30000);
		}
		
		if(RC_Motor_control  == 1) //纯电机反馈控制
		{
			float yawsend;
			yawsend = (-1*motor_PID_cal(&yaw.Motor_SpeedPID,&yaw.Motor_PositionPID,&yaw.qorquePID,yaw.ffout,0,
																					yaw.setPoint,yaw.receiveSpeed,
																					yaw.Angle_zeroCheck,yaw.qorque,doubleLoop));
			yaw.sendData = yawsend;
			yaw.sendData = LIMIT_MAX_MIN(yawsend,30000,-30000);

			float pitchsend;
			pitchsend = (-1*motor_PID_cal(&pitch.Motor_SpeedPID,&pitch.Motor_PositionPID,&pitch.qorquePID,pitch.ffout,0,
																					pitch.setPoint,pitch.receiveSpeed,
																					pitch.Angle_zeroCheck,pitch.qorque,doubleLoop));
			pitch.sendData = pitchsend;
			pitch.sendData = LIMIT_MAX_MIN(pitchsend,30000,-30000);

		}
	}
  if(ContralMode==Auto)
	{
		float yawsend;
		yawsend = (-1*motor_PID_cal(&yaw.AutospeedPID,&yaw.AutopositionPID,&yaw.qorquePID,yaw.ffout,0,
																			  yaw.setPoint,yaw.receiveSpeed,
																			  yaw.Angle_zeroCheck,yaw.qorque,yaw.mode)); //imuYaw
		yaw.sendData = yawsend;
		yaw.sendData = LIMIT_MAX_MIN(yawsend,30000,-30000);
		//Grativity = 250.3*imu.pitchAngle - 7.3829;
		float pitchsend;
		pitchsend = (-1*motor_PID_cal(&pitch.AutospeedPID,&pitch.AutopositionPID,&pitch.qorquePID,pitch.ffout,0,
																			  pitch.setPoint,pitch.receiveSpeed,
																			  pitch.Angle_zeroCheck,pitch.qorque,pitch.mode)) + Grativity;//imuPitch
		pitch.sendData = pitchsend;
	  pitch.sendData = LIMIT_MAX_MIN(pitchsend,30000,-30000);
	}
	if(ContralMode == Lost)
	{
		/*
			掉电模式下电机电流为0,下电
		*/
		yaw.sendData = 0;
		pitch.sendData = 0;
	}
	GM6020_Send_can1();
}

void GM6020_Send_can1()
{
	CanTxMsg tx_message;
	tx_message.IDE = CAN_ID_STD;    
	tx_message.RTR = CAN_RTR_DATA; 
	tx_message.DLC = 0x08;
	tx_message.StdId = 0x1FF;

	yaw.sendData = LIMIT_MAX_MIN(yaw.sendData,30000,-30000);
	pitch.sendData = LIMIT_MAX_MIN(pitch.sendData,30000,-30000) ;
	
	/* OX1FF标识符对应的ID:205-208 */
	//yaw.receiveId=207(3)  pitch.receiveId=206(2)
		/* Yaw motor control data (ID = 207 -- BIT:011) */
	tx_message.Data[2] = pitch.sendData>>8&0xff;
	tx_message.Data[3] = pitch.sendData&0xff;
	tx_message.Data[4] = yaw.sendData>>8&0xff;
	tx_message.Data[5] = yaw.sendData&0xff;
	
	CAN_Transmit(CAN1,&tx_message);
}

int test_see_left=0,test_see_right=0;
/* M2006 send data */
void M2006_control(void)
{
	/* PID */
	pluck.sendData = (int16_t)motor_PID_cal(&pluck.speedPID,&pluck.positionPID,&pluck.qorquePID,0,0,
																				pluck.setPoint,pluck.receiveSpeed,
																				pluck.Angle_zeroCheck,pluck.qorque,pluck.mode);
  frimition[0].sendData = (int16_t)motor_PID_cal(&frimition[0].speedPID,&frimition[0].positionPID,&frimition[0].qorquePID,0,0,
																				frimition[0].setPoint,frimition[0].receiveSpeed,
																				frimition[0].Angle_zeroCheck,frimition[0].qorque,frimition[0].mode);
  frimition[1].sendData = (int16_t)motor_PID_cal(&frimition[1].speedPID,&frimition[1].positionPID,&frimition[1].qorquePID,0,0,
																				frimition[1].setPoint,frimition[1].receiveSpeed,
																				frimition[1].Angle_zeroCheck,frimition[1].qorque,frimition[1].mode);
																				
	test_see_left=frimition[1].receiveSpeed;
	test_see_right=frimition[0].receiveSpeed;
																																								
	M2006_Send_Can1();
}

void M2006_Send_Can1()
{
	CanTxMsg tx_message;
	tx_message.IDE = CAN_ID_STD;    
	tx_message.RTR = CAN_RTR_DATA; 
	tx_message.DLC = 0x08;
	tx_message.StdId = 0x200;
	
	pluck.sendData = LIMIT_MAX_MIN(pluck.sendData,10000,-10000);//-10-10A	
#if Plane_ID == 2
	frimition[0].sendData = LIMIT_MAX_MIN(frimition[0].sendData,10000,-10000);	
	frimition[1].sendData = LIMIT_MAX_MIN(frimition[1].sendData,10000,-10000);	
#elif Plane_ID == 1
	frimition[0].sendData = LIMIT_MAX_MIN(frimition[0].sendData,16384,-16384);	
	frimition[1].sendData = LIMIT_MAX_MIN(frimition[1].sendData,16384,-16384);	
#endif	

	/* 发送数据赋值 */

	tx_message.Data[0] = pluck.sendData>>8&0xff;
	tx_message.Data[1] = pluck.sendData&0xff;
	tx_message.Data[2] = frimition[0].sendData>>8&0xff;
	tx_message.Data[3] = frimition[0].sendData&0xff;
	tx_message.Data[4] = frimition[1].sendData>>8&0xff;
	tx_message.Data[5] = frimition[1].sendData&0xff;
	
	CAN_Transmit(CAN1,&tx_message);
}

/* 多种模式PID整合 */
static float motor_PID_cal(	pid_t *speedPID,pid_t *positionPID,pid_t *qorquePID,float angleFF,float speedFF,float setPoint,float speedFeedback,float positionFeedback,float qorquefeedback,uint8_t mode )
{
	if(mode == openLoop)
	{
		return setPoint;
	}
	else if(mode == speedLoop)//速度环 电机
	{
		return pid_calc(speedPID,speedFeedback,setPoint);
	}
	else if(mode == positionLoop)//位置环 电机 
	{
		return pid_calc(positionPID,positionFeedback,setPoint);
	}
	else if(mode ==doubleLoop)//双环 位置环输入速度环,外到内
	{
		float temp;
		temp = pid_calc(positionPID,positionFeedback,setPoint);
		return pid_calc(speedPID,speedFeedback,temp);
	}
	else if(mode == imuPitch) //陀螺仪反馈控制
	{
		float temp;
		temp = pid_calc(positionPID,imu.pitchAngle,setPoint);
		
		return pid_calc(speedPID,imu.pitchSpeed,temp);	
	}
	else if(mode == imuYaw)		//陀螺仪反馈控制
	{
		float temp;
		temp = pid_calc(positionPID,imu.yawAngle ,setPoint);
		return pid_calc(speedPID,imu.yawSpeed,temp);
	}
	else if(mode == tripleloop)//三环控制,电机反馈
	{
		float temp,temptemp;
		temp = pid_calc(positionPID,positionFeedback,setPoint);
		temptemp = pid_calc(speedPID,speedFeedback,temp);
		return pid_calc(qorquePID,qorquefeedback,temptemp);	
	}
	else 
		return 0;
}

/* end */















