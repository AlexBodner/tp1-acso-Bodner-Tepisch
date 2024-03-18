#pragma once
#include "../shell.h" 
#include <stdio.h>
#include <assert.h>
#include <string.h>
void addsExtendedReg(char * restOfInstruction);
void addsImm(char * restOfInstruction);
void addsExtendedReg(char * restOfInstruction);
void addsImm(char * restOfInstruction);
void subsExtendedReg(char * restOfInstruction);
void subsImm(char * restOfInstruction);
void compImm(char * restOfInstruction);
void AndsShiftedReg(char * restOfInstruction);
void EorShiftedReg(char * restOfInstruction);
void OrrShiftedReg(char * restOfInstruction);

