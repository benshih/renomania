# MPLAB IDE generated this makefile for use with GNU make.
# Project: irLCD.mcp
# Date: Mon Aug 31 16:22:18 2009

AS = MPASMWIN.exe
CC = mcc18.exe
LD = mplink.exe
AR = mplib.exe
RM = rm

irLCD.cof : main.o RenLCD.o
	$(LD) /l"C:\mcc18\lib" /k"C:\mcc18\lkr" "..\..\..\..\mcc18\lkr\18f25k20.lkr" "main.o" "RenLCD.o" /o"irLCD.cof" /M"irLCD.map" /W

main.o : main.c ../../../../mcc18/h/stdlib.h ../../../../mcc18/h/stdio.h ../../../../mcc18/h/delays.h RenLCD.h main.c ../../../../mcc18/h/p18f25k20.h ../../../../mcc18/h/stdarg.h ../../../../mcc18/h/stddef.h ../../../../mcc18/h/p18cxxx.h
	$(CC) -p=18F25K20 /i"C:\mcc18\h" "main.c" -fo="main.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

RenLCD.o : RenLCD.c ../../../../mcc18/h/stdlib.h ../../../../mcc18/h/stdio.h ../../../../mcc18/h/delays.h RenLCD.h RenLCD.c ../../../../mcc18/h/p18f25k20.h ../../../../mcc18/h/stdarg.h ../../../../mcc18/h/stddef.h ../../../../mcc18/h/p18cxxx.h
	$(CC) -p=18F25K20 /i"C:\mcc18\h" "RenLCD.c" -fo="RenLCD.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

clean : 
	$(RM) "main.o" "RenLCD.o" "irLCD.cof" "irLCD.hex"

