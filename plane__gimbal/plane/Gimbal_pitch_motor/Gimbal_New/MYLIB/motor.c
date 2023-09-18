/* include */
#include "config.h"

/* define */

/* declare */
static float motor_PID_cal(	pid_t *speedPID,pid_t *positionPID,float setPoint,float speedFeedback,float positionFeedback,uint8_t mode );
extern struct GM6020 yaw;
extern struct GM6020 pitch;
extern struct M2006  pluck;
extern struct IMU 	 imu;
extern int ContralMode;
extern struct state droneState;
/* GM6020 send data (pitch and yaw) */
void GM6020_control(void)
{
	/* PID */
	if(ContralMode==Rc)
	{
		yaw.sendData = (short)motor_PID_cal(&yaw.speedPID,&yaw.positionPID,
																			  yaw.setPoint,yaw.receiveSpeed,
																			  yaw.Angle_zeroCheck,yaw.mode);
	
	  pitch.sendData = (short)motor_PID_cal(&pitch.speedPID,&pitch.positionPID,
																			  pitch.setPoint,pitch.receiveSpeed,
																			  pitch.Angle_zeroCheck,pitch.mode);
	}
	if(ContralMode==Pc)
	{
		yaw.sendData = (short)motor_PID_cal(&yaw.PcspeedPID,&yaw.PcpositionPID,
																			  yaw.setPoint,yaw.receiveSpeed,
																			  yaw.Angle_zeroCheck,yaw.mode);
	
	  pitch.sendData = (short)motor_PID_cal(&pitch.PcspeedPID,&pitch.PcpositionPID,
																			  pitch.setPoint,pitch.receiveSpeed,
																			  pitch.Angle_zeroCheck,pitch.mode);
	}
  if(ContralMode==Auto)
	{
		yaw.sendData = (short)motor_PID_cal(&yaw.AutospeedPID,&yaw.AutopositionPID,
																			  yaw.setPoint,yaw.receiveSpeed,
																			  yaw.Angle_zeroCheck,yaw.mode);
	
	  pitch.sendData = (short)motor_PID_cal(&pitch.AutospeedPID,&pitch.AutopositionPID,
																			  pitch.setPoint,pitch.receiveSpeed,
																			  pitch.Angle_zeroCheck,pitch.mode);
	}

	CanTxMsg tx_message;
	tx_message.IDE = CAN_ID_STD;    
	tx_message.RTR = CAN_RTR_DATA; 
	tx_message.DLC = 0x08;
	tx_message.StdId = 0x1FF;

	yaw.sendData = LIMIT_MAX_MIN(yaw.sendData,30000,-30000);
	pitch.sendData = LIMIT_MAX_MIN(pitch.sendData,30000,-30000);
	
	/* OX1FF标识符对应的ID:205-208 */
	if((pitch.receiveId == 0x206)&&(yaw.receiveId == 0x207))
	{
		/* Pitch motor control data (ID = 206 -- BIT:010) */
		tx_message.Data[0] = pitch.sendData>>8&0xff;
		tx_message.Data[1] = pitch.sendData&0xff;
		/* Yaw motor control data (ID = 207 -- BIT:011) */
		tx_message.Data[2] = yaw.sendData>>8&0xff;
		tx_message.Data[3] = yaw.sendData&0xff;
	}
		
	CAN_Transmit(CAN1,&tx_message);
}


/* Protocol 1.0 */

/* MX64 action commend */
void send_MX64_action(void)
{
	uint8_t i = 0;
	uint8_t MX64_buffer[6];
	
	/* head */
	MX64_buffer[0]=0xFF;	
	MX64_buffer[1]=0xFF;	
	
	MX64_buffer[2]=0xFE;	
	MX64_buffer[3]=0x02;	
	MX64_buffer[4]=0x05;	//commend:action
	
	/* check */
	MX64_buffer[5]=0xFA;	
	
	for(i=0;i<=5;i++)
	{
		USART_SendData(USART2,MX64_buffer[i]);
		while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);
	}
}

