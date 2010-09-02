# MPLAB IDE generated this makefile for use with GNU make.
# Project: lcdFinal.mcp
# Date: Tue Apr 13 23:06:46 2010

AS = MPASMWIN.exe
CC = mcc18.exe
LD = mplink.exe
AR = mplib.exe
RM = rm

lcdFinal.cof : main.o RenLCD.o
	$(LD) /l"C:\mcc18\lib" /k"C:\mcc18\lkr" "..\..\..\..\mcc18\lkr\18f25k20.lkr" "main.o" "RenLCD.o" /u_CRUNTIME /u_DEBUG /z__MPLAB_BUILD=1 /z__MPLAB_DEBUG=1 /o"lcdFinal.cof" /M"lcdFinal.map" /W

main.o : main.c ../../../../mcc18/h/stdlib.h ../../../../mcc18/h/stdio.h ../../../../mcc18/h/delays.h RenLCD.h main.c ../../../../mcc18/h/p18f25k20.h ../../../../mcc18/h/stdarg.h ../../../../mcc18/h/stddef.h ../../../../mcc18/h/p18cxxx.h
	$(CC) -p=18F25K20 /i"C:\mcc18\h" "main.c" -fo="main.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

RenLCD.o : ../irLcd/RenLCD.c ../../../../mcc18/h/stdlib.h ../../../../mcc18/h/stdio.h ../../../../mcc18/h/delays.h ../../../../mcc18/h/timers.h ../irLcd/RenLCD.h ../irLcd/RenLCD.c ../../../../mcc18/h/p18f25k20.h ../../../../mcc18/h/stdarg.h ../../../../mcc18/h/stddef.h ../../../../mcc18/h/p18cxxx.h ../../../../mcc18/h/pconfig.h
	$(CC) -p=18F25K20 /i"C:\mcc18\h" "C:\Nerd Stuff\uC\C 18\irLcd\RenLCD.c" -fo="RenLCD.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

clean : 
	$(RM) "main.o" "RenLCD.o" "lcdFinal.cof" "lcdFinal.hex"

