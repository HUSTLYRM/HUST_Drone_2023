#ifndef _ACTIONCONTROLTASK_H
#define _ACTIONCONTROLTASK_H

#include "config.h"

/*--------------------------ģʽ����----------------------------*/

#define Lost 0
#define Rc   1
#define Pc   2
#define Auto 3

typedef enum
{
    MouseKeyMode=0x001,
    RemoteMode  =0x002,
    CeaseMode   =0x000
} CtrMode_typedef;

typedef enum
{
    Chassis_Act_Mode			=	0x001,
    Chassis_SelfProtect_Mode  =	0x002,
    Chassis_CeaseMode   	=	0x000,
    Chassis_Single_Mode		= 0x005,
	  Chassis_Sniper_Mode   = 0x004,
	  Chassis_Solo_Mode     = 0x003
	  
} ChassisMode_typedef;

typedef enum
{
    Gimbal_Act_Mode			=	0x001,
    Gimbal_Armor_Mode  	=	0x002,
    Gimbal_CeaseMode   	=	0x000,
    Gimbal_TurnHeadMode = 0x003,
    Gimbal_SingleMode		= 0x004,
	  Gimbal_Lob_Mode     = 0x005
} GimbalMode_typedef;

typedef enum
{
    Shoot_Act_Mode			=	0x001,
    Shoot_CeaseMode   	=	0x000
} ShootMode_typedef;

enum
{
    Left=0,
    Right=1,
};

enum
{
    In=0,
    Out=1,
};

enum         //���ӵ�λ��
{
    LF=2,
    LB=0,
    RF=3,
    RB=1,
};
enum      //ƽ�� ǰ�� ��ת
{
    x=0,
    y=1,
    w=2,
};

enum
{
    now=0,
    last=1,
    next=2,
};

enum
{
    on=0x1,
    off=0x0,
};

//������Ϣ�����壬�������ߣ��ڴ���������ݴ���ȣ��ڵ���ʱ�ṩ����

typedef struct
{
    short pre_key_z;
    short z_rise_flag;
    short pre_key_x;
    short x_rise_flag;
    short pre_key_c;
    short c_rise_flag;
    short pre_key_v;
    short v_rise_flag;
	  short pre_key_g;
    short g_rise_flag;
    short pre_key_b;
    short b_rise_flag;
    short pre_key_f;
    short f_rise_flag;
    short pre_key_r;
    short r_rise_flag;	
    short pre_mouse_l;
    short mouse_l_rise_flag;

    short pre_mouse_r;
    short mouse_r_rise_flag;

} Key_typedef;

typedef struct
{
    ChassisMode_typedef 	ChassisMode;
    GimbalMode_typedef		GimbalMode;
    ShootMode_typedef			ShootMode;
    CtrMode_typedef		CtrMode;
} HeroStatus_typedef;
void GraphicsUpdate(void);
void 	HeroStatus_Cal(void);
void SetInputMode(RC_Ctl_t* RC_Ctl);
void Powerdown_Process(void);
void Remote_Process(RC_Ctl_t* RC_Ctl);
void Mouse_Key_Process(RC_Ctl_t* RC_Ctl);
void Change_Sensitivity(void);
void ActionControl_task(void *pvParameters);
#endif