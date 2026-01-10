/*
 * GPIO.c
 *
 * Created: 14/07/2025 10:42:13 
 *  Author: AAMA
 */ 

//-----------------------------
//Includes
//-----------------------------
#include "Inc/GPIO.h"

//=======================================
//===============APIs====================
//=======================================

/**================================================================
* @Fn              -MCAL_GPIO_Init
* @brief           -Initialize GPIO pin with the given configuration
* @param [in]      -GPIOx: where x can be (A..D) to select GPIO port
* @param [in]      -pinconfig: pointer to GPIO pin configuration structure
* @retval          -none
* Note             -Must enable RCC for GPIO before calling this function
*/
void MCAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_Pinconfig_t *pinconfig)
{
	if(pinconfig->GPIO_MODE == GPIO_MODE_INPUT_FLO)
	{
		GPIOx->DDR &= ~(pinconfig->pinNumber);
		GPIOx->PORT &= ~(pinconfig->pinNumber);
	}
	else if ( pinconfig->GPIO_MODE == GPIO_MODE_INPUT_PU )
	{
		GPIOx->DDR &= ~(pinconfig->pinNumber);
		GPIOx->PORT |= pinconfig->pinNumber;
	}
	else
	{
		GPIOx->DDR |= pinconfig->pinNumber;
		GPIOx->PORT &= ~(pinconfig->pinNumber);
	}
}


/**================================================================
* @Fn              -MCAL_GPIO_DeInit
* @brief           -Resets all GPIO registers for the selected port
* @param [in]      -GPIOx: where x can be (A..D) to select GPIO port
* @retval          -none
* Note             -Resets the port to its default state
*/
void MCAL_GPIO_DeInit(GPIO_TypeDef *GPIOx)
{
	GPIOx->DDR  = 0X00  ;
	GPIOx->PORT = 0X00  ;
}

/**================================================================
* @Fn              -MCAL_GPIO_ReadPin
* @brief           -Reads the value of a specific GPIO pin
* @param [in]      -GPIOx: where x can be (A..D) to select GPIO port
* @param [in]      -pinNumber: pin number to read (0 to 15)
* @retval          -The input pin value (0 or 1)
* Note             -Make sure the pin is configured as input
*/
uint8_t MCAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint8_t pinNumber)
{
	if( GPIOx->PIN & pinNumber )
	{
		return GPIO_PIN_SET;
	}
	return GPIO_PIN_RESET;
}

/**================================================================
* @Fn              -MCAL_GPIO_ReadPort
* @brief           -Reads the input data of the entire GPIO port
* @param [in]      -GPIOx: where x can be (A..D) to select GPIO port
* @retval          -16-bit value representing the state of all port pins
* Note             -Each bit in the return value represents a pin (bit 0 -> PIN0, ..., bit 15 -> PIN15)
*/
uint16_t MCAL_GPIO_ReadPort(GPIO_TypeDef *GPIOx)
{
	uint8_t port_value;
	port_value= (uint8_t) (GPIOx->PIN);
	return port_value;
}

/**================================================================
* @Fn              -MCAL_GPIO_WritePin
* @brief           -Writes a value to a specific GPIO pin
* @param [in]      -GPIOx: where x can be (A..D) to select GPIO port
* @param [in]      -pinNumber: specifies the pin to be written (use @ref GPIO_PINS_Define)
* @param [in]      -value: pin value (GPIO_PIN_SET or GPIO_PIN_RESET)
* @retval          -none
* Note             -Pin must be configured as output or alternate function output
*/
void MCAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint8_t pinNumber, uint8_t value)
{
	if ( value == GPIO_PIN_SET )
	{
		GPIOx->PORT |= pinNumber ;
	}
	else if(  value == GPIO_PIN_RESET)
	{
		GPIOx->PORT &= ~( pinNumber ) ;
	}
}

/**================================================================
* @Fn              -MCAL_GPIO_WritePort
* @brief           -Writes a value to the entire GPIO port
* @param [in]      -GPIOx: where x can be (A..D) to select GPIO port
* @param [in]      -value: 8-bit value to write to the port
* @retval          -none
* Note             -Make sure the port pins are configured as outputs
*/
void MCAL_GPIO_WritePort(GPIO_TypeDef *GPIOx, uint8_t value)
{
	GPIOx->PORT=  value;
}

/**================================================================
* @Fn              -MCAL_GPIO_TogglePin
* @brief           -Toggles the state of a specific GPIO pin
* @param [in]      -GPIOx: where x can be (A..D) to select GPIO port
* @param [in]      -pinNumber: specifies the pin to toggle (use @ref GPIO_PINS_Define)
* @retval          -none
* Note             -Pin must be configured as output
*/
void MCAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint8_t pinNumber)
{
	GPIOx->PORT ^=pinNumber;
}
