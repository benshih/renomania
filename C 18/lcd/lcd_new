#include <p18f25k20.h>
#include <delays.h>
#include <stdio.h>
#include <stdlib.h>

#pragma config FOSC = INTIO67, LVP = OFF

void SendUART(char *c)
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

void SendUARTchar(char c)
{
		TXREG = c;
		Delay1KTCYx(5);
}

void main(void)
{
	char str[16];
	int i = 0; int j = 0;
	str[0] = 't'; str[1] = 'e'; str[2] = 's'; str[3] = 't';
    str[4] = 's'; str[5] = 't'; str[6] = 'r'; str[7] = ' ';

	OSCCONbits.IRCF0 = 1;	//4MHz internal oscillator
	OSCCONbits.IRCF1 = 0;	//T = 1ms 
	OSCCONbits.IRCF2 = 1;

	TRISA = 0x00;			//PORTA output
	TRISB = 0x00;			//PORTB output

	SPBRG = 25;				//Baud Rate 9600 for 16MHz
	TRISC = TRISC | 0xC0;	//RX & TX TRIS controls to 1
	TXSTAbits.SYNC = 0;		//asynchronous operation
	RCSTAbits.SPEN = 1;		//TX/CK I/O pins as output
	TXSTAbits.TX9 = 0;		//8-bit transmission
	BAUDCONbits.CKTXP = 0;	//no data polarity
	TXSTAbits.TXEN = 1;		//enables transmitter circuitry

	Delay1KTCYx(20);		//20ms
	PORTB = 0x00;
	PORTAbits.RA7 = 1;		//Write on
	PORTAbits.RA4 = 1;
	PORTAbits.RA3 = 0;
	Delay1KTCYx(20);		//20ms

	PORTB=0x38;				//Function set
	PORTAbits.RA7 = 0; 		// RA7 - RS
	PORTAbits.RA1 = 0; 		// RA1 - R/W
	PORTAbits.RA2 = 1; 		// RA2 - E
	PORTAbits.RA2 = 0;
	PORTB = 0x00;
	Delay1KTCYx(20);		//20ms

	PORTB=0x01;				//CLEAR DISPLAY
	PORTAbits.RA7 = 0;
	PORTAbits.RA1 = 0;
	PORTAbits.RA2 = 1;
	PORTAbits.RA2 = 0;
	PORTB = 0x00;
	Delay1KTCYx(20);		//20ms

	PORTB=0x06;				//ENTRY MODE SET
	PORTAbits.RA7 = 0;
	PORTAbits.RA1 = 0;
	PORTAbits.RA2 = 1;
	PORTAbits.RA2 = 0;
	PORTB = 0x00;
	Delay1KTCYx(20);		//20ms


	PORTB=0x0C;				//DISPLAY ON
	PORTAbits.RA7 = 0;		//No Cursor
	PORTAbits.RA1 = 0;
	PORTAbits.RA2 = 1;
	PORTAbits.RA2 = 0;
	PORTB = 0x00;

	Delay10KTCYx(100);		//1s		

    PORTAbits.RA7 = 1;
	PORTAbits.RA1 = 0;

	for( j = 1000; j >= 0; j-- )
	{
		PORTAbits.RA7 = 1;
		sprintf(str,"%4d",j);
		for( i = 0; i < strlen(str); i++ )
		{
			PORTB = str[i];

			PORTAbits.RA2 = 1;
			PORTAbits.RA2 = 0;
		}
		Delay1KTCYx(1);

		PORTB=0x02;				// return to home
		PORTAbits.RA7 = 0;
		PORTAbits.RA1 = 0;
		PORTAbits.RA2 = 1;
		PORTAbits.RA2 = 0;
		PORTB = 0x00;	
	
		PORTB=0x01;				//CLEAR DISPLAY
		PORTAbits.RA7 = 0;
		PORTAbits.RA1 = 0;
		PORTAbits.RA2 = 1;
		PORTAbits.RA2 = 0;
		PORTB = 0x00;
	
		SendUART(str);
	}
	while( 1 );
}

