
#ifndef __GRAPHICS_SEND_TASK_H
#define __GRAPHICS_SEND_TASK_H

#include "stdint.h"


#define SEND_MAX_SIZE    128    //上传数据最大的长度
#define frameheader_len  5       //帧头长度
#define cmd_len          2       //命令码长度
#define crc_len          2       //CRC16校验
/* 裁判系统数据包参数 */
#define HEADER_LEN 			 5       //帧头长度
#define CMD_LEN          2       //命令码长度
#define CRC_LEN          2       //尾部CRC16校验
#define DRAWING_PACK     15      //画1个图数据包长度
/*屏幕宽度*/
#define SCREEN_WIDTH 1080
#define SCREEN_LENGTH 1920			//屏幕分辨率
/* 比赛类型 */
#define Game_Type_RMUC 1	 //超级对抗赛
#define Game_Type_RMUT 2	 //单项赛
#define Game_Type_RMUA 3	 //人工智能挑战赛
#define Game_Type_RMUL_3V3 4 //高校联盟赛3V3
#define Game_Type_RMUL_1V1 5 //高校联盟赛1V1

/* 比赛阶段 */
#define Game_Progress_Unstart 0	  //未开始比赛
#define Game_Progress_Prepare 1	  //准备阶段
#define Game_Progress_SelfCheck 2 //自检阶段
#define Game_Progress_5sCount 3	  // 5s倒计时
#define Game_Progress_Battle 4	  //对战中
#define Game_Progress_Calculate 5 //比赛结算中

/* 比赛结果 */
#define Game_Result_Draw 0	  //平局
#define Game_Result_RedWin 1  //红方胜利
#define Game_Result_BlueWin 2 //蓝方胜利

/* 警告信息 */
#define Warning_Yellow 1  //黄牌警告
#define Warning_Red 2	  //红牌警告
#define Warning_Failure 3 //判负

/* 机器人ID */
#define Robot_ID_Red_Hero 1			//红方英雄
#define Robot_ID_Red_Engineer 2		//红方工程
#define Robot_ID_Red_Infantry3 3	//红方步兵3
#define Robot_ID_Red_Infantry4 4	//红方步兵4
#define Robot_ID_Red_Infantry5 5	//红方步兵5
#define Robot_ID_Red_Aerial 6		//红方无人机
#define Robot_ID_Red_Sentry 7		//红方哨兵
#define Robot_ID_Red_Darts 8		//红方飞镖
#define Robot_ID_Red_Radar 9		//红方雷达
#define Robot_ID_Blue_Hero 101		//蓝方英雄
#define Robot_ID_Blue_Engineer 102	//蓝方工程
#define Robot_ID_Blue_Infantry3 103 //蓝方步兵3
#define Robot_ID_Blue_Infantry4 104 //蓝方步兵4
#define Robot_ID_Blue_Infantry5 105 //蓝方步兵5
#define Robot_ID_Blue_Aerial 106	//蓝方无人机
#define Robot_ID_Blue_Sentry 107	//蓝方哨兵
#define Robot_ID_Blue_Darts 108		//蓝方飞镖
#define Robot_ID_Blue_Radar 109		//蓝方雷达

/* 操作手ID */
#define Cilent_ID_Red_Hero 0x0101		//红方英雄操作手
#define Cilent_ID_Red_Engineer 0x0102	//红方工程操作手
#define Cilent_ID_Red_Infantry3 0x0103	//红方步兵3操作手
#define Cilent_ID_Red_Infantry4 0x0104	//红方步兵4操作手
#define Cilent_ID_Red_Infantry5 0x0105	//红方步兵5操作手
#define Cilent_ID_Red_Aerial 0x0106		//红方飞手
#define Cilent_ID_Blue_Hero 0x0165		//蓝方英雄操作手
#define Cilent_ID_Blue_Engineer 0x0166	//蓝方工程操作手
#define Cilent_ID_Blue_Infantry3 0x0167 //蓝方步兵3操作手
#define Cilent_ID_Blue_Infantry4 0x0168 //蓝方步兵4操作手
#define Cilent_ID_Blue_Infantry5 0x0169 //蓝方步兵5操作手
#define Cilent_ID_Blue_Aerial 0x016A	//蓝方飞手

/* 裁判系统数据段内容ID */
#define Drawing_Delete_ID				0x0100
#define Drawing_Graphic1_ID     		0x0101
#define Drawing_Graphic2_ID 			0x0102
#define Drawing_Graphic5_ID     		0x0103
#define Drawing_Graphic7_ID     		0x0104
#define Drawing_Char_ID     			0x0110

/*UI操作类型*/
#define Op_None 0			//空操作
#define Op_Add 1			//增加图形
#define Op_Change 2		//修改图形
#define Op_Delete 3		//删除图形
#define Op_Init		1		//初始化，也就是增加图层

/*UI图形颜色*/
#define Red_Blue 0
#define Yellow   1
#define Green    2
#define Orange   3
#define Purple	 4
#define Pink     5
#define Cyan		 6
#define Black    7
#define White    8


//图形数据
typedef __packed struct		//绘制UI UI字符串数据
{
	uint8_t graphic_name[3];	//图形名作为客户端指引
	uint32_t operate_tpye:3;	//UI图形操作
	uint32_t graphic_tpye:3;	//UI图形种类
	uint32_t layer:4;					//UI图层0-9
	uint32_t color:4;					//UI图像颜色0-8
	uint32_t start_angle:9;
	uint32_t end_angle:9;	
	uint32_t width:10;		//线宽
	uint32_t start_x:11;	//起始坐标X
	uint32_t start_y:11;	//起始坐标Y
	uint32_t radius:10;	
	uint32_t end_x:11;	//截止坐标X
	uint32_t end_y:11;	//截止坐标Y
}graphic_data_struct_t;	

