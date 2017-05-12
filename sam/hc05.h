//Micricontroller		:All 8-bit AVR Micricontrollers with Similar Register Configuration with ATmega16
//System Clock			:8 MHz
//Header File Version 		:1.1
//Author			:Arun Kumar Garg 
//				:ABLab Solutions
//				:www.ablab.in
//				:arun@ablab.in
//Date				:1st June 2015

/*Copyright (c) 2011 ABLab Solutions All rights reserved.
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the 	following disclaimer in the documentation 
     and/or other materials provided with the distribution.
   * Neither the name of the copyright holders nor the names of contributors may be used to endorse or promote products derived from this software without 
     specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */


/*The functions and macros defined in this header file are for HC-05 Bluetooth Module*/

#ifndef	_HC05_H_
#define	_HC05_H_ 	1


#include<avr/io.h>
#include<util/delay.h>
#include "usart.h"
char hc_05_buffer1[25], hc_05_buffer2[50];
char temp;

void hc_05_bluetooth_transmit_byte(char data_byte);
char hc_05_bluetooth_receive_byte(void);
void hc_05_bluetooth_transmit_string(char *transmit_string);
void hc_05_bluetooth_transmit_command(char *transmit_string);
char *hc_05_bluetooth_receive_string(char *receive_string, unsigned char terminating_character);
unsigned char hc_05_bluetooth_at_command_mode_test(void);
unsigned char hc_05_bluetooth_device_name_change(char *device_name);
unsigned char hc_05_bluetooth_get_version(void);
unsigned  char hc_05_bluetooth_change_baud_rate(long int baud_rate);
unsigned  char hc_05_bluetooth_pin_change(char *new_pin);


void hc_05_bluetooth_transmit_byte(char data_byte)
{
	usart_data_transmit(data_byte);
}
char hc_05_bluetooth_receive_byte(void)
{
	return usart_data_receive();
}
void hc_05_bluetooth_transmit_string(char *transmit_string)
{
	usart_string_transmit(transmit_string);
}
char *hc_05_bluetooth_receive_string(char *receive_string, unsigned char terminating_character)
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
unsigned char hc_05_bluetooth_at_command_mode_test(void)
{
	UBRR1L=51;
	_delay_ms(500);
	usart_string_transmit("AT");
	usart_data_transmit(0x0d);
	usart_data_transmit(0x0a);
	usart_string_receive(hc_05_buffer1,0x0d);
	temp=usart_data_receive();
	if(!(strcmp(hc_05_buffer1,"OK")))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

unsigned  char hc_05_bluetooth_change_baud_rate(long int baud_rate)
{
	UBRR1L=51;
	_delay_ms(500);
	
	if(baud_rate==4800)
	{
		usart_string_transmit("AT+UART=4800,0,0");
	}
	else if(baud_rate==9600)
	{
		usart_string_transmit("AT+UART=9600,0,0");
	}
	else if(baud_rate==19200)
	{
		usart_string_transmit("AT+UART=19200,0,0");
	}
	else if(baud_rate==38400)
	{
		usart_string_transmit("AT+UART=38400,0,0");
	}
	else if(baud_rate==57600)
	{
		usart_string_transmit("AT+UART=57600,0,0");
	}
	else if(baud_rate==115200)
	{
		usart_string_transmit("AT+UART=115200,0,0");
	}
	else if(baud_rate==230400)
	{
		usart_string_transmit("AT+UART=230400,0,0");
	}
	else if(baud_rate==460800)
	{
		usart_string_transmit("AT+UART=460800,0,0");
	}
	else if(baud_rate==921600)
	{
		usart_string_transmit("AT+UART=921600,0,0");
	}
	else if(baud_rate==1382400)
	{
		usart_string_transmit("AT+UART=1382400,0,0");
	}
	else
	{
		;
	}
	usart_data_transmit(0x0d);
	usart_data_transmit(0x0a);
	usart_string_receive(hc_05_buffer1,13);
	temp=usart_data_receive();
	
	if(!(strcmp(hc_05_buffer1,"OK")))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

unsigned char hc_05_bluetooth_device_name_change(char *device_name)
{
	UBRR1L=51;
	_delay_ms(500);
	
	usart_string_transmit("AT+NAME=");
	usart_string_transmit(device_name);
	usart_data_transmit(0x0d);
	usart_data_transmit(0x0a);
	
	usart_string_receive(hc_05_buffer1,13);
	temp=usart_data_receive();
	
	if(!(strcmp(hc_05_buffer1,"OK")))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
unsigned char hc_05_bluetooth_get_version(void)
{
	UBRR1L=51;
	_delay_ms(500);
	
	unsigned char i=9,j=0;
	usart_string_transmit("AT+VERSION?");
	usart_data_transmit(0x0d);
	usart_data_transmit(0x0a);
	
	usart_string_receive(hc_05_buffer2,13);
	temp=usart_data_receive();
	
	usart_string_receive(hc_05_buffer1,13);
	temp=usart_data_receive();
		
	for(i=9;hc_05_buffer2[i]!=0;i++)
	{
		hc_05_buffer2[j]=hc_05_buffer2[i];
		j++;
	}
	hc_05_buffer2[j]=0;
	
	if(!(strcmp(hc_05_buffer1,"OK")))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

unsigned  char hc_05_bluetooth_pin_change(char *new_pin)
{
	UBRR1L=51;
	_delay_ms(500);
	
	usart_string_transmit("AT+PSWD=");
	usart_string_transmit(new_pin);
	usart_data_transmit(0x0d);
	usart_data_transmit(0x0a);
	
	usart_string_receive(hc_05_buffer1,13);
	temp=usart_data_receive();
	
	if(!(strcmp(hc_05_buffer1,"OK")))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

#endif










