#include <p18f25k20.h>
#include <timers.h>
#include <delays.h>
#include <stdlib.h>
#include <stdio.h>
#include <i2c.h>

#define LED0 PORTAbits.RA0
#define LED1 PORTAbits.RA1
#define LED2 PORTAbits.RA2
#define LED3 PORTAbits.RA3
#define LED4 PORTAbits.RA4
#define OSCmask 0b10001111
#define Fosc 5 //4MHz

#define ADDR 0xff //slave address

int duration;
unsigned char var;
char data, temp, tempData;
char str[16];
int speeds[4] = {5000, 8000, 8000, 5000};
char[10] prevIndices;
char curIndex = 0;
int average = 700;

//*****************************
//     FUNCTION PROTOTYPES
//*****************************

void initUART(void);
void SendUART(char*);
void SendUARTchar(char);

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
	temp = 1;
	LED0 = 0; //indicator lamp on
	LED1 = 1;
	LED2 = 1;
	LED3 = 1;
	LED4 = 1;
	init();
	initUART();
	SendUARTchar('a');
	initI2C();
	initTimers();
	while(1){
	}		
}	

/*******************************
	     HELPER FUNCTIONS
********************************/

void init(){
	//set internal oscillator speed
	OSCCON = (OSCCON&OSCmask)|(Fosc<<4);   

	//port directions
	TRISA = 0x00; //all output
	TRISB = 0x00; //all output
	TRISC = 0x18; //all but RC3,4 are output

	PORTBbits.RB0=0; //right PMOS off
	PORTBbits.RB1=0; //left PMOS off
	PORTBbits.RB2=1; //right NMOS off
	PORTBbits.RB3=1; //left NMOS off
}	

void low_isr (void) {

//	SSPBUF=0;
//	SSPSTATbits.UA=0;
//	SSPSTATbits.BF=0;
//	LED0 = !LED0;
	tempData=getcI2C();
	if(tempData == (ADDR << 1))
		tempData = getcI2C();
	data = tempData;
	LED2 = !LED2;
	PIR1bits.SSPIF = 0;
}

void high_isr (void)	{
	PIE1bits.SSPIE = 0;
	
	if(temp){
		LED0 = 1;
		LATB = 0x05;


		prevIndices[curIndex] = data-0x30;
		
		duration = speeds[data-0x30];
		//duration = speeds[0];
		WriteTimer1(0xffff-duration);
	}	
	else{
		LED0 = 0;
		LATB = 0x0C;
		WriteTimer1(0xffff-(20000-(duration)));
		//SendUARTchar(data);
	}	
	temp = !temp;
	PIR1bits.TMR1IF = 0;
	
	PIE1bits.SSPIE = 1;
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
    IPR1bits.TMR1IP = 1;    //Set Timer1 to LowP
    PIE1bits.TMR1IE = 1;    //Enable TMR1 interrupt    
    
    WriteTimer1(0xffff);
    T1CONbits.TMR1ON = 1; //timer 0 on
}

void initI2C(void){
	SSPCON1bits.SSPM0 = 0; //I am a slave
	SSPCON1bits.SSPM1 = 1; //I am a slave
	SSPCON1bits.SSPM2 = 1; //I am a slave
	SSPCON1bits.SSPM3 = 0; //I am a slave
	//SSPADD = 0xff << 1; //enter slave address
	SSPADD = ADDR << 1;
	
	PIE1bits.SSPIE = 1;	//enable MSSP interrupts
	IPR1bits.SSPIP = 0; //MSSP are high priority	
	
	INTCONbits.PEIE = 1;    //Turn on LP interrupts
    INTCONbits.GIE = 1;     //Turn on HP interrupts
    
 	Delay1KTCYx(1500);
    SSPCON1bits.SSPEN = 1;
}	

void initUART()
{
	SPBRG = 23;				//Baud Rate 19200 for 4MHz
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
	Delay1KTCYx(1);
}