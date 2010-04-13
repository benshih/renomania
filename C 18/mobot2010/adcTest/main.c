#include "main.h"
#include "init.h"

//*****************************
//		Global Variables
//*****************************
char ran1, ran2, flag;
unsigned short int adc1;

//*****************************
//		INTERRUPT PROTOTYPES
//*****************************
#pragma config FOSC = INTIO67, LVP = OFF

#pragma code high_vector = 0x08         //setup the high ISR vector
void interrupt_at_high_vector(void) {
	_asm GOTO high_isr _endasm
}
#pragma code
#pragma interrupt high_isr      //the high ISR
void high_isr (void) {
	INTCONbits.TMR0IF = 0;
	TMR0H = 0x0B;
	TMR0L = 0xDB;
   	if(ran2 == 0) ran2 = 1;
    else ran2 = 0;
}

#pragma code low_vector = 0x18          //setup the low ISR vector
void interrupt_at_low_vector(void) {
     _asm GOTO low_isr _endasm
}
#pragma code
#pragma interruptlow low_isr    //the low ISR
void low_isr (void)	{
	PIR1bits.TMR1IF = 0;
	TMR1H = 0x0B;
	TMR1L = 0xDB;	
	if(ran1 == 0) ran1 = 1;
	else ran1 = 0;
}

//*****************************
//		MAIN FUNCTION
//*****************************

void main(void)
{
	//RA0:AN0   RA1:AN1   RA2:AN2   RA3:AN3
	//RA5:AN4   RB2:AN8   RB3:AN9   RB1:AN10
	//RB4:AN11	RB0:AN12
	
    OSCCONbits.IRCF0 = 1;   //4MHz internal oscillator
    OSCCONbits.IRCF1 = 0;   //T = 1ms
    OSCCONbits.IRCF2 = 1;
	initTMR0();
	initPORTS(); 	
	initADC();
	T0CONbits.TMR0ON = 1;
while(1){
 		PORTB = (ran2<<5);
 		SetChanADC(ADC_CH1);
 		ConvertADC();
 		while(BusyADC());
 		adc1 = ReadADC();
 		if(adc1 > 512) PORTAbits.RA0=1;
	 	else PORTAbits.RA0=0;		 		
 	}
}