#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "shell.h"
#include "HashMap/HashMap.h"

// #include "utils/branch.h"
// #include "utils/load_store.h"
// #include "utils/mathOps.h"
// #include "utils/shift.h"

int opLengths[] ={0,4+1, 5+1, 6+1,7+1,8+1,9+1,10+1,11+1,23} ;

void HLT(char * restOfInstruction){
    puts("hlt");
    RUN_BIT = 0;
    NEXT_STATE.PC+= 4;
    return ;
}

void addsExtendedReg(char * restOfInstruction){
    puts("addsExtendedReg");
    //ADDS Xn + Xm to Xd

    //guardamos Rm
    char * RmStr = malloc(sizeof(char) * (5));
    strncpy(RmStr, restOfInstruction, 5);
    int RmNum= (int) strtol(RmStr, NULL, 2);
    int rmContent = CURRENT_STATE.REGS[RmNum];


    //guardamos Rn
    char * RnStr = malloc(sizeof(char) * (5));
    strncpy(RnStr, restOfInstruction+5+6 , 5);
    int RnNum= (int) strtol(RnStr, NULL, 2);
    int rnContent = CURRENT_STATE.REGS[RnNum];

    //guardamos Rd
    char * RdStr = malloc(sizeof(char) * (5));
    strncpy(RdStr, restOfInstruction+5+6+5 , 5);
    int RdNum= (int) strtol(RdStr, NULL, 2);

    //Hacemos la operacion
    int result = rnContent + rmContent;
    printf("Rd %i\n", RdNum);
    NEXT_STATE.FLAG_N = (result < 0);  // Si result es menor que 0, FLAG_N se establece a 1 (verdadero), de lo contrario se establece a 0 (falso)
    NEXT_STATE.FLAG_Z = (result == 0); // Si result es igual a 0, FLAG_Z se establece a 1 (verdadero), de lo contrario se establece a 0 (falso)


    NEXT_STATE.REGS[RdNum]  = result;
    printf("Result %i\n", result);
    NEXT_STATE.PC+= 4;
    free(RmStr);
    free(RnStr);
    free(RdStr);

    return ;
}

void addsImm(char * restOfInstruction){
    puts("addsImm");
    //ADDS Xn + imm to Xd
    //bits 23 y 22 son de shift y deberian ser 00 
    //imm del 21 al 10 inclusives 
    //Rn del 9 al 5 
    //Rd 4 al 0

    // guardamos el inmediato 
    char * immStr = malloc(sizeof(char) * (12));
    strncpy(immStr, restOfInstruction+2 , 12);
    int immNum= (int) strtol(immStr, NULL, 2);

    char * shiftBytes = malloc(sizeof(char) * (2));
    strncpy(shiftBytes, restOfInstruction , 2);
    int shiftStatus = strcmp(shiftBytes,"01");
    int shiftStatus2 = strcmp(shiftBytes,"00");
    if (shiftStatus ==0){
        //hacemos shift 12 a la izq
        immNum = immNum << 12;
    }
    else if(shiftStatus2 ==0){
        //hacemos sin shift
    } 
    else{
        //no hacer nada porque no es la instruccion adecuada? preguntar
    }

    //guardamos Rn
    char * RnStr = malloc(sizeof(char) * (5));
    strncpy(RnStr, restOfInstruction+14 , 5);
    int RnNum= (int) strtol(RnStr, NULL, 2);
    int rnContent = CURRENT_STATE.REGS[RnNum];

    //guardamos Rd
    char * RdStr = malloc(sizeof(char) * (5));
    strncpy(RdStr, restOfInstruction+14+5 , 5);
    int RdNum= (int) strtol(RdStr, NULL, 2);

    //Hacemos la operacion
    int result = rnContent + immNum;
    printf("Rd %i\n", RdNum);
    NEXT_STATE.FLAG_N = (result < 0);  // Si result es menor que 0, FLAG_N se establece a 1 (verdadero), de lo contrario se establece a 0 (falso)
    NEXT_STATE.FLAG_Z = (result == 0); // Si result es igual a 0, FLAG_Z se establece a 1 (verdadero), de lo contrario se establece a 0 (falso)


    NEXT_STATE.REGS[RdNum]  = result;
    printf("Result %i\n", result);
    NEXT_STATE.PC+= 4;
    free(shiftBytes);
    free(immStr);
    free(RnStr);
    free(RdStr);
    return ;
}


