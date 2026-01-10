/*
 * UART.c
 *
 * Created: 15/07/2025 09:30:27 
 *  Author: AAMA
 */ 

//Includes
#include "Inc/UART.h"


/*
 * ===============================================================
 *
 *                      Generic Variable
 *
 * ===============================================================
 */
USART_config_t* g_USART_Config = NULL ;

void (*GPU_IRQ_CALL[3])(void);

//=======================================
//===============APIs====================
//=======================================

/**================================================================
* @Fn            -MCAL_USART_Init
* @brief         -Initializes USART with user-defined settings (mode, baud rate, frame format...)
* @param [in]    -USART_Config: pointer to config structure with all parameters
* @retval        -none
* Note           -Supports interrupt via callback in config
*/
void MCAL_USART_Init( USART_config_t* USART_Config )
{
	uint16_t  BRR ;
	g_USART_Config = USART_Config ;

	//-------------------- Communication Mode --------------------
	if (USART_Config->Communication_MODE == USART_Communication_MODE_synchronous_Master || USART_Config->Communication_MODE == USART_Communication_MODE_synchronous_Slave)
	{
		UCSRC |= 1<<7;
		UCSRC |= 1<<6;
		if(USART_Config->Clock_Polarity == USART_Clock_Polarity_Trans_Falling)
		{
			UCSRC |= ( 1<<7 ) ;
			UCSRC |= USART_Config->Clock_Polarity ;
		}
		else if(USART_Config->Clock_Polarity == USART_Clock_Polarity_Trans_Rising)
		{
			UCSRC |= ( 1<<7 ) ;
			UCSRC &= USART_Config->Clock_Polarity ;
		}
		BRR= (((float)(F_CPU))/(2.0 * USART_Config->BAUDRATE)) - 0.5 ;
	}
	else if(USART_Config->Communication_MODE == USART_Communication_MODE_Asynchronous_Double_Speed)
	{
		UCSRC |= ( 1<<7 ) ;
		UCSRC &= ~( 1<<6 ) ;
		USART->UCSRA |= 1<<1 ;
		BRR= (((float)(F_CPU))/(8.0 * USART_Config->BAUDRATE)) - 0.5 ;
	}
	else
	{
		UCSRC |= (1<<7) ;
		UCSRC &= ~(1<<6) ;
		USART->UCSRA &= ~(1<<1) ;
		BRR= (((float)(F_CPU))/(16.0 * USART_Config->BAUDRATE)) - 0.5;
	}

	//-------------------- Data Bits --------------------
	switch(USART_Config->NUM_DATA_BIT)
	{
		case USART_NUM_DATA_BIT_5:
		{
			USART->UCSRB &=~(1<<2);
			UCSRC |= (1<<7);
			UCSRC &= ~(3<<1);
			break;
		}
		case USART_NUM_DATA_BIT_6:
		{
			USART->UCSRB &= ~(1<<2);
			UCSRC |= (1<<7);
			UCSRC &= ~(3<<1);
			UCSRC |= (1<<1);
			break;
		}
		case USART_NUM_DATA_BIT_7 :
		{
			USART->UCSRB &= ~(1<<2);
			UCSRC |= (1<<7);
			UCSRC &= ~(3<<1);
			UCSRC |= (2<<1);
			break;
		}
		case USART_NUM_DATA_BIT_8 :
		{
			USART->UCSRB &= ~(1<<2);
			UCSRC |= (1<<7);
			UCSRC |= (0b11<<1);
			break;
		}
		case USART_NUM_DATA_BIT_9 :
		{
			USART->UCSRB |= (1<<2);
			UCSRC |= (1<<7);
			UCSRC |= (0b11<<1);
			break;
		}
		default:
		break;
	}

	//-------------------- Stop Bits --------------------
	switch(USART_Config->NUM_STOP_BIT)
	{
		case USART_NUM_STOP_BIT_1:
		{
			UCSRC |= (1<<7);
			UCSRC &= USART_Config->NUM_STOP_BIT;
			break;
		}
		case USART_NUM_STOP_BIT_2:
		{
			UCSRC |= (1<<7);
			UCSRC |= USART_Config->NUM_STOP_BIT;
			break;
		}
		default:
		break;
	}

	//-------------------- Parity --------------------
	switch(USART_Config->PARITY)
	{
		case USART_PARITY_Disable:
		{
			UCSRC |= (1<<7);
			UCSRC &= USART_Config->PARITY;
			break;
		}
		case USART_PARITY_Even_Parity:
		{
			UCSRC |= (1<<7 );
			UCSRC |= USART_Config->PARITY;
			break;
		}
		case USART_PARITY_Odd_Parity:
		{
			UCSRC |= (1<<7);
			UCSRC |= USART_Config->PARITY;
			break;
		}
		default:
		break;
	}

	//-------------------- Baud Rate --------------------
	UBRRH &= ~(1<<7);
	UBRRH = (BRR>>8);
	USART->UBRRL = (uint8_t)BRR;

	//-------------------- Interrupts --------------------
	switch(USART_Config->IRQ_EN)
	{
		case USART_IRQ_ENABLE_Disable:
		{
			USART_INT_RX_Disable();
			USART_INT_TX_Disable();
			USART_INT_UDR_Disable();
			EXTI_GLOBAL_Disable();
			break;
		}
		case USART_IRQ_ENABLE_TX:
		{
			GPU_IRQ_CALL[0]=USART_Config->P_IRQ_CALL;
			USART_INT_TX_Enable();
			EXTI_GLOBAL_Enable();
			break;
		}
		case USART_IRQ_ENABLE_UDR:
		{
			GPU_IRQ_CALL[1]=USART_Config->P_IRQ_CALL;
			USART_INT_UDR_Enable();
			EXTI_GLOBAL_Enable();
			break;
		}
		case USART_IRQ_ENABLE_RX:
		{
			GPU_IRQ_CALL[2]=USART_Config->P_IRQ_CALL ;
			USART_INT_RX_Enable();
			EXTI_GLOBAL_Enable();
			break;
		}
		default:
		break;
	}

	//-------------------- Mode (TX/RX) --------------------
	USART->UCSRB |= USART_Config->MODE ;

	//-------------------- GPIO for XCK (Sync Only) --------------------
	GPIO_Pinconfig_t pinconfig ;
	if( g_USART_Config->Communication_MODE == USART_Communication_MODE_synchronous_Master )
	{
		//master
		pinconfig.pinNumber = GPIO_PIN_0 ;
		pinconfig.GPIO_MODE = GPIO_MODE_OUTPUT ;
		MCAL_GPIO_Init(GPIOB , &pinconfig ) ;
	}
	else if( g_USART_Config->Communication_MODE == USART_Communication_MODE_synchronous_Slave )
	{
		//Slave
		pinconfig.pinNumber = GPIO_PIN_0 ;
		pinconfig.GPIO_MODE = GPIO_MODE_INPUT_PU ;
		MCAL_GPIO_Init(GPIOB , &pinconfig ) ;
	}
}

