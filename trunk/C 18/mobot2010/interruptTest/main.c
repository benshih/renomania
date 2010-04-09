#include <p18f25k20.h>
#include <stdio.h>
#include <timers.h>
#include <adc.h>


//*****************************
//              FUNCTION PROTOTYPES
//*****************************
char ran, ran2, flag, adc0;
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
#pragma interrupt high_isr      //the high ISR=
void high_isr (void) {
               PIR1bits.TMR1IF = 0;
               if(ran2 == 0) {
                       WriteTimer1(0x0BDB);
                       PORTAbits.RA2 = 1;
                       ran2 = 1;
               }
               else {
                       WriteTimer1(0x0BDB);
                       PORTAbits.RA2 = 0;
                       ran2 = 0;
               }
}

#pragma code low_vector = 0x18          //setup the low ISR vector
void interrupt_at_low_vector(void) {
       _asm GOTO low_isr _endasm
}
#pragma code
#pragma interruptlow low_isr    //the low ISR
void low_isr (void) {
               INTCONbits.TMR0IF = 0;
               if(ran == 0) {
                       WriteTimer0(0x0BDB);
                       PORTAbits.RA1 = 1;
                       ran = 1;
               }
               else {
                       WriteTimer0(0x0BDB);
                       PORTAbits.RA1 = 0;
                       ran = 0;
               }
}

//*****************************
//              MAIN
//*****************************


void main(void)
{
       //Initializing Interrupts
       RCONbits.IPEN = 1;      //Initialize the IP
       INTCONbits.GIE = 1;     //Turn on HP interrupts
       INTCONbits.PEIE = 1;    //Turn on LP interrupts

       INTCONbits.TMR0IE = 1;  //Enable TMR0 interrupt
       PIE1bits.TMR1IE = 1;    //Enable TMR1 interrupt
       INTCON2bits.TMR0IP = 0; //Set Timer0 to LowP
       IPR1bits.TMR1IP = 1;            //Set Timer1 to HighP

       //Osscilator Timer
       OSCCONbits.IRCF0 = 1;   //4MHz internal oscillator
       OSCCONbits.IRCF1 = 0;   //T = 1ms
       OSCCONbits.IRCF2 = 1;

       ran, ran2 = 0;
       TRISA = 0x00;

       //Initializing Timer 0 settings
       T0CONbits.T08BIT = 0;
       T0CONbits.T0CS = 0;
       T0CONbits.T0SE = 0;
       T0CONbits.PSA = 0;

       /** 8 bits prescale **/
       T0CONbits.T0PS0 = 0;
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


       /** START YOUR ENGINES!! **/
       T0CONbits.TMR0ON = 1;
       Delay100TCYx(1000);
       T1CONbits.TMR1ON = 1;

 while(1){     }
}