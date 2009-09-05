#include <p18f25k20.h>
#include "RenLCD.h"

#pragma config FOSC = INTIO67, LVP = OFF

//*****************************
//		FUNCTION PROTOTYPES
//*****************************

void InitializeUART(void);
void SendUART(char*);
void SendUARTchar(char);

//*****************************
//		FUNCTIONS
//*****************************

void InitializeUART()
{
	SPBRG = 12;				//Baud Rate 19200 for 4MHz
	TRISC = TRISC | 0xC0;	//RX & TX TRIS controls to 1
	TXSTAbits.SYNC = 0;		//asynchronous operation
	RCSTAbits.SPEN = 1;		//TX/CK I/O pins as output
	TXSTAbits.TX9 = 0;		//8-bit transmission
	BAUDCONbits.CKTXP = 0;	//no data polarity
	BAUDCONbits.BRG16 = 1;	//16-bit Baud Rate Generator
	TXSTAbits.TXEN = 1;		//enables transmitter circuitry
}
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

//*****************************
//		MAIN
//*****************************

void main(void)
{
	char str[16];
	int i = 0; int j = 0;

	OSCCONbits.IRCF0 = 1;	//4MHz internal oscillator
	OSCCONbits.IRCF1 = 0;	//T = 1ms 
	OSCCONbits.IRCF2 = 1;

	TRISA = 0x00;			//PORTA output
	TRISB = 0x00;			//PORTB output
	TRISC = TRISC | 0x01;	//RC0 input
	
	InitializeUART();
	InitializeLCD();
	
	CommandLCD(0x01);
	SetLine2();
	sprintf(str,"Ready");
	WriteLCD(str);
	SendUART(str);

	SetLine1();
	SetLine1();
	PORTAbits.RA6 = 1;
	while( 1 )
	{
		if(PORTCbits.RC0 == 1)
		{
			sprintf(str,"1");
		}
		else if(PORTCbits.RC0 ==0)
		{
			sprintf(str,"0");
		}
		SendUART(str);	
	}
}