void subs_compExtendedReg(char * restOfInstruction){
    puts("subs_compExtendedReg");
        //guardamos Rm
    char * RmStr = malloc(sizeof(char) * (5));
    strncpy(RmStr, restOfInstruction, 5);
    int RmNum= (int) strtol(RmStr, NULL, 2);
    int rmContent = CURRENT_STATE.REGS[RmNum];


    //guardamos Rn
    char * RnStr = malloc(sizeof(char) * (5));
    strncpy(RnStr, restOfInstruction+5+6 , 5);
    int RnNum= (int) strtol(RnStr, NULL, 2);
    int rnContent = CURRENT_STATE.REGS[RnNum];

    //guardamos Rd
    char * RdStr = malloc(sizeof(char) * (5));
    strncpy(RdStr, restOfInstruction+5+6+5 , 5);
    int RdNum= (int) strtol(RdStr, NULL, 2);

    //Hacemos la operacion
    int result =   rnContent-rmContent;
    printf("Rd %i\n", RdNum);
    NEXT_STATE.FLAG_N = (result < 0);  // Si result es menor que 0, FLAG_N se establece a 1 (verdadero), de lo contrario se establece a 0 (falso)
    NEXT_STATE.FLAG_Z = (result == 0); // Si result es igual a 0, FLAG_Z se establece a 1 (verdadero), de lo contrario se establece a 0 (falso)
    
    if (RdNum == 31){
        puts("cmpExtendedReg");
        NEXT_STATE.PC+= 4;
    }
    else{
        puts("subsExtendedReg");
        NEXT_STATE.REGS[RdNum]  = result;
        printf("Result %i\n", result);
        NEXT_STATE.PC+= 4;
    }
    free(RmStr);
    free(RnStr);
    free(RdStr);
    return ;
}


void subs_compImm(char * restOfInstruction){
    // Revisar como se guarda la resta
    puts("subsImm");
    
    //ADDS Xn - imm to Xd
    //bits 23 y 22 son de shift y deberian ser 00 
    //imm del 21 al 10 inclusives 
    //Rn del 9 al 5 
    //Rd 4 al 0
    // guardamos el inmediato 
    char * immStr = malloc(sizeof(char) * (12));
    strncpy(immStr, restOfInstruction+2 , 12);
    int immNum= (int) strtol(immStr, NULL, 2);


    char * shiftBytes = malloc(sizeof(char) * (2));
    strncpy(shiftBytes, restOfInstruction , 2);
    int shiftStatus = strcmp(shiftBytes,"01");
    int shiftStatus2 = strcmp(shiftBytes,"00");
    if (shiftStatus ==0){
        //hacemos shift 12 a la izq
        immNum = immNum << 12;
    }
    else if(shiftStatus2 ==0){
        //hacemos sin shift
    } 
    else{
        //no hacer nada porque no es la instruccion adecuada? preguntar
    }

    //guardamos Rn
    char * RnStr = malloc(sizeof(char) * (5));
    strncpy(RnStr, restOfInstruction+14 , 5);
    int RnNum= (int) strtol(RnStr, NULL, 2);
    int rnContent = CURRENT_STATE.REGS[RnNum];

    //guardamos Rd
    char * RdStr = malloc(sizeof(char) * (5));
    strncpy(RdStr, restOfInstruction+14+5 , 5);
    int RdNum= (int) strtol(RdStr, NULL, 2);

    

    //Hacemos la operacion
    int result = rnContent - immNum;
    printf("Resultado de rest %i \n", result);
    NEXT_STATE.FLAG_N = (result < 0);  // Si result es menor que 0, FLAG_N se establece a 1 (verdadero), de lo contrario se establece a 0 (falso)
    NEXT_STATE.FLAG_Z = (result == 0); // Si result es igual a 0, FLAG_Z se establece a 1 (verdadero), de lo contrario se establece a 0 (falso)

    if (RdNum == 31){
        puts("cmpImm");
        NEXT_STATE.PC+= 4;
    }
    else{
        puts("subsImm");
        NEXT_STATE.REGS[RdNum]  = result;
        printf("Result %i\n", result);
        NEXT_STATE.PC+= 4;
    }
    free(immStr);
    free(RnStr);
    free(RdStr);
    free(shiftBytes);
    return ;
}

void compImm(char * restOfInstruction){
    puts("compImm");
    //CMP Xn - imm 
    char * immStr = malloc(sizeof(char) * (12));
    strncpy(immStr, restOfInstruction+2 , 12);
    int immNum= (int) strtol(immStr, NULL, 2);


    char * shiftBytes = malloc(sizeof(char) * (2));
    strncpy(shiftBytes, restOfInstruction , 2);
    int shiftStatus = strcmp(shiftBytes,"01");
    int shiftStatus2 = strcmp(shiftBytes,"00");
    if (shiftStatus ==0){
        //hacemos shift 12 a la izq
        immNum = immNum << 12;
    }
    else if(shiftStatus2 ==0){
        //hacemos sin shift
    } 
    else{
        //no hacer nada porque no es la instruccion adecuada? preguntar
    }

    //guardamos Rn
    char * RnStr = malloc(sizeof(char) * (5));
    strncpy(RnStr, restOfInstruction+14 , 5);
    int RnNum= (int) strtol(RnStr, NULL, 2);
    int rnContent = CURRENT_STATE.REGS[RnNum];

    //guardamos Rd
    char * RdStr = malloc(sizeof(char) * (5));
    strncpy(RdStr, restOfInstruction+14+5 , 5);
    int RdNum= (int) strtol(RdStr, NULL, 2);



    //Hacemos la operacion
    int result = rnContent - immNum;
    printf("Resultado de rest %i \n", result);
    NEXT_STATE.FLAG_N = (result < 0);  // Si result es menor que 0, FLAG_N se establece a 1 (verdadero), de lo contrario se establece a 0 (falso)
    NEXT_STATE.FLAG_Z = (result == 0); // Si result es igual a 0, FLAG_Z se establece a 1 (verdadero), de lo contrario se establece a 0 (falso)


    printf("Result %i\n", result);
    NEXT_STATE.PC+= 4;
    free(immStr);
    free(RnStr);
    free(RdStr);
    free(shiftBytes);
    return ;
}

