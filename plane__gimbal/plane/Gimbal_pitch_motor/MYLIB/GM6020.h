#ifndef __GM6020_H__
#define __GM6020_H__

#include "config.h"

/** 
  * 输出值范围为 -30000~30000
	
  * ID 设置的范围为1-8
  * 发送的标识符 0x200 --- 对应id为 1-4
  * DATA[0] id-1 高8位  
  * DATA[1] id-1 低8位
  * DATA[2] id-2 高8位
  * DATA[3] id-2 低8位
  * DATA[4] id-3 高8位
  * DATA[5] id-3 低8位
  * DATA[6] id-4 高8位
  * DATA[7] id-4 低8位
  * 发送的标识符 0x1FF --- 对应id为 5-7
  * DATA[0] id-5 高8位  
  * DATA[1] id-5 低8位
  * DATA[2] id-6 高8位
  * DATA[3] id-6 低8位
  * DATA[4] id-7 高8位
  * DATA[5] id-7 低8位
  * DATA[6] id-8 高8位
  * DATA[7] id-8 低8位
	
  * 反馈id 0x20+id(如id为1，标识符为 0x201)
  * 反馈内容
  * DATA[0] 转子机械角度高8位
  * DATA[1] 转子机械角度低8位
  * DATA[2] 转子转速高8位
  * DATA[3] 转子转速低8位
  * DATA[4] 实际输出转矩高8位
  * DATA[5] 实际输出转矩低8位
	
  * 发送频率 1kHZ
  * 转子机械角度范围 0~8191
  * 减速比 36:1 
	*/
  
struct GM6020
{
	uint8_t id;
	uint32_t sendId;
	uint32_t receiveId;
	uint8_t mode;
	
	int16_t sendData;
	uint16_t receiveAngle;
	float Angle_zeroCheck;
	int16_t receiveSpeed;
	int16_t qorque;//力矩
	float setPoint;
	
	pid_t speedPID;//rc pid 速度环		电机陀螺仪控制情况
	pid_t positionPID;//rc pid 位置环
	pid_t qorquePID;
	
	FuzzyPID speedPID_fuzzy;//rc 模糊pid速度环
	FuzzyPID positionPID_fuzzy;//rc 模糊pid位置环
	
	pid_t AutospeedPID;//辅瞄 pid 速度环		陀螺仪IMU控制
	pid_t AutopositionPID;//辅瞄 pid 位置环
	
	pid_t Motor_SpeedPID;	//纯电机控制
	pid_t Motor_PositionPID;
	
	ZeroCheck_Typedef zero;
	
	FeedForward_Typedef AngelFF,SpeedFF;
	float ffout;
	
	float feedback_angle;
	float set_angle;
	
	
	float imuSpeed;//云台电机速度
	float imuAngle;//云台电机角度
	uint32_t updateTime;
	uint8_t Ever_Lost;
};

#endif // _GM6020_H__















