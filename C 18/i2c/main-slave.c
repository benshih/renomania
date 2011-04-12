#include <p18f25k20.h>
#include <i2c.h>

#define LED0 LATAbits.LATA0
#define OSCmask 0b10001111
#define Fosc 5 //4MHz

#define ADDR 0x2 //slave address

unsigned char var;

void init(void);

void main(){
	init();
	LED0 = 1; //indicator lamp on
	while(1){
		if(DataRdyI2C()){
			var = getcI2C();
		}	
	}		
}	

void init(){
	//set internal oscillator speed
	OSCCON = (OSCCON&OSCmask)|(Fosc<<3);   

	//port directions
	TRISA = 0x00; //all output
	TRISB = 0x00; //all output
	TRISC = 0x18; //all but RC3,4 are output
}	