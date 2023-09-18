
/**********************************************************************************************************
 * @文件     Graphics_Send.c
 * @说明     裁判系统图形发送
 * @版本  	 V1.1
 * @作者     文博通
 * @日期     2023.4
**********************************************************************************************************/
#include "config.h"
extern unsigned char SendToSentey_Buff[USART1_JUD_DMA_send_buffersize];
extern short q_rising_flag,w_rising_flag,e_rising_flag,r_rising_flag,
		a_rising_flag,s_rising_flag,d_rising_flag,f_rising_flag,g_rising_flag,
		z_rising_flag,x_rising_flag,c_rising_flag,v_rising_flag,b_rising_flag,
		shift_rising_flag,ctrl_rising_flag,mouse_Press_l_rising_flag,mouse_Press_r_rising_flag;

unsigned char JudgeSend[128];
extern tGameInfo  JU_Recv;
extern int shoot_flag;
extern short ContralMode;
extern struct IMU 	 imu;
extern RC_Ctl_t RC_Ctl;
int Char_Change_Array[8];					//0表示没变化，非0表示有变化
uint8_t Transmit_Pack[128];//裁判系统数据帧
uint8_t data_pack[DRAWING_PACK*7] = {0};//数据段部分
uint8_t DMAsendflag;
uint8_t optype = Op_Add;
uint8_t Init_Cnt = 10;

float top_distance;
float bottom_distance;
float left_distance;
float right_distance;
uint32_t UITask_RunTime = 0UL;
uint32_t UISendTask_high_water;

short last_g_rising_flag = 0;//初始化时未按下z
short UI_Change_Mode = 0;
///**********************************************************************************************************
//*函 数 名: JudgementCustomizeGraphics
//*功能说明: 裁判系统自定义图形发送函数
//*形    参: 无
//*返 回 值: 无
//**********************************************************************************************************/
//ext_student_interactive_header_data_t custom_grapic_draw;			//自定义图像绘制
//ext_student_interactive_char_header_data_t custom_char_draw;
////装载图形
//void JudgementCustomizeGraphics(int Op_type)
//{
//		custom_grapic_draw.data_cmd_id=0x0104;//绘制七个图形（内容ID，查询裁判系统手册）
//		//custom_grapic_draw.data_cmd_id=0x0301;//绘制七个图形（内容ID，查询裁判系统手册）
//		custom_grapic_draw.sender_ID=JU_Recv.robot_id;//发送者ID，机器人对应ID
//		if(JU_Recv.robot_id == 106)//蓝飞机
//				custom_grapic_draw.receiver_ID = 0x016A;
//		if(JU_Recv.robot_id == 6)//红飞机
//				custom_grapic_draw.receiver_ID = 0x0106;

///*********************************自定义图像数据***********************************/
//		referee_data_load_Graphic(Op_type);
//}

/**********************************************************************************************************
*函 数 名: GraphicSendtask
*功能说明: 图形发送任务
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/

void GraphicSendtask(void *pvParameters)
{
	UI_Distance_Init();
  while (1) 
	{
		optype = (Init_Cnt == 0)?Op_Change:Op_Add;	
		
		if(RC_Ctl.key.q == 1)//操作手按一次 Q 则Init_Cnt +10
		//if(RC_Ctl.key.q == 1)//操作手按一次 Q 则Init_Cnt +10
		{
			//F405.Last_Init_Flag = F405.Graphic_Init_Flag;
			Init_Cnt += 10;
		}
		
		if(Init_Cnt > 0)
		{
			
			Char_Init();
			Init_Cnt--;
			//ShootLines_Init();
		}
		UI_Distance_Change();//更改瞄准框
		ShootLines_Init();
		YawUI_Change(imu.yawAngle,Init_Cnt);
		PitchUI_Change( imu.pitchAngle,Init_Cnt);//Pitch角度绘制
		GimbalMode_Change(ContralMode,Init_Cnt);//云台模式绘制
		FrictionMode_Change(shoot_flag,Init_Cnt);//摩擦轮模式绘制
		
		
		/* 任务延时在数据发送函数中*/
		
		UITask_RunTime++;
#if INCLUDE_uxTaskGetStackHighWaterMark
		UISendTask_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif	 
    }
}

void UI_Distance_Init()
{
	top_distance = 0.5;
	bottom_distance = 0.44;
	left_distance = 0.45;
	right_distance = 0.55;	
}

void UI_Distance_Change()
{
	//if(z_rising_flag == 1 && last_z_rising_flag == 0)
	if(RC_Ctl.key.g == 1 && last_g_rising_flag == 0)
	{	
		if(UI_Change_Mode == 0)
		{
			UI_Change_Mode = 1;//UI移动模式
		}
		else if(UI_Change_Mode == 1)
		{
			UI_Change_Mode = 0;//禁止UI移动模式
		}
	}
	//last_z_rising_flag = z_rising_flag;
	last_g_rising_flag = RC_Ctl.key.g;
	if(UI_Change_Mode == 1)
	{
		top_distance = top_distance + RC_Ctl.key.w * 0.04 - RC_Ctl.key.s * 0.04;
		bottom_distance = bottom_distance + RC_Ctl.key.w * 0.04 - RC_Ctl.key.s * 0.04;
		left_distance = left_distance + RC_Ctl.key.d * 0.04 - RC_Ctl.key.a * 0.04;
		right_distance = right_distance + RC_Ctl.key.d * 0.04 - RC_Ctl.key.a * 0.04;
	}
}
//void Shoot_Line_Change()
//{

