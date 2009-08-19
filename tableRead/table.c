#include <p18f1220.h>

#pragma config LVP=OFF, OSC=INTIO2, WDT=OFF

#define buffB 0x80
#define column 0x81

void wait(int);
char nextCol(void);

void main(void)
{
	TRISB = 0x00;
	PORTB = 0x00;
	OSCCON=0x70;                //Osc=8MHz
	while(1)
	{	
		_asm
			movlw	0x01
			movwf	column, 1	
		_endasm
		PORTB = nextCol();
		wait(10000);
		PORTB = nextCol();
		wait(10000);
	}	
}	
void wait(int delay)
{
	int x = 0;
	while(x<=delay)
		x++;
}
char nextCol(void)
{
	_asm
		incf 	column, 1, 1
		movf 	column, 0, 1
		addwf 	PCL, 1, 1
		
		retlw 	0x00
		retlw	0x01	
	_endasm
}	
