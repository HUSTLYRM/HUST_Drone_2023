/* include */
#include "config.h"

/* define */
short MotorPositionKp,MotorPositionKi;
RC_Ctl_t 							RC_Ctl;
PC_Recv_t 						PC_Recv;
tGameInfo 						JU_Recv;
tGraphConfig_integer	Graph_config_integer;
ext_student_conmunicate_header_data_t SendToSentry;
struct GM6020 yaw;//云台电机yaw轴,yaw为航向
struct GM6020 pitch;//云台电机pitch轴,pitch为俯仰角
struct M2006  pluck;//电机
struct M2006  frimition[2];
struct IMU 		imu;
unsigned char SaveBuffer[2*USART1_JUD_DMA_receive_buffersize+5] = {0};
FloatlongType position_x,position_y,position_z,position_yaw;
FloatlongType bulletSpeed;
extern int Shoot;
/* declare */
extern unsigned char SendToPC_Buff[USART4_PC_DMA_send_buffersize];
extern unsigned char SendToAno_Buff[USART2_ANO_DMA_send_buffersize];
extern unsigned char ReceiveFromJUD_Buff[USART1_JUD_DMA_receive_buffersize];
extern unsigned char SendToSentey_Buff[USART1_JUD_DMA_send_buffersize];
extern ext_student_interactive_header_data_t   send_message;


float yaw_feedback_angle;
float pitch_feedback_angle;

/* RC data solve */
void RemoteReceive(volatile unsigned char rx_buffer[])
{
	RC_Ctl.rc.ch0 = (rx_buffer[0]| (rx_buffer[1] << 8)) & 0x07ff; 
	RC_Ctl.rc.ch1 = ((rx_buffer[1] >> 3) | (rx_buffer[2] << 5)) & 0x07ff; 
	RC_Ctl.rc.ch2 = ((rx_buffer[2] >> 6) | (rx_buffer[3] << 2) | (rx_buffer[4] << 10)) & 0x07ff;
	RC_Ctl.rc.ch3 = ((rx_buffer[4] >> 1) | (rx_buffer[5] << 7)) & 0x07ff;
	RC_Ctl.rc.s1 = ((rx_buffer[5] >> 4)& 0x0003); //LEFT 
	RC_Ctl.rc.s2 = ((rx_buffer[5] >> 6)& 0x0003); //RIGHT 
	
	if((RC_Ctl.rc.ch0-1024<15)&&(RC_Ctl.rc.ch0-1024>-15)) RC_Ctl.rc.ch0=1024;
	if((RC_Ctl.rc.ch1-1024<15)&&(RC_Ctl.rc.ch1-1024>-15)) RC_Ctl.rc.ch1=1024;
	if((RC_Ctl.rc.ch2-1024<10)&&(RC_Ctl.rc.ch2-1024>-10)) RC_Ctl.rc.ch2=1024;
	if((RC_Ctl.rc.ch3-1024<10)&&(RC_Ctl.rc.ch3-1024>-10)) RC_Ctl.rc.ch3=1024;	
	
	RC_Ctl.mouse.x = rx_buffer[6] | (rx_buffer[7] << 8); 
	RC_Ctl.mouse.y = rx_buffer[8] | (rx_buffer[9] << 8);
	RC_Ctl.mouse.z = rx_buffer[10] | (rx_buffer[11] << 8);
	RC_Ctl.mouse.press_l = rx_buffer[12]; 
	RC_Ctl.mouse.press_r = rx_buffer[13];
	RC_Ctl.key.w = rx_buffer[14]&0x01;
	RC_Ctl.key.s = (rx_buffer[14]>>1)&0x01;
	RC_Ctl.key.a = (rx_buffer[14]>>2)&0x01;
	RC_Ctl.key.d = (rx_buffer[14]>>3)&0x01;
	RC_Ctl.key.shift =(rx_buffer[14]>>4)&0x01;
	RC_Ctl.key.ctrl = (rx_buffer[14]>>5)&0x01;
	RC_Ctl.key.q = (rx_buffer[14]>>6)&0x01;
	RC_Ctl.key.e = (rx_buffer[14]>>7)&0x01;	
	RC_Ctl.key.r = (rx_buffer[15])&0x01;
	RC_Ctl.key.f = (rx_buffer[15]>>1)&0x01;
	RC_Ctl.key.g = (rx_buffer[15]>>2)&0x01; 
	RC_Ctl.key.z = (rx_buffer[15]>>3)&0x01;
	RC_Ctl.key.x = (rx_buffer[15]>>4)&0x01;
	RC_Ctl.key.c = (rx_buffer[15]>>5)&0x01;
	RC_Ctl.key.v = (rx_buffer[15]>>6)&0x01;
	RC_Ctl.key.b = (rx_buffer[15]>>7)&0x01;

	RC_Ctl.DisConnect = 0;
	/* update receive time for selfcheck */
	RC_Ctl.updateTime = xTaskGetTickCount();
}

