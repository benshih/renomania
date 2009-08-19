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
	movlw 	0x02				;sets just A1 as input
	movwf 	TRISA
	clrf 	TRISB
	
	bcf 	STATUS, RP0
	
wait	
	clrf 	PORTB
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
	bsf 	PORTA, 0 			;standby lamp is on
	goto 	standby

standby				
	btfss 	PORTA, 1			;skip if intput is HIGH
	goto 	standby
	goto 	receive

receive
	bcf 	PORTA, 0 			;standby lamp is off
	movlw 	d'39'
	call 	delayShort 			;3.9ms
	goto 	accept
save
	rlf 	PORTB
	goto 	accept
accept
	btfsc 	PORTA, 1 			;is it high again?
	goto 	one1 				;if low
	bcf 	PORTB, 0
	goto 	next1 				;if high
one1
	bsf 	PORTB, 0
	movlw 	d'6'
	call 	delayShort
next1
	movlw 	d'12'
	call 	delayShort
	
	decfsz 	h'75', 1
	goto 	save
stop
	movlw 	d'250'	
	call 	delayShort
	movlw 	d'250'	
	call 	delayShort
	movlw 	d'250'	
	call 	delayShort
	movlw 	d'250'	
	call 	delayShort
	movlw 	d'250'	
	call 	delayShort
	movlw 	d'250'	
	call 	delayShort
	movlw 	d'250'	
	call 	delayShort
	movlw 	d'250'	
	call 	delayShort
	goto 	wait
;-----------------------
;----SUBROUTINES--------
;-----------------------
	
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


	

	
