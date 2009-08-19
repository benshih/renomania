
	LIST P=18F1220, F=INHX32 ;directive to define processor and file format
	#include <P18F1220.INC>	 ;processor specific variable definitions

	ORG		0x0004
		
	goto Main

Main:
	clrf	PORTA
	movlw	0x7F
	movwf	ADCON1
	clrf	TRISA
	bsf		PORTA, 0

		END