/**================================================================
* @Fn            -MCAL_USART_DeInit
* @brief         -Deinitializes the USART peripheral by disabling its clock,
*                 clearing related registers, and disabling global interrupts.
* @retval        -None
* @note          -This will fully reset the USART configuration.
*/
void MCAL_USART_DeInit(void)
{
	USART_INT_RX_Disable() ;
	USART_INT_TX_Disable() ;
	USART_INT_UDR_Disable();
	EXTI_GLOBAL_Disable()  ;
	USART->UBRRL = 0 ;
	USART->UCSRB = 0 ;
	UCSRC = 0b10000110;
	UBRRH = 0;
	USART->UCSRA &=~(1<<1)	;
}

/**================================================================
* @Fn            -MCAL_USART_ReceiveData
* @brief         -Receives data via USART. Supports 5 to 9-bit data frames.
*                 Can operate using polling or interrupt-based reception.
* @param [out]   -PRXBuffer: Pointer to variable where the received data will be stored
* @param [in]    -polling_EN: Enable or Disable polling (Enable = wait until data is received)
* @retval        -None
* @note          -Make sure USART is properly configured before calling this function
*/
void MCAL_USART_ReceiveData(uint16_t* PRXBuffer, enum polling_mechanism polling_EN)
{
	if (polling_EN == Enable)
	{	
		while(!(USART->UCSRA & 1<<7)) ;
		if (g_USART_Config->NUM_DATA_BIT == USART_NUM_DATA_BIT_9)
		{
			*(PRXBuffer) = ((( USART->UCSRB >> 1) & 0X01 ) << 8 ) ;
		}
		
		*((uint16_t*)PRXBuffer) = (USART->UDR & (uint8_t)0xff);
	}
	// non block mode
	else
	{
		if((USART->UCSRA & 1<<7))
		{
			if (g_USART_Config->NUM_DATA_BIT == USART_NUM_DATA_BIT_9)
			{
				*(PRXBuffer) = ((( USART->UCSRB >> 1) & 0X01 ) << 8 ) ;
			}
			
			*((uint16_t*)PRXBuffer) = ( USART->UDR & (uint8_t)0xff);
		}
	}
}

