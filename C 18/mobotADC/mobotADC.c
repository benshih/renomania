#include <p18f1220.h>
#include <adc.h>

unsigned char buffA;
unsigned char buffB;

#pragma config WDT = OFF, LVP = OFF, OSC = HS

void init(void)
{
	TRISA = 0xff;	//PORTA is an input
	TRISB = 0x00;	//PORTB is an output
	TRISBbits.TRISB2 = 1;
	PORTA = 0x00;
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
	int back = 33;
	int thres, white, black;
	int ADCres0, ADCres1;
	unsigned char PB;
	int count;
	//OpenADC(ADC_FOSC_32 & ADC_RIGHT_JUST,ADC_CH0 & ADC_INT_OFF & ADC_VREFPLUS_EXT & ADC_VREFMINUS_VSS, 0); 	
	init();
	thres = white = black = 0;
	while(1)
	{
		wait(10000);	
		PB = PORTB & 0xFC;
		PORTB = PB | 0x01;
		wait(10000);
		PB = PORTB & 0xFC;
		PORTB = PB | 0x02;
		/*SetChanADC(ADC_CH0);
		ConvertADC();						
		while(BusyADC());			
		ADCres0 = ReadADC()/2;
		SetChanADC(ADC_CH1);
		ConvertADC();						
		while(BusyADC());			
		ADCres1 = ReadADC()/2;
		if((ADCres1+ADCres0)/2>thres)
		{
			PORTB = back;	//not forward
		}
		else
		{
			PORTB = back; //backwards	
		}	
		
		if(PORTAbits.RA4==1)
		{
			white = (ADCres1+ADCres0)/2;	
		}	
		else if(PORTAbits.RA2==1)
		{
			black = (ADCres1+ADCres0)/2;
		}	
		thres = (white+black)/2;*/
	}		
}