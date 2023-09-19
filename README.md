# 2023_Hust_Drone
2023无人机云台开源

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

