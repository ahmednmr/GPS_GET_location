/*
 * UART.c
 *
 *  Created on: Oct 11, 2019
 *      Author: Nmr
 */

#include "UART.h"





void UART_init(void)
{
//	unsigned int Value_of_UBBR = 0;


//	    UCSRA= (1<<U2X);
		UCSRB = (1 << RXEN) | (1 << TXEN)|(1<<RXCIE);
       UCSRC = (1 << URSEL) | (3 << UCSZ0);
				UBRRL = 51;
				UBRRH =0;
}


void UART_SendChar(char data)
{
	while (!( UCSRA & (1<<UDRE))) ;  /* Wait for empty transmit buffer */

	UDR = data;                     /* Put data into buffer, sends the data */
}


char UART_getChar(void)
{
	while (!(UCSRA & (1<<RXC)));   /* Wait until reveived flag set */

	return UDR;                   /* Return data in received buffer */
}


void UART_SEND_string(char *array)
{
	int i=0;
    while(array[i]!='\0')
    {

    	UART_SendChar(array[i]);
    	i++;
    }

}


void UART_SEND_long_number(unsigned long number)
{
   char buffer[20];

   ultoa(number,buffer,10);
   UART_SEND_string(buffer);


}


void UART_SEND_number(int number)
{
   char buffer[20];

   itoa(number,buffer,10);
   UART_SEND_string(buffer);


}


