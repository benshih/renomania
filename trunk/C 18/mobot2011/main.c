#include <p18f25k20.h>
#include <stdio.h>
#include <timers.h>
#include <adc.h>
#include "RenLCD.h"

#define dur 0xFA23
#define LED0 PORTAbits.RA4 
#define LED1 PORTAbits.RA5 
#define LED2 PORTAbits.RA6 
#define LED3 PORTAbits.RA7 

//*****************************
//              GLOBAL VARIABLES
//*****************************

int led[4];
int max;
int servoPos[4] = {1300, 1400, 1600, 1700};

//*****************************
//              FUNCTION PROTOTYPES
//*****************************
char ran1, ran2, flag, adc0;
void low_isr(void);
void high_isr(void);
unsigned short invert(unsigned short);
void InitializeTimers (void);

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
	//WriteTimer0(0xDB);
   //	if(ran2 == 0) ran2 = 1;
   // else ran2 = 0;
}

#pragma code low_vector = 0x18          //setup the low ISR vector
void interrupt_at_low_vector(void) {
     _asm GOTO low_isr _endasm
}
#pragma code
#pragma interruptlow low_isr    //the low ISR
void low_isr (void)	{
	PIR1bits.TMR1IF = 0;
	
	if(ran1 == 0){
		ran1 = 1;
		PORTCbits.RC0 = 0;
		if(led[1] > 400)
			WriteTimer1(invert(servoPos[2]));
		
	}	
	else{
		ran1 = 0;
		PORTCbits.RC0 = 1;
		if(led[1] > 400)
			WriteTimer1(invert(servoPos[1]));
		
	}
}

//*****************************
//             DURATION CALCULATOR
//*****************************


unsigned short invert(unsigned short value)
{
	return 0x1FFFB - value;
}

//*****************************
//              TIMER
//*****************************

void InitializeTimers()
{
	OpenTimer0(	TIMER_INT_OFF &
				T0_16BIT &
				T0_SOURCE_INT &
				T0_PS_1_1);	
//	CloseTimer2();
}

//*****************************
//              MAIN
//*****************************


void main(void)
{
	char line1[16], line2[16];
	char str[16];
	int i = 0; int j = 0; int k = 0;
	unsigned char ir[16];


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
	TRISB = 0x00;
	TRISC = 0x00;

          
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
    T1CONbits.RD16 = 1;
    T1CONbits.T1RUN = 0;
    T1CONbits.T1OSCEN = 0;
    T1CONbits.TMR1CS = 0;

	/** 8 bits prescale **/
    T1CONbits.T1CKPS0 = 0;
    T1CONbits.T1CKPS1 = 0;

	WriteTimer0(dur);
   	WriteTimer1(invert(dur));

    /** START YOUR ENGINES!! **/
    //T0CONbits.TMR0ON = 1;
    T1CONbits.TMR1ON = 1;

   	INTCONbits.TMR0IE = 1;  //Enable TMR0 interrupt
    PIE1bits.TMR1IE = 1;    //Enable TMR1 interrupt
    INTCONbits.PEIE = 1;    //Turn on LP interrupts
    INTCONbits.GIE = 1;     //Turn on HP interrupts

	Delay10KTCYx(50);

	//TRISA means 00000001 so only RA0 is an input (1 is input) while the rest are output(0 is output)
	TRISA = 0x0F;			//PORTA output
	//Precautionary
	TRISB = 0x00;			//PORTB output

//	InitializeUART();
	InitializeLCD();
	InitializeTimers();
	
	CommandLCD(0x01);
	SetLine2();
	sprintf(str,"\r\rIR Detector Test\r\n");
	WriteLCD(str);
//	SendUART(str);
	ClearLCD();


	SetLine1();
	SetLine2();


while(1){
	//change the servo
	//	LATC = (ran2<<2) | (ran1<<1); //not sure about this line of code because of portc/latc being special

	//read the sensor data
		//Obtain value for first LED (RA0)
		OpenADC(ADC_FOSC_32 & ADC_RIGHT_JUST,ADC_CH0 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 0); 	
		LED0 = 1; //Turns on IR LED0	
		ConvertADC();
		while(BusyADC());
		//sprintf(str, "RA0: %d ", ReadADC());
		//SendUART(str);
		led[0] = ReadADC(); //atoi(str);
		LED0 = 0;

		//Obtain value for second LED (RA1)
		OpenADC(ADC_FOSC_32 & ADC_RIGHT_JUST,ADC_CH1 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 0); 	
		LED1 = 1; //Turns on IR LED1	
		ConvertADC();
		while(BusyADC());
		//sprintf(str, "RA1: %d ", ReadADC());
		//SendUART(str); //send info through uart
		led[1] = ReadADC(); // = atoi(str);
		LED1 = 0;
	
		//Obtain value for third LED (RA2)
		OpenADC(ADC_FOSC_32 & ADC_RIGHT_JUST,ADC_CH2 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 0); 	
		LED2 = 1;
		ConvertADC();
		while(BusyADC());
		//sprintf(str, "RA2: %d ", ReadADC());
		//SendUART(str);
		led[2] = ReadADC(); // = atoi(str);
		LED2 = 0;

		//Obtain value for fourth LED (RA3)
		OpenADC(ADC_FOSC_32 & ADC_RIGHT_JUST,ADC_CH3 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 0); 	
		LED3 = 1;
		ConvertADC();
		while(BusyADC());
		//sprintf(str, "RA3: %d\r\n", ReadADC());
		//SendUART(str);
		led[3] = ReadADC(); // = atoi(str);
		LED3 = 0;

		sprintf(line1, "%d %d  ", led[0], led[1]);
		sprintf(line2, "%d %d  ", led[2], led[3]);
		SetLine1();
		WriteLCD(line1);
		SetLine2();
		WriteLCD(line2);

		//Delay10KTCYx(10);

 	}
}