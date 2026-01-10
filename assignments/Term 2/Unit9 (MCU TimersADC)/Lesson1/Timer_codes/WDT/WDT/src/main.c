#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define TOGGLE_BIT(PORT, BIT)			PORT ^= (1<<BIT)

#define LED_DDR		DDRC
#define LED_PORT	PORTC
#define LED_PIN		0
void WDT_ON(void);
void WDT_OFF(void);


void WDT_ON(void)
{
	WDTCR = (1 << WDP0) | (1 << WDP1) | (1 << WDP2) | (1 << WDE);
	WDTCR &= ~(1<<WDP0);
}

void WDT_OFF(void)
{
	WDTCR = (1<<WDTOE)|(1<<WDE);
	WDTCR = 0x00;
}

int main(void)
{
	LED_DDR |= 1<<LED_PIN;
	
    while (1) 
    {
		WDT_ON();
		TOGGLE_BIT(LED_PORT, 0);
		_delay_ms(900);
		WDT_OFF();
    }
}

