/*******************************************************************************/
/**
\file       timer0.c
\author     Juan Campeche
\version    0.1
\date       11/09/2017
*/

/*-- Includes ----------------------------------------------------------------*/
#include "timer0.h"


void (*ISR_func_ptr)(void);

void Timer0_Init(void)
{
    uint32_t div;
    uint32_t tcclks;
    
    

    /** Enable peripheral clock. */
    PMC_EnablePeripheral(ID_TC0);

    /** Configure TC for a 1Hz frequency and trigger on RC compare. */
    TC_FindMckDivisor(500, BOARD_MCK/2, &div, &tcclks, BOARD_MCK);

    TC_Configure(TC0, 0, tcclks | TC_CMR_CPCTRG);
    TC0->TC_CHANNEL[0].TC_RC = (BOARD_MCK / div);

    /* Configure and enable interrupt on RC compare */
    NVIC_ClearPendingIRQ(TC0_IRQn);
    NVIC_EnableIRQ(TC0_IRQn);

    NVIC_SetPriority(TC0_IRQn ,2);
    TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;

    Timer0_Start(&Timer0_ISR_Callback);
    

}

void Timer0_Start(void (*func_ptr)(void))
{
  ISR_func_ptr = func_ptr;
  TC_Start(TC0, 0);
}




void TC0_Handler(void)
{
    volatile uint32_t dummy;
    /* Clear status bit to acknowledge interrupt */
    dummy = TC0->TC_CHANNEL[ 0 ].TC_SR;
    //NVIC_DisableIRQ(TC0_IRQn);    
  //LED_Set(0);    
  //uint8_t x;	
  //for(x=0;x<80;x++){ 
  //LED_Toggle( 0 );  
  //}       
  //  LED_Clear(0);
    LED_Set(0);
    ISR_func_ptr();
}







