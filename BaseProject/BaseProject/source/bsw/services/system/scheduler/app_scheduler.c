/*******************************************************************************/
/**
\file       app_scheduler.c
\brief      Multi-thread Task scheduler.
\author     Abraham Tezmol
\version    0.1
\date       09/09/2008
\author     Francisco Matinez
\version    0.2
\date       06/21/2016
\author     Francisco Matinez
\version    0.2
\date       09/04/2017
*/

/** Variable types and common definitions */

/** Scheduler function prototypes definitions */
#include "app_scheduler.h"
#include "chip.h"

typedef struct  
{
  uint8_t runTask;
  uint8_t taskOverload;
  uint8_t tickValue;
  TaskIdType taskId;
  FuncPtr tskFcnPtr;
}TaskCtrlType;

/* -- Global Variables --------------------------------------------------------*/
uint8_t gu8Scheduler_Status;
uint8_t gu8Scheduler_Counter;
uint8_t u8_10ms_Counter;
uint8_t u8_50ms_Counter;
uint8_t u8_100ms_Counter;
TaskIdType gu8Scheduler_Thread_ID;
TaskIdType gu8Scheduler_Thread_ID_Backup;

TaskCtrlType task_ctrl_array[TASK_MAXNUM];

/*******************************************************************************/
/**
* \brief    Periodic Interrupt Timer Service routine.                            \n
            This interrupt is the core of the task scheduler.                   \n
            It is executed every 500us                                          \n
            It defines 3 basic threads from which other 3 threads are derived:  \n
            a) 1ms thread (basic) ->  100ms thread (derived)                    \n
            b) 2ms A thread (basic)-> 50ms thread (derived)                     \n
            c) 2ms B thread (basic)-> 10ms thread (derived)                     \n
            It partitions core execution time into time slices (500us each one).\n 
            This arrangement assures core will have equal task loading across time.\n   
            For more information on how time slice is assigned to each thread,  \n
            refer to file "S12X Task Scheduler Layout.xls"
* \author   Abraham Tezmol
* \param    void
* \return   void
* \todo
*/

void vfnScheduler_Callback(void)
{
	
	/*-- Update scheduler control variable --*/
	gu8Scheduler_Counter++;
	
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/*  1ms execution thread - used to derive two execution threads:                */
	/*  a) 1ms thread (highest priority tasks)                                      */
	/*  b) 100ms thread (lowest priority tasks)                                     */
	/*  As any other thread on this scheduling scheme,                              */
	/*  all tasks must be executed in <= 500us                                      */
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	if ((gu8Scheduler_Counter & 0x01) == 0x01)
	{
		u8_100ms_Counter++;
		/*-- Allow 100 ms periodic tasks to be executed --*/
		if (u8_100ms_Counter >= 100)
		{
			gu8Scheduler_Thread_ID = TASK_100MS;
			u8_100ms_Counter = 0;
			task_ctrl_array[(uint8_t)TASK_100MS].runTask=1;
			task_ctrl_array[(uint8_t)TASK_100MS].tickValue = gu8Scheduler_Counter;
		}
		/*-- Allow 1 ms periodic tasks to be executed --*/
		else
		{
			gu8Scheduler_Thread_ID = TASK_1MS;
		}
		task_ctrl_array[(uint8_t)TASK_1MS].runTask=1;
		task_ctrl_array[(uint8_t)TASK_1MS].tickValue = gu8Scheduler_Counter;
	}
	else
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		/*  2ms execution thread - used to derive two execution threads:                */
		/*  a) 2ms group A thread (high priority tasks)                                 */
		/*  b) 50ms thread (second lowest priority tasks)                               */
		/*  As any other thread on this scheduling scheme,                              */
		/*  all tasks must be executed in <= 500us                                      */
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		if ((gu8Scheduler_Counter & 0x02) == 0x02)
		{
			u8_50ms_Counter++;
			/*-- Allow 50 ms periodic tasks to be executed --*/
			if (u8_50ms_Counter >= 25)
			{
				gu8Scheduler_Thread_ID = TASK_50MS;
				u8_50ms_Counter = 0;
				task_ctrl_array[(uint8_t)TASK_50MS].runTask=1;
				task_ctrl_array[(uint8_t)TASK_50MS].tickValue = gu8Scheduler_Counter;
			}
			/*-- Allow 2 ms group A periodic tasks to be executed --*/
			else
			{
				gu8Scheduler_Thread_ID = TASK_2MSA;
			}
			task_ctrl_array[(uint8_t)TASK_2MSA].runTask=1;
			task_ctrl_array[(uint8_t)TASK_2MSA].tickValue = gu8Scheduler_Counter;
		}
		else
		{
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
			/*  2ms execution thread - used to derive two execution threads:                */
			/*  a) 2ms group B thread (high priority tasks)                                 */
			/*  b) 10ms thread (medium priority tasks)                                      */
			/*  As any other thread on this scheduling scheme,                              */
			/*  all tasks must be executed in <= 500us                                      */
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
			if ((gu8Scheduler_Counter & 0x03) == 0x00)
			{
				u8_10ms_Counter++;
				/*-- Allow 10 ms periodic tasks to be executed --*/
				if (u8_10ms_Counter >= 5)
				{
					gu8Scheduler_Thread_ID = TASK_10MS;
					u8_10ms_Counter = 0;
					task_ctrl_array[(uint8_t)TASK_10MS].runTask=1;
					task_ctrl_array[(uint8_t)TASK_10MS].tickValue = gu8Scheduler_Counter;
				}
				/*-- Allow 2 ms group B periodic tasks to be executed --*/
				else
				{
					gu8Scheduler_Thread_ID = TASK_2MSB;
				}
				task_ctrl_array[(uint8_t)TASK_2MSB].runTask=1;
				task_ctrl_array[(uint8_t)TASK_2MSB].tickValue = gu8Scheduler_Counter;
			}
		}
	}
}

