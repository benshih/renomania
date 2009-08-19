	list		p=16f877a			; list directive to define processor
	#include	<p16f877a.inc>		; processor specific variable definitions
	
	__CONFIG _CP_OFF & _WDT_OFF & _BODEN_OFF & _PWRTE_ON & _HS_OSC & _WRT_OFF & _LVP_OFF & _CPD_OFF
	
start
	bcf 	STATUS, RP0
	bcf 	STATUS, RP1
	clrf 	PORTA
	clrf 	PORTB
	
	bsf 	STATUS, RP0
	movlw 	0x06				;sets all of portA as digital
	movwf 	ADCON1
	movlw 	0x0E				;sets A1,1,2,3 as input
	movwf 	TRISA
	clrf 	TRISB	

	bcf 	STATUS, RP0
wait	
	movlw 	d'250'	
	call 	delayShort
	movlw 	d'250'	
	call 	delayShort
	movlw 	d'250'	
	call 	delayShort
	movlw 	d'250'	
	call 	delayShort
	movlw 	d'7'
	movwf 	h'75'
	bcf		PORTA, 5
	bsf 	PORTA, 0 			;standby lamp is on
	goto 	standby

standby							
	btfss 	PORTA, 1			;skip if intput is HIGH
	goto 	standby
	goto 	receive

receive
	clrf 	h'76'
	bcf 	PORTA, 0 			;standby lamp is off
	movlw 	d'27'
	call 	delayShort 			;2.7ms
	goto 	accept
accept
	rlf 	h'76'
	btfsc 	PORTA, 1 			;sample
	goto 	one 				;if high
	bcf 	h'76', 0			;if low
	goto 	next 				
one
	bsf 	h'76', 0
next
	movlw 	d'6'
	call 	delayShort
	
	decfsz 	h'75', 1
	goto 	accept

	btfss	PORTA, 2		;save input?
	goto	verify
	movf	h'76', 0
	movwf	h'77'
verify
	movf	h'77', 0
	xorwf	h'76', 1
	movf	h'76', 1
	btfsc	STATUS, Z
	bsf		PORTA, 5

	goto	wait
	
	
;--------------------------
;-------SUBROUTINES--------
;--------------------------
	
delayShort
	movwf 	h'71'
part2
	movlw 	d'48'
	movwf 	h'70'
part1	
	nop
	nop
	decfsz 	h'70', 1
	goto 	part1
	decfsz 	h'71', 1
	goto 	part2
	return

	END                       	; directive 'end of program'