void AndsShiftedReg(char * restOfInstruction){
    puts("AndsShiftedReg");    
    //ADDS Xn & imm to Xd
    //bits 23 y 22 son de shift y deberian ser 00 
    //N es 0 en 21
    //Rm del 20 al 16 
    //imm del 15 al 10 inclusives 
    //Rn del 9 al 5 
    //Rd 4 al 0
    
    // guardamos el Rm 
    char * RmStr = malloc(sizeof(char) * (5));
    strncpy(RmStr, restOfInstruction+3 , 5);
    uint64_t RmNum= (uint64_t) strtol(RmStr, NULL, 2);

    // guardamos el inmediato 
    char * immStr = malloc(sizeof(char) * (6));
    strncpy(immStr, restOfInstruction+8 , 6);
    uint64_t immNum= (uint64_t) strtol(immStr, NULL, 2);

    //guardamos Rn
    char * RnStr = malloc(sizeof(char) * (5));
    strncpy(RnStr, restOfInstruction+14 , 5);
    uint64_t RnNum= (uint64_t) strtol(RnStr, NULL, 2);

    char * RdStr = malloc(sizeof(char) * (5));
    strncpy(RnStr, restOfInstruction+19 , 5);
    uint64_t RdNum= (uint64_t) strtol(RnStr, NULL, 2);
    
    uint64_t results = CURRENT_STATE.REGS[RnNum] & CURRENT_STATE.REGS[RmNum];

    NEXT_STATE.REGS[RdNum] = results;
    
    NEXT_STATE.FLAG_N = ((results>>63) & 1);  // Si result es menor que 0, FLAG_N se establece a 1 (verdadero), de lo contrario se establece a 0 (falso)
    NEXT_STATE.FLAG_Z = (results == 0); // Si result es igual a 0, FLAG_Z se establece a 1 (verdadero), de lo contrario se establece a 0 (falso)

    printf("Result %lu\n", results);
    NEXT_STATE.PC+= 4;
    free(RmStr);
    free(immStr);
    free(RnStr);
    free(RdStr);
    return ;
}

void EorShiftedReg(char * restOfInstruction){
    puts("EorShiftedReg");    
    //ADDS Xn & imm to Xd
    //bits 23 y 22 son de shift y deberian ser 00
    //N es 0 en 21
    //Rm del 20 al 16 
    //imm del 15 al 10 inclusives 
    //Rn del 9 al 5 
    //Rd 4 al 0
    
    // shift
    char * shiftBytes = malloc(sizeof(char) * (2));
    strncpy(shiftBytes, restOfInstruction , 2);
    shiftBytes[2] = '\0';

    int shiftStatusLSL = strcmp(shiftBytes,"00");
    int shiftStatusLSR = strcmp(shiftBytes,"01");

    // guardamos el Rm 
    char * RmStr = malloc(sizeof(char) * (5));
    strncpy(RmStr, restOfInstruction+3 , 5);
    int RmNum= (int) strtol(RmStr, NULL, 2);
    int64_t RmContent = CURRENT_STATE.REGS[RmNum];

    // guardamos el inmediato 
    char * immStr = malloc(sizeof(char) * (6));
    strncpy(immStr, restOfInstruction+8 , 6);
    int immNum= (int) strtol(immStr, NULL, 2);


    //guardamos Rn
    char * RnStr = malloc(sizeof(char) * (5));
    strncpy(RnStr, restOfInstruction+14 , 5);
    int RnNum= (int) strtol(RnStr, NULL, 2);
    uint64_t rnContent = CURRENT_STATE.REGS[RnNum];
    
    char * RdStr = malloc(sizeof(char) * (5));
    strncpy(RdStr, restOfInstruction+19, 5);
    int RdNum= (int) strtol(RdStr, NULL, 2);

    if (shiftStatusLSL ==0){
        puts("LSL");
        //hacemos shift izq
        rnContent << RmContent;
    }
    else if(shiftStatusLSR ==0){
        puts("LSR");
        //hacemos shift der
        rnContent >> RmContent;
    } 
    else{
        //no hacer nada porque no es la instruccion adecuada? preguntar
    }


    //Hacemos la operacion
    uint64_t result = rnContent ^ RmContent;

    NEXT_STATE.REGS[RdNum]  = result;
;
    printf("Result %lu\n", result);
    NEXT_STATE.PC+= 4;
    free(RmStr);
    free(immStr);
    free(RnStr);
    free(RdStr);
    free(shiftBytes);
    return ;
}

