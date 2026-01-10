/*
 * IncFile1.h
 *
 * Created: 15/07/2025 09:29:44 
 *  Author: AAMA
 */ 


#ifndef UART_H_
#define UART_H_

//includes
#include "Atmega32.h"
#include "GPIO.h"

/*******************************************************************************
                           User Configuration structures
*******************************************************************************/
typedef struct
{
	uint8_t MODE 						; 	//specified the USART (TX/RX) to be configured.
	//this parameter must be set based on  @ref USART_MODE_Define
	
	uint8_t Clock_Polarity 				; 	//specified the USART Transmitted Data Changed (Transmitted as Rising XCK Edge and Received Falling XCK Edge ) or (Transmitted as Falling XCK Edge and Received Rising XCK Edge ).
	//this parameter must be set based on  @ref USART_Clock_Polarity_Define
	
	uint8_t Communication_MODE 			; 	//specified the USART as Asynchronous Normal Mode or Asynchronous Double Speed Mode or Synchronous Master Mode or Slave Synchronous mode.
	//this parameter must be set based on  @ref USART_Communication_MODE_Define

	uint8_t NUM_DATA_BIT				;	//specified the number of data bit (5 --> 9 ).
	//this parameter must be set based on  @ref USART_NUM_DATA_BIT_Define

	uint8_t NUM_STOP_BIT				;	//specified the number of stop bit ( 0.5 or 1 or 1.5 or 2 ).
	//this parameter must be set based on  @ref USART_NUM_STOP_BIT_Define

	uint16_t BAUDRATE					;	//specified the baudrate  .
	//this parameter must be set based on  @ ref  USART_BAUDRATE_Define

	uint8_t PARITY						;	//specified the parity disable or enable (odd or even ).
	//this parameter must be set based on  @ ref  USART_PARITY_Define

	uint8_t IRQ_EN						;	//specified the interrupt (disable or enable ).
	//this parameter must be set based on  @ref USART_IRQ_ENABLE_Define

	void (*P_IRQ_CALL)(void)			;	// set the function which will be call at IRQ happen
}USART_config_t;


enum polling_mechanism
{
	Enable ,
	Disable
};

/******************************************************************************
                           User Define References Macros
*******************************************************************************/
//@ref UART_Mode_define
#define USART_MODE_TX			(uint8_t)(1<<3)
#define USART_MODE_RX			(uint8_t)(1<<4)
#define USART_MODE_TX_RX		(uint8_t)(1<<4|1<<3)

//@ref USART_Clock_Polarity_Define
#define USART_Clock_Polarity_Trans_Rising			(uint8_t)(~(1<<0))
#define USART_Clock_Polarity_Trans_Falling			(uint8_t)(1<<0)

//@ref  USART_Communication_MODE_Define
#define USART_Communication_MODE_Asynchronous_Double_Speed		0
#define USART_Communication_MODE_Asynchronous_Normal			1
#define USART_Communication_MODE_synchronous_Master				2
#define USART_Communication_MODE_synchronous_Slave				3

//@ref UART_BaudRate_define
#define USART_BAUDRATE_2400							2400
#define USART_BAUDRATE_4800							4800
#define USART_BAUDRATE_9600							9600
#define USART_BAUDRATE_14400						14400
#define USART_BAUDRATE_19200						19200
#define USART_BAUDRATE_28800						28800
#define USART_BAUDRATE_38400						38400
#define USART_BAUDRATE_57600						57600
#define USART_BAUDRATE_76800						76800
#define USART_BAUDRATE_115200						115200
#define USART_BAUDRATE_230400						230400
#define USART_BAUDRATE_250000						250000
#define USART_BAUDRATE_500000						500000
#define USART_BAUDRATE_1000000						1000000

//@ref USART_NUM_DATA_BIT_Define
#define USART_NUM_DATA_BIT_5						1
#define USART_NUM_DATA_BIT_6						2
#define USART_NUM_DATA_BIT_7						3
#define USART_NUM_DATA_BIT_8						4
#define USART_NUM_DATA_BIT_9						5

//@ref USART_NUM_STOP_BIT_Define
#define USART_NUM_STOP_BIT_1						(uint8_t)(~(1<<3))
#define USART_NUM_STOP_BIT_2						(uint8_t)(1<<3)

//@ref UART_Parity_define
#define USART_PARITY_Disable						(uint8_t)(~(3<<4))
#define USART_PARITY_Even_Parity					(uint8_t)(2<<4)
#define USART_PARITY_Odd_Parity						(uint8_t)(3<<4)

//@ref UART_IRQ_define
#define USART_IRQ_ENABLE_Disable					0
#define USART_IRQ_ENABLE_TX							1	//Transmission complete
#define USART_IRQ_ENABLE_UDR						2   //Transmit data register empty
#define USART_IRQ_ENABLE_RX						    3   //Received complete

//=======================================
//===============APIs====================
//=======================================
void MCAL_USART_Init(USART_config_t* USART_Config);
void MCAL_USART_DeInit(void);
void MCAL_USART_ReceiveData(uint16_t* PRXBuffer, enum polling_mechanism polling_EN);
void MCAL_USART_SendData(uint16_t* PTXBuffer, enum polling_mechanism polling_EN);
void MCAL_USART_Send_String(uint8_t* PTXBuffer, enum polling_mechanism polling_EN);
void MCAL_USART_Receive_String(uint8_t* PRXBuffer, enum polling_mechanism polling_EN);

#endif