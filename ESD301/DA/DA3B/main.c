
#define BAUD 9600
#define F_CPU 16000000UL
#define BAUD_PRESCALLER (((F_CPU / (BAUD * 16UL))) - 1)
#include <time.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <util/setbaud.h>
#include <avr/interrupt.h>
#include <stdlib.h>


/****FUNCTION DECLARATIONS***/
void USART_putstring(char* StringPtr);
void USART_init(void);
void timer_init();
void read_temp();
void adc_init (void); //function to initialize ADC


/*****GLOABALS*****/
volatile float my_temp; // temp float
char tempString[15]; // temp string for conversion
int timerCheck = 0;
volatile float tempC;
volatile float tempF;

/*SENTENCES*/
char letConnect[] = "Lets get connected.... ";
char connect[] = "CONNECTED! :D";
char dots[] = "..... ";
char tempSent[] = "Temp: ";
char Space[] = "\n";


int main()
{
	
	
	USART_putstring(letConnect);
	USART_putstring(dots);
	USART_putstring(Space);
	USART_putstring(dots);
	USART_putstring(Space);
	USART_putstring(connect);
	_delay_ms(100);
	USART_init();
	adc_init();
	while(1)
	{
		_delay_ms(1000);
		//read_temp();
	
	snprintf(tempString, sizeof(tempString), "%f\r\n", my_temp); 
		USART_putstring(tempString);
		
	}
}


void USART_init( void )
{
	UBRR0H = 0;
	UBRR0L = F_CPU/16/BAUD - 1; // Used for the BAUD prescaler
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
	TCCR1B |= 5; //(1 << CS12) | (1 << CS10); // Sets prescaler to 1024
	TIMSK1 = (1 << TOIE1); // Enables overflow flag
	TCNT1 = 49911; // 1 second delay = (0xFFFF) - TCNT = 65535 - 15624 = 49911
	sei();
}
void USART_putstring(char* StringPtr)
{
	while ((*StringPtr != '\0')){
		while (!(UCSR0A & (1 << UDRE0))); 
		UDR0 = *StringPtr;
		StringPtr++;
	}
}

void timer_init()
{
	TCCR0A = 0;					//NORMAL MODE OPERATION
	TCCR0B  = 0X05;				//THE PRESCALER SET TO 1024
	TCNT0 = 0X00;				//COUNTER VALUE = 0
	TIMSK0 = (1<<TOIE0);		//ENABLE INTERRUPT
	sei();						//ENABLE GLOBAL INTERRUPT
}

void adc_init()
{
	ADMUX =  (0<<REFS1) | //REFERENCE SELECTION BITS
	(1<<REFS0) | 
	(0<<ADLAR) | 
	(1<<MUX2)  | 
	(0<<MUX1)  | 
	(0<<MUX0);
	
	ADCSRA = (1<<ADEN)  | //ADC ENABLE
	(0<<ADSC)  | 
	(0<<ADATE) | 
	(0<<ADIF)  | 
	(0<<ADIE)  | 
	(1<<ADPS2) | 
	(1<<ADPS1) | 
	(1<<ADPS0);
}

void read_temp()
{
	unsigned char i = 10;	
	//adc_temp = 0;			
	while(i--)
	{
		ADCSRA |= (1<<ADSC);	   
		while(ADCSRA & (1<<ADSC)); 
		my_temp += ADC;           
	}
	my_temp = my_temp/10;		   //Average of LM35 values 
    my_temp = (my_temp*1.8) +32; //farenheit :D
	
	
}
ISR (TIMER1_OVF_vect)
{
	//USART_putstring("Temp: ");
	timerCheck++;
	read_temp();
	//_delay_ms(1000); //delay 1 second :D
	

	
}