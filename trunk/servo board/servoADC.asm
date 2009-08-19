	list		p=16f877a	; list directive to define processor
	#include	<p16f877a.inc>	; processor specific variable definitions
	
	__CONFIG _CP_OFF & _WDT_OFF & _BODEN_OFF & _PWRTE_ON & _HS_OSC & _WRT_OFF & _LVP_OFF & _CPD_OFF

Bank0:		MACRO
			bcf 	STATUS, RP0
			bcf 	STATUS, RP1
			ENDM
Bank1:		MACRO
			bsf 	STATUS, RP0
			bcf 	STATUS, RP1
			ENDM
Bank2:		MACRO
			bcf 	STATUS, RP0
			bsf 	STATUS, RP1
			ENDM
Bank3:		MACRO
			bsf 	STATUS, RP0
			bsf 	STATUS, RP1
			ENDM

SERVO0		EQU		0x20
SERVO1		EQU		0x21
SERVO2		EQU		0x22		
SERVO3		EQU		0x23
SERVO4		EQU		0x24
STATE 		EQU		0x70		;Bit 0 is servo run bit
ServoLoop	EQU		0x71		;Set to 250
TEMPH		EQU		0x7E
DELAY		EQU		0x7F

	ORG 	0x000
	goto 	initPORTS
	
	ORG 	0x004
	bsf		STATE, 0			;Servo run bit on
	bcf		PIR1, TMR1IF		;Must turn off manually
	movlw	0xB0				;Setting these two registers
	movwf	TMR1L				;will make the interrupt go 
	movlw	0x3C				;off every 20ms.
	movwf	TMR1H				;
	retfie		
	
initPORTS
	Bank0
	clrf	PORTA
	clrf 	PORTB
	clrf	STATE
	Bank1
	movlw	0x0B
	movwf	TRISA				;Input: RA0,1,3
	clrf	TRISB				;Input: None
	goto	initADC

initADC
	Bank1
	movlw	0x04   				;AN0,1,3
	movwf 	ADCON1 				;Vref+ = Vcc
								;Left just
	goto 	initRegs
	
initRegs
	Bank0
	clrf	SERVO0
	clrf	SERVO1
	clrf	SERVO2
	clrf	SERVO3
	clrf	SERVO4
	clrf	STATE
	clrf	TEMPH
	movlw	d'250'
	movwf	ServoLoop
	goto 	initTMR1

initTMR1
	Bank0
	clrf 	INTCON				;Turn off interrupts and flags
	clrf 	PIR1				;Clear TMR1IF 
	Bank1
	clrf	PIE1				;Clear TMR1IE
	Bank0
	movlw	0x04 				;Timer off		 , internal Fosc/4
	movwf 	T1CON				;Don't syncronize, prescaler is 1:1
	Bank1
	bsf		PIE1, TMR1IE		;Enable the tmr1 interrupt
	Bank0
	bsf		T1CON, TMR1ON		;Timer1 on
	bsf		INTCON, PEIE
	bsf		INTCON, GIE			;Global interrupt enabled	
	goto 	servoON
	
main
	btfss 	STATE, 0
	goto	main
	goto 	servoON
servoON
	Bank0
	bcf		STATE, 0
	movlw	b'00000011'
	movwf	PORTB
	movlw	d'250'
	call	delay400us
	movwf	ServoLoop
	goto	servoRUN
	
servoRUN						;make sure this loop is 18 cycles
	decf	SERVO0, 1			;servo 0
	btfsc	STATUS, Z
	bcf		PORTB, 0
	decf	SERVO1, 1			;servo 1
	btfsc	STATUS, Z
	bcf		PORTB, 1
	decf	SERVO2, 1			;servo 2
	btfsc	STATUS, Z
	bcf		PORTB, 2
	decf	SERVO3, 1			;servo 3
	btfsc	STATUS, Z
	bcf		PORTB, 3
	decf	SERVO4, 1			;servo 4
	btfsc	STATUS, Z
	bcf		PORTB, 4
	decfsz	ServoLoop
	goto 	servoRUN
	goto 	ADC0
ADC0
	Bank0
	movlw	0x01				;Fosc/2, AN0, no GO, ADON
	movwf	ADCON0 	
	call	delay50us							 
	bsf		ADCON0, 2 			;GO!
	call 	checkDONE
	Bank0
	movf	ADRESH, 0			;Truncating the 2 lower most bits
	movwf	SERVO0				;increases servo stability
	bcf		SERVO0, 0
	bcf		SERVO0, 1
	goto	ADC1
ADC1
	Bank0
	movlw	0x09				;Fosc/2, AN1, no GO, ADON
	movwf	ADCON0 	
	call	delay50us							 
	bsf		ADCON0, 2 			;GO!
	call 	checkDONE
	Bank0
	movf	ADRESH, 0			;Truncating the 2 lower most bits
	movwf	SERVO1				;increases servo stability
	bcf		SERVO1, 0
	bcf		SERVO1, 1
	goto	main

	
delay400us 						;1000 xtal cycles
	movlw 	d'249'
	movwf 	DELAY
pt0
	nop
	nop
	decfsz 	DELAY, 1
	goto 	pt0
	nop
	nop
	nop
	return
	
delay50us 						;125 xtal cycles
	movlw 	d'24'
	movwf 	DELAY
pt1
	nop
	nop
	decfsz 	DELAY, 1
	goto 	pt1
	nop
	nop
	nop
	return
	
checkDONE
a
	btfsc 	ADCON0, 2 			;done?
	goto 	a 					;if not
	return
	
	
neutral
	Bank0
	movlw 	d'125'
	movwf	SERVO0
	movlw 	d'125'
	movwf	SERVO1
	movlw 	d'125'
	movwf	SERVO2
	movlw 	d'125'
	movwf	SERVO3
	movlw 	d'125'
	movwf	SERVO4
	return
	
	END