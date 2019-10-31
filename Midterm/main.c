#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#define BAUDRATE 115200
#define BAUD_PRESCALLER (int)round(((((double)F_CPU / ((double)BAUDRATE * 8.0))) - 1.0))

void USART_init(void); 
void USART_transmit(unsigned char data); 
void USART_putstring(char* StringPtr); 
void ADC_init (void); //function to initialize ADC
void ESP_init();
void UART_sendString(volatile unsigned char AT[]);
float readTemperatue();


//VARIABLES
volatile float myTemp; //holds temp
char * tempString[10];
volatile unsigned char AT[];

void UART_sendString(volatile unsigned char AT[])
{
	volatile unsigned char len = 0;
	volatile unsigned char i;
	while(AT[len] != 0)
	{
		len++;
	}
	for(i = 0x00; i < len; i++){
		// Wait for the transmitter to finish
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = AT[i];
	}
}
int main(void)
{
	USART_init(); 
	ADC_init (); 
	ESP_init();
	
	TCCR0A = 0x00; 
	TCCR0B |= (1 << CS02); 
	TCNT0 = 6;
	TIMSK0 |= (1 << TOIE0);
	sei(); 
	
	while (1)
	{
		
	}
}
/*INITALIZE FUNCTIONS*/
void USART_init(void)
{
	UCSR0A = (1 << U2X0);
	UBRR0H = (uint8_t)(BAUD_PRESCALLER >> 8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (3 << UCSZ00);
}
void ESP_init()
{
	UART_sendString("AT\r\n");
	

	USART_putstring("AT+CWMODE=1\r\n");
	_delay_ms(20);
	
	USART_putstring("AT+CWJAP=\"Alira\",\"Password\"\r\n");
	_delay_ms(20);
	// Enable connection
	UART_sendString("AT+CIPMUX=0\r\n");
	_delay_ms(50);
	
}

void ADC_init (void)
{
	ADMUX = (0<<REFS1)|
	(1<<REFS0)|
	(0<<ADLAR)|
	(1<<MUX2)|
	(0<<MUX1)|	// ADC5 PC4
	(0<<MUX0);
	ADCSRA = (1<<ADEN)|
	(0<<ADSC)|
	(0<<ADATE)|
	(0<<ADIF)|
	(0<<ADIE)|
	(0<<ADPS2)|	// ADC Prescaler
	(1<<ADPS1)|
	(1<<ADPS0);
	
}
/*END OF INITS*/
void USART_transmit(unsigned char data)
{
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}
float readTemperatue()
{
	
		ADCSRA |= (1<<ADSC);
		if(ADCSRA & (1<<ADSC));
		myTemp = ADC;
		_delay_ms(50);
	
	
}
void USART_putstring(char *StringPtr)
{
	while ((*StringPtr != '\0')){ 
		while (!(UCSR0A & (1 << UDRE0))); 
		UDR0 = *StringPtr; 
		StringPtr++; 
	}
}
ISR(TIMER0_COMPA_vect)
{
	 _delay_ms(10);
		UART_sendString("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
		_delay_ms(20);
		UART_sendString("AT+CIPSEND=45\r\n");
		_delay_ms(20);
	
		UART_sendString("GET /update?key=IF6XQMA7HD4SUDOL&field1=");
		
		readTemperatue();
		snprintf(tempString, sizeof(tempString), "%f\r\n", myTemp); 
		UART_sendString(tempString);
		UART_sendString("\r\n\r\n");
3.	}
