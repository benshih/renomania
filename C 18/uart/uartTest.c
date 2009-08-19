// Can be fitted for other microcontrollers by modifying the
// oscillator configurations to fit the baud rate settings.


#include <p18f25k20.h>
#include <stdio.h>
#include <stdlib.h>

#pragma config FOSC = INTIO67, LVP = OFF

void SendUSART(char *c)
{
	int n, m;

	for(n = 0; n < strlen(c);n++)	//steps through char array
	{				//until null character is reached
		for(m=0;m<1000;m++);	//delay transmitting
		TXREG = c[n];
	}
}

void main(void)
{	
	char str[10];		//allocate 10 bytes (9 chars, 1 null character)
	int n=0;
	int m = 0;

	OSCCONbits.IRCF0 = 1;	//16MHz internal oscillator
	OSCCONbits.IRCF1 = 1;
	OSCCONbits.IRCF2 = 1;

	//PORTAbits.RA7 = 1;

	SPBRG = 25;				//Baud Rate 9600 for 16MHz
	TRISC = TRISC | 0xC0;	//RX & TX TRIS controls to 1
	TXSTAbits.SYNC = 0;		//asynchronous operation
	RCSTAbits.SPEN = 1;		//TX/CK I/O pins as output
	TXSTAbits.TX9 = 0;		//8-bit transmission
	BAUDCONbits.CKTXP = 0;	//no data polarity
	TXSTAbits.TXEN = 1;		//enables transmitter circuitry
		
	TRISA = 0x00;			//porta is output
	PORTA = 0XFF;			//turn on indicator lamp
	sprintf(str, "abcdef%d",800);	
	SendUSART(str);

	while(1);			//end program
}


