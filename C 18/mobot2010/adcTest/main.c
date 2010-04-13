#include "main.h"
#include "init.h"

//*****************************
//		Global Variables
//*****************************
char ran1, ran2;
char flag;
//Initialize
unsigned short int black[10]	= {0,0,0,0,0,0,0,0,0,0};
unsigned short int white[10] 	= {0,0,0,0,0,0,0,0,0,0};
unsigned short int thres[10]	= {0,0,0,0,0,0,0,0,0,0};
unsigned char line[10]			= {0,0,0,0,0,0,0,0,0,0};

//*****************************
//		INTERRUPT PROTOTYPES
//*****************************
#pragma config FOSC = INTIO67, LVP = OFF
#pragma code high_vector = 0x08	//setup the high ISR vector
void interrupt_at_high_vector(void) 
	{_asm GOTO high_isr _endasm}
#pragma code
#pragma interrupt high_isr      //the high ISR
void high_isr(void){
	INTCONbits.TMR0IF = 0;
	TMR0H = 0x0B;
	TMR0L = 0xDB;
   	if(ran2 == 0) ran2 = 1;
    else ran2 = 0;
}
#pragma code low_vector = 0x18	//setup the low ISR vector
void interrupt_at_low_vector(void) 
	{_asm GOTO low_isr _endasm}
#pragma code
#pragma interruptlow low_isr    //the low ISR
void low_isr(void){
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
	//RA0:AN0   RA1:AN1   RA2:AN2   RA3:AN3   RA5:AN4
	//RB2:AN8   RB3:AN9   RB1:AN10  RB4:AN11  RB0:AN12
    initCLK(OSC_4MHZ);
	initTMR0();
	initPORTS(); 	
	initADC();
	T0CONbits.TMR0ON = 1;
while(1){
 		SetChanADC(ADC_CH1); ConvertADC();
 		while(BusyADC());
 		if(ReadADC() > thres[0]) line[0] = 1;
	 	else line[0] = 0;		 		
 	}
}