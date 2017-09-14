/****************************************************************************************************/
/**
\file       led_ctrl.h
\brief      MCU abstraction level - LED control
\author     Abraham Tezmol
\version    1.0
\project    Tau 
\date       24/June/2016
*/
/****************************************************************************************************/

#ifndef __SERIAL_CTRL_H        /*prevent duplicated includes*/
#define __SERIAL_CTRL_H

/*****************************************************************************************************
* Include files
*****************************************************************************************************/
//typedef  char      ( * uart_char_callback)(void);
/** Core modules */
#include "compiler.h"
//#include "typedefs.h"

/** Used modules */
/*#include "component_uart.h"  */


/*****************************************************************************************************
* Declaration of module wide TYPEs 
*****************************************************************************************************/


/*****************************************************************************************************
* Definition of module wide MACROs / #DEFINE-CONSTANTs 
*****************************************************************************************************/
#define BASE_UART       	UART4
#define BASE_ID         	ID_UART4
#define BASE_IRQ        	UART4_IRQn
/** Pins description corresponding to Rxd,Txd, (UART pins) */
#define UART_PINS        	{PINS_UART4}

#define SERIAL_TX_MAX_SIZE  64
#define SERIAL_RX_MAX_SIZE	64

typedef  uint8_t      ( * uart_char_callback)(void);

/*****************************************************************************************************
* Declaration of module wide FUNCTIONS
*****************************************************************************************************/

/** Configures Serial channel BASE_UART on SAMV71 board */


void vfnSerialCtrl_Init( void );

void vfnSerialCtrl_Transfer(void);

uint8_t vfnSerialCtrl_Transfer_callback(void);





/**************************************************************************************************/

#endif /* __SERIAL_CTRL_H */