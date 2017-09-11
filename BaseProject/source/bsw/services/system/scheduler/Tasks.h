/*******************************************************************************/
/**
\file       Tasks.h
\author     Francisco Matinez
\version    0.2
\date       06/21/2016
*/

#ifndef __APP_TASKS
#define __APP_TASKS

/*-- Includes ----------------------------------------------------------------*/

/*-- Variables -------------------------------------------------------*/
/*------------------------------------------------------------------------------
 *         Exported Variables
 *----------------------------------------------------------------------------*/
extern uint8_t ExtTsk_Activated;


/*-- Defines -----------------------------------------------------------------*/


/*-- Macros ------------------------------------------------------------------*/


extern void vfnTsk_1ms(void);
extern void vfnTsk_2msA(void);
extern void vfnTsk_2msB(void);
extern void vfnTsk_10ms(void);
extern void vfnTsk_50ms(void);
extern void vfnTsk_100ms(void);

/**
 *  \brief  External Triggered Task is executed if there is a higher priority
 *          task than the task currently in execution
 *
 */
extern void vfnTsk_ExtTriggered(void);

/*============================================================================*/
#endif /*__APP_TASKS */
