/*******************************************************************************/
/**
\file       app_scheduler.h
\brief      Task scheduler function prototypes
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

#ifndef APP_SCHEDULER_H        /*prevent duplicated includes*/
#define APP_SCHEDULER_H

/*-- Includes ----------------------------------------------------------------*/
#include <stdint.h>
/*-- Types Definitions -------------------------------------------------------*/
typedef void (*FuncPtr)(void);
typedef uint8_t TaskIdType;

typedef struct  
{
  uint8_t taskPriority;
  TaskIdType taskId;
  FuncPtr tskFcnPtr;
}TaskType;

/*-- Defines -----------------------------------------------------------------*/


/* Global Task Scheduler Status definitions */

#define  TASK_1MS     0x00u
#define  TASK_2MSA    0x01u
#define  TASK_2MSB    0x02u
#define  TASK_10MS    0x03u
#define  TASK_50MS    0x04u
#define  TASK_100MS   0x05u
#define  TASK_MAXNUM  0x06u


#define TASK_SCHEDULER_INIT             0x00u   
#define TASK_SCHEDULER_RUNNING          0x01u
#define TASK_SCHEDULER_OVERLOAD_1MS     0x02u
#define TASK_SCHEDULER_OVERLOAD_2MS_A   0x03u
#define TASK_SCHEDULER_OVERLOAD_2MS_B   0x04u
#define TASK_SCHEDULER_HALTED           0xAAu

#define TASK_SCHEDULER_OVERLOAD			(0x55u)

/*-- Macros ------------------------------------------------------------------*/


/*-- Function Prototypes -----------------------------------------------------*/

/** Sheduler Initalization (arming) */
void vfnScheduler_Init(TaskType *TaskArray);

/** Scheduler kick-off function */
void vfnScheduler_Start(void);

/** Scheduler stop function */
void vfnScheduler_Stop(void);

/** Multi-thread round robin task scheduler */
void vfnTask_Scheduler(void);

/*******************************************************************************/

#endif /* APP_SCHEDULER_H */