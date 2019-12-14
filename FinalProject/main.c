#ifndef F_CPU
#define F_CPU 16000000UL
#endif

// Global constants for uart
#define BAUD 115200
#define FOSC 16000000
#define UBRR FOSC/8/BAUD-1

#define APDS9960_WRITE 0x72
#define APDS9960_READ 0x73

//include standard libraries
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include <math.h>


//include custom libraries
#include "APDS9960_def.h"
#include "i2c_master.h"


//Function declarations
void getValues(void);
// void TIMER1_init();
void init_APDS9960(void);
void usart_init();
void USART_putstring(volatile unsigned char *StringPtr);


//string for colors
volatile unsigned char Red[10];
volatile unsigned char Green[10];
volatile unsigned char Blue[10];
void ESP_init()
{
	
	USART_putstring("AT+RST\r\n");
	_delay_ms(20);
	USART_putstring("AT\r\n");
	_delay_ms(20);
	USART_putstring("AT+CWMODE=3\r\n");
	_delay_ms(20);
	USART_putstring("AT+CWJAP=\"Alira\",\"Password\"\r\n");
	_delay_ms(20);

	
}
void ESP_connect()
{
		printf("Connecting...\n");
		_delay_ms(10);
		// Enable connection
		USART_putstring("AT+CIPMUX=0\r\n");
		_delay_ms(50);
		USART_putstring("AT+CIPSTART=\"TCP\",\"184.106.153.149\",80\r\n");
		_delay_ms(50);
		USART_putstring("AT+CIPSEND=104\r\n");
		_delay_ms(50);
}
void ESP_SendVals()
{
		printf("Sending...Please Hold\n");
		_delay_ms(10);
		USART_putstring("GET /update?api_key=RWTD9OCIEJ1UQ8JE&field1=");	// Connect to proper key
		USART_putstring(Red);		// Send adc data
		USART_putstring("&field2=");
		USART_putstring(Green);		// Send adc data
		USART_putstring("&field3=");
		USART_putstring(Blue);		// Send adc data
}
//AT commands

volatile unsigned char RESET[] = "AT+RST\r\n"; // Get AT Firmware info
volatile unsigned char LINEBREAK[] = "\r\n"; // end of temperature transmission
volatile unsigned char CLOSE[] = "AT+CIPCLOSE\r\n";


uint16_t redVal, greenVal, blueVal;


int main(void){
	I2C_Init();
	usart_init(9600);
	init_APDS9960();

	ESP_init();




	while(1){
		
		
		ESP_connect();
		
		getValues();
		ESP_SendVals();

	}
	
	return 0;
}


void init_APDS9960(void){
	uint8_t setup;
	
	I2C_ReadReg(APDS9960_WRITE, APDS9960_ID, &setup,1);
	if(setup != APDS9960_ID_1) while(1);
	setup = 1 << 1 | 1<<0 | 1<<3 | 1<<4;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_ENABLE, &setup, 1);
	setup = DEFAULT_ATIME;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_ATIME, &setup, 1);
	setup = DEFAULT_WTIME;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_WTIME, &setup, 1);
	setup = DEFAULT_PROX_PPULSE;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_PPULSE, &setup, 1);
	setup = DEFAULT_POFFSET_UR;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_POFFSET_UR, &setup, 1);
	setup = DEFAULT_POFFSET_DL;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_POFFSET_DL, &setup, 1);
	setup = DEFAULT_CONFIG1;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_CONFIG1, &setup, 1);
	setup = DEFAULT_PERS;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_PERS, &setup, 1);
	setup = DEFAULT_CONFIG2;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_CONFIG2, &setup, 1);
	setup = DEFAULT_CONFIG3;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_CONFIG3, &setup, 1);
	
}
 

void getValues(void){
	
	
		I2C_Start((APDS9960_WRITE);
		I2C_Write(APDS9960_RDATAH); // set pointer
		I2C_Stop();
		I2C_Start((APDS9960_READ);
		Red = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
		I2C_Stop();

		I2C_Start((APDS9960_WRITE);
		I2C_Write(APDS9960_GDATAH); // set pointer
		I2C_Stop();
		I2C_Start((APDS9960_READ);
		Green = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
		I2C_Stop();

		I2C_Start((APDS9960_WRITE);
		I2C_Write(APDS9960_BDATAH); // set pointer
		I2C_Stop();
		I2C_Start((APDS9960_READ);
		Blue = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
		I2C_Stop();

	
	itoa(redVal, dummy, 10); //convert char to ascii
	for(i = 0 ; i < 10 ; i++){
		Red[i] = dummy[i]; //move converted ascii
	}	
	itoa(greenVal, dummy, 10); //convert char to ascii
	for(i = 0 ; i < 10 ; i++){
		Green[i] = dummy[i]; //move converted ascii
	}	
	itoa(blueVal, dummy, 10); //convert char to ascii
	for(i = 0 ; i < 10 ; i++){
		Blue[i] = dummy[i]; //move converted ascii
	}
	
	
}

void usart_init() {
	UBRR0H = ((UBRR) >> 8);
	UBRR0L = UBRR;
	UCSR0A |= (1<< U2X0); // divisor baud = 8
	UCSR0B |= (1 << TXEN0); // Enable transmission
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // 8 bits
}

void USART_putstring(volatile unsigned char *StringPtr)
{
	while ((*StringPtr != '\0')){ 
		while (!(UCSR0A & (1 << UDRE0))); 
		UDR0 = *StringPtr; 
		StringPtr++; 
	}
}