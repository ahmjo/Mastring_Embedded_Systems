/*
 * EXTI.h
 *
 * Created: 05/08/2025 07:07:53 م
 *  Author: AAMA
 */ 


#ifndef EXTI_H_
#define EXTI_H_

//Includes
#include "Atmega32.h"
#include "GPIO.h"

typedef struct
{
	uint8_t 			EXTI_Number ;//specified the EXTI  Number--. (INT0 , INT1 , INT2 ).
	//this parameter must be set based on @ref EXTI_Define
		
	uint8_t				Trigger_case;//specified Rising or Falling or Both Trigger  .
	//this parameter must be set based on @ref EXTI_Trigger_Define
		
	uint8_t				EXTI_EN;  	//Enable or Disable EXTI_IRQ (EXTI MASK and EXTI Global) .
	//this parameter must be set based on @ref EXTI_IRQ_Define
		
	void (*P_IRQ_CALL)(void);      // set the function which will be call at IRQ happen
	
}EXTI_PinConfig_t;

//==================================================================
//-----------------------------
//Macros Configuration References
//-----------------------------
//@ref EXTI_Define
#define EXTI_INT0								0
#define EXTI_INT1								1
#define EXTI_INT2								2

//@ref EXTI_Trigger_Define
#define EXTI_Trigger_Rising			  			0
#define EXTI_Trigger_Falling					1
#define EXTI_Trigger_Rising_and_Falling			2

//@ref EXTI_IRQ_Define
#define EXTI_IRQ_Disable			  			0
#define EXTI_IRQ_Enable					        1

//==========================================================================
/*
* ===============================================
* APIs Supported by "MCAL EXTI DRIVER"
* ===============================================
*/
void MCAL_EXTI_GPIO_Init(EXTI_PinConfig_t* EXTI_Config);
void MCAL_EXTI_GPIO_DeInit(void);

#endif /* EXTI_H_ */