//}
/**********************************************************************************************************
*函 数 名: Send_UIPack
*功能说明: 发送整个UI数据包（数据段首部和内容）
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void Send_UIPack(uint16_t data_cmd_id, uint16_t SendID,uint16_t receiverID, uint8_t* data, uint16_t pack_len)
{
	student_interactive_header_data_t	custom_interactive_header;
	custom_interactive_header.data_cmd_id = data_cmd_id;
	custom_interactive_header.send_ID = SendID;
	custom_interactive_header.receiver_ID = receiverID;

	uint8_t header_len = sizeof(custom_interactive_header);//数据段首部长度
	
	memcpy((void*)(SendToSentey_Buff + 7), &custom_interactive_header, header_len);	//将数据段的数据段进行封装（封装段首）
	memcpy((void*)(SendToSentey_Buff + 7 + header_len), data, pack_len);			//将数据帧的数据段进行封装（封装数据）

	Send_toReferee(0x0301,pack_len + header_len);//打包并发送整帧数据
}
/**********************************************************************************************************
*函 数 名: Send_toReferee
*功能说明: 打包数据帧并发送给裁判系统
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void Send_toReferee(uint16_t cmd_id, uint16_t data_len)
{
	static uint8_t seq = 0;
	static uint8_t Frame_Length;
	Frame_Length = HEADER_LEN + CMD_LEN + CRC_LEN + data_len; 
 
	//帧首部封装
	{
		SendToSentey_Buff[0] = 0xA5;
		memcpy(&SendToSentey_Buff[1],(uint8_t*)&data_len, sizeof(data_len));//数据段中data的长度
		//Transmit_Pack[3] = seq ++ ;
		SendToSentey_Buff[3] = seq++;
		Append_CRC8_Check_Sum(SendToSentey_Buff,HEADER_LEN);  //帧头校验CRC8
	}
	
	//命令ID
	memcpy(&SendToSentey_Buff[HEADER_LEN],(uint8_t*)&cmd_id, CMD_LEN);
	
	//尾部数据校验CRC16
	Append_CRC16_Check_Sum(SendToSentey_Buff,Frame_Length);  

	uint8_t send_cnt = 3;//发送次数连续发3次
		
	while (	send_cnt )
	{
		send_cnt --;
		DMA_ClearFlag(DMA2_Stream7,DMA_IT_TCIF7);//通过UART+DMA的方式发送
		DMA2_Stream7->NDTR = Frame_Length;
		DMA_Cmd(DMA2_Stream7, ENABLE);	  

		DMAsendflag=1;//DMA传输完成标志，在中断中置0

		while (DMAsendflag == 1)
		{
		}
		vTaskDelay(35);
	}

}
/**********************************************************************************************************
*函 数 名: Deleta_Layer
*功能说明: 清除图层
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void Deleta_Layer(uint8_t layer , uint8_t deleteType)
{
	static ext_client_custom_graphic_delete_t Delete_Graphic;//必须为静态变量，否则函数结束时会销毁该变量内存
	Delete_Graphic.layer = layer;
	Delete_Graphic.operate_tpye = deleteType;
	Send_UIPack(Drawing_Delete_ID, JU_Recv.robot_id, JU_Recv.robot_id + 0x100, (uint8_t *)&Delete_Graphic, sizeof(Delete_Graphic)); // 画字符

}

/**********************************************************************************************************
*函 数 名: Char_Draw
*功能说明: 绘制字符
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void Char_Draw(uint8_t layer,int Op_Type,uint16_t startx,uint16_t starty, uint8_t size, uint8_t len, uint16_t line_width, int color,uint8_t name[],uint8_t * str_data)
{
	graphic_data_struct_t * P_graphic_data;
	P_graphic_data = CharGraphic_Draw(0,Op_Type, startx, starty, size, len ,line_width , color , name);
	memcpy(data_pack , (uint8_t*)P_graphic_data ,DRAWING_PACK);
	memset(&data_pack[DRAWING_PACK], 0, 30);
	memcpy(&data_pack[DRAWING_PACK] , (uint8_t*)str_data ,len);
	Send_UIPack(Drawing_Char_ID, JU_Recv.robot_id, JU_Recv.robot_id + 0x100, data_pack, DRAWING_PACK + 30); // 绘制字符

}

/**********************************************************************************************************
*函 数 名: CharGraphic_Draw
*功能说明: 得到字符图形数据结构体
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
graphic_data_struct_t* CharGraphic_Draw(uint8_t layer,int Op_Type,uint16_t startx,uint16_t starty, uint8_t size, uint8_t len, uint16_t line_width, int color,uint8_t name[])
{


	static graphic_data_struct_t drawing;//必须为静态变量，否则函数结束时会销毁该变量内存
	memcpy(drawing.graphic_name,name,3);																			//图案名称，3位
	drawing.layer = layer;
	drawing.operate_tpye = Op_Type;
	drawing.graphic_tpye = TYPE_CHAR;//7为字符数据
	drawing.color = color;
	drawing.start_x=startx;
	drawing.start_y=starty;

	drawing.start_angle = size;//字体大小
	drawing.end_angle = len;//字符长度
	drawing.width = line_width;

	for(uint8_t i = DRAWING_PACK;i < DRAWING_PACK + 30;i++)
		data_pack[i] = 0;
	return &drawing;

}

/**********************************************************************************************************
*函 数 名: FloatData_Draw
*功能说明: 得到绘制浮点图形结构体
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
graphic_data_struct_t* FloatData_Draw(uint8_t layer,int Op_Type,uint16_t startx,uint16_t starty, float data_f, uint8_t size ,uint8_t valid_bit, uint16_t line_width, int color,uint8_t name[])
{
	static graphic_data_struct_t drawing;//必须为静态变量，否则函数结束时会销毁该变量内存
	static int32_t Data1000;
	Data1000 = (int32_t)(data_f * 1000 );
	memcpy(drawing.graphic_name,name,3);																			//图案名称，3位
	drawing.layer = layer;
	drawing.operate_tpye = Op_Type;
	drawing.graphic_tpye = TYPE_FLOAT;//5为浮点数据
	drawing.width = line_width;//线宽
	drawing.color = color;
	drawing.start_x = startx;
	drawing.start_y = starty;
	drawing.start_angle = size;//字体大小
	drawing.end_angle = valid_bit;//有效位数

	drawing.radius = Data1000 & 0x03ff;
	drawing.end_x  = (Data1000 >> 10) & 0x07ff;
	drawing.end_y  = (Data1000 >> 21) & 0x07ff;	
	return &drawing;

}


/**********************************************************************************************************
*函 数 名: Line_Draw
*功能说明: 直线图形数据结构体
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
graphic_data_struct_t* Line_Draw(uint8_t layer,int Op_Type,uint16_t startx,uint16_t starty,uint16_t endx,uint16_t endy, uint16_t line_width, int color,uint8_t name[])
{
	static graphic_data_struct_t drawing;//必须为静态变量，否则函数结束时会销毁该变量内存
	memcpy(drawing.graphic_name,name,3);																			//图案名称，3位
	drawing.layer = layer;
	drawing.operate_tpye = Op_Type;
	drawing.graphic_tpye = TYPE_LINE;
	drawing.width = line_width;
	drawing.color = color;
	drawing.start_x=startx;
	drawing.start_y=starty;
	drawing.end_x=endx;
	drawing.end_y=endy;
	return &drawing;

}

/**********************************************************************************************************
*函 数 名: Rectangle_Draw
*功能说明: 矩形图形数据结构体
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
graphic_data_struct_t* Rectangle_Draw(uint8_t layer,int Op_Type,uint16_t startx,uint16_t starty,uint16_t endx,uint16_t endy, uint16_t line_width, int color,uint8_t name[])
{
	static graphic_data_struct_t drawing;//必须为静态变量，否则函数结束时会销毁该变量内存
	memcpy(drawing.graphic_name,name,3);																			//图案名称，3位
	drawing.layer = layer;
	drawing.operate_tpye = Op_Type;
	drawing.graphic_tpye = TYPE_RECTANGLE;
	drawing.width = line_width;
	drawing.color = color;
	drawing.start_x=startx;
	drawing.start_y=starty;
	drawing.end_x=endx;
	drawing.end_y=endy;
	return &drawing;

}

/**********************************************************************************************************
*函 数 名: Circle_Draw
*功能说明: 圆形图形数据结构体
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
graphic_data_struct_t* Circle_Draw(uint8_t layer,int Op_Type,uint16_t startx,uint16_t starty,uint32_t radius, uint16_t line_width, int color,uint8_t name[])
{
	static graphic_data_struct_t drawing;//必须为静态变量，否则函数结束时会销毁该变量内存
	memcpy(drawing.graphic_name,name,3);																			//图案名称，3位
	drawing.layer = layer;
	drawing.operate_tpye = Op_Type;
	drawing.graphic_tpye = TYPE_CIRCLE;
	drawing.width = line_width;
	drawing.color = color;
	drawing.start_x=startx;
	drawing.start_y=starty;
  drawing.radius = radius;
	return &drawing;

}
/**********************************************************************************************************
*函 数 名: PitchUI_Change
*功能说明: Pitch角度绘制
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void PitchUI_Change(short Pitch_100 ,uint8_t Init_Cnt)
{
	static uint8_t PitchName[] = "Pit";
	graphic_data_struct_t * P_graphic_data;
	P_graphic_data = FloatData_Draw(0,optype,0.80 * SCREEN_LENGTH + 140, 0.6 * SCREEN_WIDTH , Pitch_100, 20 ,4 , 2, Yellow, PitchName);
	memcpy(data_pack , (uint8_t*)P_graphic_data ,DRAWING_PACK);

	Send_UIPack(Drawing_Graphic1_ID, JU_Recv.robot_id, JU_Recv.robot_id + 0x100, data_pack, DRAWING_PACK); // 画字符

}

void YawUI_Change(short Yaw_100 ,uint8_t Init_Cnt)
{
	static uint8_t YawName[] = "yaw";
	graphic_data_struct_t * P_graphic_data;
	P_graphic_data = FloatData_Draw(0,optype,0.80 * SCREEN_LENGTH + 140, 0.45 * SCREEN_WIDTH , Yaw_100, 20 ,4 , 2, Yellow, YawName);
	memcpy(data_pack , (uint8_t*)P_graphic_data ,DRAWING_PACK);
	Send_UIPack(Drawing_Graphic1_ID, JU_Recv.robot_id, JU_Recv.robot_id + 0x100, data_pack, DRAWING_PACK); // 画字符

}
/**********************************************************************************************************
*函 数 名: GimbalMode_Change
*功能说明: 云台模式绘制
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void GimbalMode_Change(char Gimbal_Flag,uint8_t Init_Cnt)
{
	static uint8_t Gimbal_State_Name[] = "nor";
	static uint8_t Lost_Char[] = "LOST";
	static uint8_t Normal_Char[] = "Normal_RC";
	static uint8_t PC_Char[] = "PC";
	static uint8_t Auto_Char[] = "AutoAim";

	if(Gimbal_Flag == 0)
		Char_Draw(0,optype,0.9*SCREEN_LENGTH,0.55*SCREEN_WIDTH,20,sizeof(Lost_Char),2,Yellow,Gimbal_State_Name,Lost_Char);
	else if(Gimbal_Flag == 1)
		Char_Draw(0,optype,0.9*SCREEN_LENGTH,0.55*SCREEN_WIDTH,20,sizeof(Normal_Char),2,Yellow,Gimbal_State_Name,Normal_Char);
	else if(Gimbal_Flag == 2)
		Char_Draw(0,optype,0.9*SCREEN_LENGTH,0.55*SCREEN_WIDTH,20,sizeof(PC_Char),2,Yellow,Gimbal_State_Name,PC_Char);
	else if(Gimbal_Flag == 3)
		Char_Draw(0,optype,0.9*SCREEN_LENGTH,0.55*SCREEN_WIDTH,20,sizeof(Auto_Char),2,Yellow,Gimbal_State_Name,Auto_Char);
}

/**********************************************************************************************************
*函 数 名: FrictionMode_Change
*功能说明: 摩擦轮开启标志绘制
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void FrictionMode_Change(char Fric_Flag,uint8_t Init_Cnt)
{
	static uint8_t Fri_Name[] = "Fri";
	static uint8_t On_Char[] = "ON";
	static uint8_t Off_Char[] = "OFF";

	
	if(shoot_flag == 1)
		Char_Draw(0,optype,0.9*SCREEN_LENGTH,0.5*SCREEN_WIDTH,20,sizeof(On_Char),2,Yellow,Fri_Name,On_Char);
	else 
		Char_Draw(0,optype,0.9*SCREEN_LENGTH,0.5*SCREEN_WIDTH,20,sizeof(Off_Char),2,Yellow,Fri_Name,Off_Char);
}
/**********************************************************************************************************
*函 数 名: Char_Init
*功能说明: 字符数据初始化
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void Char_Init(void)
{
	static uint8_t CharName1[] = "pit";
	static uint8_t CharName2[] = "gim";
	static uint8_t CharName3[] = "fri";
	static uint8_t CharName4[] = "yaw";

	/*				PITCH字符			*/
	uint8_t pitch_char[] = "PITCH :";
	Char_Draw(0,Op_Add, 0.80 * SCREEN_LENGTH, 0.6 * SCREEN_WIDTH, 20, sizeof(pitch_char) ,2 , Orange , CharName1,pitch_char);
