#include <p18f25k20.h>
#include <timers.h>
#include <delays.h>
#include <stdlib.h>
#include <stdio.h>
#include <i2c.h>

#define LED0 LATAbits.LATA0
#define LED1 LATAbits.LATA1
#define OSCmask 0b10001111
#define Fosc 5 //4MHz

#define ADDR 0x2 //slave address

int duration;
unsigned char var;
char data, temp;
char str[16];


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
	temp = 0;
	LED0 = 1; //indicator lamp on
	LED1 = 1;
	init();
	initI2C();
	initUART();
	initTimers();
	
	while(1){
	}		
}	

/*******************************
	     HELPER FUNCTIONS
********************************/

void init(){
	//set internal oscillator speed
	OSCCON = (OSCCON&OSCmask)|(Fosc<<3);   

	//port directions
	TRISA = 0x00; //all output
	TRISB = 0x00; //all output
	TRISC = 0x18; //all but RC3,4 are output
}	

void high_isr (void) {
	PIR1bits.SSPIF = 0;
	data=getcI2C();
  	if(data==4) LED0 = 0;
}

void low_isr (void)	{
	PIR1bits.TMR1IF = 0;
	LED1 = !LED1;
	
	if(temp){
		duration = data;
		WriteTimer1(0xFFFF-duration);
	}	
	else{
		WriteTimer1(0xFFFF-(10000-(duration)));
	}		
	temp = !temp;
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
    PIE1bits.TMR1IE = 1;    //Enable TMR1 interrupt    
    
    WriteTimer1(0xf000);
    T1CONbits.TMR1ON = 1; //timer 0 on
}

void initI2C(void){
	SSPCON1bits.SSPM0 = 0; //I am a slave
	SSPCON1bits.SSPM1 = 1; //I am a slave
	SSPCON1bits.SSPM2 = 1; //I am a slave
	SSPCON1bits.SSPM3 = 0; //I am a slave
	SSPADD = 0xff << 1; //enter slave address
	
	
	PIE1bits.SSPIE = 1;	//enable MSSP interrupts
	IPR1bits.SSPIP = 1; //MSSP are high priority	
	
	INTCONbits.PEIE = 1;    //Turn off LP interrupts
    INTCONbits.GIE = 1;     //Turn on HP interrupts
    
    Delay1KTCYx(500);
    SSPCON1bits.SSPEN = 1;
}	

void initUART()
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