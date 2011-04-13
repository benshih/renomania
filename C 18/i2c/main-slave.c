#include <p18f25k20.h>
#include <timers.h>
#include <i2c.h>

#define LED0 LATAbits.LATA0
#define OSCmask 0b10001111
#define Fosc 5 //4MHz

#define ADDR 0x2 //slave address

unsigned char var;
char data;

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
	init();
	initI2C();
//	initTimers();
	LED0 = 1; //indicator lamp on
	while(1){
		if(data==0x03) LED0 = 0;
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
	//INTCONbits.TMR0IF = 0;
	PIR1bits.SSPIF = 0;
	if(DataRdyI2C()){
		data = getcI2C();
	}	
}

void low_isr (void)	{
	PIR1bits.TMR1IF = 0;
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
//    OpenTimer1( TIMER_INT_ON	&
//    			T1_16BIT_RW		&
//    			T1_SOURCE_INT	&
//    			T1_PS_1_1		&
//    			T1_OSC1EN_OFF	&
//    			T1_SYNC_EXT_OFF);
//    			
    //Initializing Interrupts
    RCONbits.IPEN = 1;      //Initialize the IP
	INTCON2bits.TMR0IP = 1; //Set Timer0 to HighP
    IPR1bits.TMR1IP = 0;    //Set Timer1 to LowP
   	INTCONbits.TMR0IE = 1;  //Enable TMR0 interrupt
    PIE1bits.TMR1IE = 0;    //Disable TMR1 interrupt    

    
    WriteTimer0(0x8000);
//   	WriteTimer1(invert(dur));
    
    T0CONbits.TMR0ON = 1;
    T1CONbits.TMR1ON = 0;
}

void initI2C(void){
	SSPCON1 = (SSPCON1 & 0xF0) | 0x06; //I am a slave
	SSPADD = ADDR << 1; //enter slave address
	
	
	PIE1bits.SSPIE = 1;	//enable MSSP interrupts
	IPR1bits.SSPIP = 1; //MSSP are high priority	
	
	INTCONbits.PEIE = 0;    //Turn off LP interrupts
    INTCONbits.GIE = 1;     //Turn on HP interrupts
}	