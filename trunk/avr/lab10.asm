.equ DDRB=0x17
.equ PORTB=0x18
.equ PINB=0x16
.equ DDRC=0x14
.equ PORTC=0x15
.equ PINC=0x13
.equ DDRD=0x11
.equ PORTD=0x12
.equ PIND=0x10
.equ ADCSRA=0x06
.equ ADMUX=0x07
.equ ADCH=0x05
.equ ADCL=0x04



.org 0x0000
entry:
	;Set up ports
	clr r16
	out DDRC,r16
	ldi r16,0xff
	out DDRD,r16
	ldi r16,0x01
	out DDRB,r16
	out PORTB,r16
	ldi r16,0x00
	out PORTD,r16

	;Set up adc
	ldi r16,0x80 ;ADC enable
	out ADCSRA,r16
	ldi r16,0x20 ;Left align ADC result
	out ADMUX,r16

	ldi r17,0x00
	ldi r18,0x01

main:
	in r16,ADCSRA
	sbr r16,0x40 ;ADC start conversion
	out ADCSRA,r16
adc_wait:
	in r16,ADCSRA
	andi r16,0x10
	cpi r16,0x10
	brne adc_wait ;Wait for conversion to complete
	in r16,ADCH
	out PORTD,r16
	out PORTB,r17 ;Toggle WR on DAC chip
	out PORTB,r18
	rjmp main
