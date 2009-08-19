#include <p18f1220.h>

#pragma config LVP=OFF, OSC=INTIO2, WDT=OFF

unsigned char buffA;
unsigned char buffB;

void init(void)
{
	TRISA = 0x00;
	TRISB = 0x00;		
	PORTA = 0xFF;
	PORTB = 0x00;
}	
void wait(int delay)
{
	int x = 0;
	while(x<=delay)
		x++;
}
void updateBuffers(void)
{
	buffA = PORTA;
	buffB = PORTB;	
}
void updatePorts(void)
{
	PORTA = buffA;
	PORTB = buffB;
}	

void main(void)
{
	init();
	while(1)
	{
		updateBuffers();
		updatePorts();	
	}
}   