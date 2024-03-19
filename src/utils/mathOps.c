#include "mathOps.h"


void addsExtendedReg(char * restOfInstruction){
    puts("addsExtendedReg");
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
    int64_t result = rnContent + rmContent;
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
    int64_t immNum= (int64_t) strtol(immStr, NULL, 2); //checkear este int64_t

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
    int64_t rnContent = CURRENT_STATE.REGS[RnNum];

    //guardamos Rd
    char * RdStr = malloc(sizeof(char) * (5));
    strncpy(RdStr, restOfInstruction+14+5 , 5);
    int RdNum= (int) strtol(RdStr, NULL, 2);

    //Hacemos la operacion
    int64_t result = rnContent + immNum;
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
    int64_t result =   rnContent-rmContent;
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
    puts("subs_compImm");
    
    //ADDS Xn - imm to Xd
    //bits 23 y 22 son de shift y deberian ser 00 
    //imm del 21 al 10 inclusives 
    //Rn del 9 al 5 
    //Rd 4 al 0
    // guardamos el inmediato 
    char * immStr = malloc(sizeof(char) * (12));
    strncpy(immStr, restOfInstruction+2 , 12);
    int64_t immNum= (int64_t) strtol(immStr, NULL, 2);


    char * shiftBytes = malloc(sizeof(char) * (2));
    strncpy(shiftBytes, restOfInstruction , 2);
    int shiftStatus = strcmp(shiftBytes,"01");
    if (shiftStatus ==0){
        //hacemos shift 12 a la izq
        immNum = immNum << 12;
    }


    //guardamos Rn
    char * RnStr = malloc(sizeof(char) * (5));
    strncpy(RnStr, restOfInstruction+14 , 5);
    int RnNum= (int) strtol(RnStr, NULL, 2);
    int64_t rnContent = CURRENT_STATE.REGS[RnNum];

    //guardamos Rd
    char * RdStr = malloc(sizeof(char) * (5));
    strncpy(RdStr, restOfInstruction+14+5 , 5);
    int RdNum= (int) strtol(RdStr, NULL, 2);

    

    //Hacemos la operacion
    int64_t result = rnContent - immNum;
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
    uint64_t immNum= (uint64_t) strtol(immStr, NULL, 2); //checkear uint64_t

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
    int64_t RmContent = CURRENT_STATE.REGS[RmNum];

    // guardamos el inmediato 
    char * immStr = malloc(sizeof(char) * (6));
    strncpy(immStr, restOfInstruction+8 , 6);
    int immNum= (int) strtol(immStr, NULL, 2);

    //guardamos Rn
    char * RnStr = malloc(sizeof(char) * (5));
    strncpy(RnStr, restOfInstruction+14 , 5);
    int RnNum= (int) strtol(RnStr, NULL, 2);
    int64_t rnContent = CURRENT_STATE.REGS[RnNum];

    //Hacemos la operacion
    int result = rnContent | RmContent;
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

// ----------------------EXTRAS----------------------

void subImmediate(char * restOfInstruction){
     puts("subImm");
     char * immStr = malloc(sizeof(char) * (12));
    strncpy(immStr, restOfInstruction+2 , 12);
    int64_t immNum= (int64_t) strtol(immStr, NULL, 2);


    char * shiftBytes = malloc(sizeof(char) * (2));
    strncpy(shiftBytes, restOfInstruction , 2);
    int shiftStatus = strcmp(shiftBytes,"01");
    if (shiftStatus ==0){
        //hacemos shift 12 a la izq
        immNum = immNum << 12;
    }


    //guardamos Rn
    char * RnStr = malloc(sizeof(char) * (5));
    strncpy(RnStr, restOfInstruction+14 , 5);
    int RnNum= (int) strtol(RnStr, NULL, 2);
    int64_t rnContent = CURRENT_STATE.REGS[RnNum];

    //guardamos Rd
    char * RdStr = malloc(sizeof(char) * (5));
    strncpy(RdStr, restOfInstruction+14+5 , 5);
    int RdNum= (int) strtol(RdStr, NULL, 2);



    //Hacemos la operacion
    int64_t result = rnContent - immNum;

    NEXT_STATE.REGS[RdNum]  = result;
    NEXT_STATE.PC+= 4;
    free(immStr);
    free(RnStr);
    free(RdStr);
    free(shiftBytes);
    return ;
}

void subExtendedReg(char * restOfInstruction){
    puts("subExtendedReg");

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
    int64_t result =   rnContent-rmContent;


    NEXT_STATE.REGS[RdNum]  = result;
    NEXT_STATE.PC+= 4;
    free(RmStr);
    free(RnStr);
    free(RdStr);

    return ;
    }

void addImmediate(char * restOfInstruction){
     puts("addsImm");

    char * immStr = malloc(sizeof(char) * (12));
    strncpy(immStr, restOfInstruction+2 , 12);
    int64_t immNum= (int) strtol(immStr, NULL, 2);

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
    int64_t rnContent = CURRENT_STATE.REGS[RnNum];

    //guardamos Rd
    char * RdStr = malloc(sizeof(char) * (5));
    strncpy(RdStr, restOfInstruction+14+5 , 5);
    int64_t RdNum= (int64_t) strtol(RdStr, NULL, 2);

    //Hacemos la operacion
    int64_t result = rnContent + immNum;

    NEXT_STATE.REGS[RdNum]  = result;
    printf("Result %i\n", result);
    NEXT_STATE.PC+= 4;
    free(shiftBytes);
    free(immStr);
    free(RnStr);
    free(RdStr);
    return ;
}

void addExtendedReg(char * restOfInstruction){
    puts("addExtendedReg");
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
    int64_t result = rnContent + rmContent;

    NEXT_STATE.REGS[RdNum]  = result;
    printf("Result %i\n", result);
    NEXT_STATE.PC+= 4;
    free(RmStr);
    free(RnStr);
    free(RdStr);

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



