/* Includes */
#include "config.h"

/* define */

/* declare */
void vSystemInit( void );
void vDataInit( void );
void vDelayMsForInit( uint16_t t );

int main( void )
{
	for(int i =0;i<100000;i++)
	{
		for(int j=0;j<1000;j++);
	}
	
	vSystemInit();
	
	vDataInit();

	vCreateStartTask();
	
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
	vFrictionConfig();
	
	vDelayMsForInit( 800 );
}

void vDataInit( void )
{
	vDelayMsForInit( 100 );
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