void OrrShiftedReg(char * restOfInstruction){
    puts("OrrShiftedReg");    
    //ADDS Xn & imm to Xd
    //bits 23 y 22 son de shift y deberian ser 00
    //N es 0 en 21
    //Rm del 20 al 16 
    //imm del 15 al 10 inclusives 
    //Rn del 9 al 5 
    //Rd 4 al 0
    
    // guardamos el Rm 
    char * RmStr = malloc(sizeof(char) * (5));
    strncpy(RmStr, restOfInstruction+3 , 5);
    int RmNum= (int) strtol(RmStr, NULL, 2);

    // guardamos el inmediato 
    char * immStr = malloc(sizeof(char) * (6));
    strncpy(immStr, restOfInstruction+8 , 6);
    int immNum= (int) strtol(immStr, NULL, 2);

    //guardamos Rn
    char * RnStr = malloc(sizeof(char) * (5));
    strncpy(RnStr, restOfInstruction+14 , 5);
    int RnNum= (int) strtol(RnStr, NULL, 2);
    int rnContent = CURRENT_STATE.REGS[RnNum];

    //Hacemos la operacion
    int result = rnContent | RmNum;
    // NEXT_STATE.FLAG_N = (result < 0);  // Si result es menor que 0, FLAG_N se establece a 1 (verdadero), de lo contrario se establece a 0 (falso)
    // NEXT_STATE.FLAG_Z = (result == 0); // Si result es igual a 0, FLAG_Z se establece a 1 (verdadero), de lo contrario se establece a 0 (falso)

;
    printf("Result %i\n", result);
    NEXT_STATE.PC+= 4;
    free(RmStr);
    free(immStr);
    free(RnStr);
    return ;
}


int64_t signExtend26to64(long value) {
    // Verificar si el bit 25 (el bit más significativo de imm26) está establecido
    if (value & (1 << 25)) {
        // Extender el signo
        value |= (-0UL << 26);
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
    }
    else if (condNum == 1){
        printf("BNE\n");
        if (CURRENT_STATE.FLAG_Z == 0){
            NEXT_STATE.PC += immNum;
        }
    }
    else if (condNum == 12){
        printf("BGT\n");
        if (CURRENT_STATE.FLAG_N == 0){
            NEXT_STATE.PC += immNum;
        }
    }
    else if (condNum == 11){
        printf("BLT\n");
        if (CURRENT_STATE.FLAG_N != 0){
            NEXT_STATE.PC += immNum;
        }
    }
    else if (condNum == 10){
        printf("BGE\n");
        if (CURRENT_STATE.FLAG_N == 0){
            NEXT_STATE.PC += immNum;
        }
    }
    else if (condNum == 13){
        printf("BLE\n");
        if (!(CURRENT_STATE.FLAG_N == 0)){
            NEXT_STATE.PC += immNum;
        }
    }
    else{
        //no hacer nada porque no es la instruccion adecuada? preguntar
    };
    NEXT_STATE.PC += 4;
    return ;
}