//	vTaskDelay(10);
	/*				Gimbal字符			*/
	uint8_t gim_char[] = "Gimbal :";
	Char_Draw(0,Op_Add, 0.80 * SCREEN_LENGTH, 0.55 * SCREEN_WIDTH, 20, sizeof(gim_char) ,2 , Orange , CharName2,gim_char);
	/*				字符			*/
	uint8_t fir_char[] = "Friction :";
	Char_Draw(0,Op_Add, 0.80 * SCREEN_LENGTH, 0.50 * SCREEN_WIDTH, 20, sizeof(fir_char) ,2 , Orange , CharName3,fir_char);
	
	uint8_t OutPost_char[] = "YAW :";
	Char_Draw(0,Op_Add, 0.80 * SCREEN_LENGTH, 0.45 * SCREEN_WIDTH, 20, sizeof(OutPost_char) ,2 , Orange , CharName4,OutPost_char);
}

/**********************************************************************************************************
*函 数 名: ShootLines_Init
*功能说明: 车道线初始化
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void ShootLines_Init(void)
{
	static uint8_t ShootLineName1[] = "ss1";
	static uint8_t ShootLineName2[] = "ss2";
	static uint8_t ShootLineName3[] = "ss3";
	static uint8_t ShootLineName4[] = "ss4";
	static uint8_t ShootLineName5[] = "ss5";
	static uint8_t ShootLineName6[] = "ss6";
	static uint8_t ShootLineName7[] = "ss7";
	
	graphic_data_struct_t * P_graphic_data;
	//第一条准线
//	P_graphic_data = Line_Draw(1,Op_Add,SCREEN_LENGTH * 0.40, SCREEN_WIDTH * 0.6, SCREEN_LENGTH * 0.6 , SCREEN_WIDTH * 0.6 ,1 , Green , ShootLineName1);
//	memcpy(data_pack , (uint8_t*)P_graphic_data ,DRAWING_PACK);
//	//竖准线
//	P_graphic_data = Line_Draw(1,Op_Add,SCREEN_LENGTH * 0.5, SCREEN_WIDTH * 0.5, SCREEN_LENGTH * 0.5 , SCREEN_WIDTH * 0.44,1 , Green , ShootLineName2);
//	memcpy(&data_pack[DRAWING_PACK] , (uint8_t*)P_graphic_data ,DRAWING_PACK);
//	//横准线
//	P_graphic_data = Line_Draw(1,Op_Add,SCREEN_LENGTH * 0.45, SCREEN_WIDTH * 0.47, SCREEN_LENGTH * 0.55 , SCREEN_WIDTH * 0.47 ,1 , Green , ShootLineName5);
//	memcpy(&data_pack[2*DRAWING_PACK] , (uint8_t*)P_graphic_data ,DRAWING_PACK);
	//左竖线
	P_graphic_data = Line_Draw(1,optype,SCREEN_LENGTH * left_distance, SCREEN_WIDTH * top_distance, SCREEN_LENGTH * left_distance , SCREEN_WIDTH * bottom_distance,1 , Green , ShootLineName1);
	memcpy(&data_pack[DRAWING_PACK] , (uint8_t*)P_graphic_data ,DRAWING_PACK);
	//右竖线
	P_graphic_data = Line_Draw(1,optype,SCREEN_LENGTH * right_distance, SCREEN_WIDTH * top_distance, SCREEN_LENGTH * right_distance , SCREEN_WIDTH * bottom_distance,1 , Green , ShootLineName2);
	memcpy(&data_pack[2*DRAWING_PACK] , (uint8_t*)P_graphic_data ,DRAWING_PACK);
	//上横线
	P_graphic_data = Line_Draw(1,optype,SCREEN_LENGTH * left_distance, SCREEN_WIDTH * top_distance, SCREEN_LENGTH * right_distance , SCREEN_WIDTH * top_distance ,1 , Green , ShootLineName3);
	memcpy(&data_pack[3*DRAWING_PACK] , (uint8_t*)P_graphic_data ,DRAWING_PACK);
	//下横线
	P_graphic_data = Line_Draw(1,optype,SCREEN_LENGTH * left_distance, SCREEN_WIDTH * bottom_distance, SCREEN_LENGTH * right_distance , SCREEN_WIDTH * bottom_distance ,1 , Green , ShootLineName4);
	memcpy(&data_pack[4*DRAWING_PACK] , (uint8_t*)P_graphic_data ,DRAWING_PACK);
	//竖准心线
	P_graphic_data = Line_Draw(1,optype,SCREEN_LENGTH * (right_distance + left_distance)/2 , SCREEN_WIDTH * (top_distance + 0.05), SCREEN_LENGTH * (right_distance + left_distance)/2 , SCREEN_WIDTH * (bottom_distance - 0.05),1 , Green , ShootLineName5);
	memcpy(&data_pack[5*DRAWING_PACK] , (uint8_t*)P_graphic_data ,DRAWING_PACK);
	//横准心上线
	P_graphic_data = Line_Draw(1,optype,SCREEN_LENGTH * (left_distance -0.05), SCREEN_WIDTH * (top_distance + bottom_distance)/2, SCREEN_LENGTH * (right_distance + 0.05) , SCREEN_WIDTH * (top_distance + bottom_distance)/2 ,1 , Green , ShootLineName6);
	memcpy(&data_pack[6*DRAWING_PACK] , (uint8_t*)P_graphic_data ,DRAWING_PACK);
	//横准心下线
	P_graphic_data = Line_Draw(1,optype,SCREEN_LENGTH * (left_distance -0.02), SCREEN_WIDTH * ((top_distance + bottom_distance)/2-0.03), SCREEN_LENGTH * (right_distance + 0.02) , SCREEN_WIDTH * ((top_distance + bottom_distance)/2-0.03) ,1 , Green , ShootLineName7);
	memcpy(&data_pack[7*DRAWING_PACK] , (uint8_t*)P_graphic_data ,DRAWING_PACK);
//	P_graphic_data = Line_Draw(1,Op_Add,SCREEN_LENGTH * 0.45, SCREEN_WIDTH * 0.47, SCREEN_LENGTH * 0.55 , SCREEN_WIDTH * 0.47 ,1 , Green , ShootLineName5);
//	memcpy(&data_pack[2*DRAWING_PACK] , (uint8_t*)P_graphic_data ,DRAWING_PACK);
//	P_graphic_data = Line_Draw(1,Op_Add,SCREEN_LENGTH * 0.40, SCREEN_WIDTH * 0.44, SCREEN_LENGTH * 0.6 , SCREEN_WIDTH * 0.44 ,1 , Green , ShootLineName3);
//	memcpy(&data_pack[2*DRAWING_PACK] , (uint8_t*)P_graphic_data ,DRAWING_PACK);
//	
//	P_graphic_data = Line_Draw(1,Op_Add,SCREEN_LENGTH * 0.40, SCREEN_WIDTH * 0.47, SCREEN_LENGTH * 0.6 , SCREEN_WIDTH * 0.47 ,1 , Green , ShootLineName4);
//	memcpy(&data_pack[3*DRAWING_PACK] , (uint8_t*)P_graphic_data ,DRAWING_PACK);


	Send_UIPack(Drawing_Graphic7_ID, JU_Recv.robot_id, JU_Recv.robot_id + 0x100, data_pack, DRAWING_PACK * 7); // 画两个图形
 }

/**********************************************************************************************************
*函 数 名: JudgementCustomizeChar
*功能说明: 图形数据包分段打包
*形    参: 操作类型
*返 回 值: 无
**********************************************************************************************************/
//void JudgementCustomizeChar(int Op_type)
//{
//		custom_char_draw.data_cmd_id=0x0110;//绘制字符
//		custom_char_draw.sender_ID=JU_Recv.robot_id;//发送者ID，机器人对应ID
//		if(JU_Recv.robot_id == 106)//蓝飞机
//				custom_grapic_draw.receiver_ID = 0x016A;
//		if(JU_Recv.robot_id == 6)//红飞机
//				custom_grapic_draw.receiver_ID = 0x0106;