/* UART4 receive PC data */
void PCReceive(unsigned char PCReceivebuffer[])//辅瞄接收函数
{
	/* Gimbal pos data */
	if(PCReceivebuffer[0] == '!'&&PCReceivebuffer[1]==0x05)
	{
	  PC_Recv.PCTargetPitch = (float)((short)((PCReceivebuffer[2]<<8)|(PCReceivebuffer[3])))/100.0f;//这里不能转为float，不然负数传过来会变为正数
		PC_Recv.PCTargetYaw = ((float)((int)((PCReceivebuffer[4]<<24)|(PCReceivebuffer[5]<<16)|(PCReceivebuffer[6]<<8)|(PCReceivebuffer[7])))/100.0f);
		
		/* update receive time for selfcheck */
		PC_Recv.updateTime = xTaskGetTickCount();
		PC_Recv.PC_Receive_Cnt++;
	}
	
	PC_Recv.DisConnect=0;
}
	float TempPitch,TempYaw;
	short floattoshort1;
	int floattoshort2;
	int pctick=0;
/* UART4 send PC data */
void PCSend(void)
{
	
	/* clear uart DMA trans flag */
	DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);

	SendToPC_Buff[0] = '!';
	
	SendToPC_Buff[1] = 0x05;
	
	/* 这里坐标系各轴正方向需要确定 */
	TempPitch = imu.pitchAngle;		
	//TempPitch = INS.Pitch;
	floattoshort1 = (short)(TempPitch*100);

	SendToPC_Buff[2] = (unsigned char)((floattoshort1>>8) & 0xff);
	SendToPC_Buff[3] = (unsigned char)(floattoshort1 & 0xff);

	TempYaw = imu.yawAngle;
	//TempYaw = INS.YawTotalAngle;
	floattoshort2 = (int)(TempYaw*100);
	
	
	SendToPC_Buff[4] = (unsigned char)((floattoshort2>>24) & 0xff);//高位
	SendToPC_Buff[5] = (unsigned char)((floattoshort2>>16) & 0xff);  
	
	SendToPC_Buff[6] =(unsigned char)((floattoshort2>>8) & 0xff);
	SendToPC_Buff[7] =(unsigned char)((floattoshort2) & 0xff);//低位
	if(JU_Recv.robot_id==106){SendToPC_Buff[8] =(unsigned char)(1);}//蓝飞机
	if(JU_Recv.robot_id==6){SendToPC_Buff[8] =(unsigned char)(0);}	//红飞机
//	SendToPC_Buff[8] =(unsigned char)(JU_Recv.robot_id);

	
	Append_CRC8_Check_Sum(SendToPC_Buff,USART4_PC_DMA_send_buffersize);
	
	if(pctick%5==0)
	DMA_Cmd(DMA1_Stream4, ENABLE);
	
	pctick++;
	
}

