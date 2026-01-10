/*
 * I2C.c
 *
 * Created: 05/08/2025 07:47:10 م
 *  Author: AAMA
 */ 


//Includes
#include "Inc/I2C.h"

/*
 * ===============================================================
 *                      Generic Variable
 * ===============================================================
 */

I2C_config_t* g_I2C_Config = NULL;

//===============================================================================
/*
 * ===============================================================
 *                   API Function Definitions
 * ===============================================================
 */

/**================================================================
* @Fn            -MCAL_I2C_Init
* @brief         -Configures and initializes the I2C peripheral
* @param [in]    -I2C_Config: Pointer to structure with speed, address, IRQ, and callback
* @retval        -None
* @note          -Call before using I2C communication functions
*/
void MCAL_I2C_Init(I2C_config_t* I2C_Config)
{
	g_I2C_Config = I2C_Config;
	I2C->TWAR = I2C_Config->Own_Address;
	I2C->TWSR = I2C_Config->Prescaler;
	if(I2C_Config->IRQ_EN == I2C_IRQ_ENABLE_Enable)
	{
		TWCR |=I2C_Config->IRQ_EN;
		EXTI_GLOBAL_Enable();
	}
	else
	{
		TWCR &=I2C_Config->IRQ_EN;
		EXTI_GLOBAL_Disable();
	}
	I2C->TWBR =(uint8_t)(((F_CPU/I2C_Config->Speed_MODE)-16)/(2*1));
	//enable I2C and ACK
	TWCR |=(1<<2 ) | (1<<6);
}

/**================================================================
* @Fn            -MCAL_I2C_Master_TX
* @brief         -Sends one 8-bit data byte from master to slave over I2C
* @param [in]    -Address: Slave device address
* @param [in]    -PTXBuffer: Data byte to send
* @retval        -None
* @note          -Includes I2C start and stop conditions internally
*/
void MCAL_I2C_Master_TX(uint8_t Address , uint8_t PTXBuffer)
{
	//Send start bit
	I2C_Generate_Start();
	//Send address slave
	I2C->TWDR = (Address) ;
	TWCR |=(1<<7) ;
	while(!(Get_flag_stutus(TWINT))); //Wait until receive ACK from Slave
	//Send data
	I2C->TWDR =PTXBuffer;
	TWCR |=(1<<7) ;
	while(!(Get_flag_stutus(TWINT))); //Wait until receive ACK from Slave
	//Send stop bit
	I2C_Generate_Stop();
}

/**================================================================
* @Fn            -MCAL_I2C_Master_RX
* @brief         -Receives one 8-bit data byte from slave over I2C
* @param [in]    -Address: Address of the slave device
* @retval        -The received data byte
* @note          -START and STOP conditions are handled inside the function
*/
uint8_t MCAL_I2C_Master_RX(uint8_t Address)
{
	uint8_t data ;
	//Send start bit
	I2C_Generate_Start();
	//Send address slave
	I2C->TWDR = (Address) ;
	TWCR |=(1<<7) ;
	while(!(Get_flag_stutus(TWINT)));  //Wait until receive ACK from Slave
	// receive data from slave
	TWCR |=(1<<7) ;
	while(!(Get_flag_stutus(TWINT))); // Wait until receive ACK from Slave ( complete receive data )
	// return data and generate stop
	data= I2C->TWDR ;
	I2C_Generate_Stop();
	return data ;
}

/**================================================================
* @Fn            -I2C_Generate_Start
* @brief         -Creates a START condition to begin I2C transmission
* @retval        -None
* @note          -Used in master mode before addressing the slave
*/
void I2C_Generate_Start(void)
{
	TWCR =(1<<7) | (1<<5) | (1<<2);
	while(!(Get_flag_stutus(TWINT)));
}

