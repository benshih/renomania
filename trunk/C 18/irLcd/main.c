#include <p18f25k20.h>
#include <timers.h>
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

	OSCCONbits.IRCF0 = 1;	//4MHz internal oscillator
	OSCCONbits.IRCF1 = 0;	//T = 1ms 
	OSCCONbits.IRCF2 = 1;

	TRISA = 0x00;			//PORTA output
	TRISB = 0x00;			//PORTB output
	TRISC = TRISC | 0x01;	//RC0 input
	
	InitializeUART();
	InitializeLCD();
	InitializeTimers();
	
	CommandLCD(0x01);
	SetLine2();
	sprintf(str,"Ready");
	WriteLCD(str);
	SendUART(str);

	SetLine1();
	SetLine1();
	PORTAbits.RA6 = 1;
	while(1)
	{
		i = 0;
		while(i<4000)
		{
			while(PORTCbits.RC0 == 1);
			WriteTimer0(0);
			while(PORTCbits.RC0 == 0);
			i = ReadTimer0();
		}
		while(PORTCbits.RC0 == 1);
		for(k=0;k<4;k++)
		{
			for(j=0;j<8;j++)
			{
				while(PORTCbits.RC0 == 0);
				WriteTimer0(0);
				while(PORTCbits.RC0 == 1);	
				i = ReadTimer0();
			
				ir[k] = ir[k] << 1;
				if(i<900)
					ir[k] = ir[k] | 0x01;			
			}
		}
		k--;
		while(k>=0)
		{
			sprintf(str,"%x",ir[3-k]);
			SendUART(str);	
			k--;
		}
	}
}

