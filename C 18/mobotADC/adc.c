#include <p18f1220.h>
#include <adc.h>

//#pragma config LVP = OFF

unsigned char buffA;
unsigned char buffB;

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
	//int tiltX, tiltY;
	//int zeroX, zeroY;
	unsigned char col, row;
	short int ADCresult;
	int count;
	OpenADC(ADC_FOSC_32 & ADC_RIGHT_JUST,ADC_CH0 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 0); 	
	init();
	while(1)
	{
		ConvertADC();						
		while(BusyADC());			
		PORTB = ReadADC();
	}
}