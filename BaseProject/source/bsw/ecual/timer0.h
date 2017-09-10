/*
 * timer0.h
 *
 *  Created on: Sep 9, 2017
 */

#ifndef TIMER0_H        /*prevent duplicated includes*/
#define TIMER0_H


/*------------------------------------------------------------------------------
 *         Exported Variables
 *----------------------------------------------------------------------------*/
extern uint8_t timer0_interrupt;

/*------------------------------------------------------------------------------
 *         Global Functions
 *----------------------------------------------------------------------------*/

/**
 *  \brief Handler for TC0 interrupt.
 */
void TC0_Handler(void);

/**
 *  \brief Configures the Timer Counter 0.
 */
extern void _ConfigureTc(void);


#endif /* TIMER0_H */
