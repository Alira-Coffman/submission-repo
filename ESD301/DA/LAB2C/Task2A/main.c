/*
 * assignment2c-2a.c
 *
 * Created: 10/9/2019 7:45:06 PM
 * Author : alira
 ** Design a delay subroutine to generate a waveform on PORTB.3 with 40% DC and 0.625 sec period  using TIMER0_OVF_vect interrupt mechanism in
 normal mode.
 * Need4*10^6 cycles with a prescailer of 1024 ( 4X10^6 / 1024 = 3906.25 / 255 = 15.32) On
 * Need6 * 10^ 6 cycles (6X10^6 / 1024 = 5859.38 / 255 = 22.98)
 */

#define F_CPU 16000000UL
#include<avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
int main(void)
{
	DDRB |= (1 << 2); //PB1 as output
	TCNT0 = 52; // initial value
	//enable interrupts
	TCCR0B |= (1 << CS02) | (1 << CS00);
	TIMSK0 |= (1 << TOIE0);
	sei();
	// set prescaler to 64 and start the timer
	while (1)
	{
		//main loop
	}
	
}

ISR (TIMER0_OVF_vect) // timer0 overflow interrupt
{
	int count = 0;
	int off = 0;
	while(off != 1)
		{
			if(TCNT0 == 0xff)
			{
				count++;
				TCNT0 = 0x00;		// reset timer
			}
			if(count == 22)
			{
				TCNT0 = 12;
				PORTB ^= (1 << 2);
				off = 1;
			}
			
		}
	
}