void Beq(char * restOfInstruction){
    puts("Beq");    
    //guardamos Target
    char * targetStr = malloc(sizeof(char) * 26); // Ejemplo de 26 bits + terminador nulo.
    strncpy(targetStr, restOfInstruction, 26);
    int64_t targetOffset = strtol(targetStr, NULL, 2);
    if(targetStr[0]=='1'){
        targetOffset |= (~0UL << 26);
    }
    
    targetOffset <<= 2;

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
    char * targetStr = malloc(sizeof(char) * 32); // Ejemplo de 26 bits + terminador nulo.
    strncpy(targetStr, restOfInstruction, 32);
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


void Stur(char 
* restOfInstruction){
    puts("Stur");
    //bit 22 es N 
    //imm del 20 al 12 inclusives 
    //00 del 11 al 10 
    //Rn del 9 al 5 
    //Rd 4 al 0
    

    char * immStr = malloc(9); 
    strncpy(immStr, restOfInstruction, 9);
    int offset = (int) strtol(immStr, NULL, 2);


    if ((offset & (1 << 8)) != 0) { // Comprobar si el bit más significativo está activado para extensión de signo
        offset |= ~((1 << 9) - 1); // Extensión de signo para un valor de 9 bits
    }
    free(immStr);
    


    // Decodificar Rn del 9 al 5
    char * RnStr = malloc(5);
    strncpy(RnStr, restOfInstruction + 11, 5);
    int RnNum = (int) strtol(RnStr, NULL, 2);
    uint64_t baseAddress = CURRENT_STATE.REGS[RnNum];
    free(RnStr);
    
    // extraemos Rt, el índice del registro destino
    char * RtStr = malloc(5);
    strncpy(RtStr, restOfInstruction + 16, 5);
    int RtNum = (int) strtol(RtStr, NULL, 2);
    uint64_t data = CURRENT_STATE.REGS[RtNum];

    free(RtStr);

    // Realizamos la operación de desplazamiento lógico a la derecha
    // uint64_t result = rnContent >> shiftAmount;
    uint64_t effectiveAddress = baseAddress + offset - 0x10000000;
    mem_write_32(effectiveAddress, data);

    NEXT_STATE.PC  += 4;
    printf("Stored 0x%lx at simulated memory address 0x%lx\n", data, effectiveAddress);
    return ;
}

void Sturb(char 
* restOfInstruction){
    puts("Sturb");
    //bit 22 es N 
    //imm del 20 al 12 inclusives 
    //00 del 11 al 10 
    //Rn del 9 al 5 
    //Rd 4 al 0
    

    char * immStr = malloc(9); 
    strncpy(immStr, restOfInstruction, 9);
    int offset = (int) strtol(immStr, NULL, 2);


    if ((offset & (1 << 8)) != 0) { // Comprobar si el bit más significativo está activado para extensión de signo
        offset |= ~((1 << 9) - 1); // Extensión de signo para un valor de 9 bits
    }
    free(immStr);
    


    // Decodificar Rn del 9 al 5
    char * RnStr = malloc(5);
    strncpy(RnStr, restOfInstruction + 11, 5);
    int RnNum = (int) strtol(RnStr, NULL, 2);
    uint64_t baseAddress = CURRENT_STATE.REGS[RnNum];
    free(RnStr);
    
    // extraemos Rt, el índice del registro destino
    char * RtStr = malloc(5);
    strncpy(RtStr, restOfInstruction + 16, 5);
    int RtNum = (int) strtol(RtStr, NULL, 2);
    uint8_t data = CURRENT_STATE.REGS[RtNum] & 0xFF; // Solo el byte menos significativo

    free(RtStr);

    // Realizamos la operación de desplazamiento lógico a la derecha
    // uint64_t result = rnContent >> shiftAmount;
    uint64_t effectiveAddress = baseAddress + offset - 0x10000000;
    mem_write_32(effectiveAddress, data);

    NEXT_STATE.PC  += 4;
    printf("Stored byte 0x%02x at simulated memory address 0x%lx\n", data, effectiveAddress);
    return ;
}

void Sturh(char 
* restOfInstruction){
    puts("Sturh");
    //bit 22 es N 
    //imm del 20 al 12 inclusives 
    //00 del 11 al 10 
    //Rn del 9 al 5 
    //Rd 4 al 0
    

    char * immStr = malloc(9); 
    strncpy(immStr, restOfInstruction, 9);
    int offset = (int) strtol(immStr, NULL, 2);


    if ((offset & (1 << 8)) != 0) { // Comprobar si el bit más significativo está activado para extensión de signo
        offset |= ~((1 << 9) - 1); // Extensión de signo para un valor de 9 bits
    }
    free(immStr);
    


    // Decodificar Rn del 9 al 5
    char * RnStr = malloc(5);
    strncpy(RnStr, restOfInstruction + 11, 5);
    int RnNum = (int) strtol(RnStr, NULL, 2);
    uint64_t baseAddress = CURRENT_STATE.REGS[RnNum];
    free(RnStr);
    
    // extraemos Rt, el índice del registro destino
    char * RtStr = malloc(5);
    strncpy(RtStr, restOfInstruction + 16, 5);
    int RtNum = (int) strtol(RtStr, NULL, 2);
    uint16_t data = CURRENT_STATE.REGS[RtNum] & 0xFFFF; // Solo el byte menos significativo

    free(RtStr);

    // Realizamos la operación de desplazamiento lógico a la derecha
    // uint64_t result = rnContent >> shiftAmount;
    uint64_t effectiveAddress = baseAddress + offset- 0x10000000;
    mem_write_32(effectiveAddress, data);

    NEXT_STATE.PC  += 4;
    printf("Stored byte 0x%02x at simulated memory address 0x%lx\n", data, effectiveAddress);
    return ;
}

void Ldur(char 
* restOfInstruction){
    puts("Ldur");
    //bit 22 es N 
    //imm del 20 al 12 inclusives 
    //00 del 11 al 10 
    //Rn del 9 al 5 
    //Rt 4 al 0
    

    char * immStr = malloc(9); 
    strncpy(immStr, restOfInstruction, 9);
    int offset = (int) strtol(immStr, NULL, 2);


    if ((offset & (1 << 8)) != 0) { // Comprobar si el bit más significativo está activado para extensión de signo
        offset |= ~((1 << 9) - 1); // Extensión de signo para un valor de 9 bits
    }
    free(immStr);
    


    // Decodificar Rn del 9 al 5
    char * RnStr = malloc(5);
    strncpy(RnStr, restOfInstruction + 11, 5);
    int RnNum = (int) strtol(RnStr, NULL, 2);
    uint64_t baseAddress = CURRENT_STATE.REGS[RnNum];
    free(RnStr);
    
    // extraemos Rt, el índice del registro destino
    char * RtStr = malloc(5);
    strncpy(RtStr, restOfInstruction + 16, 5);
    int RtNum = (int) strtol(RtStr, NULL, 2);

    free(RtStr);

    uint64_t effectiveAddress = baseAddress + offset- 0x10000000;

    uint64_t data = mem_read_32(effectiveAddress);
    NEXT_STATE.REGS[RtNum] = data;

    NEXT_STATE.PC  += 4;
    printf("Loaded 0x%lx into X%d from simulated memory address 0x%lx\n", data, RtNum, effectiveAddress);
    return ;
}

void Ldurb(char 
* restOfInstruction){
    puts("Ldurb");
    //bit 22 es N 
    //imm del 20 al 12 inclusives 
    //00 del 11 al 10 
    //Rn del 9 al 5 
    //Rt 4 al 0
    

    char * immStr = malloc(9); 
    strncpy(immStr, restOfInstruction, 9);
    int offset = (int) strtol(immStr, NULL, 2);


    if ((offset & (1 << 8)) != 0) { // Comprobar si el bit más significativo está activado para extensión de signo
        offset |= ~((1 << 9) - 1); // Extensión de signo para un valor de 9 bits
    }
    free(immStr);
    


    // Decodificar Rn del 9 al 5
    char * RnStr = malloc(5);
    strncpy(RnStr, restOfInstruction + 11, 5);
    int RnNum = (int) strtol(RnStr, NULL, 2);
    uint64_t baseAddress = CURRENT_STATE.REGS[RnNum];
    free(RnStr);
    
    // extraemos Rt, el índice del registro destino
    char * RtStr = malloc(5);
    strncpy(RtStr, restOfInstruction + 16, 5);
    int RtNum = (int) strtol(RtStr, NULL, 2);

    free(RtStr);

    uint64_t effectiveAddress = baseAddress + offset- 0x10000000;

    uint8_t byteData = mem_read_32(effectiveAddress);
    NEXT_STATE.REGS[RnNum] = byteData; 

    NEXT_STATE.PC  += 4;
    printf("Loaded byte 0x%02x into W%d from simulated memory address 0x%lx\n", byteData, RnNum, effectiveAddress);
    return ;
}

void Ldurh(char 
* restOfInstruction){
    puts("Ldurh");
    //bit 22 es N 
    //imm del 20 al 12 inclusives 
    //00 del 11 al 10 
    //Rn del 9 al 5 
    //Rt 4 al 0
    

    char * immStr = malloc(9); 
    strncpy(immStr, restOfInstruction, 9);
    int offset = (int) strtol(immStr, NULL, 2);


    if ((offset & (1 << 8)) != 0) { // Comprobar si el bit más significativo está activado para extensión de signo
        offset |= ~((1 << 9) - 1); // Extensión de signo para un valor de 9 bits
    }
    free(immStr);
    


    // Decodificar Rn del 9 al 5
    char * RnStr = malloc(5);
    strncpy(RnStr, restOfInstruction + 11, 5);
    int RnNum = (int) strtol(RnStr, NULL, 2);
    uint64_t baseAddress = CURRENT_STATE.REGS[RnNum];
    free(RnStr);
    
    // extraemos Rt, el índice del registro destino
    char * RtStr = malloc(5);
    strncpy(RtStr, restOfInstruction + 16, 5);
    int RtNum = (int) strtol(RtStr, NULL, 2);

    free(RtStr);

    uint64_t effectiveAddress = baseAddress + offset- 0x10000000;

    uint16_t halfdata = mem_read_32(effectiveAddress);
    NEXT_STATE.REGS[RnNum] &= 0xFFFFFFFFFFFF0000; // Limpia los 16 bits inferiores
    NEXT_STATE.REGS[RnNum] |= halfdata; // Carga el half-data en los 16 bits inferiores


    NEXT_STATE.PC  += 4;
    printf("Loaded half-data 0x%04x into W%d from simulated memory address 0x%lx\n", halfdata, RnNum, effectiveAddress);
    return ;
}

void LslImm(char * restOfInstruction){
    puts("LslImm");
    //bit 22 es N 
    //immr del 21 al 16 inclusives 
    //imms del 15 al 10 
    //Rn del 9 al 5 
    //Rd 4 al 0

    char * NStr = malloc(1); 
    strncpy(NStr, restOfInstruction, 1);
    int N = (int) strtol(NStr, NULL, 2);
    free(NStr);


    // guardamos el inmediatoR 
    char * immrStr = malloc(6); 
    strncpy(immrStr, restOfInstruction + 1, 6);
    int immr = (int) strtol(immrStr, NULL, 2);
    free(immrStr);

    // guardamos el inmediatoS 
    char * immsStr = malloc(6); 
    strncpy(immsStr, restOfInstruction + 7, 6);
    int imms = (int) strtol(immsStr, NULL, 2);
    free(immsStr);

    int shiftAmount = (immr == 0) ? imms : 64 - immr;
    // extraemos Rn
    char * RnStr = malloc(5);
    strncpy(RnStr, restOfInstruction + 13, 5);
    int RnNum = (int) strtol(RnStr, NULL, 2);
    int rnContent = CURRENT_STATE.REGS[RnNum];
    free(RnStr);
    
    // extraemos Rd, el índice del registro destino
    char * RdStr = malloc(5 + 1);
    strncpy(RdStr, restOfInstruction + 18, 5);
    int RdNum = (int) strtol(RdStr, NULL, 2);
    free(RdStr);

    //Hacemos la operacion
    int result = rnContent << shiftAmount;
    NEXT_STATE.REGS[RdNum] = result;

    printf("Rd %i\n", RdNum);


    printf("Result %i\n", result);
    NEXT_STATE.PC  += 4;
    return ;
}

void LsrImm(char * restOfInstruction){
    puts("LsrImm");
    //bit 22 es N 
    //immr del 21 al 16 inclusives 
    //imms del 15 al 10 
    //Rn del 9 al 5 
    //Rd 4 al 0
    

    char * immrStr = malloc(6); // 6 bits para immr + espacio para null (aunque no se añadirá null)
    strncpy(immrStr, restOfInstruction + 1, 6);
    int immr = (int) strtol(immrStr, NULL, 2);
    free(immrStr);
    
    // 'imms' es 111111 para LSR, por lo que el desplazamiento es (63 - immr)
    int shiftAmount = 63 - immr;

    // Decodificar Rn del 9 al 5
    char * RnStr = malloc(5);
    strncpy(RnStr, restOfInstruction + 13, 5);
    int RnNum = (int) strtol(RnStr, NULL, 2);
    int rnContent = CURRENT_STATE.REGS[RnNum];
    free(RnStr);
    
    // extraemos Rd, el índice del registro destino
    char * RdStr = malloc(5);
    strncpy(RdStr, restOfInstruction + 18, 5);
    int RdNum = (int) strtol(RdStr, NULL, 2);
    free(RdStr);

    // Realizamos la operación de desplazamiento lógico a la derecha
    // uint64_t result = rnContent >> shiftAmount;
    int result = rnContent >> shiftAmount;
    NEXT_STATE.REGS[RdNum] = result;

    printf("Rd %i\n", RdNum);
    printf("Result %i\n", result);
    // Actualizamos el PC al siguiente valor (suponiendo que el tamaño de la instrucción es 4 bytes)
    NEXT_STATE.PC  += 4;
    return ;
}


void Movz(char * restOfInstruction) {
    puts("Movz");

    // Extraer el valor inmediato del resto de la instrucción
    // Asumiremos que el valor inmediato viene en los últimos 16 bits de la instrucción
    char * immStr = malloc(16); // 16 bits para el valor inmediato + espacio para el terminador nulo
    strncpy(immStr, restOfInstruction + 2, 16);
    uint16_t immediateValue =(uint16_t) strtol(immStr, NULL, 2);
    free(immStr);

    // Decodificar Rt del 0 al 4
    char * RtStr = malloc(5);
    strncpy(RtStr, restOfInstruction + 18, 5);
    int RtNum = strtol(RtStr, NULL, 2);
    free(RtStr);

    // Establecer el valor inmediato en el registro destino con ceros en los bits restantes
    NEXT_STATE.REGS[RtNum] = immediateValue;

    // Actualizar el PC al siguiente valor
    NEXT_STATE.PC += 4;

    printf("Moved value 0x%04x into X%d\n", immediateValue, RtNum);
}


char* toBinaryString(int n) {
  int num_bits = 32;
  char *cadena = malloc(num_bits + 1);
  if (!cadena) {
    return NULL;
  }
  for (int i = num_bits - 1; i >= 0; i--) {
    cadena[i] = (n & 1) + '0';
    n >>= 1;
  }
  cadena[num_bits] = '\0';
  return cadena;
}

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
void mul(char * restOfInstruction){
    //ADDS Xn + Xm to Xd

    //guardamos Rm
    char * RmStr = malloc(sizeof(char) * (5));
    strncpy(RmStr, restOfInstruction, 5);
    int RmNum= (int) strtol(RmStr, NULL, 2);
    int64_t rmContent = CURRENT_STATE.REGS[RmNum];


    //guardamos Rn
    char * RnStr = malloc(sizeof(char) * (5));
    strncpy(RnStr, restOfInstruction+5+6 , 5);
    int RnNum= (int) strtol(RnStr, NULL, 2);
    int64_t rnContent = CURRENT_STATE.REGS[RnNum];

    //guardamos Rd
    char * RdStr = malloc(sizeof(char) * (5));
    strncpy(RdStr, restOfInstruction+5+6+5 , 5);
    int RdNum= (int) strtol(RdStr, NULL, 2);

    //Hacemos la operacion
    int64_t result = rnContent * rmContent;

    NEXT_STATE.REGS[RdNum]  = result;
    NEXT_STATE.PC+= 4;
    free(RmStr);
    free(RnStr);
    free(RdStr);

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

char *  decode(void (**fill_func_prt) ){
    dictionary_t *  opcodesMap = NULL; //SE DESTRUYE AL FINAL DE LA FUNCION Y SE DECLARA SIEMPRE

    if (opcodesMap == NULL){ 
        opcodesMap = dictionary_create(NULL);
        // chequear los numeros extended
         dictionary_put(opcodesMap, "10101011000", &addsExtendedReg); //ya con el 1 de sf agregado 
        dictionary_put(opcodesMap, "10110001", &addsImm); // pag. 531
        dictionary_put(opcodesMap, "11101011000", &subs_compExtendedReg); // pag. 934
        dictionary_put(opcodesMap, "11110001", &subs_compImm); // pag. 936
        dictionary_put(opcodesMap, "11010100", &HLT); // 
        // dictionary_put(opcodesMap, "10110001", &compImm); // Revisar como se guarda la resta
        dictionary_put(opcodesMap, "11101010", &AndsShiftedReg); // pag.542 - Que hago con el 1 de N? - no se usa el inmediato?
        dictionary_put(opcodesMap, "11001010", &EorShiftedReg); // pag. 620 - Aca no se updatean las flags?
        dictionary_put(opcodesMap, "10101010", &OrrShiftedReg); // 
        dictionary_put(opcodesMap, "000101", &B); // pag. 550 - Nose si esta bien
        // dictionary_put(opcodesMap, "", &Imm26); // Nose si esta explicando el caso anterior o es uno nuevo
        dictionary_put(opcodesMap, "1101011000111111000000", &Br); // pag. 562 - Revisar: llamado de funcion y NEXT_STATE.PC = CURRENT_STATE.REGS[RnNum];
        dictionary_put(opcodesMap, "01010100", &Bcond); // pag. 549
        // dictionary_put(opcodesMap, "0000", &Beq); // pag. 138 - Revisar: llamado de funcion y funcionamiento
        // dictionary_put(opcodesMap, "0001", &Bne); // 542542
        // dictionary_put(opcodesMap, "1100", &Bgt); // 
        // dictionary_put(opcodesMap, "1011", &Blt); // 
        // dictionary_put(opcodesMap, "1010", &Bge); // 
        // dictionary_put(opcodesMap, "1101", &Ble); // 
        dictionary_put(opcodesMap, "110100110", &LslImm); // pag. 754 Revisar: shiftAmount
        dictionary_put(opcodesMap, "110100110", &LsrImm); // pag. 757
        dictionary_put(opcodesMap, "11111000000", &Stur); // pag. 917 Revisar: offset
        dictionary_put(opcodesMap, "00111000000", &Sturb); // pag. 918
        dictionary_put(opcodesMap, "01111000000", &Sturh); // pag. 919
        dictionary_put(opcodesMap, "11111000010", &Ldur); // pag. 739
        dictionary_put(opcodesMap, "00111000010", &Ldurb); // pag. 741
        dictionary_put(opcodesMap, "01111000010", &Ldurh); // pag. 742
        dictionary_put(opcodesMap, "110100101", &Movz); // pag. 770


        // -----------------------si queremos puntos extras----------------------

        // dictionary_put(opcodesMap, "", &AddExtendedReg); // ya con el 1 de sf agregaB
        // dictionary_put(opcodesMap, "", &AddImm); // ya con el 1 de sf agregaB
         dictionary_put(opcodesMap, "10011011000", &mul); // ya con el 1 de sf agregaB
         dictionary_put(opcodesMap, "10110100", &Cbz); // ya con el 1 de sf agregaB
         dictionary_put(opcodesMap, "10110101", &Cbnz); // ya con el 1 de sf agregaB
        puts("crea dict");

    } 
    
    char * pcContentAsString = toBinaryString(mem_read_32(CURRENT_STATE.PC));
    //printf(" pcContentAsString %s", pcContentAsString );
    puts("lee pc");
    int opcodeSize;
    for (int i = 0;i<sizeof(opLengths)/ sizeof(opLengths[0]);i++){
        char * opCodeString = malloc(sizeof(char) * (opLengths[i]));
        strncpy(opCodeString, pcContentAsString , opLengths[i]);
        printf("opcode string %s", opCodeString);
        puts("flush dns");
        if (dictionary_contains(opcodesMap, opCodeString)){
            *fill_func_prt = dictionary_get(opcodesMap,opCodeString, NULL);
            puts("Eso");
            opcodeSize =  opLengths[i];
            free(opCodeString);
            break;
        }
        free(opCodeString);
    }
    char * restOfInstruction = malloc(sizeof(char) * (32- opcodeSize));
    printf("opcodesize %i \n",opcodeSize);
    strncpy(restOfInstruction, pcContentAsString+opcodeSize , 32- opcodeSize);

    free(pcContentAsString);
    dictionary_destroy(opcodesMap);
    return restOfInstruction;
}

void process_instruction(){
    void (*func_ptr)() = NULL;
    char * instructionParams = decode(&func_ptr );
    if (func_ptr == NULL){
        puts("no se encontro");
        //la instruccion no fue encontrada
    }
    else{
        (*func_ptr)(instructionParams);
        //free(instructionParams);
        //Execute
    }
    
    //printf("printing PC %b " ,mem_read_32(CURRENT_STATE.PC));
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. 
     * 
     * Sugerencia: hagan una funcion para decode()
     *             y otra para execute()
     * 
     * */

}