/* can1 fifo 0 receive interrupt : gimbalmotor GM6020 */
/* ID : 0x205 0x206(pitch) 0x207(yaw) 0x208 */
void Can1Receive0(CanRxMsg rx_message0)
{
	switch(rx_message0.StdId)
	{
		case 0x206:
		{
	
			pitch.receiveAngle = rx_message0.Data[0]<<8 | rx_message0.Data[1];				
			pitch.receiveSpeed = rx_message0.Data[2]<<8 | rx_message0.Data[3];
			pitch.qorque = rx_message0.Data[4]<<8 | rx_message0.Data[5]; //Current
		
			pitch.Angle_zeroCheck = Get_ZeroCheck_Value_Interface(&pitch.zero,pitch.receiveAngle);
			//pitch.feedback_angle = pitch.Angle_zeroCheck/8192;
			
			/* update receive time for selfcheck */
			pitch.updateTime = xTaskGetTickCount();

			break;
		}
		case 0x207:
		{
			yaw.receiveAngle = rx_message0.Data[0]<<8 | rx_message0.Data[1];
			yaw.receiveSpeed = rx_message0.Data[2]<<8 | rx_message0.Data[3];
			yaw.qorque = rx_message0.Data[4]<<8 | rx_message0.Data[5];
		
			yaw.Angle_zeroCheck = Get_ZeroCheck_Value_Interface(&yaw.zero,yaw.receiveAngle);
			//yaw.feedback_angle = yaw.Angle_zeroCheck/8192;
			
			/* update receive time for selfcheck */
			yaw.updateTime = xTaskGetTickCount();
			break;
		}
	}
}

/* can1 fifo 1 receive interrupt : pluckmotor M2006 */
/* ID : 0x201(√) 0x202 */
void Can1Receive1(CanRxMsg rx_message1)
{
	switch(rx_message1.StdId)
	{
		case 0x201:
		{
			pluck.receiveAngle = rx_message1.Data[0]<<8 | rx_message1.Data[1];
			pluck.receiveSpeed = rx_message1.Data[2]<<8 | rx_message1.Data[3];
			pluck.qorque = rx_message1.Data[4]<<8 | rx_message1.Data[5];
		
			pluck.Angle_zeroCheck = Get_ZeroCheck_Value_Interface(&pluck.zero,pluck.receiveAngle);
			
			/* update receive time for selfcheck */
			pluck.updateTime = xTaskGetTickCount();
			break;
		}
		case 0x202:
		{
			frimition[0].receiveAngle = rx_message1.Data[0]<<8 | rx_message1.Data[1];
			frimition[0].receiveSpeed = rx_message1.Data[2]<<8 | rx_message1.Data[3];
			frimition[0].qorque = rx_message1.Data[4]<<8 | rx_message1.Data[5];
		
			frimition[0].Angle_zeroCheck = Get_ZeroCheck_Value_Interface(&frimition[0].zero,frimition[0].receiveAngle);
			
			/* update receive time for selfcheck */
			frimition[0].updateTime = xTaskGetTickCount();
			break;
		}
		case 0x203:
		{
			frimition[1].receiveAngle = rx_message1.Data[0]<<8 | rx_message1.Data[1];
			frimition[1].receiveSpeed = rx_message1.Data[2]<<8 | rx_message1.Data[3];
			frimition[1].qorque = rx_message1.Data[4]<<8 | rx_message1.Data[5];
		
			frimition[1].Angle_zeroCheck = Get_ZeroCheck_Value_Interface(&frimition[1].zero,frimition[1].receiveAngle);
			
			/* update receive time for selfcheck */
			frimition[1].updateTime = xTaskGetTickCount();
			break;
		}		
	}
}

/* can2 fifo0 receive interrupt */
/* ID : 0x201 0x202 0x203 0x204 */
void Can2Receive0(CanRxMsg rx_message0)
{
	switch(rx_message0.StdId)
	{

	}
}

