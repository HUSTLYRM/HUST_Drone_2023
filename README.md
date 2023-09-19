# 2023_Hust_Drone

2023无人机云台开源

## 简介

### 硬件配置

硬件使用STM32F405RGT6作为主控芯片，主控板为自研通用云台板或自研B板

陀螺仪采用ICM20602

电机型号为GM6020、M3508、M2006，使用C610和C620电调。

使用Jlink在SWD模式下调试。

### 功能

- 双轴云台控制
- 客户端UI界面的绘制
- 卡尔曼滤波陀螺仪解算
- 双轴云台陀螺仪软件限位

### 部署需要调节的参数

- 陀螺仪控制模式和电机角控制模式下的云台pid参数
- 无人机型号宏定义
- 无人机云台软件限位角度
- 无人机云台前馈参数
- 摩擦轮和拨盘pid参数

### 环境配置

- Keil μVision V5.30（整车主体工程部分）
- CubeMX V6.6.1（陀螺仪硬件驱动部分）
- Jscope（可视化调试工具）

## 代码结构

```py
└─plane
    │  Test.jscope
    │  
    ├─Gimbal_pitch_motor
    │  │  keilkilll.bat
    │  │  
    │  ├─CORE
    │  │      core_cm4.h
    │  │      core_cm4_simd.h
    │  │      core_cmFunc.h
    │  │      core_cmInstr.h
    │  │      startup_stm32f40_41xxx.s
    │  │      
    │  ├─FreeRTOS
    │  │     croutine.c
    │  │     event_groups.c
    │  │     list.c
    │  │     queue.c
    │  │     readme.txt
    │  │     tasks.c
    │  │     timers.c
    │  │                      
    │  ├─FWLIB
    │  │  ├─inc
    │  │  │      
    │  │  └─src
    │  │          
    │  ├─MYLIB
    │  │  │  arm_math.h
    │  │  │  bsp_referee.c
    │  │  │  bsp_referee.h
    │  │  │  can.c
    │  │  │  can.h
    │  │  │  communicate.c
    │  │  │  communicate.h
    │  │  │  config.h
    │  │  │  crc.c
    │  │  │  crc.h
    │  │  │  FeedForward.c
    │  │  │  FeedForward.h
    │  │  │  friction.c
    │  │  │  friction.h
    │  │  │  GM6020.h
    │  │  │  icm20602.c
    │  │  │  icm20602.h
    │  │  │  IMU.h
    │  │  │  ins.c
    │  │  │  ins.h
    │  │  │  iwdg.c
    │  │  │  iwdg.h
    │  │  │  kalman_filter.c
    │  │  │  kalman_filter.h
    │  │  │  laser.c
    │  │  │  laser.h
    │  │  │  led.c
    │  │  │  led.h
    │  │  │  M2006.h
    │  │  │  motor.c
    │  │  │  motor.h
    │  │  │  MX64.h
    │  │  │  nvic.c
    │  │  │  nvic.h
    │  │  │  pid.c
    │  │  │  pid.h
    │  │  │  protocol.h
    │  │  │  QuaternionEKF.c
    │  │  │  QuaternionEKF.h
    │  │  │  Referee.c
    │  │  │  Referee.h
    │  │  │  tim.c
    │  │  │  tim.h
    │  │  │  trajectory.c
    │  │  │  trajectory.h
    │  │  │  uart.c
    │  │  │  uart.h
    │  │  │  zerocheck.c
    │  │  │  zerocheck.h
    │  │  │  
    │  │  └─MathLib
    │  │        
    │  ├─MYTASK
    │  │      ActionControl_task.c
    │  │      ActionControl_task.h
    │  │      CharSendTask.c
    │  │      CharSendTask.h
    │  │      CPU_Task.c
    │  │      CPU_Task.h
    │  │      debug_task.c
    │  │      debug_task.h
    │  │      gimbal_task.c
    │  │      gimbal_task.h
    │  │      GraphicsSendTask.c
    │  │      GraphicsSendTask.h
    │  │      InsTask.c
    │  │      InsTask.h
    │  │      judge_task.c
    │  │      judge_task.h
    │  │      RefereeTask.c
    │  │      RefereeTask.h
    │  │      selfcheck_task.c
    │  │      selfcheck_task.h
    │  │      shoot_task.c
    │  │      shoot_task.h
    │  │      start_task.c
    │  │      start_task.h
    │  │      
    │  ├─OBJ
    │  ├─SEGGER_RTT_V784
    │  │  │  LICENSE.md
    │  │  │  README.md
    │  │  │  
    │  │  ├─Config
    │  │  │      SEGGER_RTT_Conf.h
    │  │  │      
    │  │  ├─Examples
    │  │  │      Main_RTT_InputEchoApp.c
    │  │  │      Main_RTT_MenuApp.c
    │  │  │      Main_RTT_PrintfTest.c
    │  │  │      Main_RTT_SpeedTestApp.c
    │  │  │      
    │  │  ├─RTT
    │  │  │      SEGGER_RTT.c
    │  │  │      SEGGER_RTT.h
    │  │  │      SEGGER_RTT_ASM_ARMv7M.S
    │  │  │      SEGGER_RTT_printf.c
    │  │  │      
    │  │  └─Syscalls
    │  │          SEGGER_RTT_Syscalls_GCC.c
    │  │          SEGGER_RTT_Syscalls_IAR.c
    │  │          SEGGER_RTT_Syscalls_KEIL.c
    │  │          SEGGER_RTT_Syscalls_SES.c
    │  │          
    │  ├─TSET_FUNC
    │  │      counter.c
    │  │      counter.h
    │  │      CPU_Task.c
    │  │      CPU_Task.h
    │  │      debug.c
    │  │      debug.h
    │  │      os_tick.c
    │  │      os_tick.h
    │  │      tools.c
    │  │      tools.h
    │  │      
    │  └─USER
    │      │  ActionCtrol——task.c
    │      │  JLinkLog.txt
    │      │  JLinkSettings.ini
    │      │  main.c
    │      │  无人机云台 - 快捷方式.lnk
    │      │  
    │      ├─DebugConfig
    │      │      
    │      ├─RTE
    │      │  └─_F405RGT6_Demo
    │      │          RTE_Components.h
    │      │          
    │      └─SystemConfig
    │              FreeRTOSConfig.h
    │              stm32f4xx.h
    │              stm32f4xx_conf.h
    │              stm32f4xx_it.c
    │              stm32f4xx_it.h
    │              system_stm32f4xx.c
    │              system_stm32f4xx.h
    │              
    └─Python
```

