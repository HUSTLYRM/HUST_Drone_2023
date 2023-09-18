/* Includes */
#include "config.h"

/* define */

/* declare */
void vSystemInit( void );
void vDelayMsForInit( uint16_t t );


//extern IMU IMUReceive;
//extern IMU_Data_t IMUData;

int main( void )
{
	vSystemInit();
	
	vDelayMsForInit(100);
	vCreateStartTask();
	vDelayMsForInit(100);
  vTaskStartScheduler();          
}

void vSystemInit( void )
{
	vLedGpioConfig();
	/* interrupt priority */
	vNvicConfig();
	/* Ano earth station */
	vUart2Config();
	/* remote control receive */
	vUart3Config();
	/* PC communicate */
	vUart4Config();
	/* Printf usart */
	vUart5Config();
	/* GM6020 motor communicate */
	vCan1Config();
	/* IMU communicate */
	vCan2Config();
	/* friction wheel motor control */
	vTim4Config();
	
	COUNTER_Configuration();
	OS_TICK_Configuration();
//	
//	INS_Init();
//	ICM20602_init(&IMUReceive, &IMUData);
	
	vFrictionConfig();
	vLaserGpioConfig();
	vUart1Config();
	vDelayMsForInit( 800 );
}

/* rough delay founction (occupy cpu) */
void vDelayMsForInit( uint16_t t )
{
	int i;
	
	for( i=0;i<t;i++)
	{
		int a=10300;
		
 		while(a--);
	}
}

/* end */









