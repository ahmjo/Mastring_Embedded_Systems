/*
 * SPI.c
 *
 * Created: 05/08/2025 07:46:56 م
 *  Author: AAMA
 */

//Includes
#include "Inc/SPI.h"

 /*
 * ===============================================================
 *                      Generic Variable
 * ===============================================================
 */
SPI_config_t* g_SPI_Config =NULL;

//=================================================================================
/*
 * ===============================================================
 *
 *                      API Function Definitions
 *
 * ===============================================================
 */
/**================================================================
* @Fn            -MCAL_SPI_Init
* @brief         -Configures and initializes the SPI peripheral
* @param [in]    -SPI_Config: Pointer to SPI config struct with mode, data order, prescaler, IRQ, etc.
* @retval        -None
* @note          -Must be called before using SPI functions
*/
void MCAL_SPI_Init(SPI_config_t* SPI_Config)
{
	g_SPI_Config = SPI_Config;
	uint8_t Temp_SPCR=0 , Temp_SPSR=0;
	//set BaudRate_Prescalers
	switch(SPI_Config->BaudRate_Prescalers)
	{
		case SPI_BaudRate_Prescalers_4:
		Temp_SPCR &=SPI_Config->BaudRate_Prescalers;
		break ;
		case SPI_BaudRate_Prescalers_16:
		case SPI_BaudRate_Prescalers_64:
		case SPI_BaudRate_Prescalers_128:
		Temp_SPCR |= SPI_Config->BaudRate_Prescalers;
		break ;
	}
	
	//Set Mode
	switch(SPI_Config->MODE)
	{
		case SPI_MODE_Master:
		Temp_SPCR |=SPI_Config->MODE;
		break ;
		case SPI_MODE_Slave:
		Temp_SPCR &=SPI_Config->MODE;
		break ;
	}
	
	// set speed Communication
	switch( SPI_Config->Speed_Communication )
	{
		case SPI_Speed_Communication_Normal :
		Temp_SPSR &=SPI_Config->Speed_Communication ;
		break ;
		case SPI_Speed_Communication_Double_Speed :
		Temp_SPSR |=SPI_Config->Speed_Communication ;
		break ;
	}
	
	//set Polarity
	switch(SPI_Config->Polarity)
	{
		case SPI_Polarity_High_level_idle_state:
		Temp_SPCR |=SPI_Config->Polarity;
		MCAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
		break;
		case SPI_Polarity_Low_level_idle_state:
		Temp_SPCR &=SPI_Config->Polarity;
		break;
	}
	
	// set Phase
	switch(SPI_Config->Phase)
	{
		case SPI_Phase_Data_Latched_On_First_Clock:
		Temp_SPCR &=SPI_Config->Phase;
		break;
		case SPI_Phase_Data_Latched_On_Second_Clock:
		Temp_SPCR |= SPI_Config->Phase;
		break;
	}
	
	//set Data_Order
	switch(SPI_Config->Data_Order)
	{
		case SPI_Data_Order_LSB_Transmitted_First:
		Temp_SPCR |=SPI_Config->Data_Order;
		break;
		case SPI_Data_Order_MSB_Transmitted_First:
		Temp_SPCR &=SPI_Config->Data_Order;
		break;
	}
	
	// set IRQ_EN
	switch(SPI_Config->IRQ_EN)
	{
		case SPI_IRQ_ENABLE_Disable:
		Temp_SPCR &=SPI_Config->IRQ_EN;
		EXTI_GLOBAL_Disable();
		break;
		case SPI_IRQ_ENABLE_Enable:
		Temp_SPCR |=SPI_Config->IRQ_EN;
		EXTI_GLOBAL_Enable();
		break;
	}
	
	SPI->SPSR = Temp_SPSR;
	SPI->SPCR = Temp_SPCR;
	SPI->SPCR |= SPI_Enable();
}

/**================================================================
* @Fn            -MCAL_SPI_DeInit
* @brief         -Disables SPI clock and interrupts, resetting it to default state
* @retval        -None
* @note          -Call this to turn off SPI completely
*/
void MCAL_SPI_DeInit(void)
{
	EXTI_GLOBAL_Disable();
	SPI->SPCR = SPI_Disable();
}

/**================================================================
* @Fn            -MCAL_SPI_ReceiveData
* @brief         -Receives 8-bit data over SPI using polling or interrupt
* @param [in]    -polling_EN: Use polling or not
* @param [out]   -PRXBuffer: Pointer to store received data
* @retval        -None
* @note          -Call after initializing SPI
*/
void MCAL_SPI_ReceiveData(uint8_t* PRXBuffer, enum SPI_polling_mechanism polling_EN)
{
	if( polling_EN == Enabled)
	{
		while(!(SPI->SPSR & (1<<7)));
	}
	*PRXBuffer = SPI->SPDR;
}

