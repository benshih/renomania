	list		p=16f877a	; list directive to define processor
	#include	<p16f877a.inc>	; processor specific variable definitions
	
	__CONFIG _CP_OFF & _WDT_OFF & _BODEN_OFF & _PWRTE_ON & _HS_OSC & _WRT_OFF & _LVP_OFF & _CPD_OFF

start

	bcf STATUS, RP0
	bcf STATUS, RP1
	clrf PORTA
	clrf PORTB
	
	bsf STATUS, RP0
	movlw 0x06	;sets all of portA as digital
	movwf ADCON1
	movlw 0x02	;sets just A1 as input
	movwf TRISA
	clrf TRISB
	
	bcf STATUS, RP0
	
wait	
	clrf PORTB
	movlw d'250'	
	call delay1ms
	movlw d'250'	
	call delay1ms
	movlw d'250'	
	call delay1ms
	movlw d'250'	
	call delay1ms
	bsf PORTA, 0 ;standby lamp is on
	goto standby

standby				
	btfss PORTA, 1 ;skip is intput is HIGH
	goto standby
	goto receive

receive
	bcf PORTA, 0 ;standby lamp is off
	movlw d'39'
	call delay1ms ;3.9ms
	
;first signal
	btfsc PORTA, 1 ;is it high again?
	goto one1 ;if not high
	bcf PORTB, 7 
	goto next1 ;if high
one1
	bsf PORTB, 7
	movlw d'6'
	call delay1ms
next1
	movlw d'12'
	call delay1ms
	
;second signal
	btfsc PORTA, 1 ;is it high again?
	goto one2 ;if not high
	bcf PORTB, 6 
	goto next2 ;if high
one2
	bsf PORTB, 6
	movlw d'6'
	call delay1ms
next2
	movlw d'12'
	call delay1ms

;third signal
	btfsc PORTA, 1 ;is it high again?
	goto one3 ;if not high
	bcf PORTB, 5 
	goto next3 ;if high
one3
	bsf PORTB, 5
	movlw d'6'
	call delay1ms
next3
	movlw d'12'
	call delay1ms

;fourth signal	
	btfsc PORTA, 1 ;is it high again?
	goto one4 ;if not high
	bcf PORTB, 4 
	goto next4 ;if high
one4
	bsf PORTB, 4
	movlw d'6'
	call delay1ms
next4
	movlw d'12'
	call delay1ms

;fifth signal
	btfsc PORTA, 1 ;is it high again?
	goto one5 ;if not high
	bcf PORTB, 3 
	goto next5 ;if high
one5
	bsf PORTB, 3
	movlw d'6'
	call delay1ms
next5
	movlw d'12'
	call delay1ms

;sixth signal		
	btfsc PORTA, 1 ;is it high again?
	goto one6 ;if not high
	bcf PORTB, 2 
	goto next6 ;if high
one6
	bsf PORTB, 2
	movlw d'6'
	call delay1ms
next6
	movlw d'12'
	call delay1ms
		
;seventh signal
	btfsc PORTA, 1 ;is it high again?
	goto one7 ;if not high
	bcf PORTB, 1 
	goto next7 ;if high
one7
	bsf PORTB, 1
	movlw d'6'
	call delay1ms
next7
	movlw d'12'
	call delay1ms

;eighth signal
	btfsc PORTA, 1 ;is it high again?
	goto one8 ;if not high
	bcf PORTB, 0 
	goto next8 ;if high
one8
	bsf PORTB, 0
	movlw d'6'
	call delay1ms
next8
	movlw d'12'
	call delay1ms

stop
	movlw d'250'	
	call delay1ms
	movlw d'250'	
	call delay1ms
	movlw d'250'	
	call delay1ms
	movlw d'250'	
	call delay1ms
	movlw d'250'	
	call delay1ms
	movlw d'250'	
	call delay1ms
	movlw d'250'	
	call delay1ms
	movlw d'250'	
	call delay1ms
	movlw d'250'	
	call delay1ms
	movlw d'250'	
	call delay1ms
	movlw d'250'	
	call delay1ms
	movlw d'250'	
	call delay1ms
	movlw d'250'	
	call delay1ms
	movlw d'250'	
	call delay1ms
	movlw d'250'	
	call delay1ms
	movlw d'250'	
	call delay1ms
	goto wait
;------------------
;----SUBROUTINES---
;------------------
	
delay1ms
	movwf h'71'
part2
	movlw d'48'
	movwf h'70'
part1	;about 1000 cycles
	nop
	nop
	decfsz h'70', 1
	goto part1
	decfsz h'71', 1
	goto part2
	return
	
	END                       	; directive 'end of program'


	

	
