/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "board.h"
#include "app_scheduler.h"
#include "Tasks.h"    
#include <stdbool.h>
#include <stdio.h>

/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/

TaskType Tasks[]={
/*  TaskPriority    TaskId   TaskFunctionPointer taskState  */
  {      5,        TASK_1MS,       vfnTsk_1ms,    TASK_STATE_SUSPENDED    },
  {      5,        TASK_2MSA,      vfnTsk_2msA,   TASK_STATE_SUSPENDED   },
  {      5,        TASK_2MSB,      vfnTsk_2msB,   TASK_STATE_SUSPENDED   },
  {      3,        TASK_10MS,      vfnTsk_10ms,   TASK_STATE_SUSPENDED   },
  {      2,        TASK_50MS,      vfnTsk_50ms,   TASK_STATE_SUSPENDED   },
  {      5,        TASK_100MS,     vfnTsk_100ms,  TASK_STATE_SUSPENDED  },
  {      1,        TASK_ISR,       vfnTsk_ISR,    TASK_STATE_SUSPENDED  }
};

/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/

/**
 *  Interrupt handler for TC0 interrupt.
 */

/**
 *  Configure Timer Counter 0 to generate an interrupt every 1s.
 */



/**
 *  \brief Configure LEDs
 *
 *  Configures LEDs \#1 and \#2 (cleared by default).
 */
static void _ConfigureLeds( void )
{
	LED_Configure( 0 ) ;
	LED_Configure( 1 ) ;
}

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/
/**
 *  \brief getting-started Application entry point.
 *
 *  \return Unused (ANSI-C compatibility).
 */
extern int main( void )
{

	/* Disable watchdog */
	WDT_Disable( WDT ) ;

	/* Output example information */
	printf( "\n\r-- Getting Started Example Workspace Updated!!! %s --\n\r", SOFTPACK_VERSION ) ;
	printf( "-- %s\n\r", BOARD_NAME ) ;
	printf( "-- Compiled: %s %s With %s--\n\r", __DATE__, __TIME__ , COMPILER_NAME);

	/* Enable I and D cache */
	SCB_EnableICache();
  SCB_EnableDCache();

	printf( "Configure LED PIOs.\n\r" ) ;
	_ConfigureLeds() ;
  //printf( "Configure UART4.\n\r" ) ;
  //vfnSerialCtrl_Init();
  printf("Configure TC.\n\r");
	Timer0_Init();
  
  	/* Initialize Task Scheduler */
	vfnScheduler_Init(&Tasks[0]);
	/* Start execution of task scheduler */
	vfnScheduler_Start();
  
  printf("Configure TC.\n\r");
  Wait(2);// Delay para desfasar TM0 y systick
	Timer0_Init();

	/*-- Loop through all the periodic tasks from Task Scheduler --*/
	for(;;)
	{
		/* Perform all scheduled tasks */
		vfnTask_Scheduler();
    //vfnSchedulerPoint();
	}

}
