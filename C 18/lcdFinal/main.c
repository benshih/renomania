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
	SPBRG = 25;				//Baud Rate 9600 for 4MHz
	TRISC = TRISC | 0xC0;	//RX & TX TRIS controls to 1
	TXSTAbits.SYNC = 0;		//asynchronous operation
	RCSTAbits.SPEN = 1;		//TX/CK I/O pins as output
	TXSTAbits.TX9 = 0;		//8-bit transmission
	BAUDCONbits.CKTXP = 0;	//no data polarity
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
	char str[25];
	int i = 0; int j = 0;

	OSCCONbits.IRCF0 = 1;	//4MHz internal oscillator
	OSCCONbits.IRCF1 = 0;	//T = 1ms 
	OSCCONbits.IRCF2 = 1;

	TRISA = 0x00;			//PORTA output
	TRISB = 0x00;			//PORTB output
	TRISC = 0x00;			//PORTC output
	
	PORTA = 0;
	PORTB = 0;
	PORTC = 0;
	
//	InitializeUART();
	InitializeLCD();
	
	sprintf(str,"0123456789abcdef");
	while(1){

		ClearLCD();
		SetLine1();
		WriteLCD(str);
		Delay10KTCYx(20);		
		ClearLCD();
		SetLine2();
		WriteLCD(str);
		Delay10KTCYx(20);
	}	
	
	while( 1 );
}

