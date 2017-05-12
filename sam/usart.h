/*The functions and macros defined in this header file are for USART of all 8-bit AVR Micricontrollers with Similar Register Configuration 
with ATmega 16*/

#ifndef	_USART_H_
#define	_USART_H_ 	1

#define FOSC 16000000// Clock Speed
#define BAUD 19200
#define MYUBRR FOSC/16/BAUD-1

#include<avr/io.h>
#include<util/delay.h>



/*The function is declared to initialize the USART with following cinfiguration:-
USART mode - Asynchronous
Baud rate - 9600
Data bits - 8
Stop bit - 1
Parity - No parity.*/

void usart_init();




/*The function is declared to transmit data.*/

void usart_data_transmit(unsigned char data );




/*The function is declared to receive data.*/

unsigned char usart_data_receive( void );




/*The function is declared to transmit string.*/

void usart_string_transmit(char *string);




/*The function is declared to receive string.*/

char *usart_string_receive(char *receive_string,unsigned char terminating_character);



/*Function defination*/

void usart_init()
{
	UBRR0H = (unsigned char)(MYUBRR>>8);
	UBRR0L = (unsigned char)MYUBRR;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN1)|(1<<TXEN1);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS1)|(3<<UCSZ10);
}


void usart_data_transmit(unsigned char data )
{
while ( !( UCSR1A & (1<<UDRE1)) );
UDR1 = data;
}

unsigned char usart_data_receive( void )
{
while ( !(UCSR1A & (1<<RXC1)));
return UDR1;
}

void usart_string_transmit(char *string)
{
while(*string)
{
usart_data_transmit(*string++);
}
}

char *usart_string_receive(char *receive_string,unsigned char terminating_character)
{
	unsigned char temp=0x00;
	for(unsigned char i=0;;i++)
	{
		*(receive_string+i)=usart_data_receive();
		if(*(receive_string+i)==terminating_character)
			break;
		else
		temp++;
	}
	*(receive_string+temp)='\0';
	return receive_string;
}
#endif
