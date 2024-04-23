/*
 * uart.c
 *
 * Created: 4/2/2024 14:53:45
 *  Author: Student
 */ 

#include "uart.h"
#include <stdio.h>
#include <avr/io.h>
 

void UART_init(uint16_t Baudrate)
{
	int ubrr = ((F_CPU/16/Baudrate)-1);
	UBRR1H = (uint8_t)(ubrr>>8);
	UBRR1L = (uint8_t)ubrr;
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);
	
	UCSR1C |= (1 << UCSZ11) | (1 << UCSZ10);
	
	UBRR1H = (uint8_t)(ubrr>>8);
	UBRR1L = (uint8_t)ubrr;
	
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);
}

void UART_SendChar(uint8_t data)
{
	while (!( UCSR1A & (1<<UDRE0))) ;
	UDR1 = data;
}

void UART_SendString(char *text)
{
	while (*text != '\0')
    {
        UART_SendChar(*text);
        text++;
    }
    // After the loop, send the null terminator
    UART_SendChar('\0');
}


int printCHAR(char character, FILE *stream)
{
	while ((UCSR1A & (1 << UDRE1)) == 0) ;

	UDR1 = character;

	return 0;
}