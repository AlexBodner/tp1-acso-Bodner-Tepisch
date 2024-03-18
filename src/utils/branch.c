#include "branch.h"


int64_t signExtend26to64(long value) {
    // Verificar si el bit 25 (el bit más significativo de imm26) está establecido
    if (value & (1 << 25)) {
        // Extender el signo
        value |= (-1L << 26);
    }
    return value;
}

void B(char * restOfInstruction){
    puts("B");    
    //imm del 25 al 0 inclusives 

    // guardamos el inmediato 
    char * immStr = malloc(sizeof(char) * (26));
    strncpy(immStr, restOfInstruction , 26);
    int64_t immNum= strtol(immStr, NULL, 2);
    free(immStr);

    immNum = signExtend26to64(immNum) << 2;
    printf("Result %ld\n", immNum);
    NEXT_STATE.PC = CURRENT_STATE.PC+immNum;
    return ;
}

void Br(char * restOfInstruction){
    puts("Br");    
    //ADDS Xn & imm to Xd
    //Rn del 9 al 5 inclusives 

    // guardamos el Rn 
    //guardamos Rn
    char * RnStr = malloc(sizeof(char) * (5));
    strncpy(RnStr, restOfInstruction , 5);
    int RnNum= (int) strtol(RnStr, NULL, 2);
    free(RnStr);
    printf("Result %i\n", RnNum);
    
    NEXT_STATE.PC = CURRENT_STATE.REGS[RnNum];
    return ;
}

void Beq(char * restOfInstruction){
    puts("Beq");    
    //guardamos Target
    char * targetStr = malloc(sizeof(char) * 33); // Ejemplo de 26 bits + terminador nulo.
    strncpy(targetStr, restOfInstruction, 33);
    long targetOffset = strtol(targetStr, NULL, 2);
    free(targetStr);
    
    if (CURRENT_STATE.FLAG_Z == 1) {
        NEXT_STATE.PC = CURRENT_STATE.PC + targetOffset;
    } else {
        // Simplemente avanzar al siguiente conjunto de instrucciones si Z no es 1.
        NEXT_STATE.PC += 4;
    }
    return ;
}

void Bne(char * restOfInstruction){
    puts("Bne");    
    //guardamos Target
    char * targetStr = malloc(sizeof(char) * 33); // Ejemplo de 26 bits + terminador nulo.
    strncpy(targetStr, restOfInstruction, 33);
    long targetOffset = strtol(targetStr, NULL, 2);
    free(targetStr);
    
    if (CURRENT_STATE.FLAG_Z == 0) {
        NEXT_STATE.PC = CURRENT_STATE.PC + targetOffset;
    } else {
        // Simplemente avanzar al siguiente conjunto de instrucciones si Z no es 1.
        NEXT_STATE.PC += 4;
    }
    return ;
}

void Bgt(char * restOfInstruction){
    puts("Bgt");    
    //guardamos Target
    char * targetStr = malloc(sizeof(char) * 33); // Ejemplo de 26 bits + terminador nulo.
    strncpy(targetStr, restOfInstruction, 33);
    long targetOffset = strtol(targetStr, NULL, 2);
    free(targetStr);
    
    if (CURRENT_STATE.FLAG_Z == 0 && CURRENT_STATE.FLAG_N == 0) {
        NEXT_STATE.PC = CURRENT_STATE.PC + targetOffset;
    } else {
        // Simplemente avanzar al siguiente conjunto de instrucciones si Z no es 1.
        NEXT_STATE.PC += 4;
    }
    return ;
}

void Blt(char * restOfInstruction){
    puts("Blt");    
    //guardamos Target
    char * targetStr = malloc(sizeof(char) * 33); // Ejemplo de 26 bits + terminador nulo.
    strncpy(targetStr, restOfInstruction, 33);
    long targetOffset = strtol(targetStr, NULL, 2);
    free(targetStr);
    
    // La instrucción BLT salta si el flag N es 1 (indicando un resultado negativo de la comparación).
    if (CURRENT_STATE.FLAG_N == 1) {
        // Realizar el salto.
        NEXT_STATE.PC = CURRENT_STATE.PC + targetOffset;
    } else {
        // Continuar con la siguiente instrucción.
        NEXT_STATE.PC += 4; // Asumiendo un tamaño de instrucción de 4 bytes.
    }

    return ;
}

void Bge(char * restOfInstruction){
    puts("Bge");    
    //guardamos Target
    char * targetStr = malloc(sizeof(char) * 33); // Ejemplo de 26 bits + terminador nulo.
    strncpy(targetStr, restOfInstruction, 33);
    long targetOffset = strtol(targetStr, NULL, 2);
    free(targetStr);
    
    // La instrucción BGE salta si el flag Z es 1 o si el flag N es 0.
    if (CURRENT_STATE.FLAG_Z == 1 || CURRENT_STATE.FLAG_N == 0) {
        // Realizar el salto.
        NEXT_STATE.PC = CURRENT_STATE.PC + targetOffset;
    } else {
        // Continuar con la siguiente instrucción.
        NEXT_STATE.PC += 4; // Asumiendo un tamaño de instrucción de 4 bytes.
    }

    return ;
}

void Ble(char * restOfInstruction){
    puts("Ble");    
    //guardamos Target
    char * targetStr = malloc(sizeof(char) * 33); // Ejemplo de 26 bits + terminador nulo.
    strncpy(targetStr, restOfInstruction, 33);
    long targetOffset = strtol(targetStr, NULL, 2);
    free(targetStr);
    
    // La instrucción BGE salta si el flag Z es 1 o si el flag N es 0.
    if (CURRENT_STATE.FLAG_Z == 1 || CURRENT_STATE.FLAG_N == 1) {
        // Realizar el salto.
        NEXT_STATE.PC = CURRENT_STATE.PC + targetOffset;
    } else {
        // Continuar con la siguiente instrucción.
        NEXT_STATE.PC += 4; // Asumiendo un tamaño de instrucción de 4 bytes.
    }
    return ;
}