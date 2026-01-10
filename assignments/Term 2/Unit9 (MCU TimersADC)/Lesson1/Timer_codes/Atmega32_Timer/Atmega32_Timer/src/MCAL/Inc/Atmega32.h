/*
 * Atmega32.h
 *
 * Created: 14/07/2025 10:31:22 
 *  Author: AAMA
 */ 


#ifndef ATMEGA32_H_
#define ATMEGA32_H_

#define F_CPU 8000000UL
#include <util/delay.h>

//-----------------------------
//Includes
//-----------------------------
#include <stdint.h>
#include <stdlib.h>

//-----------------------------
//Base addresses for Memories
//-----------------------------

//GPIOs
#define  GPIOA_Base				0X39
#define  GPIOB_Base				0X36
#define  GPIOC_Base				0X33
#define  GPIOD_Base			    0X30

//Interrupt
#define  EXTI_Base				0X54
#define SREG					*((volatile uint8_t*)0X5F)  // Global Interrupt Enable (bit number 7 )
#define GICR					*((volatile uint8_t*)0X5B)  //  Interrupt Enable (bit number 7 and 6 and 5 )

//SPI
#define  SPI_Base				0X2D

//I2C
#define  I2C_Base				0X20
#define TWCR					*((volatile uint8_t*)0X56)  //I2C

//USART
#define USART_Base				0X29
#define UBRRH					*((volatile uint8_t*)0X40)  
#define UCSRC					*((volatile uint8_t*)0X40)

// timer0
#define TIMER1_Base				0X46
#define ADC_Base				0X24
#define TCCR0					*((volatile uint8_t*)0X53)
#define TIMSK					*((volatile uint8_t*)0X59)
#define TIFR					*((volatile uint8_t*)0X58)
#define TCNT0					*((volatile uint8_t*)0X52)
#define OCR0					*((volatile uint8_t*)0X5C)

//==============================================================================
//-*-*-*-*-*-*-*-*-*-*-*-
//Peripheral register:
//-*-*-*-*-*-*-*-*-*-*-*

typedef struct
{
	uint8_t  PIN   ;
	uint8_t  DDR   ;
	uint8_t  PORT  ;
}GPIO_TypeDef;


typedef struct
{
	uint8_t  MCUCSR   ;
	uint8_t  MCUCR    ;
}EXTI_TypeDef;


typedef struct
{
	
	uint8_t  UBRRL   ;
	uint8_t  UCSRB   ;
	uint8_t  UCSRA   ;
	uint8_t  UDR	 ;
}USART_TypeDef;

typedef struct
{
	uint8_t  SPCR   ;
	uint8_t  SPSR   ;
	uint8_t  SPDR  ;
}SPI_TypeDef;

typedef struct
{
	
	uint8_t  TWBR   ;
	uint8_t  TWSR   ;
	uint8_t  TWAR   ;
	uint8_t  TWDR	 ;
}I2C_TypeDef;

typedef struct
{
	uint8_t  ICR1L    ;
	uint8_t  ICR1H    ;
	uint8_t  OCR1BL   ;
	uint8_t  OCR1BH	  ;
	uint8_t  OCR1AL   ;
	uint8_t  OCR1AH   ;
	uint8_t  TCNT1L   ;
	uint8_t  TCNT1H	  ;
	uint8_t  TCCR1B   ;
	uint8_t  TCCR1A	  ;
}TIMER1_TypeDef;

typedef struct
{
	uint8_t  ADCL   ;
	uint8_t  ADCH   ;
	uint8_t  ADCSRA   ;
	uint8_t  ADMUX	 ;
}ADC_TypeDef;

//===============================================================================
//-*-*-*-*-*-*-*-*-*-*-*-
//Peripheral Instants:
//-*-*-*-*-*-*-*-*-*-*-*

