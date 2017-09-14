/*******************************************************************************/
/**
\file       timer0.h
\author     Juan Campeche
\version    0.1
\date       11/09/2017
*/
#ifndef _TIMER0_
#define _TIMER0_

/*-- Includes ----------------------------------------------------------------*/
#include "board.h"
#include <stdint.h>
#include "app_scheduler.h"


/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

extern void Timer0_Init();

extern void Timer0_Start(void (*func_ptr)(void));

#endif 