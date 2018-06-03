#include "usart.h"

#define F_CPU 8000000UL // Assume uC operates at 8MHz

#include <avr/io.h>
#include <util/delay.h>

// USART Setup Values
#define BAUD_RATE 9600
#define BAUD_PRESCALE (((F_CPU / (BAUD_RATE * 16UL))) - 1)

#define UCSRA UCSR1A
#define UCSRB UCSR1B
#define UCSRC UCSR1C

// UCSRA
#define RXC 7
#define TXC 6
#define UDRE 5

// UCSRB
#define RXEN 4
#define TXEN 3

// UCSRC
#define UMSEL1 7
#define UMSEL0 6
#define UCSZ1 2
#define UCSZ0 1

#define UBRRL UBRR1L
#define UBRRH UBRR1H

#define UDR UDR1

////////////////////////////////////////////////////////////////////////////////
//Functionality - Initializes TX and RX on PORT D
//Parameter: None
//Returns: None
void initUSART()
{
    // Turn on the reception circuitry
    // Use 8-bit character sizes - URSEL bit set to select the UCRSC register
    // Turn on receiver and transmitter
    UCSRB |= (1 << RXEN)  | (1 << TXEN);
    //UCSRC |= (1 << UMSEL0) | (1 << UCSZ0) | (1 << UCSZ1);
    UCSRC |= (1 << UCSZ0) | (1 << UCSZ1);
    // Load lower 8-bits of the baud rate value into the low byte of the UBRR register
    UBRRL = BAUD_PRESCALE;
    // Load upper 8-bits of the baud rate value into the high byte of the UBRR register
    UBRRH = (BAUD_PRESCALE >> 8);
}
////////////////////////////////////////////////////////////////////////////////
//Functionality - checks if USART is ready to send
//Parameter: None
//Returns: 1 if true else 0
unsigned char USART_IsSendReady()
{
    return (UCSRA & (1 << UDRE));
}
////////////////////////////////////////////////////////////////////////////////
//Functionality - checks if USART has recieved data
//Parameter: None
//Returns: 1 if true else 0
unsigned char USART_HasTransmitted()
{
    return (UCSRA & (1 << TXC));
}
////////////////////////////////////////////////////////////////////////////////
// **** WARNING: THIS FUNCTION BLOCKS MULTI-TASKING; USE WITH CAUTION!!! ****
//Functionality - checks if USART has recieved data
//Parameter: None
//Returns: 1 if true else 0
unsigned char USART_HasReceived()
{
    return (UCSRA & (1 << RXC));
}
////////////////////////////////////////////////////////////////////////////////
//Functionality - Flushes the data register
//Parameter: None
//Returns: None
void USART_Flush()
{
    static unsigned char dummy;
    while ( UCSRA & (1 << RXC) ) { dummy = UDR; }
}
////////////////////////////////////////////////////////////////////////////////
// **** WARNING: THIS FUNCTION BLOCKS MULTI-TASKING; USE WITH CAUTION!!! ****
//Functionality - Sends an 8-bit char value
//Parameter: Takes a single unsigned char value
//Returns: None
void USART_Send(unsigned char key, unsigned char sendMe)
{
    while( !(UCSRA & (1 << UDRE)) );
    UDR = key;
    while( !(UCSRA & (1 << UDRE)) );
    UDR = sendMe;
}
////////////////////////////////////////////////////////////////////////////////
// **** WARNING: THIS FUNCTION BLOCKS MULTI-TASKING; USE WITH CAUTION!!! ****
//Functionality - receives an 8-bit char value
//Parameter: None
//Returns: Unsigned char data from the receive buffer
unsigned char USART_Receive()
{
    while ( !(UCSRA & (1 << RXC)) ); // Wait for data to be received
    return UDR; // Get and return received data from buffer
}