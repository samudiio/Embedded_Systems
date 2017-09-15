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
  uint8_t taskPriority;
  TaskIdType taskId;
  TaskStateType taskState;
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

uint8_t active_task_priority = 0;

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
			task_ctrl_array[(uint8_t)TASK_100MS].taskState = READY;
			task_ctrl_array[(uint8_t)TASK_100MS].tickValue = gu8Scheduler_Counter;
		}
		/*-- Allow 1 ms periodic tasks to be executed --*/
		else
		{
			gu8Scheduler_Thread_ID = TASK_1MS;
		}
		task_ctrl_array[(uint8_t)TASK_1MS].taskState = READY;
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
				task_ctrl_array[(uint8_t)TASK_50MS].taskState = READY;
				task_ctrl_array[(uint8_t)TASK_50MS].tickValue = gu8Scheduler_Counter;
			}
			/*-- Allow 2 ms group A periodic tasks to be executed --*/
			else
			{
				gu8Scheduler_Thread_ID = TASK_2MSA;
			}
			 task_ctrl_array[(uint8_t)TASK_2MSA].taskState = READY;
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
					task_ctrl_array[(uint8_t)TASK_10MS].taskState = READY;
					task_ctrl_array[(uint8_t)TASK_10MS].tickValue = gu8Scheduler_Counter;
				}
				/*-- Allow 2 ms group B periodic tasks to be executed --*/
				else
				{
					gu8Scheduler_Thread_ID = TASK_2MSB;
				}
				task_ctrl_array[(uint8_t)TASK_2MSB].taskState = READY;
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
	    task_ctrl_array[task_idx].taskState = TaskArray[task_idx].taskState;
	    task_ctrl_array[task_idx].taskPriority = TaskArray[task_idx].taskPriority;
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
    LED_Configure( 0 ) ;
    LED_Configure( 1 ) ;
    _ConfigureTc();
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

/**
 *  \brief Activates event triggered tasks according to the TaskId parameterTask
 *
 */
void vfnActivateTask(TaskIdType TaskId)
{
    if(TaskId < (uint8_t)TASK_MAXNUM)
    {
        task_ctrl_array[TaskId].taskState = READY;
    }
}

/**
 *  Check tasks in active state in order to identify higher priority task
 *  and execute it.
 *
 */
void vfnSchedulePoint(void)
{
    TaskIdType task_idx;

    /*save current active task Id and priority*/
    for (task_idx = 0;task_idx < (uint8_t)TASK_MAXNUM; task_idx++)
    {
        if(task_ctrl_array[task_idx].taskState == RUNNING)
        {
            active_task_priority = task_ctrl_array[task_idx].taskPriority;

        }
    }
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
    TaskIdType task_idx = 0;
    uint8_t ready_tasksid[TASK_MAXNUM] = { 0 };
    uint8_t ready_tasksprio = 0;
    uint8_t indx = 0;
    uint8_t location = 0;

    /*First check if any task is in ready state*/
    for (task_idx = 0;task_idx < (uint8_t)TASK_MAXNUM; task_idx++)
    {
        if( (READY == task_ctrl_array[task_idx].taskState) )
        {
            ready_tasksid[indx] = (uint8_t)task_idx;
            printf("\ntasks = %d, is ready\n\r", task_idx);
            indx++;

        }
    }

    /*At least one task is in ready*/
    if(indx >= 1)
    {
        /*for task in ready state, find the one with highest priority*/
        //Consider first element as largest
        ready_tasksprio = task_ctrl_array[ready_tasksid[0]].taskPriority;
        for (task_idx = 1; task_idx < indx; task_idx++)
        {
            if ((task_ctrl_array[ready_tasksid[task_idx]].taskPriority) > ready_tasksprio)
            {
                ready_tasksprio  = task_ctrl_array[ready_tasksid[task_idx]].taskPriority;
                location = task_idx;
            }
        }
        printf("\nMayor priodidad es = %d\n\r", ready_tasksprio);
        printf("Tarea con mayor prioridad = %d\n\r\n", task_ctrl_array[ready_tasksid[location]].taskId);

        /*only one task is ready hence it has highest priority*/
        if(indx == 1)
        {
            /*do not check priority just run task*/
            task_ctrl_array[ready_tasksid[location]].runTask = 1;
        }
        else
        {
            /*check if ready task has mayor priority that active task*/
            if(ready_tasksprio > active_task_priority)
            {
                /*only if ready task has mayor priority run it*/
                task_ctrl_array[ready_tasksid[location]].runTask = 1;
            }
        }

        if( 1 == task_ctrl_array[ready_tasksid[location]].runTask )
        {
            task_ctrl_array[ready_tasksid[location]].runTask = 0;

            if ( NULL != task_ctrl_array[ready_tasksid[location]].tskFcnPtr )
            {
                task_ctrl_array[ready_tasksid[location]].taskState = RUNNING;
                task_ctrl_array[ready_tasksid[location]].tskFcnPtr();
                task_ctrl_array[ready_tasksid[location]].taskState = SUSPENDED;
            }

            if ( gu8Scheduler_Counter != task_ctrl_array[ready_tasksid[location]].tickValue )
            {
                task_ctrl_array[ready_tasksid[location]].taskOverload = 1;
                gu8Scheduler_Status = TASK_SCHEDULER_OVERLOAD;
            }
            else{
                gu8Scheduler_Status = TASK_SCHEDULER_RUNNING;
            }
        }
    }
}
