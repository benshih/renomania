#include <p18f25k20.h>
#include <stdio.h>
#include <timers.h>
#include <adc.h>

//*****************************
//              FUNCTION PROTOTYPES
//*****************************
char ran1, ran2, flag, adc0;
void low_isr(void);
void high_isr(void);

/************************
       INTERRUPT PROTOTYPES
************************/
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
//              MAIN
//*****************************


void main(void)
{
    //Initializing Interrupts
    RCONbits.IPEN = 1;      //Initialize the IP
       
    INTCON2bits.TMR0IP = 1; //Set Timer0 to LowP
    IPR1bits.TMR1IP = 0;            //Set Timer1 to HighP

    //Oscilator Timer
    OSCCONbits.IRCF0 = 1;   //4MHz internal oscillator
    OSCCONbits.IRCF1 = 0;   //T = 1ms
    OSCCONbits.IRCF2 = 1;

    ran1 = 1; ran2 = 0;
    TRISA = 0x00;
    PORTA = 0x00;
          
    T0CONbits.TMR0ON = 0;
    T1CONbits.TMR1ON = 0;

    //Initializing Timer 0 settings
    T0CONbits.T08BIT = 0;
    T0CONbits.T0CS = 0;
    T0CONbits.T0SE = 0;
    T0CONbits.PSA = 0;

    /** 16 bits prescale **/
    T0CONbits.T0PS0 = 1;
    T0CONbits.T0PS1 = 1;
    T0CONbits.T0PS2 = 0;

    //Initializing Timer 1 settings
    T1CONbits.RD16 = 0;
    T1CONbits.T1RUN = 0;
    T1CONbits.T1OSCEN = 0;
    T1CONbits.TMR1CS = 0;

	/** 8 bits prescale **/
    T1CONbits.T1CKPS0 = 1;
    T1CONbits.T1CKPS1 = 1;

	WriteTimer0(0x0BDB);
   	WriteTimer1(0x0BDB);

    /** START YOUR ENGINES!! **/
    T0CONbits.TMR0ON = 1;
    T1CONbits.TMR1ON = 1;

   	INTCONbits.TMR0IE = 1;  //Enable TMR0 interrupt
    PIE1bits.TMR1IE = 1;    //Enable TMR1 interrupt
    INTCONbits.PEIE = 1;    //Turn on LP interrupts
    INTCONbits.GIE = 1;     //Turn on HP interrupts

while(1){
 		PORTA = (ran2<<2) | (ran1<<1);
 	}
}