/**================================================================
* @Fn            -MCAL_USART_SendData
* @brief         -Transmits data using USART (5 to 9 bits), with optional polling
* @param [in]    -PTXBuffer: Pointer to the data to send
* @param [in]    -polling_EN: Select polling or non-polling mode
* @retval        -None
* @note          -Call only after USART is properly configured
*/
void MCAL_USART_SendData(uint16_t* PTXBuffer, enum polling_mechanism polling_EN)
{
	if (polling_EN == Enable)
	{
		while(!(USART->UCSRA & 1<<5));
		if (g_USART_Config->NUM_DATA_BIT == USART_NUM_DATA_BIT_9)
		{
			USART->UCSRB &= ~(1<<0);
			if(*PTXBuffer & 0X0100)
			{
				USART->UCSRB |= 1<<0;
			}
		}
		USART->UDR = (*PTXBuffer & (uint8_t)0xFF);
	}
	// non block mode
	else
	{
		if ((USART->UCSRA & 1<<5))
		{
			if (g_USART_Config->NUM_DATA_BIT == USART_NUM_DATA_BIT_9)
			{
				USART->UCSRB &= ~(1<<0);
				if(*PTXBuffer & 0X0100)
				{
					USART->UCSRB |=1<<0;
				}
			}
			USART->UDR = (*PTXBuffer & (uint8_t)0xFF);
		}
	}	
}

/**================================================================
* @Fn            -MCAL_USART_Send_String
* @brief         -Sends a string over USART using polling or interrupts
* @param [in]    -PTXBuffer: Pointer to the data buffer
* @param [in]    -polling_EN: Use polling or not
* @retval        -None
* @note          -String must end with '#' to complete the transmission
*/
void MCAL_USART_Send_String(uint8_t* PTXBuffer, enum polling_mechanism polling_EN)
{
	if (g_USART_Config->NUM_DATA_BIT == USART_NUM_DATA_BIT_8)
	{
		while(*PTXBuffer != '#')
		{
			MCAL_USART_SendData((uint16_t*)PTXBuffer, polling_EN);
			PTXBuffer++;
		}
	}	
}

/**================================================================
* @Fn            -MCAL_USART_Receive_String
* @brief         -Receives a string from USART using polling or interrupt mode
* @param [out]   -PRXBuffer: Pointer to buffer to store received data
* @param [in]    -polling_EN: Use polling or not
* @retval        -None
* @note          -Reception ends when '#' is received
*/
void MCAL_USART_Receive_String(uint8_t* PRXBuffer, enum polling_mechanism polling_EN)
{
	if (g_USART_Config->NUM_DATA_BIT == USART_NUM_DATA_BIT_8)
	{
		while(*(PRXBuffer-1) != '#')
		{
			MCAL_USART_ReceiveData(PRXBuffer, polling_EN);
			PRXBuffer++;
		}
	}	
}

//=========================================================================================================

/*
 * ===============================================================
 *
 *                      ISR Function Definitions
 *
 * ===============================================================
 */
void __vector_13 (void) __attribute__((signal));
void __vector_13 (void)
{
	//Call function
	GPU_IRQ_CALL[2]();
}

void __vector_14 (void) __attribute__((signal));
void __vector_14 (void)
{
	//Call function
	GPU_IRQ_CALL[1]();
}

void __vector_15 (void) __attribute__((signal));
void __vector_15 (void)
{
	//Call function
	GPU_IRQ_CALL[0]();
}