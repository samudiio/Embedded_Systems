/*
 * Tasks.c
 *
 * Created: 6/21/2016 7:25:43 PM
 *  Author: Francisco Martinez
 */ 

#include "Tasks.h"
#include "led.h"

uint8_t u8100ms_Ctr=0;
uint8_t u8100ms_Ctr2=0;


void vfnTsk_1ms(void)
{ 
  //LED_Toggle( 1 );

  vfnSchedulerPoint();
}

void vfnTsk_2msA(void)
{
  //LED_Toggle( 1 );

	vfnSchedulerPoint();
}

void vfnTsk_2msB(void)
{
  //LED_Toggle( 1 );

  vfnSchedulerPoint();	
}

void vfnTsk_10ms(void)
{	
  LED_Toggle( 1 );
  Wait(9);
  vfnSchedulerPoint();
}

void vfnTsk_50ms(void)
{  
  LED_Toggle( 1 );
  Wait(49);
  vfnSchedulerPoint();
}

void vfnTsk_100ms(void)
{ 
  // LED_Toggle( 1 );
  // Wait(99);
  vfnSchedulerPoint();   
}


void vfnTsk_ISR(void)
{
  LED_Clear(0);
  Wait(58);
  LED_Set(0);
  Wait(1);
  LED_Clear(0);  
  vfnSchedulerPoint();
}
