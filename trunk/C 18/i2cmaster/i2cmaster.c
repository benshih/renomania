#include <p18f25k20.h>
#include <i2c.h>
#include <delays.h>

#define LED0 LATAbits.LATA0
#define LED1 LATAbits.LATA1
#define OSCmask 0b10001111
#define Fosc 5 //4 MHz

#define ADDR 0x2 // slave address

unsigned char var;
unsigned char slave_addr = 0xff<<1;

//*****************************
//     FUNCTION PROTOTYPES
//*****************************

void init(void);
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


void main()
{
	init();
	LED0 = 1;
	LED1 = 1;

	PIE1bits.SSPIE = 1;	//enable MSSP interrupts
	IPR1bits.SSPIP = 1; //MSSP are high priority	
	
	INTCONbits.PEIE = 0;    //Turn off LP interrupts
    INTCONbits.GIE = 1;     //Turn on HP interrupts


	//enabling serial ports
	SSPCON1bits.SSPEN = 1;

	//setting mastermode
	SSPCON1bits.SSPM3 = 1;
	SSPCON1bits.SSPM2 = 0;
	SSPCON1bits.SSPM1 = 0;
	SSPCON1bits.SSPM0 = 0;
	
	//since Fosc = 4MHz, SCK = 100Khz
	SSPADD = 0x9;

//	while(1)
//	{
//	Delay1KTCYx(1000);
//	while(1)
//	{
	IdleI2C();

	//starting  transmission
	//SSPCON2bits.SEN = 1;
	StartI2C();
	IdleI2C();
//	Delay1KTCYx(10);
//	while(SSPCON2bits.SEN) {};
	//loading slave address into buf; BF should be high after
	if(WriteI2C(slave_addr))
	{
		return;
	}
	//SSPBUF = slave_addr;
	//while(SSPSTATbits.BF);

	//wait until the slave acknowledges reciept (ACKSTAT = 0)
//	while(SSPCON2bits.ACKSTAT){};
//	LED1 = 0;	
	//send data
//	SSPBUF = 0x3;
	IdleI2C();
	if(WriteI2C(0x4))
	{
		return;
	}
	
	//wait until the slave acknowledges reciept (ACKSTAT = 0)
//	while(SSPCON2bits.ACKSTAT){};
		

	//	OpenI2C(MASTER, SLEW_ON);
	//	StartI2C();
	//	WriteI2c(slave_addr);
	//	if(
//	}
//	StopI2C();
//	SSPCON2bits.PEN = 1;
//	while(1){};
	IdleI2C();
	//Delay1KTCYx(100);

}

void init()
{
	//set internal oscillator speed
	OSCCON = (OSCCON&OSCmask)|(Fosc<<3);   

	//port directions
	TRISA = 0x00; //all output
	TRISB = 0x00; //all output
	TRISC = 0x18; //all but RC3,4 are output
}	

void high_isr (void) {
	PIR1bits.SSPIF = 0;
	LED1 = 0;
//	if(DataRdyI2C()){
//		data = getcI2C();
//	}	
}

void low_isr (void)	{
	PIR1bits.TMR1IF = 0;
}

