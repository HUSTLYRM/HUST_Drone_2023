#include "config.h"
extern unsigned char JudgeSend[30];
extern JudgeReceive_t JudgeReceive;
extern F405_typedef F405;
extern ChassisSpeed_t chassis;
int Char_Change_Array[4];					//0表示没变化，非0表示有变化
ext_student_interactive_char_header_data_t custom_char_draw;  //自定义字符绘制

/**********************************************************************************************************
*函 数 名: JudgementCustomizeChar
*功能说明: 图形数据包分段打包
*形    参: 操作类型
*返 回 值: 无
**********************************************************************************************************/
void JudgementCustomizeChar(int Op_type)
{
		custom_char_draw.data_cmd_id=0x0110;//绘制字符

		custom_char_draw.sender_ID=JudgeReceive.robot_id;//发送者ID，机器人对应ID
		if(JudgeReceive.robot_id == 103)
				custom_char_draw.receiver_ID = 0x0167;
		if(JudgeReceive.robot_id == 104)
				custom_char_draw.receiver_ID = 0x0168;
		if(JudgeReceive.robot_id == 105)
				custom_char_draw.receiver_ID = 0x0169;
		if(JudgeReceive.robot_id == 3)
				custom_char_draw.receiver_ID = 0x0103;	
		if(JudgeReceive.robot_id == 4)
				custom_char_draw.receiver_ID = 0x0104;
		if(JudgeReceive.robot_id == 5)
				custom_char_draw.receiver_ID = 0x0105;

/*********************************自定义图像数据***********************************/
		referee_data_load_String(Op_type);
}