///*********************************自定义图像数据***********************************/
//		referee_data_load_String(Op_type);
//}
/**********************************************************************************************************
*函 数 名: referee_data_load_Graphic
*功能说明: 图形数据包装载图形
*形    参: 操作类型
*返 回 值: 无
**********************************************************************************************************/
///*瞄准线偏移量*/
//int AIM_bias_x = 0;
//int AIM_bias_y = 0;
//int placece_x[17]={0,100, 20,20, 20, 40, 20, 20, 20,  40,  20,  20,  20, 40,   20,   20,  20};
//int placece_y[18]={0,-320,0,-20,-40,-60,-80,-100,-120,-140,-160,-180,-200,-220,-240,-260,-280,-300};
//#define PACK_NUM 3
//void referee_data_load_Graphic(int Op_type)
//{
//	static int pack_tick = 0;			//数据包计数器
//	/*初始化操作，轮流生成图层*/
//	if(Op_type == Op_Init)
//	{
//		switch(pack_tick % PACK_NUM)
//		{
//			case 0:
//				/*从这以下是射击辅助线*/
//				/*********************************中线*********************************/
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_name[0] = 0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_name[1] = 1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_name[2] = 8;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].operate_tpye=2;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_tpye=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].layer=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].color=Green;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].start_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].end_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].width=1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].start_x=SCREEN_LENGTH/2 - placece_x[0] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].start_y=SCREEN_WIDTH/2 + placece_y[0] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].end_x  =SCREEN_LENGTH/2 - placece_x[0] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].end_y  =SCREEN_WIDTH/2 + placece_y[1] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].radius=0;
//				
//				/*********************************横线0，中间最长线*********************************/
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_name[0] = 0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_name[1] = 1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_name[2] = 9;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].operate_tpye=2;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_tpye=1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].layer=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].color=Purple;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].start_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].end_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].width=4;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].start_x=SCREEN_LENGTH/2 - placece_x[1] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].start_y=SCREEN_WIDTH/2 + placece_y[2] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].end_x  =SCREEN_LENGTH/2 + placece_x[1] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].end_y  =SCREEN_WIDTH/2 + placece_y[2] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].radius=0;
//				
//				/*********************************横线1*********************************/
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_name[0] = 0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_name[1] = 1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_name[2] = 10;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].operate_tpye=1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_tpye=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].layer=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].color=Green;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].start_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].end_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].width=1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].start_x=SCREEN_LENGTH/2 - placece_x[2] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].start_y=SCREEN_WIDTH/2 + placece_y[3] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].end_x  =SCREEN_LENGTH/2 + placece_x[2] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].end_y  =SCREEN_WIDTH/2 + placece_y[3] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].radius=0;
//				
//				break;	
//			case 1:
//				/*********************************横线2*********************************/
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[0] = 0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[1] = 1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[2] = 11;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].operate_tpye=Op_type;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_tpye=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].layer=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].color=Green;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].width=1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_x=SCREEN_LENGTH/2 - placece_x[3] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_y=SCREEN_WIDTH/2 + placece_y[4] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_x  =SCREEN_LENGTH/2 + placece_x[3] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_y  =SCREEN_WIDTH/2 + placece_y[4] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].radius=0;
//			
//				/*********************************横线3*********************************/
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[0] = 0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[1] = 1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[2] = 12;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].operate_tpye=Op_type;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_tpye=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].layer=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].color=Purple;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].width=1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_x=SCREEN_LENGTH/2 - placece_x[4] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_y=SCREEN_WIDTH/2 + placece_y[5] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_x  =SCREEN_LENGTH/2 + placece_x[4] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_y  =SCREEN_WIDTH/2 + placece_y[5] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].radius=0;

