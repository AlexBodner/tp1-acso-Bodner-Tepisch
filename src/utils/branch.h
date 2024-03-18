#pragma once
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../shell.h"


void B(char * restOfInstruction);
void Br(char * restOfInstruction);
void Beq(char * restOfInstruction);
void Bne(char * restOfInstruction);
void Bgt(char * restOfInstruction);
void Blt(char * restOfInstruction);
void Bge(char * restOfInstruction);
void Ble(char * restOfInstruction);