#ifndef RENLCD_H
#define RENLCD_H

#include <p18f25k20.h>
#include <stdlib.h>
#include <stdio.h>
#include <delays.h>

void InitializeLCD(void);
void WriteLCD(char*);
void CommandLCD(char);
void SetLine1 (void);
void SetLine2 (void);
void ClearLCD (void);

#endif