//				/*********************************横线4*********************************/
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_name[0] = 0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_name[1] = 1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_name[2] = 13;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].operate_tpye=Op_type;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_tpye=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].layer=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].color=Green;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].start_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].end_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].width=1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].start_x=SCREEN_LENGTH/2 - placece_x[5] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].start_y=SCREEN_WIDTH/2 + placece_y[6] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].end_x  =SCREEN_LENGTH/2 + placece_x[5] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].end_y  =SCREEN_WIDTH/2 + placece_y[6] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].radius=0;

//				/*********************************横线5*********************************/
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_name[0] = 0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_name[1] = 1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_name[2] = 14;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].operate_tpye=Op_type;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_tpye=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].layer=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].color=Green;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].start_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].end_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].width=1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].start_x=SCREEN_LENGTH/2 - placece_x[6] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].start_y=SCREEN_WIDTH/2 + placece_y[7] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].end_x  =SCREEN_LENGTH/2 + placece_x[6] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].end_y  =SCREEN_WIDTH/2 + placece_y[7] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].radius=0;

//				/*********************************横线6*********************************/
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_name[0] = 0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_name[1] = 1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_name[2] = 15;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].operate_tpye=Op_type;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_tpye=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].layer=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].color=Green;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].start_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].end_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].width=1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].start_x=SCREEN_LENGTH/2 - placece_x[7] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].start_y=SCREEN_WIDTH/2 + placece_y[8] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].end_x  =SCREEN_LENGTH/2 + placece_x[7] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].end_y  =SCREEN_WIDTH/2 + placece_y[8] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].radius=0;

