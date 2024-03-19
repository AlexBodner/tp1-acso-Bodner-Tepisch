#pragma once
#include "../shell.h" 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void addsExtendedReg(char * restOfInstruction);
void addsImm(char * restOfInstruction);
void subs_compExtendedReg(char * restOfInstruction);
void subs_compImm(char * restOfInstruction);
void AndsShiftedReg(char * restOfInstruction);
void EorShiftedReg(char * restOfInstruction);
void OrrShiftedReg(char * restOfInstruction);
void addImmediate(char * restOfInstruction);
void addExtendedReg(char * restOfInstruction);
void subImmediate(char * restOfInstruction);
void subExtendedReg(char * restOfInstruction);
void mul(char * restOfInstruction);
