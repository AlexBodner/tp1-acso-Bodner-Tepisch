#include "mathOps.h"
void addsExtendedReg(char * restOfInstruction){
    puts("addsExtendedReg");
    return ;
}

void addsImm(char * restOfInstruction){
    puts("addsImm");
    //ADDS Xn + imm to Xd
    //bits 23 y 22 son de shift y deberian ser 00 
    //imm del 21 al 10 inclusives 
    //Rn del 9 al 5 
    //Rd 4 al 0
    
    char * shiftBytes = malloc(sizeof(char) * (2));
    strncpy(shiftBytes, restOfInstruction , 2);

    // guardamos el inmediato 
    char * immStr = malloc(sizeof(char) * (12));
    strncpy(immStr, restOfInstruction+2 , 12);
    int immNum= (int) strtol(immStr, NULL, 2);



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

void subsExtendedReg(char * restOfInstruction){
    puts("subsExtendedReg");
    return ;
}

void subsImm(char * restOfInstruction){
    puts("subsImm");
    // Revisar como se guarda la resta
    
    //ADDS Xn - imm to Xd
    //bits 23 y 22 son de shift y deberian ser 00 
    //imm del 21 al 10 inclusives 
    //Rn del 9 al 5 
    //Rd 4 al 0

    char * shiftBytes = malloc(sizeof(char) * (2));
    strncpy(shiftBytes, restOfInstruction , 2);
    
    // guardamos el inmediato 
    char * immStr = malloc(sizeof(char) * (12));
    strncpy(immStr, restOfInstruction+2 , 12);
    int immNum= (int) strtol(immStr, NULL, 2);

    //guardamos Rn
    char * RnStr = malloc(sizeof(char) * (5));
    strncpy(RnStr, restOfInstruction+14 , 5);
    int RnNum= (int) strtol(RnStr, NULL, 2);
    int rnContent = CURRENT_STATE.REGS[RnNum];

    //guardamos Rd
    char * RdStr = malloc(sizeof(char) * (5));
    strncpy(RdStr, restOfInstruction+14+5 , 5);
    int RdNum= (int) strtol(RdStr, NULL, 2);

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


    //Hacemos la operacion
    int result = rnContent - immNum;
    printf("Rd %i\n", RdNum);
    NEXT_STATE.FLAG_N = (result < 0);  // Si result es menor que 0, FLAG_N se establece a 1 (verdadero), de lo contrario se establece a 0 (falso)
    NEXT_STATE.FLAG_Z = (result == 0); // Si result es igual a 0, FLAG_Z se establece a 1 (verdadero), de lo contrario se establece a 0 (falso)


    NEXT_STATE.REGS[RdNum]  = result;
    printf("Result %i\n", result);
    NEXT_STATE.PC+= 4;
    free(immStr);
    free(RnStr);
    free(RdStr);
    free(shiftBytes);
    return ;
}

void compImm(char * restOfInstruction){
    puts("compImm");
    //CMP Xn - imm 
    //bits 23 y 22 son de shift y deberian ser 00 
    //imm del 21 al 10 inclusives 
    //Rn del 9 al 5 
    //Rd 4 al 0
    
    // guardamos el inmediato 
    char * immStr = malloc(sizeof(char) * (12));
    strncpy(immStr, restOfInstruction+2 , 12);
    int immNum= (int) strtol(immStr, NULL, 2);

    //guardamos Rn
    char * RnStr = malloc(sizeof(char) * (5));
    strncpy(RnStr, restOfInstruction+14 , 5);
    int RnNum= (int) strtol(RnStr, NULL, 2);
    int rnContent = CURRENT_STATE.REGS[RnNum];

    //Hacemos la operacion
    int result = rnContent - immNum;
    NEXT_STATE.FLAG_N = (result < 0);  // Si result es menor que 0, FLAG_N se establece a 1 (verdadero), de lo contrario se establece a 0 (falso)
    NEXT_STATE.FLAG_Z = (result == 0); // Si result es igual a 0, FLAG_Z se establece a 1 (verdadero), de lo contrario se establece a 0 (falso)


    printf("Result %i\n", result);
    NEXT_STATE.PC+= 4;
    free(immStr);
    free(RnStr);
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
    int result = rnContent & RmNum;
    NEXT_STATE.FLAG_N = (result < 0);  // Si result es menor que 0, FLAG_N se establece a 1 (verdadero), de lo contrario se establece a 0 (falso)
    NEXT_STATE.FLAG_Z = (result == 0); // Si result es igual a 0, FLAG_Z se establece a 1 (verdadero), de lo contrario se establece a 0 (falso)

    printf("Result %i\n", result);
    NEXT_STATE.PC+= 4;
    free(RmStr);
    free(immStr);
    free(RnStr);
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
    int result = rnContent ^ RmNum;
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