## 编程规范

### 1. 文件规范

- 命名：大驼峰型(ChassisTask.c, AlgorithmOfCRC.c)
- 所有源文件在文件开头添加注释，注释规范如下：

```
/**
	*@brief 简述
	*@author 作者
	*@date 2021-09-18
*/
```

- 除内联函数外，所有函数不得在头文件实例化
- 头文件应使用 ifndef 进行保护

```
<sample.h>
#ifndef __SAMPLE_H
#define __SAMPLE_H
#endif
```

### 2. 函数规范

#### 1. 注释模板

```
/**
	*@brief 简述
	*@param1 参数1
	*@param2 参数2（有几个参数就写几个）
	*@return 返回值
*/
```

#### 2. 命名规范

小驼峰(myName)

动词+名词(getLength, setParam) or 名词+动词(usartInit, dataRst)

#### 3. 作用域规范

- 只在当前文件内部使用的函数：声明为static
- 需要外部调用的函数：使用接口函数+函数指针 详见4. C语言实现类

### 3. 变量规范

#### 1. 命名规范

| 类型           | 规范                  | 示例                             |
| -------------- | --------------------- | -------------------------------- |
| 结构体类型定义 | 大驼峰，最后加_t      | typedef struct{ }MotorData_t;    |
| 结构体变量     | 大驼峰                | MotorData_t YawMotor;            |
| 一般变量       | 全小写，下划线分隔    | present_value                    |
| 指针变量       | 以p_开头              | int *p_value                     |
| 全局变量       | 以g_开头              | g_judge                          |
| 宏定义，常量   | 全部大写              | REMOTE_RX_BUFFER_MAX             |
| 枚举类型       | 全部大写，最后加_ENUM | typedef enum{ }MOTOR_MODEL_ENUM; |

#### 2. 使用规范

- 变量名称基本原则：
  - 禁止无意义的命名
  - 禁止出现拼音。请使用简介明了的英文或缩写
  - 好的代码是不解自明的：通过变量名即可明白该变量的作用。必要时添加注释辅助说明。
- 同一模块的变量，使用结构体封装管理 如Remote_Data_t
- 特定模块的变量，封装在函数内部，定义为函数内部变量(static)
- 文件内部变量均标为static
- 必需时才声明为全局变量，用g_标注

## 如何使用

本工程中，步兵的各项功能均模块化，可以直接移植相关.c和.h文件到其他工程，简单修改API和硬件配置即可完成编译要求。

Keil编译完成无错误之后，通过Jlink的SWD工作模式下载到主控板，即可运行程序。

## 硬件和软件框图

### 结构分层

任务层:任务多线程执行函数,通过任务调度优先级分别调用,将具体动作功能分线程调度

行为层:具体的机器人动作

控制运算层:对数据进行处理,PID计算,卡尔曼滤波,前馈计算等

通信层:机器人各个部件的信息交流协议和通信格式

硬件驱动层:单片机外设配置初始化和驱动编写

### 遥控器控制

---电机反馈模式   	 ---键鼠模式

---陀螺仪反馈模式    ---辅瞄模式

---掉电保护模式   	---遥控模式

左摇杆:上为单发,下为连发射击,左右控制无

右摇杆:云台控制上下左右

## 优化方向

1. 优化程序架构

   提高模块化程度，将各模块代码进行进一步封装,做到各个功能模块去耦合，更换硬件或者修改拓扑时不需要对软件程序进行大规模修改。尽量利用宏定义或全局变量进行操作

2. 增加异常检测和应对机制

   在硬件出现问题时可以尽可能保持云台维持正常,设置保护模式等,不至于直接瘫痪失去战斗能力。

3. 添加LOG系统

   目前无人机还未添加LOG系统,未使用SD卡进行状态数据存储,后续可以加入SD卡的LOG系统来对开机状态下的云台各参数进行数据监控和复盘

4. 更稳定、易调试的控制系统设计

   pid控制已经满足上场对抗的基本要求，但是为了追求辅瞄以及飞行状态下击打地面单位更好的效果，我们希望能够探索更有效快速的控制系统，与此同时不能失去调试成本和效果之间的平衡。

5. 更好的防枪管抖动设计

   目前利用pid参数的调试能尽可能解决无人机在高射速射频的情况下枪管不抖动,但是这牺牲了Pitch轴的响应速度,后续会探索在防抖动和响应速度二者兼具的系统,以提高无人机云台的射击响应能力

## 特别鸣谢

陀螺仪解算及云台控制器：哈尔滨工程大学 创梦之翼战队 王洪玺 https://github.com/WangHongxi2001/RoboMaster-C-Board-INS-Example

