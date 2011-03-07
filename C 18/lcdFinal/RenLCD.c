#include "RenLCD.h"

#define LCD_RS PORTCbits.RC3
#define LCD_RW PORTCbits.RC2
#define LCD_EN PORTCbits.RC1

void InitializeLCD()
{
	//Function set
	Delay10KTCYx(2);
	CommandLCD(0x38);		//8-bit Data Line, 2-lines, 5x8 dots
	//Delay10KTCYx(2);
	CommandLCD(0x0F);
//	Delay10KTCYx(2);
	CommandLCD(0x06);
//	Delay10KTCYx(2);
	CommandLCD(0x02);
}

void WriteLCD(char *c)		//Writes Characters to LCD
{
	unsigned char i = 0;
	LCD_RS = 1;
	LCD_RW = 0;
	LCD_EN=0;
	for( i = 0; c[i] != '\0'; i++ )
	{
		PORTB = c[i];
		LCD_EN = 1;
		LCD_EN = 0;
		//Nop();
		//Nop();
	}
}

void CommandLCD(char c)		//Sends Commands to LCD
{
	PORTB=c;				
	LCD_RS = LCD_RW = 0;
	LCD_EN = 1;
	LCD_EN = 0;
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