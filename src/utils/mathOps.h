#pragma once
#include "../shell.h" 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

void addsExtendedReg(char * restOfInstruction);
void addsImm(char * restOfInstruction);
void subs_compExtendedReg(char * restOfInstruction);
void subs_compImm(char * restOfInstruction);
void AndsShiftedReg(char * restOfInstruction);
void EorShiftedReg(char * restOfInstruction);
void OrrShiftedReg(char * restOfInstruction);
void AddImmediate(char * restOfInstruction);
void AddExtendedReg(char * restOfInstruction);
void subImmediate(char * restOfInstruction);
void subExtendedReg(char * restOfInstruction);
void mul(char * restOfInstruction);
