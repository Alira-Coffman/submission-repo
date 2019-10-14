/*
 * assignment2c-1b.c
 *
 * Created: 10/9/2019 7:29:57 PM
 * Author : alira
 Connect a switch to PORTC.3 (active high - turn on the pull up transistor) to poll for
 an event to turn on the led at PORTB.2 for 1.333 sec after the event using Timer 0 – normal mode. Count OVF
 occurrence if needed. Do not use interrupts
 
 81. with prescaler of 1024
 */ 

#include <avr/io.h>


int main(void)
{
	int delay;
	DDRB |= (1<<2); // portb.2 is ouptut
	PORTB |= (1<<2); //turn LED OFF.
	//set inputs portc.3
	DDRC &=(0<<3);
	PORTC |= (1<<3);
	
	
    /* Replace with your application code */
    while (1) 
    {
		delay = 0;
		if(!(PINC & (1<<3)))
		{
			PORTB &= ~(1 << 2); //turn on LED 
			int count = 0;
			TCCR0A = 0x00;
			TCNT0 = 0x00; //start timer
			TCCR0B |= (1 << CS02) | (1 << CS00);
			while(delay != 1)
			{
				
				if(TCNT0 == 0xff)
				{
					count++;
					TCNT0 = 0x00;		// reset timer
				}
				if(count == 81)
				{
					TCCR0A = 0x00; // Turn off timer1,
					PORTB |= (1 << 2); //turn off led
					delay = 1;
			
				}
			}
			
			
		}
	}
}