/* can2 fifo1 receive interrupt : imu */
/* ID : 0x100(√) 0x101(√) */
extern IMU IMUReceive;
void Can2Receive1(CanRxMsg rx_message1)
{
	switch(rx_message1.StdId)
	{
		case 0x100:
		{
//		  memcpy(&IMUReceive.Gyro[0], &rx_message1.Data[0], 2);
//			memcpy(&IMUReceive.Gyro[1], &rx_message1.Data[2], 2);
//			memcpy(&IMUReceive.Gyro[2], &rx_message1.Data[4], 2);
			memcpy(&imu.pitchAngle, &rx_message1.Data[0], 4);
			memcpy(&imu.pitchSpeed, &rx_message1.Data[4], 4);
			/* update receive time for selfcheck */
			imu.updateTime = xTaskGetTickCount();
			break;
		}
		 
		case 0x101:
		{
//      memcpy(&IMUReceive.Acc[0], &rx_message1.Data[0], 2);
//			memcpy(&IMUReceive.Acc[1], &rx_message1.Data[2], 2);
//			memcpy(&IMUReceive.Acc[2], &rx_message1.Data[4], 2);		
			memcpy(&imu.yawAngle, &rx_message1.Data[0], 4);
			memcpy(&imu.yawSpeed, &rx_message1.Data[4], 4);
			imu.yawSpeed = -imu.yawSpeed;
			imu.yawAngle = -imu.yawAngle;
			/* update receive time for selfcheck */
			imu.updateTime = xTaskGetTickCount();		
		  break;
		}
	}
}

/* start once usart1 dma send (for judgement) */
static void USART1_JUDGE_DMA_SEND(uint16_t sendsize)
{
	DMA_Cmd(DMA2_Stream7, DISABLE);
	/* set dma sendbuffer size */
	DMA_SetCurrDataCounter(DMA2_Stream7, sendsize);
	/* start usart dma send */
	DMA_Cmd(DMA2_Stream7, ENABLE);
}

/* start once usart2 dma send (for ANO) */
static void USART2_ANO_DMA_SEND(uint16_t sendsize)
{
	DMA_Cmd(DMA1_Stream6, DISABLE);
	/* set dma sendbuffer size */
	DMA_SetCurrDataCounter(DMA1_Stream6, sendsize);
	/* start usart dma send */
	DMA_Cmd(DMA1_Stream6, ENABLE);
}

