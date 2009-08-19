.equ DDRB=0x17
.equ PORTB=0x18
.equ PINB=0x16
.equ DDRC=0x14
.equ PORTC=0x15
.equ PINC=0x13
.equ DDRD=0x11
.equ PORTD=0x12
.equ PIND=0x10

.equ SPL=0x3d
.equ SPH=0x3e

.equ ADCSRA=0x06
.equ ADMUX=0x07
.equ ADCH=0x05
.equ ADCL=0x04

.org 0x0000
entry:
	;Stack initialization
	ldi r16,0x04
	out SPH,r16
	ldi r16,0x5f
	out SPL,r16

	;Set up ports
	clr r16
	out DDRC,r16 ;PORTC input
	ldi r16,0xff
	out DDRD,r16 ;PORTD output
	ldi r16,0x01
	out DDRB,r16 ;PORTB[0] output
	clr r16
	out PORTD,r16
	out PORTB,r16

	;Set up adc
	ldi r16,0x80 ;ADC enable
	out ADCSRA,r16
	ldi r16,0x20 ;Left align ADC result
	out ADMUX,r16

main:
	clr	r25
incr:
	inc r25
	cpi	r25, 0x00
	breq next
	out PORTD, r25
	rjmp incr
next:
	dec r25
	rjmp decr
decr:
	dec r25
	cpi r25, 0xFF
	breq main
	out PORTD, r25
	rjmp decr
	rjmp main

adc_read:
	in r25,ADCSRA
	sbr r25,0x40 ;ADC start conversion
	out ADCSRA,r25
adc_wait:
	in r25,ADCSRA
	andi r25,0x10
	cpi r25,0x10
	brne adc_wait ;Wait for conversion to complete
	in r25,ADCH
	ret

dac_write:
	out PORTD,r25
	clr r25
	out PORTB,r25 ;Toggle WR on DAC chip
	ldi r25,0x01
	out PORTB,r25
	ret
