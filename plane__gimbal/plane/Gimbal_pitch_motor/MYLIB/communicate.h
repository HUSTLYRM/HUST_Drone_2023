#ifndef __COMMUNICATE_H__
#define __COMMUNICATE_H__

#include "config.h"

/* for ano_af v6 high btye in front */
/* for ano_af v7 low  byte in front */
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)		) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

#define JudgeSentey_DataLength			19
#define JudgeSentey_GraphLength		20
#define Graph_config_length 		15
#define GRAPH_1_LENGTH					30

#define JudgeSentey_CmdId					0x0301
#define JudgeOSD_DataId_DeletG	0x0100
#define JudgeOSD_DataId_1Graph	0x0101
#define JudgeOSD_DataId_2Graph	0x0102
#define JudgeOSD_DataId_5Graph	0x0103
#define JudgeOSD_DataId_7Graph	0x0104
#define JudgeOSD_DataId_String	0x0110
#define RED_DRONE_ID						6
#define BLUE_DRONE_ID						106
#define RED_DRONE_OSD_ID				0x0106
#define BLUE_DRONE_OSD_ID				0x016A

#define Dart_DataLength 1
#define ENERGY_POINT						0x10

/* osd graph operate type */
#define OPERATE_ADD			1
#define OPERATE_CHANGE	2
#define OPERATE_DELETE	3

/* osd graph gragh type */
//#define TYPE_CIRCLE			2
//#define TYPE_FLOAT			5
//#define TYPE_INTEGER		6
//#define TYPE_CHAR				7	

/* osd graph color */
#define COLOR_BLACK			7


/* RC structure */
typedef __packed struct{
	uint16_t ch0;
	uint16_t ch1;
	uint16_t ch2;
	uint16_t ch3;
	uint8_t s1;
	uint8_t s2;
}Remote;

typedef __packed 	struct{
	int16_t  x;
	int16_t y;
	int16_t z;
	uint8_t press_l;
	uint8_t press_r;
}Mouse;//鼠标控制按键

typedef __packed struct
{
	unsigned short w,s,a,d,q,e,r,f,g,z,x,c,v,b,shift,ctrl;
}Key;//键盘控制按键

typedef __packed struct{
	Remote rc;
	Mouse mouse;
	Key key;
  uint32_t updateTime;
	short DisConnect;
	uint8_t Ever_Lost;
}RC_Ctl_t;

/* PC Receive structure */
//辅瞄接收
typedef struct{
	float PCTargetPitch;
	float PCTargetYaw;
	short DisConnect;
	uint32_t PC_Receive_Cnt;
	uint32_t updateTime;
}PC_Recv_t;

typedef struct{
	/* status data */
	uint8_t robot_id;
	
	/* position data */
	float position_x;
	float position_y;
	float position_z;
	float position_yaw;
	
	/* drone energy data */
	uint16_t energy_point;
	uint8_t attack_time;
	
	/* realtime shot data */
	uint8_t bullet_type;
	uint8_t bullet_freq;
	float bullet_speed;
	
	/* bullet remain number */
	uint16_t bullet_remaining_num;
}tGameInfo;

typedef __packed struct{
	uint8_t graphic_name[3];
	uint32_t operate_tpye:3;
	uint32_t graphic_tpye:3;
	uint32_t layer:4;
	uint32_t color:4;
	uint32_t start_angle:9;
	uint32_t end_angle:9;
	uint32_t width:10;
	uint32_t start_x:11;
	uint32_t start_y:11;
	uint32_t integer_data:32;
}tGraphConfig_integer;

typedef __packed struct{
	uint8_t graphic_name[3];
	uint32_t operate_tpye:3;
	uint32_t graphic_tpye:3;
	uint32_t layer:4;
	uint32_t color:4;
	uint32_t start_angle:9;
	uint32_t end_angle:9;
	uint32_t width:10;
	uint32_t start_x:11;
	uint32_t start_y:11;
	uint32_t float_data:32;
}tGraphConfig_float;

typedef __packed struct{
	uint8_t graphic_name[3];
	uint32_t operate_tpye:3;
	uint32_t graphic_tpye:3;
	uint32_t layer:4;
	uint32_t color:4;
	uint32_t start_angle:9;
	uint32_t end_angle:9;
	uint32_t width:10;
	uint32_t start_x:11;
	uint32_t start_y:11;
	uint32_t radius:10;
	uint32_t end_x:11;
	uint32_t end_y:11;
}tGraphConfig_string;

typedef __packed struct{
	uint8_t graphic_name[3];
	uint32_t operate_tpye:3;
	uint32_t graphic_tpye:3;
	uint32_t layer:4;
	uint32_t color:4;
	uint32_t start_angle:9;
	uint32_t end_angle:9;
	uint32_t width:10;
	uint32_t start_x:11;
	uint32_t start_y:11;
	uint32_t radius:10;
	uint32_t end_x:11;
	uint32_t end_y:11;
}tGraphConfig_picture;
typedef __packed struct
{
	uint16_t data_cmd_id;	//数据段内容ID
	uint16_t sender_ID;	//发送者ID
	uint16_t receiver_ID;	//接受者ID
	uint8_t  data[2];
}ext_student_conmunicate_header_data_t;	

void Can1Receive0(CanRxMsg rx_message0);
void Can1Receive1(CanRxMsg rx_message1);

void Can2Receive0(CanRxMsg rx_message0);
void Can2Receive1(CanRxMsg rx_message1);

void RemoteReceive(volatile unsigned char rx_buffer[]);

void PCReceive(unsigned char PCReceivebuffer[]);
void PCSend(void);

void ANOSend(void);

void JudgeReceive(unsigned char ReceiveBuffer[]);
void JudgeSentey(uint8_t datatype);

void AppReceive(unsigned char AppReceivebuffer[]);

#endif


/* end */










