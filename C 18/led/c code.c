#include <p18f1220.h>

void wait(int delay)
{
	int x = 0;
	while(x<=delay)
		x++;
}	

void main(void)
{	
	TRISA = 0;
	while(1)
	{
		PORTA = 1;
		wait(250);
		PORTA = 0;
		wait(250);
	}
}
	