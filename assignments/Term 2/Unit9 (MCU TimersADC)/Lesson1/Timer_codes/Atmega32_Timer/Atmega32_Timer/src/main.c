#define F_CPU 8000000UL
#include "MCAL/Inc/GPIO.h"
#include "MCAL/Inc/Timer0.h"

void TIMER0_Callback_For_PA0(void)
{
	MCAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
}

int main(void)
{
	GPIO_Pinconfig_t led_pin_config;
	led_pin_config.pinNumber = GPIO_PIN_0;
	led_pin_config.GPIO_MODE = GPIO_MODE_OUTPUT;
	MCAL_GPIO_Init(GPIOA, &led_pin_config);

	TIMER0_config_t TIMER0_Config;
	TIMER0_Config.Timer0_Mode = TIMER0_MODE_CTC;
	TIMER0_Config.Clock_Source = TIMER0_Clock_Source_clk_1024;
	TIMER0_Config.Compare_Value = 250;
	TIMER0_Config.Output_Compare_pin = TIMER0_Output_Compare_pin_Disable;
	
	TIMER0_Config.OCIE0_EN = TIMER0_OCIE0_ENABLE_Disable;
	TIMER0_Config.TOIE0_EN = TIMER0_TOIE0_ENABLE_Enable;
	TIMER0_Config.P_OCIE0_CALL = NULL;
	TIMER0_Config.P_TOIE0_CALL = TIMER0_Callback_For_PA0;

	MCAL_TIMER0_Init(&TIMER0_Config);
	
	while (1)
	{
		
	}
}
