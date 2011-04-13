#include <p18f25k20.h>
#include <i2c.h>

#define LED0 LATAbits.LATA0
#define OSCmask 0b10001111
#define Fosc 5 //4 MHz

#define ADDR 0x2 // slave address

unsigned char var;
unsigned char slave_addr = 0x2;
void init(void);

void main()
{
	init();
	LED0 = 1;

	//enabling serial ports
	SSPCON1bits.SSPEN = 1;

	//setting mastermode
	SSPCON1bits.SSPM3 = 1;
	SSPCON1bits.SSPM2 = 0;
	SSPCON1bits.SSPM1 = 0;
	SSPCON1bits.SSPM0 = 0;
	
	//since Fosc = 4MHz, SCK = 500Khz
	SSPADD = 0x9;

//	while(1)
//	{
	//starting  transmission
	SPPCON2bits.SEN = 1;
	//loading slave address into buf; BF should be high after
	SSPBUF = ADDR;
	//wait until the slave acknowledges reciept (ACKSTAT = 0)
	while(SSPCON2bits.ACKSTAT){};
		
	//send data
	SSPBUF = 0x3;
		
	//wait until the slave acknowledges reciept (ACKSTAT = 0)
	while(SSPCON2bits.ACKSTAT){};
		

	//	OpenI2C(MASTER, SLEW_ON);
	//	StartI2C();
	//	WriteI2c(slave_addr);
	//	if(
//	}


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