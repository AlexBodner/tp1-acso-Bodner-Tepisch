#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "../shell.h"
#include <stdlib.h>

void B(char * restOfInstruction);
void Br(char * restOfInstruction);
void Bcond(char * restOfInstruction);
void Cbz(char * restOfInstruction);
void Cbnz(char * restOfInstruction);