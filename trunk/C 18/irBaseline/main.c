#include <p18f25k20.h>
#include <timers.h>
#include <delays.h>
#include <stdlib.h>
#include <adc.h>

#include "RenLCD.h"

#define OSCmask 0b10001111
#define Fosc 5 //4MHz

char str[16];
int analog[2];

//*****************************
//     FUNCTION PROTOTYPES
//*****************************

void init(void);
void initTimers(void);
void initI2C(void);
void low_isr(void);
void high_isr(void);

/************************
       INTERRUPT SETUP
************************/

#pragma config FOSC = INTIO67, LVP = OFF

#pragma code high_vector = 0x08  //setup the high ISR vector
void int_high(void) {_asm GOTO high_isr _endasm}
#pragma code
#pragma interrupt high_isr      //the high ISR

#pragma code low_vector = 0x18   //setup the low ISR vector
void int_low(void) {_asm GOTO low_isr _endasm}
#pragma code
#pragma interruptlow low_isr    //the low ISR

//*****************************
//              MAIN
//*****************************

void main(){
	Delay1KTCYx(50);
	init();
	InitializeLCD();
	//initTimers();

	ClearLCD();
	while(1){
		OpenADC(ADC_FOSC_32 & ADC_RIGHT_JUST,ADC_CH0 & ADC_INT_OFF &  ADC_VREFPLUS_VDD  & ADC_VREFMINUS_VSS, 0); 		
		ConvertADC();
		while(BusyADC());
		analog[0]=ReadADC();
		OpenADC(ADC_FOSC_32 & ADC_RIGHT_JUST,ADC_CH1 & ADC_INT_OFF &  ADC_VREFPLUS_VDD  & ADC_VREFMINUS_VSS, 0); 		
		ConvertADC();
		while(BusyADC());
		analog[1]=ReadADC();
		
		SetLine1();
		sprintf(str, "%4d %4d", analog[0],analog[1]);
		WriteLCD(str);
		Delay10KTCYx(10);
	}		
}	

/*******************************
	     HELPER FUNCTIONS
********************************/

void init(){
	//set internal oscillator speed
	OSCCON = (OSCCON&OSCmask)|(Fosc<<4);   

	//port directions
	TRISA = 0xff; //all input
	TRISB = 0x00; //all output
	TRISC = 0x00; //all but RC3,4 are output
}	

void high_isr (void) {
	PIR1bits.SSPIF = 0;
}

void low_isr (void)	{
	PIR1bits.TMR1IF = 0;
	WriteTimer1(0xFFFF-40000);
	sprintf(str, "%4d %4d", analog[0],analog[1]);
	WriteLCD(str);
}

void initTimers()
{
    T1CONbits.TMR1ON = 0; //timer 1 off
    
	// Initializing Timer 1 settings
    OpenTimer1( TIMER_INT_ON	&
    			T1_16BIT_RW		&
    			T1_SOURCE_INT	&
    			T1_PS_1_1		&
    			T1_OSC1EN_OFF	&
    			T1_SYNC_EXT_OFF);
    			
    //Initializing Interrupts
    RCONbits.IPEN = 1;      //Initialize the IP
    IPR1bits.TMR1IP = 0;    //Set Timer1 to LowP
    PIE1bits.TMR1IE = 0;    //Enable TMR1 interrupt    
    
    WriteTimer1(0xffff-40000);
    T1CONbits.TMR1ON = 0; //timer 0 on
}

