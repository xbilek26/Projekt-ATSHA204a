/*
 * uart.h
 *
 * Created: 4/2/2024 14:53:28
 *  Author: Student
 */ 


#ifndef UART_H_
#define UART_H_

#include <stdio.h>

void UART_init(uint16_t Baudrate);
void UART_SendChar(uint8_t data);
void UART_SendString(char *text);
int printCHAR(char character, FILE *stream);

#endif /* UART_H_ */