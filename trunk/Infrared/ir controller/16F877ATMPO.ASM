
	list		p=16f877a	; list directive to define processor
	#include	<p16f877a.inc>	; processor specific variable definitions
	
	__CONFIG _CP_OFF & _WDT_OFF & _BODEN_OFF & _PWRTE_ON & _HS_OSC & _WRT_OFF & _LVP_ON & _CPD_OFF

SIG0 	EQU		0x70

;-----------------------------------;
;---------------MACROs--------------;
;-----------------------------------;
Bank0:	MACRO
		bcf 	STATUS, RP0
		bcf 	STATUS, RP1
		ENDM
Bank1:	MACRO
		bsf 	STATUS, RP0
		bcf 	STATUS, RP1
		ENDM
Bank2:	MACRO
		bcf 	STATUS, RP0
		bsf 	STATUS, RP1
		ENDM
Bank3:	MACRO
		bsf 	STATUS, RP0
		bsf 	STATUS, RP1
		ENDM
;-----------------------------------;
;-------------Main Code-------------;
;-----------------------------------;
	ORG		0x000				;start
	goto	initPORTS
	
	ORG		0x004				;interrupt
	

initPORTS
	Bank0
	clrf	PORTA				
	clrf	PORTB	
	clrf	PORTC			
	Bank1
	clrf	TRISA				;output
	clrf	TRISB				;output
	clrf	TRISC				;output
	goto	initREGS
	
initREGS
	movlw	d'255'
	movwf	SIG0	
	goto	initTMR2
	
initTMR2
	Bank0
	clrf	INTCON				;disable all interrrupts
	clrf	PIR1				;clear TMR2IF
	Bank1
	clrf	PIE1				;clear TMR2IE
	movlw	d'66'				;38kHz with 2.5MHz XTAL
	movwf	PR2					
	Bank0
	movlw	0x00
	movwf	T2CON				;Timer2 off
	goto	initPWM
	
initPWM
	Bank0
	movlw	d'33'
	movwf	CCPR1L
	movlw	0x08
	movwf	CCP1CON
	
main
	goto 	main
;-----------------------------------;
;------------SUBROUTINES------------;
;-----------------------------------;
delay250us
	movlw 	d'124'
	movwf 	h'70'
partA							;about 100 cycles
	nop
	nop
	decfsz 	h'70'
	goto 	part1
	nop
	nop
	nop
	return
		
delay500us
	movlw 	d'249'
	movwf 	h'70'
part1							;about 100 cycles
	nop
	nop
	decfsz 	h'70', 
	goto 	part1
	nop
	nop
	nop
	return
	
delay2ms
	movlw 	d'4'
	movwf 	h'71'
part2
	movlw 	d'249'
	movwf 	h'70'
part3	
	nop
	nop
	decfsz 	h'70', 
	goto 	part3
	nop
	nop
	nop
	decfsz h'71'
	goto 	part2
	return
	END                       ; directive 'end of program'

