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

Rout		EQU		0x20
Gout		EQU		0x21
Bout		EQU		0x22		
Rcount		EQU		0x25
Gcount		EQU		0x26
Bcount		EQU		0x27
STATE 		EQU		0x70		;Bit 0 is servo run bit
LedLoop		EQU		0x71		;Set to 250
COMP		EQU		0x7D
TEMPH		EQU		0x7E
DELAY		EQU		0x7F

	ORG 	0x000
	goto 	initPORTS
	
	ORG 	0x004
	decfsz	REPEATER
	bsf		STATE, 	0			;Servo run bit on
	bcf		PIR1, 	TMR1IF		;Must turn off manually
	movlw	0x2B				;Setting these two registers
	movwf	TMR1L				;will make the interrupt go 
	movlw	0xCF				;off every 20ms.
	movwf	TMR1H
	retfie		
	
initPORTS
	Bank0
	clrf	PORTA
	clrf 	PORTB
	Bank1
	clrf	TRISA
	clrf	TRISB			
	goto 	initRegs
	
initRegs
	Bank0
	movlw	d'83'
	movwf	Rcount
	movlw	d'170'
	movwf	Gcount
	clrf	Bcount
	clrf	STATE
	clrf	TEMPH
	movlw	d'255'
	movwf	LedLoop
	clrf	Rout
	clrf	Gout
	clrf	Bout
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
	bsf		PIE1, 	TMR1IE		;Enable the tmr1 interrupt
	Bank0
	bsf		T1CON, 	TMR1ON		;Timer1 on
	bsf		INTCON, PEIE
	bsf		INTCON, GIE			;Global interrupt enabled	
	goto 	COLORout
	
main
	btfss 	STATE, 	0
	goto	main
	goto 	servoON
servoON
	Bank0
	bcf		STATE, 	0
	movlw	b'00000111'
	movwf	PORTA
	movlw	d'255'
	movwf	LedLoop
	goto	LedRUN
	
LedRUN						;make sure this loop is 18 cycles
	decf	Rout, 	1			;Red LED
	btfsc	STATUS, Z
	bcf		PORTA, 	0
	decf	Gout, 	1			;Green LED
	btfsc	STATUS, Z
	bcf		PORTA, 	1
	decf	Bout, 	1			;Blue LED
	btfsc	STATUS, Z
	bcf		PORTA,	2			
	nop
	nop
	nop
	nop
	nop
	nop
	nop	
	decfsz	LedLoop
	goto 	LedRUN
	goto 	COLORout
COLORout						;move new color values 
	Bank0						;into respective registers
	movlw	b'00100000'
	incf	Rcount,	1
	btfsc	STATUS, C
	xorwf	STATE,	1
	movlw	b'01000000'
	incf	Gcount,	1
	btfsc	STATUS, C
	xorwf	STATE, 	1
	movlw	b'10000000'
	incf	Bcount,	1
	btfsc	STATUS, C
	xorwf	STATE,	1
	
	movf	Rcount,	0
	btfss	STATE,	5
	comf	Rcount,	0
	movwf	Rout
	movf	Gcount,	0
	btfss	STATE,	6
	comf	Gcount,	0
	movwf	Gout
	movf	Bcount,	0
	btfss	STATE,	7
	comf	Bcount,	0
	movwf	Bout
		
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
	
	END