/**********************************************************************************************************
*函 数 名: referee_data_load_String
*功能说明: 图形数据包装载字符串
*形    参: 操作类型
*返 回 值: 无
**********************************************************************************************************/
float c_pos_x[9] = {0.57,0.34,0.4,0.52,0.34,0.42,0.62,0.5,0.42};
float c_pos_y[9] = {0.65,0.15,0.8,0.1,0.1,0.15,0.1,0.8,0.1};
void referee_data_load_String(int Op_type)
{
	static int tick;
	static char Mag_State[2][6] = {"OPEN","CLOSE"};
	static char Gimbal_State[3][7] = {"NORMAL","TX2","OFF"};
	static char Chassis_State[4][7] = {"OFF","NORMAL","ROTATE","SOLO"};
	static char Laser_State[2][4] = {"ON","OFF"};
	/*初始化操作，轮流生成图层*/
	if(Op_type == Op_Init)
	{
		switch(tick%9)
		{
			/*静态字符*/
			case 0:
			/*******************************pitch 字符*********************************/
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[0] = 0;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[1] = 41;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[2] = 0;
			custom_char_draw.char_custom.grapic_data_struct.operate_tpye=Op_type;
			custom_char_draw.char_custom.grapic_data_struct.graphic_tpye=7;
			custom_char_draw.char_custom.grapic_data_struct.layer=9;
			custom_char_draw.char_custom.grapic_data_struct.color=Orange;
			custom_char_draw.char_custom.grapic_data_struct.start_angle=25;
			custom_char_draw.char_custom.grapic_data_struct.end_angle=strlen("PITCH:");
			custom_char_draw.char_custom.grapic_data_struct.width=3;
			custom_char_draw.char_custom.grapic_data_struct.start_x=c_pos_x[0]*SCREEN_LENGTH;
			custom_char_draw.char_custom.grapic_data_struct.start_y=c_pos_y[0]*SCREEN_WIDTH;
			memset(custom_char_draw.char_custom.data,'\0',sizeof(custom_char_draw.char_custom.data));
			strcpy(custom_char_draw.char_custom.data,"PITCH:");
			break;
			case 1:
			/*******************************Magazine 字符*********************************/
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[0] = 0;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[1] = 41;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[2] = 1;
			custom_char_draw.char_custom.grapic_data_struct.operate_tpye=Op_type;
			custom_char_draw.char_custom.grapic_data_struct.graphic_tpye=7;
			custom_char_draw.char_custom.grapic_data_struct.layer=9;
			custom_char_draw.char_custom.grapic_data_struct.color=Orange;
			custom_char_draw.char_custom.grapic_data_struct.start_angle=25;
			custom_char_draw.char_custom.grapic_data_struct.end_angle=strlen("MAG:");
			custom_char_draw.char_custom.grapic_data_struct.width=3;
			custom_char_draw.char_custom.grapic_data_struct.start_x=c_pos_x[1]*SCREEN_LENGTH;
			custom_char_draw.char_custom.grapic_data_struct.start_y=c_pos_y[1]*SCREEN_WIDTH;
			memset(custom_char_draw.char_custom.data,'\0',sizeof(custom_char_draw.char_custom.data));
			strcpy(custom_char_draw.char_custom.data,"MAG:");
			break;
			case 2:
			/*******************************Chassis 字符*********************************/
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[0] = 0;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[1] = 41;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[2] = 2;
			custom_char_draw.char_custom.grapic_data_struct.operate_tpye=Op_type;
			custom_char_draw.char_custom.grapic_data_struct.graphic_tpye=7;
			custom_char_draw.char_custom.grapic_data_struct.layer=9;
			custom_char_draw.char_custom.grapic_data_struct.color=Orange;
			custom_char_draw.char_custom.grapic_data_struct.start_angle=25;
			custom_char_draw.char_custom.grapic_data_struct.end_angle=strlen("CHASSIS:");
			custom_char_draw.char_custom.grapic_data_struct.width=3;
			custom_char_draw.char_custom.grapic_data_struct.start_x=c_pos_x[2]*SCREEN_LENGTH;
			custom_char_draw.char_custom.grapic_data_struct.start_y=c_pos_y[2]*SCREEN_WIDTH;
			memset(custom_char_draw.char_custom.data,'\0',sizeof(custom_char_draw.char_custom.data));
			strcpy(custom_char_draw.char_custom.data,"CHASSIS:");
			break;
			case 3:
			/*******************************Gimbal 字符*********************************/
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[0] = 0;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[1] = 41;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[2] = 3;
			custom_char_draw.char_custom.grapic_data_struct.operate_tpye=Op_type;
			custom_char_draw.char_custom.grapic_data_struct.graphic_tpye=7;
			custom_char_draw.char_custom.grapic_data_struct.layer=9;
			custom_char_draw.char_custom.grapic_data_struct.color=Orange;
			custom_char_draw.char_custom.grapic_data_struct.start_angle=25;
			custom_char_draw.char_custom.grapic_data_struct.end_angle=strlen("GIMBAL:");
			custom_char_draw.char_custom.grapic_data_struct.width=3;
			custom_char_draw.char_custom.grapic_data_struct.start_x=c_pos_x[3]*SCREEN_LENGTH;
			custom_char_draw.char_custom.grapic_data_struct.start_y=c_pos_y[3]*SCREEN_WIDTH;
			memset(custom_char_draw.char_custom.data,'\0',sizeof(custom_char_draw.char_custom.data));
			strcpy(custom_char_draw.char_custom.data,"GIMBAL:");
			break;
			case 4:
			/*******************************Laser 字符*********************************/
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[0] = 0;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[1] = 41;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[2] = 4;
			custom_char_draw.char_custom.grapic_data_struct.operate_tpye=Op_type;
			custom_char_draw.char_custom.grapic_data_struct.graphic_tpye=7;
			custom_char_draw.char_custom.grapic_data_struct.layer=9;
			custom_char_draw.char_custom.grapic_data_struct.color=Orange;
			custom_char_draw.char_custom.grapic_data_struct.start_angle=25;
			custom_char_draw.char_custom.grapic_data_struct.end_angle=strlen("LASER:");
			custom_char_draw.char_custom.grapic_data_struct.width=3;
			custom_char_draw.char_custom.grapic_data_struct.start_x=c_pos_x[4]*SCREEN_LENGTH;
			custom_char_draw.char_custom.grapic_data_struct.start_y=c_pos_y[4]*SCREEN_WIDTH;
			memset(custom_char_draw.char_custom.data,'\0',sizeof(custom_char_draw.char_custom.data));
			strcpy(custom_char_draw.char_custom.data,"LASER:");
			break;
			
			/*可变状态字符*/
			case 5:
			/*******************************弹仓盖开关状态*********************************/
MAG:  custom_char_draw.char_custom.grapic_data_struct.graphic_name[0] = 0;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[1] = 41;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[2] = 5;
			custom_char_draw.char_custom.grapic_data_struct.operate_tpye=Op_type;
			custom_char_draw.char_custom.grapic_data_struct.graphic_tpye=7;
			custom_char_draw.char_custom.grapic_data_struct.layer=9;
			custom_char_draw.char_custom.grapic_data_struct.color=Green;
			custom_char_draw.char_custom.grapic_data_struct.start_angle=25;
			custom_char_draw.char_custom.grapic_data_struct.end_angle=strlen(Mag_State[F405.Mag_Flag]);
			custom_char_draw.char_custom.grapic_data_struct.width=2;
			custom_char_draw.char_custom.grapic_data_struct.start_x=c_pos_x[5]*SCREEN_LENGTH;
			custom_char_draw.char_custom.grapic_data_struct.start_y=c_pos_y[5]*SCREEN_WIDTH;
			memset(custom_char_draw.char_custom.data,'\0',sizeof(custom_char_draw.char_custom.data));
			strcpy(custom_char_draw.char_custom.data,Mag_State[F405.Mag_Flag]);
			break;
			case 6:
			/*******************************云台状态*********************************/
GIMBAL:custom_char_draw.char_custom.grapic_data_struct.graphic_name[0] = 0;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[1] = 41;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[2] = 6;
			custom_char_draw.char_custom.grapic_data_struct.operate_tpye=Op_type;
			custom_char_draw.char_custom.grapic_data_struct.graphic_tpye=7;
			custom_char_draw.char_custom.grapic_data_struct.layer=9;
			custom_char_draw.char_custom.grapic_data_struct.color=Green;
			custom_char_draw.char_custom.grapic_data_struct.start_angle=25;
			custom_char_draw.char_custom.grapic_data_struct.end_angle=strlen(Gimbal_State[F405.Gimbal_Flag]);
			custom_char_draw.char_custom.grapic_data_struct.width=2;
			custom_char_draw.char_custom.grapic_data_struct.start_x=c_pos_x[6]*SCREEN_LENGTH;
			custom_char_draw.char_custom.grapic_data_struct.start_y=c_pos_y[6]*SCREEN_WIDTH;
			memset(custom_char_draw.char_custom.data,'\0',sizeof(custom_char_draw.char_custom.data));
			strcpy(custom_char_draw.char_custom.data,Gimbal_State[F405.Gimbal_Flag]);
			break;
			case 7:
			/*******************************底盘状态*********************************/
CHASSIS:custom_char_draw.char_custom.grapic_data_struct.graphic_name[0] = 0;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[1] = 41;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[2] = 7;
			custom_char_draw.char_custom.grapic_data_struct.operate_tpye=Op_type;
			custom_char_draw.char_custom.grapic_data_struct.graphic_tpye=7;
			custom_char_draw.char_custom.grapic_data_struct.layer=9;
			custom_char_draw.char_custom.grapic_data_struct.color=F405.Follow_state?White:Green;
			custom_char_draw.char_custom.grapic_data_struct.start_angle=25;
			custom_char_draw.char_custom.grapic_data_struct.end_angle=strlen(Chassis_State[F405.Chassis_Flag]);
			custom_char_draw.char_custom.grapic_data_struct.width=F405.Follow_state?5:2;
			custom_char_draw.char_custom.grapic_data_struct.start_x=c_pos_x[7]*SCREEN_LENGTH;
			custom_char_draw.char_custom.grapic_data_struct.start_y=c_pos_y[7]*SCREEN_WIDTH;
			memset(custom_char_draw.char_custom.data,'\0',sizeof(custom_char_draw.char_custom.data));
			strcpy(custom_char_draw.char_custom.data,Chassis_State[F405.Chassis_Flag]);
			break;
			case 8:
			/*******************************激光开关状态*********************************/
LASER:custom_char_draw.char_custom.grapic_data_struct.graphic_name[0] = 0;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[1] = 41;
			custom_char_draw.char_custom.grapic_data_struct.graphic_name[2] = 8;
			custom_char_draw.char_custom.grapic_data_struct.operate_tpye=Op_type;
			custom_char_draw.char_custom.grapic_data_struct.graphic_tpye=7;
			custom_char_draw.char_custom.grapic_data_struct.layer=9;
			custom_char_draw.char_custom.grapic_data_struct.color=Green;
			custom_char_draw.char_custom.grapic_data_struct.start_angle=25;
			custom_char_draw.char_custom.grapic_data_struct.end_angle=strlen(Laser_State[F405.Laser_Flag]);
			custom_char_draw.char_custom.grapic_data_struct.width=2;
			custom_char_draw.char_custom.grapic_data_struct.start_x=c_pos_x[8]*SCREEN_LENGTH;
			custom_char_draw.char_custom.grapic_data_struct.start_y=c_pos_y[8]*SCREEN_WIDTH;
			memset(custom_char_draw.char_custom.data,'\0',sizeof(custom_char_draw.char_custom.data));
			strcpy(custom_char_draw.char_custom.data,Laser_State[F405.Laser_Flag]);
			break;
		}
		tick++;
	}else if(Op_type == Op_Change)		//如果是标志为修改
	{
		/*寻找是哪个发生了变化*/
		if(Char_Change_Array[0] == Op_Change)
		{
			Char_Change_Array[0] = Op_None;
			goto MAG;
		}else if(Char_Change_Array[1] == Op_Change)
		{
			Char_Change_Array[1] = Op_None;
			goto GIMBAL;
		}else if(Char_Change_Array[2] == Op_Change)
		{
			Char_Change_Array[2] = Op_None;
			goto CHASSIS;
		}else if(Char_Change_Array[3] == Op_Change)
		{
			Char_Change_Array[3] = Op_None;
			goto LASER;
		}
	}
}