/**================================================================
* @Fn            -MCAL_SPI_SendData
* @brief         -Sends 8-bit data over SPI using polling or interrupt
* @param [in]    -PTXBuffer: Pointer to data buffer
* @param [in]    -polling_EN: Use polling or not
* @retval        -None
* @note          -SPI must be initialized first
*/
void MCAL_SPI_SendData(uint8_t* PTXBuffer, enum SPI_polling_mechanism polling_EN)
{
	SPI->SPDR = *PTXBuffer;
	if( polling_EN == Enabled)
	{
		while(!(SPI->SPSR & (1<<7)));
	}
	
}

/**================================================================
* @Fn            -MCAL_SPI_Receive_Send_Data
* @brief         -Sends and receives 8-bit data over SPI (full duplex) using polling or interrupt
* @param [in,out] -PTXBuffer: Pointer to buffer for both sending and receiving data
* @param [in]    -polling_EN: Use polling or not
* @retval        -None
* @note          -SPI must be initialized before using this function
*/
void MCAL_SPI_Receive_Send_Data(uint8_t* PTXBuffer, enum SPI_polling_mechanism polling_EN)
{
	if(polling_EN == Enabled)
	{
		while(!(SPI->SPSR & (1<<7)));
	}
	SPI->SPDR = *PTXBuffer ;
	
	if( polling_EN == Enabled)
	{
		while(!(SPI->SPSR & (1<<7)));
	}
	*PTXBuffer = SPI->SPDR ;
}

/**================================================================
* @Fn            -MCAL_SPI_Set_Master_Pin
* @brief         -Sets the SPI master pins (MOSI, SCK, SS) as output
* @retval        -None
* @note          -Use this before enabling SPI in master mode
*/
void MCAL_SPI_Set_Master_Pin(void)
{
	//PB4---> SS
	//PB5--->MOSI
	//PB6--->MISO
	//PB7--->SCK
	GPIO_Pinconfig_t Pin_SPI_Conf;

	//PB4---> SS
	Pin_SPI_Conf.GPIO_MODE = GPIO_MODE_OUTPUT;
	Pin_SPI_Conf.pinNumber = GPIO_PIN_4;
	MCAL_GPIO_Init(GPIOB,&Pin_SPI_Conf);
	//PB5--->MOSI
	Pin_SPI_Conf.GPIO_MODE = GPIO_MODE_OUTPUT;
	Pin_SPI_Conf.pinNumber = GPIO_PIN_5;
	MCAL_GPIO_Init(GPIOB,&Pin_SPI_Conf);
	//PB6--->MISO
	Pin_SPI_Conf.GPIO_MODE = GPIO_MODE_INPUT_FLO;
	Pin_SPI_Conf.pinNumber = GPIO_PIN_6;
	MCAL_GPIO_Init(GPIOB,&Pin_SPI_Conf);
	//PB7--->SCK
	Pin_SPI_Conf.GPIO_MODE = GPIO_MODE_OUTPUT;
	Pin_SPI_Conf.pinNumber = GPIO_PIN_7;
	MCAL_GPIO_Init(GPIOB,&Pin_SPI_Conf);
}

/**================================================================
* @Fn            -MCAL_SPI_Set_Slave_Pin
* @brief         -Sets SPI pins for slave mode (MISO output, others input)
* @retval        -None
* @note          -Call before enabling SPI as slave
*/
void MCAL_SPI_Set_Slave_Pin(void)
{
	GPIO_Pinconfig_t Pin_SPI_Conf ;
	
	//PB4---> SS
	Pin_SPI_Conf.GPIO_MODE = GPIO_MODE_INPUT_FLO;
	Pin_SPI_Conf.pinNumber = GPIO_PIN_4;
	MCAL_GPIO_Init(GPIOB,&Pin_SPI_Conf);
	//PB5--->MOSI
	Pin_SPI_Conf.GPIO_MODE = GPIO_MODE_INPUT_FLO;
	Pin_SPI_Conf.pinNumber = GPIO_PIN_5;
	MCAL_GPIO_Init(GPIOB,&Pin_SPI_Conf);
	//PB6--->MISO
	Pin_SPI_Conf.GPIO_MODE = GPIO_MODE_OUTPUT;
	Pin_SPI_Conf.pinNumber = GPIO_PIN_6;
	MCAL_GPIO_Init(GPIOB,&Pin_SPI_Conf);
	//PB7--->SCK
	Pin_SPI_Conf.GPIO_MODE = GPIO_MODE_INPUT_FLO;
	Pin_SPI_Conf.pinNumber = GPIO_PIN_7;
	MCAL_GPIO_Init(GPIOB,&Pin_SPI_Conf);
}

//=================================================================================

/*
 * ===============================================================
 *                      ISR Function Definitions
 * ===============================================================
 */

void __vector_12 (void) __attribute__((signal));
void __vector_12 (void)
{
	g_SPI_Config->P_IRQ_CALL() ;
}