/**================================================================
* @Fn            -Get_flag_status
* @brief         -Checks if current I2C status matches the given flag
* @param [in]    -flag: Expected I2C status code
* @retval        -FlagStatus (SET or RESET)
* @note          -Based on value in TWSR register
*/
FlagStatus Get_flag_stutus(Status flag)
{
	FlagStatus current_state ;
	switch( flag )
	{
		case TWINT :
		if (TWCR & 1<<7 )
		current_state=SET ;
		else
		current_state=RESET ;
		break;
		case START_Transmitted :
		if ((I2C->TWSR & 0XF8) != 0X08 )
		current_state=SET ;
		else
		current_state=RESET ;
		break;
		case SLA_W_Transmitted :
		if ((I2C->TWSR & 0XF8) != 0X18 )
		current_state=SET ;
		else
		current_state=RESET ;
		break;
		case M_DATA_Transmitted :
		if ((I2C->TWSR & 0XF8) != 0X28 )
		current_state=SET ;
		else
		current_state=RESET ;
		break;
		case SLA_W_Received :
		if ((I2C->TWSR & 0XF8) != 0X60 )
		current_state=SET ;
		else
		current_state=RESET ;
		break;
		case S_DATA_Received :
		if ((I2C->TWSR & 0XF8) != 0X80 )
		current_state=SET ;
		else
		current_state=RESET ;
		break;
		case SLA_R_Transmitted :
		if ((I2C->TWSR & 0XF8) != 0X40 )
		current_state=SET ;
		else
		current_state=RESET ;
		break;
		case M_DATA_Received :
		if ((I2C->TWSR & 0XF8) != 0X50 )
		current_state=SET ;
		else
		current_state=RESET ;
		break;
		case SLA_R_Received :
		if ((I2C->TWSR & 0XF8) != 0XA8 )
		current_state=SET ;
		else
		current_state=RESET ;
		break;
		case S_DATA_Transmitted :
		if ((I2C->TWSR & 0XF8) != 0XB8 )
		current_state=SET ;
		else
		current_state=RESET ;
		break;
	}
	return current_state ;
}

/**================================================================
* @Fn				-I2C_Generate_Stop
* @brief		  	-generate stop condition to end transmitter .
* @retval 			-none
* Note				-none
*/
void I2C_Generate_Stop(void)
{
	TWCR =(1<<7) | (1<<4) |(1<<2);
}

/**================================================================
* @Fn            -MCAL_I2C_Slave_TX
* @brief         -Sends 8-bit data from slave to master over I2C
* @param [in]    -PTXBuffer: Byte to send
* @retval        -None
* @note          -Call this after slave has been addressed by master
*/
void MCAL_I2C_Slave_TX(uint8_t PTXBuffer)
{
	// wait until receive address from master
	while(!(Get_flag_stutus(TWINT)));
	// send data to master
	I2C->TWDR = PTXBuffer ;
	TWCR |=(1 << 7)  ;
	while(!(Get_flag_stutus(TWINT)));
	// clear flag
	TWCR |=(1 << 7) ;
}

/**================================================================
* @Fn            -MCAL_I2C_Slave_RX
* @brief         -Receives one byte (8-bit) of data from the I2C master to the slave.
* @retval        -The data byte received from the master.
* @note          -Call this function after the slave has been addressed by the master for writing.
*/
uint8_t MCAL_I2C_Slave_RX( void )
{
	// set ACK , enable I2C , clear flag
	TWCR =(1 << 7) | (1 << 6) | (1 << 2);
	
	// wait until receive address
	while(!(Get_flag_stutus(TWINT)));

	// receive data from master
	TWCR =(1 << 7) | (1 << 6) | (1 << 2);
	while(!(Get_flag_stutus(TWINT)));

	return I2C->TWDR;
}

/*
 * ===============================================================
 *                      ISR Function Definitions
 * ===============================================================
 */

void __vector_19 (void) __attribute__((signal));
void __vector_19 (void)
{
	g_I2C_Config->P_IRQ_CALL();
}