/*******************************************************************************/
/**
* \brief    Scheduler - Periodic Interrup Timer Initialization
* \author   Abraham Tezmol
* \param    void
* \return   void
* \todo     
*/
void vfnScheduler_Init(TaskType *TaskArray)
{   
  TaskIdType task_idx;
  /* Init Global and local Task Scheduler variables */
  gu8Scheduler_Counter = 0;   
  u8_10ms_Counter = 0;
  u8_50ms_Counter = 0;
  u8_100ms_Counter = 0;
  gu8Scheduler_Status = TASK_SCHEDULER_INIT;
	for (task_idx = 0; task_idx < (uint8_t)TASK_MAXNUM; task_idx++)
	{
		task_ctrl_array[task_idx].tskFcnPtr = TaskArray[task_idx].tskFcnPtr;
		task_ctrl_array[task_idx].taskId = TaskArray[task_idx].taskId;
	}
}

/*******************************************************************************/
/**
* \brief    Scheduler Start - Once time base is armed, start execution of   \n
            Multi-thread Round Robin scheduling scheme.                     \n
            This function requires prior execution of "vfnScheduler_Init"
* \author   Abraham Tezmol
* \param    void
* \return   void
* \todo     
*/
void vfnScheduler_Start(void)
{
    TimeTick_Configure(vfnScheduler_Callback);
    gu8Scheduler_Status = TASK_SCHEDULER_RUNNING;
}

/*******************************************************************************/
/**
* \brief    Scheduler Stop - stop execution of Multi-thread Round Robin scheduling scheme.
* \author   Abraham Tezmol
* \param    void
* \return   void
* \todo     
*/
void vfnScheduler_Stop(void)
{  
    /* Update scheduler status accordingly */
    gu8Scheduler_Status = TASK_SCHEDULER_HALTED;
}

/*******************************************************************************/
/**
* \brief    Multi-thread round robin task Scheduler  (non-preemtive)        \n
            It calls the different tasks based on the status of             \n   
            "gu8Scheduler_Thread_ID". This variable is modified by          \n
            ISR "vfnScheduler_PIT_Isr"                                        \n
            List of tasks shall be defined @ "tasks.h" file
* \author   Abraham Tezmol
* \param    void
* \return   void
* \todo     
*/
void vfnTask_Scheduler(void)
{
	TaskIdType task_idx;
	
	for (task_idx = 0;task_idx < (uint8_t)TASK_MAXNUM; task_idx++)
	{
		if( 1 == task_ctrl_array[task_idx].runTask )
		{
			task_ctrl_array[task_idx].runTask = 0;
			if ( NULL != task_ctrl_array[task_idx].tskFcnPtr )
			{
				task_ctrl_array[task_idx].tskFcnPtr();
			}
			if ( gu8Scheduler_Counter != task_ctrl_array[task_idx].tickValue )
			{
				task_ctrl_array[task_idx].taskOverload = 1;
				gu8Scheduler_Status = TASK_SCHEDULER_OVERLOAD;
			}
			else{
				gu8Scheduler_Status = TASK_SCHEDULER_RUNNING;
			}
		}
	}

}
