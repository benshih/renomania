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

void WriteLCD(char *c)
{
	int i = 0;
	PORTAbits.RA7 = 1;
	PORTAbits.RA1 = 0;
	for( i = 0; i < strlen(c); i++ )
	{
		PORTB = c[i];
		PORTAbits.RA2 = 1;
		PORTAbits.RA2 = 0;
	}
}

void CommandLCD(char c)
{
	PORTB=c;				//Line2
	PORTAbits.RA7 = 0;		//
	PORTAbits.RA1 = 0;
	PORTAbits.RA2 = 1;
	PORTAbits.RA2 = 0;
	PORTB = 0x00;
}
void SetLine1()
{
	CommandLCD(0x80);
}

void SetLine2()
{
	CommandLCD(0xC0);
}

void ClearLCD()
{
	CommandLCD(0x01);
}

void main(void)
{
	char str[16];
	int i = 0; int j = 0;

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

	//Function set
	CommandLCD(0x38);		//8-bit Data Line, 2-lines, 5x8 dots
	Delay1KTCYx(20);		//20ms

	ClearLCD();
	Delay1KTCYx(20);		//20ms

	//Entry Mode Set
	CommandLCD(0x06);		//Cursor right, Shift OFF
	Delay1KTCYx(20);		//20ms

	Delay10KTCYx(100);		//1s		
	CommandLCD(0x0C);		//Display ON, Cursor OFF
	
	for( j = 300; j >= 0; j-- )
	{
		SetLine2();

		sprintf(str,"%4d",j);
		WriteLCD(str);
		Delay1KTCYx(1);	
		SendUART(str);
	}
	while( 1 );
}
