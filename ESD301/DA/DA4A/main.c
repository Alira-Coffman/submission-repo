/*
 * DA4A.c
 *
 * Created: 11/27/2019 12:58:34 PM
 * Author : Alira
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)
/*GLOBAL VARIABLES */
volatile uint8_t Direction = 0;
volatile int readValue;
volatile float adc_in = 0;
char buff[5];

ISR(ADC_vect);
ISR(PCINT1_vect);
/*ADC FUNCTIONS*/
void adc_init(void)
{
	ADMUX = (0<<REFS1)|    // Reference Selection Bits
	(1<<REFS0)|
	(0 << ADLAR);
	
	ADCSRA = (1<<ADEN)|
	(1<<ADATE)|
	(1<<ADIE)|
	(1<<ADPS2)|
	(1<<ADPS1)|
	(1<<ADPS0);

	ADCSRB = (1 << ADTS1) | (1 << ADTS0);
}
uint16_t clearADCChannel(uint8_t channel){
	ADMUX = (ADMUX & 0xF0)|(channel & 0x0F); //clear channel
}
void ADCEnableDisable(int on)
{
	if(on)
	ADCSRA |=(1<<ADSC);
	else
	{
		ADCSRA &= ~((1<<ADEN)|(1<<ADIE));
	}
	
}
//MIAN PORTS
void InitPort(){
	
	DDRC  |= (0<<PINC0);
	PORTC |= (0<<PINC0);
	DIDR0 |= (1<<ADC0D);
	PORTC |= (1<<PINC1);
	PCICR |= (1<<PCIE1);
	PCMSK1 |= (1<<PCINT9);
	DDRB |= (1<<PINB1)|(1<<PINB2);
	PORTB &= ~(1<<PINB2);
	
}

/*TIMER FUNCTIONS*/
void TimersInit()
{
	/*initalize timer 0 for */
	// Set Initial Timer value
	TCNT0 = 0;
	// Place TOP timer value to Output compare register
	OCR0A = 60;
	// Set CTC mode
	//  and make toggle PD6/OC0A pin on compare match
	TCCR0A |= (1<<COM0A0)|(1<<WGM01);

	/*TIMER 0 INITALIZED*/

	/*TIMER 1 */
	TCCR1A |=(1<<COM1A1)|(1<<COM1A0)|(1<<COM1B0)|(1<<COM1B1)|(1<<WGM10);
	/*Timer 1*/
}
void startTimer1()
{
	
		TCCR1B |= (1<<CS12)|(1<<CS10);
	
	
}
void startTimer2()
{
TCCR0B |= (1<<CS01);
}
void ToggleTimer1(void){
	TCCR1B ^= (1<<CS12);
	TCCR1B ^= (1<<CS10);
}
void stopTimer0()
{
	TCCR0B &= ~(1<<CS01);
	TIMSK0 &= ~(1<<OCIE0A);
}

int main()
{

	InitPort();

	adc_init();
	clearADCChannel(0);
	TimersInit();
	startTimer1();
	startTimer2();

	ADCEnableDisable(1);

	sei();
	OCR1AH = 0x00;

	while(1)
	{
		readValue = (adc_in/1023)*100;
		readValue *=2;
		readValue*= (0xFA);
		OCR1A = (readValue+10);
	}

}

ISR(ADC_vect){
	volatile uint8_t ADC_in_L = 0;
	volatile uint8_t ADC_in_H = 0;
	// clear timer compare match flag
	TIFR0 = (1<<OCF0A);
	// Toggle pin PD2 to track the end of ADC conversion
	PIND = (1<<PIND2);
	ADC_in_L = ADCL;
	ADC_in_H = ADCH;
	adc_in = ((ADC_in_H << 8)|(ADC_in_L));
}

ISR(PCINT1_vect){
	if(!(PINC & (1 << PINC1))){
		OCR1A = 0;
		DDRB^= (1<<PINB1);
	}
}

