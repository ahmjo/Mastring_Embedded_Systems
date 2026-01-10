/*
 * Timer0.c
 *
 * Created: 29/09/2025 10:01:11 م
 *  Author: AAMA
 */ 


#include "Inc/Timer0.h"

/*
 * ===============================================================
 *
 *                      Generic Variable
 *
 * ===============================================================
 */

TIMER0_config_t* g_TIM0_Config = NULL;


//===============================================================================

/**================================================================
* @Fn            -MCAL_TIMER0_Init
* @brief         -Initializes TIMER0 according to the specified parameters in TIMER0_Config.
* @param [in]    -TIMER0_Config: Pointer to configuration structure for TIMER0.
* @retval        -none
* @note          -Supports Normal mode and CTC mode.
*/
void MCAL_TIMER0_Init(TIMER0_config_t* TIMER0_Config)
{
	g_TIM0_Config = TIMER0_Config;
	uint8_t Temp_TCCR0=0 , Temp_TIMSK=0;
	
	//Set timer0 mode
	Temp_TCCR0 =TIMER0_Config->Timer0_Mode;
	
	//Set Clock Source
	Temp_TCCR0 |= TIMER0_Config->Clock_Source;
	
	//Set Output Compare pin
	if (TIMER0_Config->Output_Compare_pin != TIMER0_Output_Compare_pin_Disable)
	{
		Temp_TCCR0 |= TIMER0_Config->Output_Compare_pin;
		GPIO_Pinconfig_t pinconfig;
		pinconfig.pinNumber = GPIO_PIN_3;
		pinconfig.GPIO_MODE = GPIO_MODE_OUTPUT;
		MCAL_GPIO_Init(GPIOB , &pinconfig );
	}
	
	//Set Output Compare pin TOIE0_EN
	Temp_TIMSK |=TIMER0_Config->TOIE0_EN;
	
	//Set Output Compare pin OCIE0_EN
	Temp_TIMSK |=TIMER0_Config->OCIE0_EN;
	
	//Set Compare Value
	OCR0 = TIMER0_Config->Compare_Value;
	
	//Set IRQ_EN
	if (TIMER0_Config->TOIE0_EN==TIMER0_TOIE0_ENABLE_Enable)
	{
		TIMR0_TOIE0_INT_Enable();
		EXTI_GLOBAL_Enable();
	}
	else
	{
		TIMR0_TOIE0_INT_Disable();
		EXTI_GLOBAL_Disable();
	}
	
	if (TIMER0_Config->OCIE0_EN==TIMER0_OCIE0_ENABLE_Enable)
	{
		TIMR0_OCIE0_INT_Enable();
		//EXTI_GLOBAL_Enable();
	}
	else
	{
		TIMR0_OCIE0_INT_Disable();
		//EXTI_GLOBAL_Disable();
	}
	
	TCCR0 = Temp_TCCR0;
	TIMSK = Temp_TIMSK;
}


/**================================================================
* @Fn            -MCAL_TIMER0_Set_Compare_Value
* @brief         -Sets the compare value for TIMER0 in CTC mode.
* @param [in]    -Compare_Value: The value to be loaded into the Output Compare Register (OCR0).
* @retval        -none
* @note          -Used when TIMER0 is configured in CTC mode.
*/
void MCAL_TIMER0_Set_Compare_Value(uint8_t Compare_Value)
{
	OCR0 = Compare_Value;
}

/**================================================================
* @Fn            -MCAL_TIMER0_Read_Compare_Value
* @brief         -Reads the current compare value from TIMER0 (OCR0 register).
* @retval        -The current value stored in OCR0.
* @note          -Useful for checking or debugging the compare value.
*/
uint8_t MCAL_TIMER0_Read_Compare_Value(void)
{
	return OCR0;
}

/**================================================================
* @Fn            -MCAL_TIMER0_Read_Counter_Value
* @brief         -Reads the current counter value from TIMER0 (TCNT0 register).
* @retval        -The current value stored in TCNT0.
* @note          -Useful for monitoring the timer progress in real-time.
*/
uint8_t MCAL_TIMER0_Read_Counter_Value(void)
{
	return TCNT0;
}

/* ===============================================================
 *
 *                      ISR Function Definitions
 *
 * ===============================================================*/

void __vector_10 (void) __attribute__((signal));
void __vector_10 (void)
{
	g_TIM0_Config->P_OCIE0_CALL();
}

void __vector_11 (void) __attribute__((signal));
void __vector_11 (void)
{
	g_TIM0_Config->P_TOIE0_CALL();
}