//				/*********************************横线7*********************************/
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_name[0] = 0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_name[1] = 1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_name[2] = 16;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].operate_tpye=Op_type;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_tpye=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].layer=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].color=5;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].start_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].end_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].width=1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].start_x=SCREEN_LENGTH/2 - placece_x[8] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].start_y=SCREEN_WIDTH/2 + placece_y[9] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].end_x  =SCREEN_LENGTH/2 + placece_x[8] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].end_y  =SCREEN_WIDTH/2 + placece_y[9] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].radius=0;

//				/*********************************横线8*********************************/
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_name[0] = 0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_name[1] = 1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_name[2] = 17;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].operate_tpye=Op_type;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_tpye=7;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].layer=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].color=Green;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].start_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].end_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].width=1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].start_x=SCREEN_LENGTH/2 - placece_x[9] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].start_y=SCREEN_WIDTH/2 + placece_y[10] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].end_x  =SCREEN_LENGTH/2 + placece_x[9] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].end_y  =SCREEN_WIDTH/2 + placece_y[10] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].radius=0;
//				break;
//			case 2:
//				/*********************************横线9*********************************/
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[0] = 0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[1] = 1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_name[2] = 18;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].operate_tpye=Op_type;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].graphic_tpye=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].layer=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].color=Green;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].width=1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_x=SCREEN_LENGTH/2 - placece_x[10] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].start_y=SCREEN_WIDTH/2 + placece_y[11] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_x  =SCREEN_LENGTH/2 + placece_x[10] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].end_y  =SCREEN_WIDTH/2 + placece_y[11] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[0].radius=0;
//			
//				/*********************************横线10*********************************/
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[0] = 0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[1] = 1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_name[2] = 19;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].operate_tpye=Op_type;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].graphic_tpye=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].layer=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].color=Green;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].width=1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_x=SCREEN_LENGTH/2 - placece_x[11] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].start_y=SCREEN_WIDTH/2 + placece_y[12] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_x  =SCREEN_LENGTH/2 + placece_x[11] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].end_y  =SCREEN_WIDTH/2 + placece_y[12] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[1].radius=0;

