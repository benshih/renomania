#include "RenLCD.h"

void InitializeLCD()
{
	Delay1KTCYx(50);		//50ms
	PORTB = 0x00;

	//Function set
	CommandLCD(0x38);		//8-bit Data Line, 2-lines, 5x8 dots
	Delay1KTCYx(20);		//20ms
	ClearLCD();
	Delay1KTCYx(20);		//20ms
	//Entry Mode Set
	CommandLCD(0x06);		//Cursor right, Shift OFF
	Delay10KTCYx(100);		//1s		
	CommandLCD(0x0C);		//Display ON, Cursor OFF
}

void WriteLCD(char *c)		//Writes Characters to LCD
{
	int i = 0;
	PORTAbits.RA7 = 1;
	PORTAbits.RA1 = 0;
	for( i = 0; c[i] != '\0'/*i < strlen(c)*/; i++ )
	{
		PORTB = c[i];
		PORTAbits.RA2 = 1;
		PORTAbits.RA2 = 0;
		Nop();
		Nop();
	}
}

void CommandLCD(char c)		//Sends Commands to LCD
{
	PORTB=c;				//Line2
	PORTAbits.RA7 = 0;		//
	PORTAbits.RA1 = 0;
	PORTAbits.RA2 = 1;
	PORTAbits.RA2 = 0;
	PORTB = 0x00;
}

void SetLine1()				//Set cursor to beginning of first line
{
	CommandLCD(0x80);
}

void SetLine2()				//Set cursor to beginning of second line
{
	CommandLCD(0xC0);
}

void ClearLCD()				//clear the whole screen
{
	CommandLCD(0x01);
	Delay1KTCYx(1);		//1ms
}