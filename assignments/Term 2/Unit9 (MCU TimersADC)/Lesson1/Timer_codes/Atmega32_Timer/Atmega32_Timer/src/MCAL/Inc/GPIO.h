/*
 * GPIO.h
 *
 * Created: 14/07/2025 10:30:36
 *  Author: AAMA
 */ 


#ifndef GPIO_H_
#define GPIO_H_

//Include
#include "Atmega32.h"


typedef struct
{
	uint8_t pinNumber 			 ; //specified the GPIO pin to be configured .
	//this parameter must be set based on  @ ref  GPIO_PINS_Define

	uint8_t GPIO_MODE			; //specified the operating  for the select pin .
	//this parameter must be set based on  @ ref  GPIO_MODE_Define
}GPIO_Pinconfig_t;

//=========================================================================
//-----------------------------
//Macros Configuration References
//-----------------------------

//@ref GPIO_PINS_Define
#define GPIO_PIN_0			((uint8_t)0X01)
#define GPIO_PIN_1			((uint8_t)0X02)
#define GPIO_PIN_2			((uint8_t)0X04)
#define GPIO_PIN_3			((uint8_t)0X08)
#define GPIO_PIN_4			((uint8_t)0X10)
#define GPIO_PIN_5			((uint8_t)0X20)
#define GPIO_PIN_6			((uint8_t)0X40)
#define GPIO_PIN_7			((uint8_t)0X80)


//@ref GPIO_MODE_Define
#define GPIO_MODE_INPUT_FLO			1
#define GPIO_MODE_INPUT_PU			2
#define GPIO_MODE_OUTPUT			3


//@ref GPIO_PINS_state
#define GPIO_PIN_SET				1
#define GPIO_PIN_RESET				0

//===========================================================================
/*
* ===============================================
* ================APIs===========================
* ===============================================
*/
void MCAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_Pinconfig_t *pinconfig );
void MCAL_GPIO_DeInit(GPIO_TypeDef *GPIOx );
uint8_t MCAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint8_t pinNumber );
uint16_t MCAL_GPIO_ReadPort(GPIO_TypeDef *GPIOx );
void MCAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint8_t pinNumber, uint8_t value);
void MCAL_GPIO_WritePort(GPIO_TypeDef *GPIOx, uint8_t value);
void MCAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint8_t pinNumber );


#endif /* GPIO_H_ */