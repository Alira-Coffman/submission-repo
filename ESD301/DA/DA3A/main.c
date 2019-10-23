/*

 */ 
/*UART TestAtmega328 DIP TX PD1 (pin3)Atmega328 DIP RX PD0 (pin2) */
#define BAUD 9600
#define F_CPU 16000000UL
#include <time.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <util/setbaud.h>
#include <avr/interrupt.h>
#include <stdlib.h>

//srand(time(NULL));


void USART_send(char data); 
void USART_putstring(char* StringPtr); 
void USART_init(void); 
int randomValue;
char stringtype[] = "My String: "; 
char inttype[] = "My Favorite Integer is: "; 
char floattype[] = "My Floating Point: "; 
char Space[] = "\n"; 

char String[] = "This is a string that I am printing...."; 
char floating[15]; 
volatile float my_float = 1006.3452; // Sets the float value

int main(void)
{
	USART_init(); // Initializes the analog to digital functions as well as OVF interrupt
	srand(time(NULL));   // Initialization, should only be called once.
	int r = rand();      // Returns a pseudo-random integer between 0 and RAND_MAX.
	char arr[10] = "";
	randomValue = r;

	while(1)
	{
		// main loop
	}
}

ISR (TIMER1_OVF_vect)
{
	USART_putstring(Space); 
	USART_putstring(stringtype); 
	USART_putstring(String); 
	USART_putstring(Space); 
	USART_putstring(inttype); 
	USART_send(randomValue); 
	USART_putstring(Space); 
	USART_putstring(floattype); 
	snprintf(floating, sizeof(floating), "%f\r\n", my_float); 
	USART_putstring(floating); 
	USART_putstring(Space); 
	TCNT1 = 49911; 
}

void USART_init( void )
{
	UBRR0H = 0;
	UBRR0L = F_CPU/16/BAUD - 1; 
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); 
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);   
	TCCR1B |= 5; 
	TIMSK1 = (1 << TOIE1); 
	TCNT1 = 49911; 
	sei();
}

void USART_send(char data)
{
	while (!(UCSR0A & (1 << UDRE0))); 
	UDR0 = data; // UDR0 register grabs the value given from the parameter
	
}

void USART_putstring(char *StringPtr)
{
	while ((*StringPtr != '\0')){ // Until it reaches the end of the line, it will keep looping
		while (!(UCSR0A & (1 << UDRE0))); // Until UDRE0 goes high, it will keep looping
		UDR0 = *StringPtr; // UDR0 register grabs the value given from the parameter
		StringPtr++; // but it does it by every character as shown here
	}
}
