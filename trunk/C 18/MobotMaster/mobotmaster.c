#include <p18f25k20.h>
#include <stdio.h>
#include <timers.h>
#include <adc.h>
#include <i2c.h>
#include "RenLCD.h"

#define dur 0xFA23
#define OSCmask 0b0001111
#define Fosc 5

#define LED0 PORTAbits.RA4 
#define LED1 PORTAbits.RA5 
#define LED2 PORTAbits.RA6 
#define LED3 PORTAbits.RA7 

//*****************************
//      GLOBAL VARIABLES
//*****************************

int led[4];
int max;
int servoPos[4] = {1000, 1100, 1350, 1500};
char ran1, ran2, flag, adc0;
int hightime;
unsigned char slave_addr = 0xff << 1;

//*****************************
//     FUNCTION PROTOTYPES
//*****************************

void InitializeUART(void);
void SendUART(char*);
void SendUARTchar(char);

unsigned short invert(unsigned short);
void initTimers (void);
void init(void);
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

void main(void)
{
	char line1[16], line2[16];
	char str[16];
	int i = 0; int j = 0; int k = 0;
	unsigned char ir[16];
	int lmax;

    ran1 = 1; ran2 = 0;

	init();
	initTimers();
	Delay10KTCYx(50);
//	InitializeUART();
	InitializeLCD();
	CommandLCD(0x01);
//	SendUART(str);

	ClearLCD();
	SetLine1();
	SetLine2();

	while(1)
	{
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
		
		//find the index of the max LED value
		lmax = 0;
		for(i = 1; i < 4; i++)
		{
			if(led[i] > led[lmax])
			{
				lmax = i;
			}
		}
		max = lmax;

		sprintf(line1, "%d %d  ", hightime);
		sprintf(line2, "%d %d  ", led[2], led[3]);
		SetLine1();
		WriteLCD(line1);
		SetLine2();
		WriteLCD(line2);
 	}
}

void high_isr (void) 
{
	INTCONbits.TMR0IF = 0;
	PIR1bits.SSPIF = 0;
	//WriteTimer0(0xDB);
   //	if(ran2 == 0) ran2 = 1;
   // else ran2 = 0;
}

void low_isr (void)	
{
	PIR1bits.TMR1IF = 0;
	
	if(ran1 == 0){
		ran1 = 1;
		//servo ON cycle
		LATCbits.LATC0 = 1;
		//if we have a substantial LED value, take it
		if(led[max] > 400)
		{
			WriteTimer1(invert(servoPos[max]));
			hightime = servoPos[max];
		}
		//otherwise keep going straight
		else
		{
			WriteTimer1(0xFB4F);
		}
	}	
	else{
		ran1 = 0;
		//servo OFF cycle
		LATCbits.LATC0 = 0;
		//if we have a substantial LED value, take it
		if(led[max] > 400)
		{
			WriteTimer1(invert(20000 - hightime));
		}
		// otherwise, keep going straigt
		else
		{
			WriteTimer1(0xB68F);
		}

		//I2C functionality
		IdleI2C();
		StartI2C();					//start protocol
		IdleI2C();
		if(WriteI2C(slave_addr))	//Write returns 0 on success
		{
			return;
		}
		IdleI2C();
		if(WriteI2C(max))			//send index
		{
			return;
		}
		IdleI2C();
		StopI2C();					//finish
		IdleI2C();
	}
}

unsigned short invert(unsigned short value)
{
	return 0xFFFF - value;
}

void init()
{
	//set internal oscillator speed
	OSCCON = (OSCCON&OSCmask)|(Fosc<<4);   

	//TRISA means 00000001 so only RA0 is an input (1 is input) while the rest are output(0 is output)
	TRISA = 0x0F;			//PORTA output
	//Precautionary
	TRISB = 0x00;			//PORTB output
	TRISC = 0x18;
}	

void initTimers()
{
	//turn timers off
	T0CONbits.TMR0ON = 0;
    T1CONbits.TMR1ON = 0;
	
	//Initializing Timer 0 settings
	OpenTimer0(	TIMER_INT_ON 	&
				T0_16BIT 		&	
				T0_SOURCE_INT 	&
				T0_EDGE_RISE	&
				T0_PS_1_16);	
    T0CONbits.PSA = 0; //prescale off
	
	//Initializing Timer 1 settings
    OpenTimer1( TIMER_INT_ON	&
    			T1_16BIT_RW		&
    			T1_SOURCE_INT	&
    			T1_PS_1_1		&
    			T1_OSC1EN_OFF	&
    			T1_SYNC_EXT_OFF);
    			
    //Initializing Interrupts
    RCONbits.IPEN = 1;      //Initialize the IP

   	INTCONbits.TMR0IE = 0;  //Disable TMR0 interrupt
	INTCON2bits.TMR0IP = 1; //Set Timer0 to HighP

    PIE1bits.TMR1IE = 1;    //Enable TMR1 interrupt 
    IPR1bits.TMR1IP = 0;    //Set Timer1 to LowP
   
 
	PIE1bits.SSPIE = 1;		//enable MSSP interrupts
	IPR1bits.SSPIP = 1; 	//MSSP are high priority	

	INTCONbits.PEIE = 1;    //Turn on LP interrupts
    INTCONbits.GIE = 1;     //Turn on HP interrupts
   

    WriteTimer0(dur);
   	WriteTimer1(invert(dur));
    
    T0CONbits.TMR0ON = 0;
    T1CONbits.TMR1ON = 1;

	//enabling serial ports
	SSPCON1bits.SSPEN = 1;

	//setting mastermode
	SSPCON1bits.SSPM3 = 1;
	SSPCON1bits.SSPM2 = 0;
	SSPCON1bits.SSPM1 = 0;
	SSPCON1bits.SSPM0 = 0;
	
	//since Fosc = 4MHz, SCK = 100Khz
	SSPADD = 0x9;

}

void InitializeUART()
{
	SPBRG = 12;				//Baud Rate 19200 for 4MHz
	TRISC = TRISC | 0xC0;	//RX & TX TRIS controls to 1
	TXSTAbits.SYNC = 0;		//asynchronous operation
	RCSTAbits.SPEN = 1;		//TX/CK I/O pins as output
	TXSTAbits.TX9 = 0;		//8-bit transmission
	BAUDCONbits.CKTXP = 0;	//no data polarity
	BAUDCONbits.BRG16 = 1;	//16-bit Baud Rate Generator
	TXSTAbits.TXEN = 1;		//enables transmitter circuitry
}

void SendUART(char *c)
{
	char temp;
	int i = 0;

	do
	{
		temp = c[i++];
		TXREG = temp;
		Delay1KTCYx(5);
	} while( c[i] != '\0' );
}

void SendUARTchar(char c)
{
	TXREG = c;
	Delay1KTCYx(5);
}