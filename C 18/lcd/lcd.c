#include <p18f25k20.h>
#include <delays.h>

#pragma config FOSC = INTIO67, LVP = OFF

void main(void)
{
	OSCCONbits.IRCF0 = 1;	//4MHz internal oscillator
	OSCCONbits.IRCF1 = 0;	//T = 1ms 
	OSCCONbits.IRCF2 = 1;

	TRISA = 0x00;	//PORTA output
	TRISB = 0x00;	//PORTB output

	PORTB = 0x00;
	PORTAbits.RA7 = 1;		//RA7 on
	PORTAbits.RA4 = 1;
	PORTAbits.RA3 = 0;
	Delay1KTCYx(20);		//20ms

	PORTB=0x38;				//Function set
	PORTAbits.RA0 = 0;
	PORTAbits.RA1 = 0;
	PORTAbits.RA2 = 1;
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
	PORTAbits.RA0 = 0;
	PORTAbits.RA1 = 0;
	PORTAbits.RA2 = 1;
	PORTAbits.RA2 = 0;
	PORTB = 0x00;
	Delay1KTCYx(20);		//100ms

	PORTB=0x04;				//ENTRY MODE SET
	PORTAbits.RA0 = 0;
	PORTAbits.RA1 = 0;
	PORTAbits.RA2 = 1;
	PORTAbits.RA2 = 0;
	PORTB = 0x00;
	Delay1KTCYx(20);		//100ms


	PORTB=0x0F;				//DISPLAY ON
	PORTAbits.RA0 = 0;
	PORTAbits.RA1 = 0;
	PORTAbits.RA2 = 1;
	PORTAbits.RA2 = 0;
	PORTB = 0x00;

	Delay1KTCYx(250);		//1s
	Delay1KTCYx(250);		
	Delay1KTCYx(250);		
	Delay1KTCYx(250);		

	//PORTAbits.RA7 = 0;

	PORTB= 0x44;				//DISPLAY w
	PORTAbits.RA3 = 1;
	PORTAbits.RA1 = 0;
	PORTAbits.RA2 = 1;
	PORTAbits.RA2 = 0;
	PORTB = 0x00;

//	PORTB=0x0F;				//DISPLAY ON
//	PORTAbits.RA0 = 0;
//	PORTAbits.RA1 = 0;
//	PORTAbits.RA2 = 1;
//	PORTAbits.RA2 = 0;
//	PORTB = 0x00;

	while(1);
}

