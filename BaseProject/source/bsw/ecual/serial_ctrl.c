/****************************************************************************************************/
/**
\file       serial_ctrl.c
\brief      MCU abstraction level - Serial UART control
\author     Abraham Tezmol
\version    1.0
\project    Tau 
\date       21/June/2016
*/
/****************************************************************************************************/

/*****************************************************************************************************
* Include files
*****************************************************************************************************/
/** Own headers */
#include     "serial_ctrl.h"

/** Other modules */
#include     "compiler.h"
#include     "board.h"
#include     "pmc.h"
#include     "pio.h"
#include     "uart.h"

/****************************************************************************************************
* Definition of module wide MACROs / #DEFINE-CONSTANTs 
*****************************************************************************************************/


/*****************************************************************************************************
* Definition of module wide VARIABLEs 
*****************************************************************************************************/

uint8_t 		u8SerialCtrl_TxData[] =
{"The Atmel® | SMART™ SAM V71 Xplained Ultra evaluation kit is ideal for evaluating and prototyping with the Atmel SAM V71, SAM V70, SAM S70 and SAM E70 ARM® Cortex®-M7 based microcontrollers\n\r\n\rExample by Abraham Tezmol\n\r\n\r"};
uint8_t 		u8SerialCtrl_RxData[SERIAL_RX_MAX_SIZE];
uint8_t  * 		pu8SerialCtrl_RxDataPtr;
uint8_t  * 		pu8SerialCtrl_ReadRxDataPtr;
uint8_t  *		pu8SerialCtrl_ReadTxDataPtr;
uint16_t  		u16SerialCtrl_RxLength;
uint16_t 		u16SerialCtrl_TxLength;

/****************************************************************************************************
* Declaration of module wide FUNCTIONs 
****************************************************************************************************/

/****************************************************************************************************
* Declaration of module wide TYPEs 
*****************************************************************************************************/

/****************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs 
*****************************************************************************************************/

/****************************************************************************************************
* Code of module wide FUNCTIONS
*****************************************************************************************************/


/*****************************************************************************************************/
/**
* \brief    Configures Serial channel BASE_UART on SAMV71 board
* \author   Abraham Tezmol
* \param    void
* \return   void
*/


uart_char_callback callback[] = {vfnSerialCtrl_Transfer_callback};

void vfnSerialCtrl_Configure( void )
{
	PMC_EnablePeripheral(BASE_ID);
  
  const Pin my_pins[] = UART_PINS;
  PIO_Configure( my_pins, PIO_LISTSIZE( my_pins ) );
	
	/* Reset & disable receiver and transmitter, disable interrupts */
	UART_Configure(BASE_UART, (UART_MR_CHMODE_NORMAL | UART_MR_PAR_NO | UART_MR_BRSRCCK_PERIPH_CLK), 115200, BOARD_MCK );

	/* Clear pending IRQs and Set priority of IRQs */
	NVIC_ClearPendingIRQ(BASE_IRQ);
	NVIC_SetPriority(BASE_IRQ, 1);

	/* Enables the UART to transfer and receive data. */
	UART_SetTransmitterEnabled (BASE_UART , 1);
	UART_SetReceiverEnabled (BASE_UART , 1);

	/*UART_EnableIt(BASE_UART, UART_IER_RXRDY);*/
	/* Enable interrupt  */
	NVIC_EnableIRQ(BASE_IRQ);	
}


/******************************************************************************************************************/
/**
* \brief    Initialize UART communication on Serial channel BASE_UART of SAMV71 board
* \author   Abraham Tezmol
* \param    void
* \return   void
*/
void vfnSerialCtrl_Init( void )
{
    vfnSerialCtrl_Configure();
}

/******************************************************************************************************************/
/**
* \brief    Start serial communication through Serial channel BASE_UART on SAMV71 board
* \author   Abraham Tezmol
* \param    void
* \return   void
*/
void vfnSerialCtrl_StartTx( void )
{
    pu8SerialCtrl_ReadTxDataPtr = &u8SerialCtrl_TxData[0];
    u16SerialCtrl_TxLength = sizeof(u8SerialCtrl_TxData);
}


/******************************************************************************************************************/
/**
* \brief    Start serial communication - transfer one byte at a time
* \author   Abraham Tezmol
* \param    void
* \return   void
*/
void vfnSerialCtrl_Transfer(void)
{		
	if (u16SerialCtrl_TxLength > 0)
	{
		/* Send out one byte at a time */
		UART_PutChar(BASE_UART, *pu8SerialCtrl_ReadTxDataPtr);
    //printf("%c", (char)BASE_UART->UART_RHR);   //Aqui si funciona tal y como esta
		/* point to next element */
		pu8SerialCtrl_ReadTxDataPtr++;
		/* update number of pending bytes to transfer */
		u16SerialCtrl_TxLength--;
	}
	else
	{
		/* Start Seriacl Control Data to transmit */
		vfnSerialCtrl_StartTx();
    
	}
}

uint8_t vfnSerialCtrl_Transfer_callback(void)
{		
	if (u16SerialCtrl_TxLength > 0)
	{
		/* Send out one byte at a time */
		//UART_PutChar(BASE_UART, *pu8SerialCtrl_ReadTxDataPtr);
    //printf("%c", (char)BASE_UART->UART_RHR);   //Aqui si funciona tal y como esta
		/* point to next element */
		//++pu8SerialCtrl_ReadTxDataPtr;
		/* update number of pending bytes to transfer */
		u16SerialCtrl_TxLength--;
    
    return *(pu8SerialCtrl_ReadTxDataPtr++);
	}
	else
	{
		/* Start Seriacl Control Data to transmit */    		
    UART_DisableIt(UART4,UART_IER_TXRDY);
    vfnSerialCtrl_StartTx();    
    
	}
}


