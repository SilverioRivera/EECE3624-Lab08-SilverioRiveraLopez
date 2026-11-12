/*****************************************************************************
 *	    File: Lab08.c
 *  Lab Name: RS232 Communication Server
 *    Author: Dr. Greg Nordstrom
 *   Created: 12/30/17
 *  Mod Date: 3/28/19, 1/17/21
 *  Modified: Silverio Rivera-Lopez
 *   ModDate: 10/20/2026
 * 
 * Processor: ATmega128A (on the ReadyAVR board)
 *
 * This program implements a communication server. Normally the server
 * simply echoes what was sent. However, there are two cases where
 * different behavior occurs:
 *    1. Pressing the ESC causes a soft reset (redisplays the title text)
 *    2. Typing the letters 'end' contiguously displays 'Bye' and halts
 *
 *****************************************************************************/

// define CPU speed (7.3728 MHz for the ATmega128A)
#define F_CPU   7372800UL

#define CR          0x0D
#define LF          0x0A
#define ESC         0x1B

#include <avr/io.h>
#include <util/delay.h>
#include "UARTLibrary.h"  // Double quotes for local library.

#define BAUDRATE 9600 // standard values from 1200 to 115200
#define DATABITS    8 // 5-8
#define STOPBITS    1 // 1,2
#define PARITY      0 // 0=none, 1=odd, 2=even

/* Your code here.  */
	void LEDblink(void) // blink led for 75 ms
	{
		PORTA &= ~(1 << PA0); // Wei: Drive the active-low LED pin low to turn it on.
		_delay_ms(75); // wait 75 ms, delay library
		
		PORTA |= (1 << PA0); // Wei: Drive the active-low LED pin high to turn it off.
	}
	void printString(const char * str) // transmits a null terminated string through USART1
	{
		while(*str != '\0') // loops stop when it reaches the end of the string
		{
			uart_tx(*str); // reads the character at the address
			str++; // moves the pointer to the next character
		}
	}
	
	void startupmessage(void)
	{
		// \r means carriage return
		// \n line feed
		// \0 null terminator
		printString("\r\n\r\n");
		
		printString("RS232 Communications Server\r\n");
		printString("Status: Active\r\n");
		printString("Silverio Rivera-Lopez\r\n");
		printString("EECE3624\r\n");
		printString("Fall 2026");
		
		printString("\r\n\r\n");
	}

int main(void)
{
	/* Your code here.  */
	unsigned char c;
	unsigned char count = 0; // tracks the characters
	
	DDRA |=(1<< PA0); // Configure LED0 as output 
	PORTA |= (1 << PA0); // Wei: Keep the active-low LED off before the first blink.
	
	uart_init(BAUDRATE, DATABITS, STOPBITS, PARITY); // Initialize UART
	// Wei: Display the startup banner immediately after UART initialization.
	startupmessage();

	// main communication loop
	while(1) 
	{
		c = uart_rx(); // wait for character from Putty
		LEDblink(); // receive character
		
		if(c == CR) // handling carriage return 
		{
			uart_tx(CR);
			uart_tx(LF);
		}
		
		else if (c == ESC) // handle ESC soft reset
		{
			count = 0;
			startupmessage();
			continue;
		}
		
		else
		{
			uart_tx(c); // echo characters
	
		}
		
		if (c == 'e') // detect the sequence starting at 'e'. When 'end is detected send out bye termination message
		{
			count = 1;
		}
		else if (c == 'n' && count == 1)
		{
			count = 2;	
		}
		else if (c == 'd' && count == 2)
		{
			count = 3;
		}
		else
		{
			
			count = 0;
		}
		
		if(count == 3) // stop communication when "end is received
		{
			printString("\r\n\r\nBye"); // display termination message
			
			while(1) // stop responding to any inputs
			{
				
			}
		}
		
	}
	return 0;
}
