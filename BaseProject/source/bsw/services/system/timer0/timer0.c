/*
 * timer0.c
 *
 *  Created on: Sep 9, 2017
 */

/*-----------------------------------------------------------------------------
 *         Headers
 *----------------------------------------------------------------------------*/
#include "board.h"
#include "timer0.h"
#include "app_scheduler.h"

/*------------------------------------------------------------------------------
 *         Local Variables
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *         Global Functions
 *----------------------------------------------------------------------------*/

/**
 *  \brief Handler for TC0 interrupt.
 *
 *  Process TC0 Event
 *  Activate external task.
 */
void TC0_Handler(void)
{
    volatile uint32_t dummy;
    /* Clear status bit to acknowledge interrupt */
    dummy = TC0->TC_CHANNEL[ 0 ].TC_SR;
    vfnActivateTask((TaskIdType)TASK_EXTTG);
    NVIC_DisableIRQ(TC0_IRQn);

}

/**
 *  \brief Configures the Timer Counter 0.
 *  TC0 interrupt handler will generates 1s interrupt and activate an external task.
 *  \note IRQ handler must be configured before invoking this function.
 */
void _ConfigureTc(void)
{
    uint32_t div;
    uint32_t tcclks;

    /** Enable peripheral clock. */
    PMC_EnablePeripheral(ID_TC0);

    /** Configure TC for a 1Hz frequency and trigger on RC compare. */
    TC_FindMckDivisor(1, BOARD_MCK/2, &div, &tcclks, BOARD_MCK);

    TC_Configure(TC0, 0, tcclks | TC_CMR_CPCTRG);
    TC0->TC_CHANNEL[0].TC_RC = (BOARD_MCK / div);

    /* Configure and enable interrupt on RC compare */
    NVIC_ClearPendingIRQ(TC0_IRQn);
    NVIC_EnableIRQ(TC0_IRQn);

    NVIC_SetPriority(TC0_IRQn ,2);
    TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;

    TC_Start(TC0, 0);
}
