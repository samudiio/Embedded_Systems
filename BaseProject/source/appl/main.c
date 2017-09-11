/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "board.h"
#include "app_scheduler.h"
#include "Tasks.h"    
#include <stdbool.h>
#include <stdio.h>

/*------------------------------------------------------------------------------
 *         Exported Variables
 *----------------------------------------------------------------------------*/
uint8_t ExtTsk_Activated;

/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/

TaskType Tasks[]={
/*  TaskPriority    TaskId      TaskState     TaskFunctionPointer */
  {      5,        TASK_1MS,    SUSPENDED,    vfnTsk_1ms           },
  {      4,        TASK_2MSA,   SUSPENDED,    vfnTsk_2msA          },
  {      4,        TASK_2MSB,   SUSPENDED,    vfnTsk_2msB          },
  {      3,        TASK_10MS,   SUSPENDED,    vfnTsk_10ms          },
  {      2,        TASK_50MS,   SUSPENDED,    vfnTsk_50ms          },
  {      1,        TASK_100MS,  SUSPENDED,    vfnTsk_100ms         },
  {      6,        TASK_EXTTG,  SUSPENDED,    vfnTsk_ExtTriggered  }
};

/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/

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

    /* Initialize Task Scheduler */
     vfnScheduler_Init(&Tasks[0]);

	/* Start execution of task scheduler */
	vfnScheduler_Start();

	/*-- Loop through all the periodic tasks from Task Scheduler --*/
	for(;;)
	{
		/* Perform all scheduled tasks */
		vfnTask_Scheduler();
		if(ExtTsk_Activated)
		{
		    ExtTsk_Activated = 0;
		    printf("External Task Execution .\n\r");
		}
	}

}