/* MX64 position set commend */
void send_MX64_position(unsigned short Control_Angle)
{
	uint8_t j = 0;
	uint8_t MX64_buffer[15];
	
	/* head */
	MX64_buffer[0]=0xFF;
	MX64_buffer[1]=0xFF;
	
	MX64_buffer[2]=0xFE;	//motor id
	MX64_buffer[3]=0x05;	//length
	MX64_buffer[4]=0x04;	//Reg Write:0x04 
	
	/* reg address */
	MX64_buffer[5]=0X1E;	//goal position reg address
	
	/* goal position */
	MX64_buffer[6]=(Control_Angle<<8)>>8;
	MX64_buffer[7]=Control_Angle>>8;
	
	/* check */
	MX64_buffer[8]=~(MX64_buffer[2]+MX64_buffer[3]+MX64_buffer[4]+MX64_buffer[5]+MX64_buffer[6]+MX64_buffer[7]);
	
	for(j=0;j<=8;j++)
	{
		USART_SendData(USART2,MX64_buffer[j]);
		while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);
	}
}

/* MX64 baudrate set */
void set_MX64_baudrate(void)
{
	uint8_t j = 0;
	uint8_t MX64_buffer[15];
	
	/* head */
	MX64_buffer[0]=0xFF;	
	MX64_buffer[1]=0xFF;	
	
	MX64_buffer[2]=0xFE;	//motor id
	MX64_buffer[3]=0x04;	//length
	MX64_buffer[4]=0x03;	//Write:0x03 
	
	/* reg address */
	MX64_buffer[5]=0X04;	//baudrate reg address
	
	/* goal baudrate : 115200 = 2000000/(16+1) */
	MX64_buffer[6]=0x10;
	
	/* check */
	MX64_buffer[7]=~(MX64_buffer[2]+MX64_buffer[3]+MX64_buffer[4]+MX64_buffer[5]+MX64_buffer[6]);
	
	for(j=0;j<=7;j++)
	{
		USART_SendData(USART2,MX64_buffer[j]);
		while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);
	}
}

/* M2006 send data */
void M2006_control(void)
{
	/* PID */
  pluck.sendData = (short)motor_PID_cal(&pluck.speedPID,&pluck.positionPID,
																				pluck.setPoint,pluck.receiveSpeed,
																				pluck.Angle_zeroCheck,pluck.mode);
	
	CanTxMsg tx_message;
	tx_message.IDE = CAN_ID_STD;    
	tx_message.RTR = CAN_RTR_DATA; 
	tx_message.DLC = 0x08;
	tx_message.StdId = 0x200;
	
	pluck.sendData = LIMIT_MAX_MIN(pluck.sendData,10000,-10000);	

	/* 发送数据赋值 */
	switch(pluck.receiveId)
	{
		case 0x201:
			tx_message.Data[0] = pluck.sendData>>8&0xff;
			tx_message.Data[1] = pluck.sendData&0xff;
			break;
	}
	CAN_Transmit(CAN1,&tx_message);
}

/* 多种模式PID整合 */
static float motor_PID_cal(pid_t *speedPID,pid_t *positionPID,
													 float setPoint,float speedFeedback,
													 float positionFeedback,uint8_t mode)
{
	
	if(mode == openLoop)
	{
		return setPoint;
	}
	else if(mode == speedLoop)
	{
		return pid_calc(speedPID,speedFeedback,setPoint);
	}
	else if(mode == positionLoop)
	{
		return pid_calc(positionPID,positionFeedback,setPoint);
	}
	else if(mode ==doubleLoop)
	{
		float temp;
		temp = pid_calc(positionPID,positionFeedback,setPoint);
		return pid_calc(speedPID,speedFeedback,temp);
	}
	else if(mode == imuPitch)
	{
		float temp;
		temp = pid_calc(positionPID,imu.pitchAngle,setPoint);
		return pid_calc(speedPID,imu.pitchSpeed,temp);
	}
	else if(mode == imuYaw)
	{
		float temp;
		temp = pid_calc(positionPID,imu.yawAngle,setPoint);
		return pid_calc(speedPID,imu.yawSpeed,temp);
	}
	else 
		return 0;
}

/* end */