/* accroding to ANO earth station V6 protocol: userdata */
void ANOSend(void)
{
	static uint8_t ANO_dataid = 1;
	static uint16_t test_count = 0;
	uint8_t senddata_cnt = 0;
	uint8_t sumcheck = 0;
	uint8_t errorcode_sum = 0;
	int16_t yawAngle_100 = 0; 
	int16_t pitchAngle_100 = 0; 
	int16_t pctarget_yawAngle_100 = 0; 
	int16_t pctarget_pitchAngle_100 = 0; 
	int16_t sintest_data_100 = 0;	

	switch(ANO_dataid)
	{
		/* id1 (F1) send gimbal angle */ 
		case 1:
		{
			senddata_cnt = 0;
			errorcode_sum = 0;
			
			SendToAno_Buff[senddata_cnt++] = 0xAA;
			SendToAno_Buff[senddata_cnt++] = 0x05;
			SendToAno_Buff[senddata_cnt++] = 0xAF;
			SendToAno_Buff[senddata_cnt++] = (0xF0+ANO_dataid);
			
			/* jump over the data length byte */
			senddata_cnt++;
			
			/* int16_t yawAngle */
			yawAngle_100 = (int16_t)(imu.yawAngle*100);
			SendToAno_Buff[senddata_cnt++] = BYTE1(yawAngle_100);
			SendToAno_Buff[senddata_cnt++] = BYTE0(yawAngle_100);
					 
			/* int16_t pitchAngle */
			pitchAngle_100 = (int16_t)(imu.pitchAngle*100);
			SendToAno_Buff[senddata_cnt++] = BYTE1(pitchAngle_100);
			SendToAno_Buff[senddata_cnt++] = BYTE0(pitchAngle_100);
						
			SendToAno_Buff[4] = senddata_cnt-5;
			
			/* sum check calculate */
			sumcheck = 0;
			
			for(uint8_t i=0; i < (SendToAno_Buff[4]+5); i++)
			{
				sumcheck += SendToAno_Buff[i]; 
			}
			
			SendToAno_Buff[senddata_cnt++] = sumcheck;
			
			USART2_ANO_DMA_SEND(senddata_cnt);
		
			break;
		}
		
		/* id2 (F2) send PC data */ 
		case 2:
		{
			senddata_cnt = 0;
			
			SendToAno_Buff[senddata_cnt++] = 0xAA;
			SendToAno_Buff[senddata_cnt++] = 0x05;
			SendToAno_Buff[senddata_cnt++] = 0xAF;
			SendToAno_Buff[senddata_cnt++] = (0xF0+ANO_dataid);
			
			/* jump over the data length byte */
			senddata_cnt++;
			
			/* int16_t PCtarget_yawangle */
			pctarget_yawAngle_100 = (int16_t)(PC_Recv.PCTargetYaw*100);
			SendToAno_Buff[senddata_cnt++] = BYTE1(pctarget_yawAngle_100);
			SendToAno_Buff[senddata_cnt++] = BYTE0(pctarget_yawAngle_100);
			
			/* int16_t PCtarget_pitchangle */
			pctarget_pitchAngle_100 = (int16_t)(PC_Recv.PCTargetPitch*100);
			SendToAno_Buff[senddata_cnt++] = BYTE1(pctarget_pitchAngle_100);
			SendToAno_Buff[senddata_cnt++] = BYTE0(pctarget_pitchAngle_100);
						
			SendToAno_Buff[4] = senddata_cnt-5;
			
			/* sum check calculate */
			sumcheck = 0;
			
			for(uint8_t i=0; i < (SendToAno_Buff[4]+5); i++)
			{
				sumcheck += SendToAno_Buff[i]; 
			}
			
			SendToAno_Buff[senddata_cnt++] = sumcheck;
			
			USART2_ANO_DMA_SEND(senddata_cnt);
			
			break;
		}
		
		/* id2 (F3) send test data */ 
		case 3:
		{
			senddata_cnt = 0;
			test_count++;
			
			SendToAno_Buff[senddata_cnt++] = 0xAA;
			SendToAno_Buff[senddata_cnt++] = 0x05;
			SendToAno_Buff[senddata_cnt++] = 0xAF;
			SendToAno_Buff[senddata_cnt++] = (0xF0+ANO_dataid);
			
			/* jump over the data length byte */
			senddata_cnt++;
			
			/* int16_t test_sindata 100times */
			sintest_data_100 = (int16_t)(sin((float)test_count*3.14f/180.0f)*100);
			SendToAno_Buff[senddata_cnt++] = BYTE1(sintest_data_100);
			SendToAno_Buff[senddata_cnt++] = BYTE0(sintest_data_100);
									
			SendToAno_Buff[4] = senddata_cnt-5;
			
			/* sum check calculate */
			sumcheck = 0;
			
			for(uint8_t i=0; i < (SendToAno_Buff[4]+5); i++)
			{
				sumcheck += SendToAno_Buff[i]; 
			}
			
			SendToAno_Buff[senddata_cnt++] = sumcheck;
			
			USART2_ANO_DMA_SEND(senddata_cnt);
			
			break;
		}
	}

	ANO_dataid++;
	
	/* choose the return number */ 
	if(ANO_TEST)
	{
		if(ANO_dataid >= 4)
		{
			ANO_dataid = 1;
		}			
	}
	else if(!ANO_TEST)
	{
		if(ANO_dataid >= 3)
		{
			ANO_dataid = 1;
		}
	}
}

