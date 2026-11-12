/*****************************************************************************
 *	    File: USARTLibrary.c
 *  Lab Name: (Needed for Lab07 and Lab08)
 *    Author: Dr. Greg Nordstrom
 *   Created: 3/3/2019
 *  Modified: Silverio Rivera-Lopez
 *  Mod Date: 11/06/2026
 * Processor: ATmega128A (on the ReadyAVR board)
 *
 * This is intended to be compiled as an AVR Library Project and
 * used by other programs. After compiling this code, copy the
 * libUSART1Library.a and USART1Library.h files to the project directory,
 * then add the library to the target project by R-clicking on the
 * project's Libraries directory and clicking Add Library. Also,
 * the USART1Library.h file must be #included in the project's .c file.
 *
 * NOTE: This library only works with the default USART1 port
 * on the ReadyAVR (so JP2 must have jumpers on PD3 and PD2).
 */

#include <avr/io.h>
#include "UARTLibrary.h"


// Wei: uart_init configures USART1 using the requested baud, 7/8 data bits,
// Wei: 1/2 stop bits, and parity (0=none, 1=odd, 2=even).
// Wei: Example: uart_init(9600, 8, 1, 0) selects 9600 baud, 8-N-1.
void uart_init(long unsigned int baudrate, unsigned char databits, unsigned char stopbits, unsigned char parity) {
    /* your code here */
	// Baud Rate Configuration
	unsigned int UBRR;
	// Wei: Calculate the USART1 divisor using the shared oscillator constant.
	UBRR = (F_OSC / (16UL * baudrate)) - 1UL;
	// Hardware registers
	UCSR1B = 0;	// Clears USART1 Control and Status register B. 
	UCSR1A = 0; // Configures normal speed by clearing U2X1. Asynchronous communication
	
	// Wei: Write the divisor to USART1's high and low baud registers.
	UBRR1H = (unsigned char)(UBRR >> 8);
	UBRR1L = (unsigned char)UBRR;
	
    UCSR1C = 0; // clearing USART 1 Control and Status Register C
	
	// set databits
	if (databits == 7){
		UCSR1C |= (1 << UCSZ11); // selecting character size
	}
	else
	{
		UCSR1C |= (1 << UCSZ11) | (1 << UCSZ10);
	}
    
	// set parity
	if (parity == 1) // odd parity
	{
		UCSR1C |= ( 1<< UPM11) | (1 << UPM10); // UPM11 and UPM10 are used to control parity
	}
	else if (parity == 2) //even parity condition 
	{
		UCSR1C |= (1 << UPM11);
	}
    // set stopbits
    if (stopbits == 2)
	{
		UCSR1C |= (1 << USBS1); // USBS1 controls the number of stop bits transmitted
	}
	// finally, enable receiver and transmitter
    UCSR1B = (1 << RXEN1) | (1 << TXEN1);
}

// This function waits for space in the USART transmit buffer. When space
// is available, the function places an unsigned character value (passed in
// as the function's only argument) in the buffer and returns. No error
// checking is performed and the function does not return a value.
void uart_tx(unsigned char data) {
    /* your code here */
	while(!(UCSR1A & (1 << UDRE1))) // transmit function
	{								// UDRE1 flag indicates that transmit data register is ready for another character
		
	}
	
	UDR1 = data;

}

// This function blocks until a character is received in the USART input
// buffer. Once a character arrives, it is passed back to the caller as
// an unsigned character. No error checking is performed.
unsigned char uart_rx(void) {
	/* your code here */
	while (!(UCSR1A & (1 << RXC1))) //receiving function. RXC1 flags indicates data is available. 
	{								// program waits for character and returns it
		
	}
	
	return UDR1;
    
}