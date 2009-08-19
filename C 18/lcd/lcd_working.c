#include <p18f25k20.h>
#include <delays.h>
#include <stdio.h>
#include <stdlib.h>

#pragma config FOSC = INTIO67, LVP = OFF

void SendUSART(char *c)
{
	char temp;
	int i = 0;

	do
	{
		temp = c[i++];
		TXREG = temp;
		Delay1KTCYx(5);
	} while( c[i] != '\0' );
}

void SendUSARTchar(char c)
{
		TXREG = c;
		Delay1KTCYx(5);
}

void main(void)
{
	char str[8];
	int i = 0; int j = 0;
	str[0] = 'a'; str[1] = 'b'; str[2] = 'c'; str[3] = 'd';
    str[4] = 'e'; str[5] = 'f'; str[6] = 'g'; str[7] = 'h';

	OSCCONbits.IRCF0 = 1;	//4MHz internal oscillator
	OSCCONbits.IRCF1 = 0;	//T = 1ms 
	OSCCONbits.IRCF2 = 1;

	TRISA = 0x00;	//PORTA output
	TRISB = 0x00;	//PORTB output

	SPBRG = 25;				//Baud Rate 9600 for 16MHz
	TRISC = TRISC | 0xC0;	//RX & TX TRIS controls to 1
	TXSTAbits.SYNC = 0;		//asynchronous operation
	RCSTAbits.SPEN = 1;		//TX/CK I/O pins as output
	TXSTAbits.TX9 = 0;		//8-bit transmission
	BAUDCONbits.CKTXP = 0;	//no data polarity
	TXSTAbits.TXEN = 1;		//enables transmitter circuitry

    //PORTAbits.RA3 = 1;

	PORTB = 0x00;
	PORTAbits.RA7 = 1;		//RA7 on
	PORTAbits.RA4 = 1;
	PORTAbits.RA3 = 0;
	Delay1KTCYx(20);		//20ms

	PORTB=0x38;				//Function set
	PORTAbits.RA7 = 0; // RA0 - RS
	PORTAbits.RA1 = 0; // RA1 - R/W
	PORTAbits.RA2 = 1; // RA2 - E
	PORTAbits.RA2 = 0;
	PORTB = 0x00;

	Delay1KTCYx(20);		//100ms

//	PORTB=0x08;				//DISPLAY OFF
//	PORTAbits.RA0 = 0;
//	PORTAbits.RA1 = 0;
//	PORTAbits.RA2 = 1;
//	PORTAbits.RA2 = 0;
//	PORTB = 0x00;
//
	PORTB=0x01;				//CLEAR DISPLAY
	PORTAbits.RA7 = 0;
	PORTAbits.RA1 = 0;
	PORTAbits.RA2 = 1;
	PORTAbits.RA2 = 0;
	PORTB = 0x00;
	Delay1KTCYx(20);		//100ms

	PORTB=0x06;				//ENTRY MODE SET
	PORTAbits.RA7 = 0;
	PORTAbits.RA1 = 0;
	PORTAbits.RA2 = 1;
	PORTAbits.RA2 = 0;
	PORTB = 0x00;
	Delay1KTCYx(20);		//100ms


	PORTB=0x0F;				//DISPLAY ON
	PORTAbits.RA7 = 0;
	PORTAbits.RA1 = 0;
	PORTAbits.RA2 = 1;
	PORTAbits.RA2 = 0;
	PORTB = 0x00;

	Delay1KTCYx(250);		//1s
	Delay1KTCYx(250);		
	Delay1KTCYx(250);		
	Delay1KTCYx(250);		

	//PORTAbits.RA7 = 0;
/*
	PORTB=0x38;				//Function set
	PORTAbits.RA7 = 0; // RA0 - RS
	PORTAbits.RA1 = 0; // RA1 - R/W
	PORTAbits.RA2 = 1; // RA2 - E
	PORTAbits.RA2 = 0;
	PORTB = 0x00;



	PORTB=0x0F;				//DISPLAY ON
	PORTAbits.RA7 = 0;
	PORTAbits.RA1 = 0;
	PORTAbits.RA2 = 1;
	PORTAbits.RA2 = 0;
	PORTB = 0x00;

	PORTB=0x06;				//Set increment/decrement off
	PORTAbits.RA7 = 0;
	PORTAbits.RA1 = 0;
	PORTAbits.RA2 = 1;
	PORTAbits.RA2 = 0;
	PORTB = 0x00;

	PORTB=0x02;				// return to home
	PORTAbits.RA7 = 0;
	PORTAbits.RA1 = 0;
	PORTAbits.RA2 = 1;
	PORTAbits.RA2 = 0;
	PORTB = 0x00;
*/
    Delay1KTCYx(500);
Delay1KTCYx(500);
Delay1KTCYx(500);Delay1KTCYx(500);

    PORTAbits.RA7 = 1;
	PORTAbits.RA1 = 0;

	for( j = 0; j < 0xFF; j++ ) {
	PORTAbits.RA7 = 1;
	for( i = 0; i < 8; i++ )
	{
	PORTB = str[i];				//DISPLAY w

	PORTAbits.RA2 = 1;
	PORTAbits.RA2 = 0;
	Delay1KTCYx(500);

	//sprintf(str, "%c",str[i]);	
	SendUSARTchar(str[i]);
	}
	//PORTAbits.RA7 = 0;
	Delay1KTCYx(500);
	Delay1KTCYx(500);
	Delay1KTCYx(500);
	Delay1KTCYx(500);
	}

//	PORTB=0x0F;				//DISPLAY ON
//	PORTAbits.RA0 = 0;
//	PORTAbits.RA1 = 0;
//	PORTAbits.RA2 = 1;
//	PORTAbits.RA2 = 0;
//	PORTB = 0x00;

	while( 1 );
}

