#include <p18f25k20.h>
#include "RenLCD.h"

#pragma config FOSC = INTIO67, LVP = OFF

//*****************************
//		FUNCTION PROTOTYPES
//*****************************

void InitializeTimers (void);
void InitializeUART(void);
void SendUART(char*);
void SendUARTchar(char);

//*****************************
//		FUNCTIONS
//*****************************

void InitializeTimers()
{
	OpenTimer0(	TIMER_INT_OFF &
				T0_16BIT &
				T0_SOURCE_INT &
				T0_PS_1_1);	
//	CloseTimer2();
}	
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
	int i = 0; int j = 0; int k = 0;
	unsigned char ir[16];
	int highTimes[6];
	int lowTime;

	OSCCONbits.IRCF0 = 1;	//4MHz internal oscillator
	OSCCONbits.IRCF1 = 0;	//T = 1ms 
	OSCCONbits.IRCF2 = 1;
	
	TRISA = 0x80;
	TRISB = 0x00;
	TRISC = 0x00;

	InitializeTimers();	
	InitializeLCD();
	
	ClearLCD();
	SetLine1();
	sprintf(str,"Ready");
	WriteLCD(str);
	SetLine2();
	sprintf(str,"Press Remote");
	WriteLCD(str);
	while(1)
	{
		while(PORTAbits.RA7 == 0);
		for(i=0;i<4;i++)
		{
			while(PORTAbits.RA7 == 1);
			WriteTimer0(0);
			while(PORTAbits.RA7 == 0);
			highTimes[i] = ReadTimer0();
		}
		WriteTimer0(0);
		while(PORTAbits.RA7 == 1);
		lowTime = ReadTimer0();
		
		ClearLCD();
		sprintf(str,"H: %d,%d,%d",highTimes[0], highTimes[1], highTimes[2]);
		WriteLCD(str);
		SetLine2();
		sprintf(str,"L: %d", lowTime);
		WriteLCD(str);
	}
}	
		