/**********************************************************************************************************
*函 数 名: Char_Change_Check
*功能说明: 检测有没有字符的变动，或者进行字符图层的初始化
*形    参: 无
*返 回 值: 
**********************************************************************************************************/
int Char_Change_Check(void)
{
	int i;
	static char last_Mag,last_Gimbal,last_Chassis,last_Laser,last_Follow;	//记录上次状态
	static int delete_flag;
	char Mag_flag,Gimbal_flag,Chassis_flag,Laser_flag,Follow_state;
	
	/*图形界面初始化提示的显示和删除*/
	if(F405.Graphic_Init_Flag == 0 || delete_flag < 3)			//删除几次，保证删除成功，就不删除了，防止占带宽
	{
		Load_Char_Init(F405.Graphic_Init_Flag);
		referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&custom_char_draw,sizeof(custom_char_draw));
	}
	/*用于图形界面初始化*/
	if(F405.Graphic_Init_Flag == 0)		
	{
		delete_flag = 0;
		return Op_Init;	//返回Init,会使一直发送Add，添加所有图层
	}else delete_flag++;
		
	/*读取云台发送的各种状态*/
	Mag_flag = F405.Mag_Flag;
	Gimbal_flag = F405.Gimbal_Flag;
	Chassis_flag = F405.Chassis_Flag;
	Laser_flag = F405.Laser_Flag;
	Follow_state = F405.Follow_state;
	
	/*有变化，标志各个位*/
	if(last_Mag != Mag_flag) Char_Change_Array[0] = Op_Change;
	if(Gimbal_flag != last_Gimbal) Char_Change_Array[1] = Op_Change;
	if(Chassis_flag != last_Chassis || Follow_state != last_Follow) Char_Change_Array[2] = Op_Change;
	if(Laser_flag != last_Laser) Char_Change_Array[3] = Op_Change;
		
	/*保存这次标志和上次比较*/
	last_Mag = Mag_flag;
	last_Gimbal = Gimbal_flag;
	last_Chassis = Chassis_flag;
	last_Laser = Laser_flag;
	last_Follow = Follow_state;
	
	/*检索有没有发生变化，如果有变化则返回修改图层*/
	for(i = 0;i<4;i++)
	{
		if(Char_Change_Array[i] == Op_Change)
			return Op_Change;
	}
	return Op_None;	//否则返回空操作，此时不会发送东西
}
/**********************************************************************************************************
*函 数 名: Load_Char_Init
*功能说明: 装载字符初始化提示
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void Load_Char_Init(char Init_Flag)
{
	custom_char_draw.data_cmd_id=0x0110;//绘制字符

	custom_char_draw.sender_ID=JudgeReceive.robot_id;//发送者ID，机器人对应ID
	if(JudgeReceive.robot_id == 103)
			custom_char_draw.receiver_ID = 0x0167;
	if(JudgeReceive.robot_id == 104)
			custom_char_draw.receiver_ID = 0x0168;
	if(JudgeReceive.robot_id == 105)
			custom_char_draw.receiver_ID = 0x0169;
	if(JudgeReceive.robot_id == 3)
			custom_char_draw.receiver_ID = 0x0103;	
	if(JudgeReceive.robot_id == 4)
			custom_char_draw.receiver_ID = 0x0104;
	if(JudgeReceive.robot_id == 5)
			custom_char_draw.receiver_ID = 0x0105;
	
	/*******************************Init:E 字符*********************************/
	custom_char_draw.char_custom.grapic_data_struct.graphic_name[0] = 0;
	custom_char_draw.char_custom.grapic_data_struct.graphic_name[1] = 41;
	custom_char_draw.char_custom.grapic_data_struct.graphic_name[2] = 9;
	custom_char_draw.char_custom.grapic_data_struct.operate_tpye=Init_Flag?Op_Delete:Op_Add;
	custom_char_draw.char_custom.grapic_data_struct.graphic_tpye=7;
	custom_char_draw.char_custom.grapic_data_struct.layer=9;
	custom_char_draw.char_custom.grapic_data_struct.color=Green;
	custom_char_draw.char_custom.grapic_data_struct.start_angle=25;
	custom_char_draw.char_custom.grapic_data_struct.end_angle=strlen("Init..ing Press E To Hide");
	custom_char_draw.char_custom.grapic_data_struct.width=4;
	custom_char_draw.char_custom.grapic_data_struct.start_x=0.34*SCREEN_LENGTH;
	custom_char_draw.char_custom.grapic_data_struct.start_y=0.56*SCREEN_WIDTH;
	custom_char_draw.char_custom.grapic_data_struct.end_x=0;
	custom_char_draw.char_custom.grapic_data_struct.end_y=0;
	custom_char_draw.char_custom.grapic_data_struct.radius = 0;
	memset(custom_char_draw.char_custom.data,'\0',sizeof(custom_char_draw.char_custom.data));
	strcpy(custom_char_draw.char_custom.data,"Init..ing Press B To Hide");
}
/**********************************************************************************************************
*函 数 名: CharSendtask
*功能说明: 字符发送任务
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void CharSendtask(void *pvParameters)
{
	 static int tick;
   while (1) {
    
		tick++;
		if(0==tick%10)	
		{
			JudgementCustomizeChar();
			referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&custom_char_draw,sizeof(custom_char_draw));
		}
			vTaskDelay(1); 
		 
#if INCLUDE_uxTaskGetStackHighWaterMark
		
#endif
    }
}