//GPIOs
#define GPIOA		((GPIO_TypeDef*)GPIOA_Base)
#define GPIOB		((GPIO_TypeDef*)GPIOB_Base)
#define GPIOC		((GPIO_TypeDef*)GPIOC_Base)
#define GPIOD		((GPIO_TypeDef*)GPIOD_Base)

//Interrupt
#define EXTI		((EXTI_TypeDef*)EXTI_Base)

//USART
#define USART		((USART_TypeDef*)USART_Base)

//SPI
#define SPI			((SPI_TypeDef*)SPI_Base)

//I2C
#define I2C			((I2C_TypeDef*)I2C_Base)

//Timer1
#define TIMER1		((TIMER1_TypeDef*)TIMER1_Base)

//=====================================================================================================
//-*-*-*-*-*-*-*-*-*-*-*-
//EXTI Enable Macros:
//-*-*-*-*-*-*-*-*-*-*-*
#define EXTI_GLOBAL_Enable()							(SREG |=1<<7)
#define EXTI_INT1_Enable()								(GICR |=1<<7)
#define EXTI_INT0_Enable()								(GICR |=1<<6)
#define EXTI_INT2_Enable()								(GICR |=1<<5)

//-*-*-*-*-*-*-*-*-*-*-*-
//EXTI Disable Macros:
//-*-*-*-*-*-*-*-*-*-*-*
#define EXTI_GLOBAL_Disable()							(SREG &=~(1<<7))
#define EXTI_INT1_Disable()								(GICR &=~(1<<7))
#define EXTI_INT0_Disable()								(GICR &=~(1<<6))
#define EXTI_INT2_Disable()								(GICR &=~(1<<5))

//-*-*-*-*-*-*-*-*-*-*-*-
//USART Interrupt Enable Macros:
//-*-*-*-*-*-*-*-*-*-*-*
#define USART_INT_RX_Enable()							(USART->UCSRB |=1<<7)
#define USART_INT_TX_Enable()							(USART->UCSRB |=1<<6)
#define USART_INT_UDR_Enable()							(USART->UCSRB |=1<<5)

//-*-*-*-*-*-*-*-*-*-*-*-
//USART Interrupt Disable Macros:
//-*-*-*-*-*-*-*-*-*-*-*
#define USART_INT_RX_Disable()							(USART->UCSRB &=~(1<<7))
#define USART_INT_TX_Disable()							(USART->UCSRB &=~(1<<6))
#define USART_INT_UDR_Disable()							(USART->UCSRB &=~(1<<5))

//-*-*-*-*-*-*-*-*-*-*-*-
//I2C Interrupt Disable Macros:
//-*-*-*-*-*-*-*-*-*-*-*
#define I2C_INT_Enable()								(TWCR |=1<<0)
#define I2C_INT_Disable()								(TWCR &=~(1<<0))

//-*-*-*-*-*-*-*-*-*-*-*-
//Timer Interrupt Disable Macros:
//-*-*-*-*-*-*-*-*-*-*-*
#define TIMR0_TOIE0_INT_Enable()						(TIMSK |=1<<0)
#define TIMR0_OCIE0_INT_Enable()						(TIMSK |=1<<1)
#define TIMR1_TOIE1_INT_Enable()						(TIMSK |=1<<2)
#define TIMR1_OCIE1A_INT_Enable()						(TIMSK |=1<<4)
#define TIMR1_OCIE1B_INT_Enable()						(TIMSK |=1<<3)

#define TIMR0_TOIE0_INT_Disable()						(TIMSK &=~(1<<0))
#define TIMR0_OCIE0_INT_Disable()						(TIMSK &=~(1<<1))
#define TIMR1_OCIE1A_INT_Disable()						(TIMSK &=~(1<<4))
#define TIMR1_OCIE1B_INT_Disable()						(TIMSK &=~(1<<3))
#define TIMR1_TOIE1_INT_Disable()						(TIMSK &=~(1<<2))
//======================================================================================================



#endif /* ATMEGA32_H_ */