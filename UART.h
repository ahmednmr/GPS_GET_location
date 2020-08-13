/*
 * UART.h
 *
 *  Created on: Oct 11, 2019
 *      Author: Nmr
 */

#ifndef UART_H_
#define UART_H_
#include <avr/io.h>
#include<stdlib.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdio.h>


#define FOSC 8000000// Clock Speed
#define BAUD 115200



void UART_init(void);
void UART_SendChar(char data);
char UART_getChar(void);
void UART_SEND_string(char *array);
void UART_SEND_long_number(unsigned long number);
void UART_SEND_number(int number);




#endif /* UART_H_ */
