#ifndef _ICM_20602_H
#define _ICM_20602_H

/* 宏定义 */
#ifndef PI
#define PI 3.14159265358979f
#endif
#define GRAVITY 9.78f //重力加速度
#define GYRO_SCALE PI / 16.3835F / 180
#define ACC_SCALE GRAVITY / 8192.0F

/*  Params */
#define GxOFFSET -0.00570511958
#define GyOFFSET 0.00908600073
#define GzOFFSET -0.00701971818
#define gNORM 9.61901569
#define G_NORM_MAX_MIN_DIFF_THRESH 0.2f
#define GYRO_DIFF_THRESH 0.35f
#define GYRO_OFFSET_MAX 0.2f

#define CALIBRATE 1 //是否实时标定

#define MAX_CALIB_TIMES 6000
#define MIN_CALIB_TIMES 4000

#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "counter.h"

/**
 * @brief 接收IMU数据定义
 */
typedef struct ICM20602
{
    /* data */
    // unsigned char GyroUpdateFlag;
    short Gyro[3];
    // unsigned char AccUpdateFlag;
    short Acc[3];
} IMU;

/**
 * @brief 解码后IMU定义
 */
typedef struct
{
    float Accel[3];

    float Gyro[3];

    /* 标定数据 */
    float AccelScale;
    float GyroOffset[3];
    float gNorm;
} IMU_Data_t;

void ICM_20602_Decode(IMU *IMUReceive, IMU_Data_t *IMUData);
void Calibrate_IMU_Offset(IMU *IMUReceive, IMU_Data_t *icm_20602);
void ICM20602_init(IMU *IMUReceive, IMU_Data_t *IMUData);

#endif // !_ICM_20602_H
