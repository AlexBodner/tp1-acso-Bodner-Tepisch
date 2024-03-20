#include "branch.h"


int64_t signExtend26to64(long value) {
    // Verificar si el bit 25 (el bit más significativo de imm26) está establecido
    if (value & (1 << 25)) {
        // Extender el signo
        value |= (-0UL << 26);
    }
    return value;
}

int64_t signExtendNto64(int64_t value,int n) {
    // Verificar si el bit n-1 (el bit más significativo de imm26) está establecido
    if (value & (1 << (n-1))) {
        // Extender el signo
        value |= (-1L << 64-n);
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

int calculate_branch_offset(int imm19) {
    // Sign extend the 19-bit immediate value to a 64-bit integer
    int64_t offset = (int64_t)imm19 << 45;  // Shift left to position the sign bit
    offset >>= 45;  // Arithmetic shift right to sign-extend
    offset *= 4;  // Multiply by 4 to account for the '00' at the end
    return offset;
}

void Bcond(char * restOfInstruction){
    puts("Bcond");    
    //imm del 23 al 5 inclusives 
    //cond del 3 al 0 inclusives 
    //guardamos cond
    //guardamos el inmediato 
    char * immStr = malloc(sizeof(char) * (19));
    strncpy(immStr, restOfInstruction , 19);
    int immNum= strtol(immStr, NULL, 2);
    free(immStr);
    immNum = calculate_branch_offset(immNum);

    char * condStr = malloc(sizeof(char) * (4));
    strncpy(condStr, restOfInstruction+20, 4);
    int condNum= (int) strtol(condStr, NULL, 2);
    free(condStr);
    
    printf("Result %i\n", condNum);
    printf("Result %d\n", immNum);
    if (condNum == 0){
        printf("BEQ\n");
        if (CURRENT_STATE.FLAG_Z == 1){
            NEXT_STATE.PC += immNum;
        }
        else{
            //no hacer nada porque no es la instruccion adecuada? preguntar
            NEXT_STATE.PC += 4;
        };
        return;
    }
    else if (condNum == 1){
        printf("BNE\n");
        if (CURRENT_STATE.FLAG_Z == 0){
            NEXT_STATE.PC += immNum;
        }
        else{
            //no hacer nada porque no es la instruccion adecuada? preguntar
            NEXT_STATE.PC += 4;
        };
        return;
    }
    else if (condNum == 12){
        printf("BGT\n");
        if (CURRENT_STATE.FLAG_N == 0){
            NEXT_STATE.PC += immNum;
        }
        else{
            //no hacer nada porque no es la instruccion adecuada? preguntar
            NEXT_STATE.PC += 4;
        };
        return;
    }
    else if (condNum == 11){
        printf("BLT\n");
        if (CURRENT_STATE.FLAG_N != 0){
            NEXT_STATE.PC += immNum;
        }
        else{
            //no hacer nada porque no es la instruccion adecuada? preguntar
            NEXT_STATE.PC += 4;
        };
        return;
    }
    else if (condNum == 10){
        printf("BGE\n");
        if (CURRENT_STATE.FLAG_N == 0){
            NEXT_STATE.PC += immNum;
        }
        else{
            //no hacer nada porque no es la instruccion adecuada? preguntar
            NEXT_STATE.PC += 4;
        };
        return;
    }
    else if (condNum == 13){
        printf("BLE\n");
        if (!(CURRENT_STATE.FLAG_N == 0)){
            NEXT_STATE.PC += immNum;
        }
        else{
            //no hacer nada porque no es la instruccion adecuada? preguntar
            NEXT_STATE.PC += 4;
        };
        return;
    }
    NEXT_STATE.PC += 4;
    return ;
}



//---------------------EXTRAS---------------------------


void Cbz(char * restOfInstruction){
    puts("Cbz");    
    char * immStr = malloc(sizeof(char) * (19));
    strncpy(immStr, restOfInstruction, 19);
    int64_t immNum = strtol(immStr, NULL, 2);
    immNum = signExtendNto64(immNum,19);
    free(immStr);

    //guardamos Rm
    char * RtStr = malloc(sizeof(char) * (5));
    strncpy(RtStr, restOfInstruction+19, 5);
    int RtNum= (int) strtol(RtStr, NULL, 2);
    int64_t rtContent = CURRENT_STATE.REGS[RtNum];
    free(RtStr);
    if (rtContent == 00){
        NEXT_STATE.PC += immNum* 4;

    }   
    else {
        // Simplemente avanzar al siguiente conjunto de instrucciones si Z no es 1.
        NEXT_STATE.PC += 4;
    }
    return ;
}

void Cbnz(char * restOfInstruction){
    puts("Cbz");    
    char * immStr = malloc(sizeof(char) * (19));
    strncpy(immStr, restOfInstruction, 19);
    int64_t immNum = strtol(immStr, NULL, 2);
    immNum = signExtendNto64(immNum,19);
    free(immStr);

    //guardamos Rm
    char * RtStr = malloc(sizeof(char) * (5));
    strncpy(RtStr, restOfInstruction+19, 5);
    int RtNum= (int) strtol(RtStr, NULL, 2);
    int64_t rtContent = CURRENT_STATE.REGS[RtNum];
    free(RtStr);
    if (rtContent != 00){
        NEXT_STATE.PC += immNum* 4;

    }   
    else {
        // Simplemente avanzar al siguiente conjunto de instrucciones si Z no es 1.
        NEXT_STATE.PC += 4;
    }
    return ;
}

