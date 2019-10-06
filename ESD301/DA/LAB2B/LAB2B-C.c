#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
	DDRB |= (1<<2); //sets portb.2
	PORTB |= (1<<2); //turn LED OFF. 
	//set inputs portc.3
	DDRC &=(0<<3);
	PORTC |= (1<<3); 
	while(1)
	{
		//PORTB |= (1<<2);
		if(!(PINC & (1<<PINC3)))
		{
			
			EICRA = 0x2;
			EIMSK =(1<<INT0);
			sei();
		}
		else
			PORTB |= (1<<2);
	}
	//connect switch to PORTC.3
	
	//when high turn on pull up
	
	//turn on led at PORTB.2 for 1.33seconds
}
ISR(INT0_vect)
{
	
	
	PORTB &= ~(1<<2); //sets to 0
	_delay_ms(1300);
}