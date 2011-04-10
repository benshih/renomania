#include "RenLCD.h"

#define LCD_RS LATCbits.LATC3
#define LCD_RW LATCbits.LATC2
#define LCD_EN LATCbits.LATC1

void InitializeLCD()
{
	//Function set
	Delay10KTCYx(5);
	CommandLCD(0x38);		//8-bit Data Line, 2-lines, 5x8 dots
	CommandLCD(0x0F);		//Display on, cursor on, no blinking
	CommandLCD(0x06);
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
		LATB = c[i];
		LCD_EN = 1;
		Nop();
		LCD_EN = 0;
		Nop();
		LCD_EN = 1;
		Nop();
		LCD_EN = 0;
		Nop();
	}
}

void CommandLCD(char c)		//Sends Commands to LCD
{
	LATB=c;				
	LCD_RS = LCD_RW = 0;
	LCD_EN = 1;
	LCD_EN = 0;
	Delay10TCYx(5);
	//LATB = 0x00;
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
	Delay1KTCYx(2);			//1.5ms
}

void LCDon(){
	CommandLCD(0x0C);
}

void LCDoff(){
	CommandLCD(0x08);
}		