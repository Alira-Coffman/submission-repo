/*
 * assignment2c-task1a.c
 *
 * Created: 10/10/2019 5:32:11 PM
 * Author : alira
 * Design a delay subroutine to generate a waveform on PORTB.3 with 40% DC and 0.625 sec period using TIMER0_COMPA_vect interrupt mechanism
in CTC mode.
 * Need4*10^6 cycles with a prescailer of 1024 ( 4X10^6 / 1024 = 3906.25 / 255 = 15.32) On
 * Need6 * 10^ 6 cycles (6X10^6 / 1024 = 5859.38 / 255 = 22.98)
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	int on = 0;
	int off = 0;
	int count = 0;
	DDRB |= (1<<2); // portb.2 is ouptut 
	PORTB |= (1<<2); //turn LED OFF.
	TCCR0A = 0x00; 
	TCNT0 = 0x00; //start timer..
	TCCR0B |= (1 << CS02) | (1 << CS00);
	//start Timer
	
	//set prescaler 
	
    /* Replace with your application code */
    while (1) 
    {	on = 0;
		off = 0;
		
		//LED On for 40%
		while(on != 1)
		{
			if(TCNT0 == 0xff)
			{
				count++;
				TCNT0 = 0x00;		// reset timer
			}
			if(count == 15)
			{
				TCCR0A = 0x00; // Turn off timer1,
				PORTB^= (1 << 2); //turn off led
				on = 1;
			}
			
			
		}
		//LED off for 60%
		TCNT0 = 0x00; //Timer reset Every time :D
			TCCR0A = 0x00;
			TCNT0 = 0x00; //start timer..
			TCCR0B |= (1 << CS02) | (1 << CS00);
			count = 0;
		while(off != 1)
		{
			if(TCNT0 == 0xff)
			{
				count++;
				TCNT0 = 0x00;		// reset timer
			}
			if(count == 22)
			{
				TCCR0A = 0x00; // Turn off timer1,
				 PORTB &= ~(1 << 2); // turn on
				off = 1;
			}
			
		}
		count = 0;
		
		TCNT0 = 0x00; //Timer reset Every time :D
    }
	
}
