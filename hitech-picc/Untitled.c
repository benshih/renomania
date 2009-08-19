#include <pic18f1220.h>
#include <htc.h>

#pragma config LVP = OFF
void init(void)
{
	TRISA = 0xff;	//PORTA is an input
	TRISB = 0x00;	//PORTB is an output
	PORTA = 0xff;
	PORTB = 0x00;	//PORTB is all low
}	
void wait(int delay)
{
	int x = 0;
	while(x<=delay)
		x++;
}

void main(void)
{
	init();
	while(1)
	{
		PORTB = 33;
		wait(1000);
		PORTB = 18;
		wait(1000);
	}			
}