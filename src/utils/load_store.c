#include "load_store.h"


void Stur(char * restOfInstruction){
    //bit 22 es N 
    //imm del 20 al 12 inclusives 
    //00 del 11 al 10 
    //Rn del 9 al 5 
    //Rd 4 al 0

    char * immStr = malloc(9); 
    strncpy(immStr, restOfInstruction, 9);
    int immNum = (int) strtol(immStr, NULL, 2);

    if ((immNum & (1 << 8)) != 0) { // Comprobar si el bit más significativo está activado para extensión de signo
        immNum |= ~((1 << 9) - 1); // Extensión de signo para un valor de 9 bits
    }
    free(immStr);
    


    // Decodificar Rn del 9 al 5
    char * RnStr = malloc(5);
    strncpy(RnStr, restOfInstruction + 9+2, 5);
    int RnNum = (int) strtol(RnStr, NULL, 2);
    uint64_t rnContent = CURRENT_STATE.REGS[RnNum];
    free(RnStr);
    
    // extraemos Rt, el índice del registro destino
    char * RtStr = malloc(5);
    strncpy(RtStr, restOfInstruction + 16, 5);
    int RtNum = (int) strtol(RtStr, NULL, 2);
    uint64_t rtContent = CURRENT_STATE.REGS[RtNum];

    free(RtStr);

    // Realizamos la operación de desplazamiento lógico a la derecha
    // uint64_t result = rnContent >> shiftAmount;
    uint32_t rtr = rtContent & 0xFFFFFFFF;
    uint32_t rtl = (rtContent >> 32) & 0xFFFFFFFF;
    mem_write_32((rnContent+immNum), rtr);
    mem_write_32((rnContent+immNum+4), rtl);

    NEXT_STATE.PC  += 4;
    return ;
}

void Sturb(char * restOfInstruction){
    // bit 22 es N 
    // imm del 20 al 12 inclusives 
    // 00 del 11 al 10 
    // Rn del 9 al 5 
    // Rd 4 al 0

    char * immStr = malloc(9); 
    strncpy(immStr, restOfInstruction, 9);
    int immNum = (int) strtol(immStr, NULL, 2);

    if (immNum & (1 << 8)) { // Si el noveno bit es 1, extendemos el signo
        immNum |= ~((1 << 8) - 1);
    }
    free(immStr);

    // Decodificar Rn del 9 al 5
    char * RnStr = malloc(5);
    strncpy(RnStr, restOfInstruction + 9+2, 5);
    int RnNum = (int) strtol(RnStr, NULL, 2);
    uint64_t rnContent = CURRENT_STATE.REGS[RnNum];
    free(RnStr);
    
    // extraemos Rt, el índice del registro destino
    char * RtStr = malloc(5);
    strncpy(RtStr, restOfInstruction + 16, 5);
    int RtNum = (int) strtol(RtStr, NULL, 2);
    uint64_t rtContent = CURRENT_STATE.REGS[RtNum];
    free(RtStr);

    // Se escribe solamente el byte menos significativo usando mem_write_32
    uint32_t rtByte = rtContent & 0xFF;

    uint32_t previousData = mem_read_32(rnContent + immNum)&0b11111111111111111111111100000000;
    
    // Asumimos que mem_write_32 escribe los 32 bits, pero los bits superiores no se deben modificar.
    // Dado que solo queremos escribir un byte, los otros deben ser 0.
    mem_write_32((rnContent + immNum), rtByte+previousData);

    NEXT_STATE.PC += 4;
    return ;
}

void Sturh(char * restOfInstruction){
    //bit 22 es N 
    //imm del 20 al 12 inclusives 
    //00 del 11 al 10 
    //Rn del 9 al 5 
    //Rd 4 al 0
    

    char * immStr = malloc(9); 
    strncpy(immStr, restOfInstruction, 9);
    int immNum = (int) strtol(immStr, NULL, 2);

    if ((immNum & (1 << 8)) != 0) { // Comprobar si el bit más significativo está activado para extensión de signo
        immNum |= ~((1 << 9) - 1); // Extensión de signo para un valor de 9 bits
    }
    free(immStr);

    // Decodificar Rn del 9 al 5
    char * RnStr = malloc(5);
    strncpy(RnStr, restOfInstruction + 9+2, 5);
    int RnNum = (int) strtol(RnStr, NULL, 2);
    uint64_t rnContent = CURRENT_STATE.REGS[RnNum];
    free(RnStr);
    
    // extraemos Rt, el índice del registro destino
    char * RtStr = malloc(5);
    strncpy(RtStr, restOfInstruction + 16, 5);
    int RtNum = (int) strtol(RtStr, NULL, 2);
    uint64_t rtContent = CURRENT_STATE.REGS[RtNum];
    free(RtStr);

    // Se escribe solamente el byte menos significativo usando mem_write_32
    uint16_t rtByte = rtContent & 0xFFFFFFFF;
    // Asumimos que mem_write_32 escribe los 32 bits, pero los bits superiores no se deben modificar.
    // Dado que solo queremos escribir un byte, los otros deben ser 0.
    mem_write_32((rnContent + immNum), rtByte);

    NEXT_STATE.PC += 4;
    return ;
}


void Ldur(char * restOfInstruction){
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

    uint64_t effectiveAddress = baseAddress + offset;

    int64_t dataLessSignificative = mem_read_32(effectiveAddress);
    int64_t dataMostSignificative = ((int64_t)mem_read_32(effectiveAddress+4))<<32;

    NEXT_STATE.REGS[RtNum] = dataLessSignificative + dataMostSignificative ;

    NEXT_STATE.PC  += 4;
    return ;
}

void Ldurb(char * restOfInstruction){
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

    uint64_t effectiveAddress = baseAddress + offset;

    int64_t dataLessSignificative = mem_read_32(effectiveAddress)&0xFF;

    NEXT_STATE.REGS[RtNum] = dataLessSignificative;

    NEXT_STATE.PC  += 4;
    return ;
}


void Ldurh(char * restOfInstruction){
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

    uint64_t effectiveAddress = baseAddress + offset;

    int64_t dataLessSignificative = mem_read_32(effectiveAddress)&0xFFFF;

    NEXT_STATE.REGS[RtNum] = dataLessSignificative;

    NEXT_STATE.PC  += 4;
    return ;
}