/* 图形绘制类型 */
typedef enum {
	TYPE_LINE = 0U,
	TYPE_RECTANGLE = 1U,
	TYPE_CIRCLE = 2U,
	TYPE_OVAL = 3U,
	TYPE_ARC = 4U,
	TYPE_FLOAT = 5U,
	TYPE_INT = 6U,
	TYPE_CHAR = 7U,
} graphic_tpye;


/*裁判系统发送信息库*详情见裁判系统串口协议*/
typedef __packed struct
{
	uint16_t data_cmd_id;
	uint16_t send_ID;
	uint16_t receiver_ID;
}student_interactive_header_data_t;//交互数据

typedef __packed struct
{
	uint8_t data[113];
}robot_interactive_data_t;//交互数据

typedef __packed struct		//绘制UI UI删除图形数据 
{
	uint8_t operate_tpye;		//0空操作  1删除单个图层  2删除所有图层
	uint8_t layer;					//图层号  0~9
}ext_client_custom_graphic_delete_t;//客户端删除图形

typedef __packed struct
{
	graphic_data_struct_t grapic_data_struct;
}ext_client_custom_graphic_single_t;//客户端绘制一个图形

typedef __packed struct
{
	graphic_data_struct_t grapic_data_struct[2];
}ext_client_custom_graphic_double_t;//客户端绘制两个图形

typedef __packed struct
{
	graphic_data_struct_t grapic_data_struct[5];
}ext_client_custom_graphic_five_t;//客户端绘制五个图形

typedef __packed struct
{
	graphic_data_struct_t grapic_data_struct[7];
}ext_client_custom_graphic_seven_t;//客户端绘制七个图形

typedef __packed struct
{
	graphic_data_struct_t grapic_data_struct;
	char data[30];
}ext_client_custom_character_t;//客户端绘制字符

//交互数据信息
typedef __packed struct
{
	uint16_t data_cmd_id;	//数据段内容ID
	uint16_t sender_ID;	//发送者ID
	uint16_t receiver_ID;	//接受者ID
	ext_client_custom_graphic_seven_t graphic_custom;//自定义图形数据: 客户端绘制七个图形
}ext_student_interactive_header_data_t;	

typedef __packed struct
{
	uint16_t data_cmd_id;	//数据段内容ID
	uint16_t sender_ID;	//发送者ID
	uint16_t receiver_ID;	//接受者ID
	ext_client_custom_character_t char_custom;//自定义字符串数据
}ext_student_interactive_char_header_data_t;



void JudgementDataSend(void);
void JudgementCustomizeGraphics(int Op_type);//图形
void JudgementCustomizeChar(int Op_type);//字符
void referee_data_pack_handle(uint8_t sof,uint16_t cmd_id, uint8_t *p_data, uint16_t len);
void referee_data_load_Graphic(int Op_type);
void referee_data_load_String(int Op_type);
void GraphicSendtask(void *pvParameters);


void referee_data_load_shootUI(uint8_t operate_type,uint8_t robot_level);
void referee_data_load_NumberUI(void);
void GraphicSendtask(void *pvParameters);

void Send_UIPack(uint16_t data_cmd_id, uint16_t SendID,uint16_t receiverID, uint8_t* data, uint16_t pack_len);
void Send_toReferee(uint16_t cmd_id, uint16_t data_len);//
//直线图形数据结构体
graphic_data_struct_t* Line_Draw(uint8_t layer,int Op_Type,uint16_t startx,uint16_t starty,uint16_t endx,uint16_t endy, uint16_t line_width, int color,uint8_t name[]);
//矩形图形数据结构体
graphic_data_struct_t* Rectangle_Draw(uint8_t layer,int Op_Type,uint16_t startx,uint16_t starty,uint16_t endx,uint16_t endy, uint16_t line_width, int color,uint8_t name[]);
//得到绘制浮点图形结构体
graphic_data_struct_t* FloatData_Draw(uint8_t layer,int Op_Type,uint16_t startx,uint16_t starty, float data_f, uint8_t size ,uint8_t valid_bit, uint16_t line_width, int color,uint8_t name[]);
//得到字符图形数据结构体
graphic_data_struct_t* CharGraphic_Draw(uint8_t layer,int Op_Type,uint16_t startx,uint16_t starty, uint8_t size, uint8_t len, uint16_t line_width, int color,uint8_t name[]);
//圆形图形数据结构体
graphic_data_struct_t* Circle_Draw(uint8_t layer,int Op_Type,uint16_t startx,uint16_t starty,uint32_t radius, uint16_t line_width, int color,uint8_t name[]);

void UI_Distance_Init(void);
void UI_Distance_Change(void);
void ShootLines_Init(void);
void Char_Init(void);
void PitchUI_Change(short Pitch_100 ,uint8_t Init_Cnt);
void YawUI_Change(short Yaw_100 ,uint8_t Init_Cnt);
void GimbalMode_Change(char Gimbal_Flag,uint8_t Init_Cnt);
void FrictionMode_Change(char Fric_Flag,uint8_t Init_Cnt);


#endif