//				/*********************************横线11*********************************/
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_name[0] = 0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_name[1] = 1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_name[2] = 20;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].operate_tpye=Op_type;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].graphic_tpye=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].layer=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].color=Green;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].start_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].end_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].width=1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].start_x=SCREEN_LENGTH/2 - placece_x[12] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].start_y=SCREEN_WIDTH/2 + placece_y[13] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].end_x  =SCREEN_LENGTH/2 + placece_x[12] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].end_y  =SCREEN_WIDTH/2 + placece_y[13] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[2].radius=0;

//				/*********************************横线12*********************************/
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_name[0] = 0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_name[1] = 1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_name[2] = 21;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].operate_tpye=Op_type;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].graphic_tpye=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].layer=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].color=Green;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].start_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].end_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].width=1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].start_x=SCREEN_LENGTH/2 - placece_x[13] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].start_y=SCREEN_WIDTH/2 + placece_y[14] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].end_x  =SCREEN_LENGTH/2 + placece_x[13] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].end_y  =SCREEN_WIDTH/2 + placece_y[14] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[3].radius=0;

//				/*********************************横线13*********************************/
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_name[0] = 0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_name[1] = 1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_name[2] = 22;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].operate_tpye=Op_type;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].graphic_tpye=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].layer=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].color=Green;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].start_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].end_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].width=1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].start_x=SCREEN_LENGTH/2 - placece_x[14] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].start_y=SCREEN_WIDTH/2 + placece_y[15] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].end_x  =SCREEN_LENGTH/2 + placece_x[14] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].end_y  =SCREEN_WIDTH/2 + placece_y[15] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[4].radius=0;