/* judgement communicate (receive) */
void JudgeReceive(unsigned char ReceiveBuffer[])
{
	short PackPoint = 0;
	uint16_t DataLen = 0;
	uint16_t cmd_id;

	/* copy receive data to double size buffer(back half) */
	memcpy(&SaveBuffer[USART1_JUD_DMA_receive_buffersize],&ReceiveBuffer[0],USART1_JUD_DMA_receive_buffersize);

	for(PackPoint=0;PackPoint<USART1_JUD_DMA_receive_buffersize;PackPoint++)
	{
		/* head byte (2020) */
		if(SaveBuffer[PackPoint]==0xA5)//SOF 0xA5
		{
			/* frame_header (5 bytes) */
			if((Verify_CRC8_Check_Sum(&SaveBuffer[PackPoint],5)==1))
			{
				//获取cmd_id
				cmd_id = (SaveBuffer[PackPoint+6])&0xff;
				cmd_id = (cmd_id<<8)|SaveBuffer[PackPoint+5];
				//获取数据长度
				DataLen = SaveBuffer[PackPoint+2]&0xff;
				DataLen = (DataLen<<8)|SaveBuffer[PackPoint+1];
				
				/* robot status data */
				if((cmd_id==0x0201)&&(Verify_CRC16_Check_Sum(&SaveBuffer[PackPoint],DataLen+9)))//0x0201主控发送机器人状态
				{
					JU_Recv.robot_id = SaveBuffer[PackPoint+7];
				}

				/* robot position data (meter as unit) */
				/* CRC16 whole bag check, so datalen + 9 */				
				if((cmd_id==0x0203)&&(Verify_CRC16_Check_Sum(&SaveBuffer[PackPoint],DataLen+9)))//0x203主控发送机器人位置
				{
					position_x.idata = SaveBuffer[PackPoint+10];
					position_x.idata = (position_x.idata<<8)|SaveBuffer[PackPoint+9];
					position_x.idata = (position_x.idata<<8)|SaveBuffer[PackPoint+8];
					position_x.idata = (position_x.idata<<8)|SaveBuffer[PackPoint+7]; 

					position_y.idata = SaveBuffer[PackPoint+14];
					position_y.idata = (position_y.idata<<8)|SaveBuffer[PackPoint+13];
					position_y.idata = (position_y.idata<<8)|SaveBuffer[PackPoint+12];
					position_y.idata = (position_y.idata<<8)|SaveBuffer[PackPoint+11]; 

					position_z.idata = SaveBuffer[PackPoint+18];
					position_z.idata = (position_z.idata<<8)|SaveBuffer[PackPoint+17];
					position_z.idata = (position_z.idata<<8)|SaveBuffer[PackPoint+16];
					position_z.idata = (position_z.idata<<8)|SaveBuffer[PackPoint+15]; 
					
					position_yaw.idata = SaveBuffer[PackPoint+22];
					position_yaw.idata = (position_yaw.idata<<8)|SaveBuffer[PackPoint+21];
					position_yaw.idata = (position_yaw.idata<<8)|SaveBuffer[PackPoint+20];
					position_yaw.idata = (position_yaw.idata<<8)|SaveBuffer[PackPoint+19];
					
					JU_Recv.position_x = position_x.fdata;
					JU_Recv.position_y = position_y.fdata;
					JU_Recv.position_z = position_z.fdata;
					JU_Recv.position_yaw = position_yaw.fdata;
					

				}
				
				/* drone energy data */
				if(cmd_id==0x0205 &&(Verify_CRC16_Check_Sum(&SaveBuffer[PackPoint],DataLen+9)))//空中机器人能量状态：0x0205
				{
					JU_Recv.attack_time = SaveBuffer[PackPoint+7];
				}
				
				/* realtime shot data */
				if((cmd_id==0x0207)&&(Verify_CRC16_Check_Sum(&SaveBuffer[PackPoint],DataLen+9)))//实时射击信息：0x0207
				{
					/* small bullet */
					if(SaveBuffer[PackPoint+7]==1)
					{
						JU_Recv.bullet_freq = SaveBuffer[PackPoint+9];//射频
						
						bulletSpeed.idata = SaveBuffer[PackPoint+12];//射速
						bulletSpeed.idata = (bulletSpeed.idata<<8)|SaveBuffer[PackPoint+12];
						bulletSpeed.idata = (bulletSpeed.idata<<8)|SaveBuffer[PackPoint+11];
						bulletSpeed.idata = (bulletSpeed.idata<<8)|SaveBuffer[PackPoint+10];
						
						JU_Recv.bullet_speed = bulletSpeed.fdata;
					}
				}
				
				/* remain bullet number */
				if((cmd_id==0x0208)&&(Verify_CRC16_Check_Sum(&SaveBuffer[PackPoint],DataLen+9)))//子弹剩余发射数：0x0208
				{
					JU_Recv.bullet_remaining_num = SaveBuffer[PackPoint+8];
					JU_Recv.bullet_remaining_num = (JU_Recv.bullet_remaining_num<<8)|SaveBuffer[PackPoint+7];
				}
			}
		}
	}
	/* copy last receive data to double size buffer(front half) */
	memcpy(&SaveBuffer[0],&SaveBuffer[USART1_JUD_DMA_receive_buffersize],USART1_JUD_DMA_receive_buffersize);
}
uint8_t sequ=0;
/* send number to judgement */  
void JudgeSentey(uint8_t datatype)
{
	int i=0;
	static uint16_t len=sizeof(SendToSentry);
	uint16_t cmdid = JudgeSentey_CmdId;
//	uint16_t dataid = JudgeOSD_DataId_1Graph;
	SendToSentry.sender_ID = JU_Recv.robot_id;
	static uint16_t receid = 0;
	SendToSentry.data_cmd_id=0x0200;

	
	
	//哨兵
	if(datatype==0x10){
	if(SendToSentry.sender_ID == RED_DRONE_ID)		SendToSentry.receiver_ID = 7;
	if(SendToSentry.sender_ID == BLUE_DRONE_ID)		SendToSentry.receiver_ID = 107;
		//SendToSentry.data[0]=0x0F&SendToSentry.data[0];
		SendToSentry.data[0]=0xF0|SendToSentry.data[0];
	}
		if(datatype==0x20){
	if(SendToSentry.sender_ID == RED_DRONE_ID)		SendToSentry.receiver_ID = 7;
	if(SendToSentry.sender_ID == BLUE_DRONE_ID)		SendToSentry.receiver_ID = 107;
		SendToSentry.data[0]=0x0F&SendToSentry.data[0];
		SendToSentry.data[0]=0xE0|SendToSentry.data[0];
	}
	
	
	
	
	
	//fire
	if(datatype==0x00){
	if(SendToSentry.sender_ID == RED_DRONE_ID)		SendToSentry.receiver_ID = 7;
	if(SendToSentry.sender_ID == BLUE_DRONE_ID)		SendToSentry.receiver_ID = 107;
		SendToSentry.data[0]=0x0F|SendToSentry.data[0];
	}
		if(datatype==0x02){
	if(SendToSentry.sender_ID == RED_DRONE_ID)		SendToSentry.receiver_ID = 7;
	if(SendToSentry.sender_ID == BLUE_DRONE_ID)		SendToSentry.receiver_ID = 107;
	
		SendToSentry.data[0]=0xF0&SendToSentry.data[0];
		SendToSentry.data[0]=0x0E|SendToSentry.data[0];

	}
	
	
	//SendToSentry.data[0]=0xFF;
	SendToSentry.data[1]=0xFF;
	/* [0]:SOF [3]:SEQ */
	SendToSentey_Buff[0] = 0xA5;
	memcpy(&SendToSentey_Buff[1],(uint8_t*)&len, sizeof(len));
	SendToSentey_Buff[3] = sequ;
	if (sequ == 0xff) sequ=0;
  else sequ++;
	
	Append_CRC8_Check_Sum(SendToSentey_Buff,5);
	memcpy(&SendToSentey_Buff[frameheader_len],(uint8_t*)&cmdid, cmd_len);
	
	memcpy(&SendToSentey_Buff[frameheader_len+cmd_len], &SendToSentry, sizeof(SendToSentry));
	
	uint16_t frame_length = frameheader_len + cmd_len + sizeof(SendToSentry) + crc_len;
	
	Append_CRC16_Check_Sum(SendToSentey_Buff,frame_length);  //一帧数据校验CRC16
	
	USART_ClearFlag(USART1,USART_FLAG_TC);
	for(i=0;i<frame_length;i++)
	{
	  USART_SendData(USART1,SendToSentey_Buff[i]);
	  while (USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET); //等待之前的字符发送完成
	}
}


void AppReceive(unsigned char AppReceiveBuffer[])
{
	for(int i = 0 ; AppReceiveBuffer[i] != 0;i++)
	{
		USART_SendData(USART2,AppReceiveBuffer[i]);
	}
}

//void App(unsigned char AppReceiveBuffer[])
//{

//}

/* end */