//				/*********************************横线14*********************************/
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_name[0] = 0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_name[1] = 1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_name[2] = 23;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].operate_tpye=Op_type;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].graphic_tpye=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].layer=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].color=Green;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].start_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].end_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].width=1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].start_x=SCREEN_LENGTH/2 - placece_x[15] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].start_y=SCREEN_WIDTH/2 + placece_y[16] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].end_x  =SCREEN_LENGTH/2 + placece_x[15] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].end_y  =SCREEN_WIDTH/2 + placece_y[16] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[5].radius=0;

//				/*********************************横线15*********************************/
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_name[0] = 0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_name[1] = 1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_name[2] = 24;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].operate_tpye=Op_type;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].graphic_tpye=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].layer=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].color=Green;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].start_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].end_angle=0;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].width=1;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].start_x=SCREEN_LENGTH/2 - placece_x[16] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].start_y=SCREEN_WIDTH/2 + placece_y[17] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].end_x  =SCREEN_LENGTH/2 + placece_x[16] + AIM_bias_x;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].end_y  =SCREEN_WIDTH/2 + placece_y[17] + AIM_bias_y;
//				custom_grapic_draw.graphic_custom.grapic_data_struct[6].radius=0;				
//				break;
//			default:
//				break;
//		}
//		pack_tick++;
//	}
//}
///**********************************************************************************************************
//*函 数 名: Graphic_Change_Check
//*功能说明: 检测有没有图形的变动，或者进行图层的初始化
//*形    参: 无
//*返 回 值: 
//**********************************************************************************************************/
//int Graphic_Change_Check(void)
//{
//	return Op_Init;
//}
///**********************************************************************************************************
//*函 数 名: referee_data_pack_handle
//*功能说明: 裁判系统图形数据打包发送
//*形    参: uint8_t sof,uint16_t cmd_id, uint8_t *p_data, uint16_t len
//*返 回 值: 无
//**********************************************************************************************************/
//uint8_t seq = 0;	
//void referee_data_pack_handle(uint8_t sof,uint16_t cmd_id, uint8_t *p_data, uint16_t len)
//{
//	int i;
//	uint16_t frame_length = frameheader_len + cmd_len + len + crc_len;   //数据帧长度	

//	memset(SendToSentey_Buff,0,frame_length);  //存储数据的数组清零
//	
//	/*****帧头打包*****/
//	SendToSentey_Buff[0] = sof;//数据帧起始字节
//	memcpy(&SendToSentey_Buff[1],(uint8_t*)&len, sizeof(len));//数据帧中data的长度
//	SendToSentey_Buff[3] = seq;//包序号
//	Append_CRC8_Check_Sum(SendToSentey_Buff,frameheader_len);  //帧头校验CRC8

//	/*****命令码打包*****/
//	memcpy(&SendToSentey_Buff[frameheader_len],(uint8_t*)&cmd_id, cmd_len);
//	
//	/*****数据打包*****/
//	memcpy(&SendToSentey_Buff[frameheader_len+cmd_len], p_data, len);
//	Append_CRC16_Check_Sum(SendToSentey_Buff,frame_length);  //一帧数据校验CRC16

//	if (seq == 0xff) seq=0;
//  else seq++;
//	
//	/*****数据上传*****/

//	USART_ClearFlag(USART1,USART_FLAG_TC);
//	for(i=0;i<frame_length;i++)
//	{
//	  USART_SendData(USART1,SendToSentey_Buff[i]);
//	  while (USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET); //等待之前的字符发送完成
//	}
//}
///**********************************************************************************************************
//*函 数 名: GraphicSendtask
//*功能说明: 图形发送任务
//*形    参: 无
//*返 回 值: 无
//**********************************************************************************************************/
//extern ext_student_interactive_char_header_data_t custom_char_draw;  //自定义字符绘制
//void GraphicSendtask(void *pvParameters)
//{
//	 static int tick;
//	 static int char_change_state,graphic_change_state;
//   while (1)
//	{
//   
//		tick++;
//		if(0==tick%5)	
//		{
//			/*字符，英文*/
////			char_change_state = Char_Change_Check();
////			if(char_change_state)			//检查有没有变化，没有变化就不发，节省带宽
////			{
////				JudgementCustomizeChar(char_change_state);
////				if(char_change_state != Op_None)
////					referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&custom_char_draw,sizeof(custom_char_draw));
////			}
//			/*浮点数，图形*/
//			graphic_change_state = Graphic_Change_Check();
//			if(graphic_change_state)
//			{
//				JudgementCustomizeGraphics(graphic_change_state);
//				if(graphic_change_state != Op_None)
//					referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&custom_grapic_draw,sizeof(custom_grapic_draw));
//			}
//		} 

//			vTaskDelay(1000); 	 
//#if INCLUDE_uxTaskGetStackHighWaterMark
//		
//#endif
//  }
//}