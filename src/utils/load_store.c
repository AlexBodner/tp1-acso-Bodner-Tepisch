#include "load_store.h"


void Stur(char * restOfInstruction){
    puts("Stur");
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
    uint32_t rnContent = CURRENT_STATE.REGS[RnNum];
    free(RnStr);
    
    // extraemos Rt, el índice del registro destino
    char * RtStr = malloc(5);
    strncpy(RtStr, restOfInstruction + 16, 5);
    int RtNum = (int) strtol(RtStr, NULL, 2);
    uint32_t rtContent = CURRENT_STATE.REGS[RtNum];

    free(RtStr);

    // Realizamos la operación de desplazamiento lógico a la derecha
    // uint64_t result = rnContent >> shiftAmount;
    uint32_t rtr = rnContent & 0xFFFFFFFF;
    uint32_t rtl = (rnContent >> 32) & 0xFFFFFFFF;
    mem_write_32((rnContent+immNum), rtr);
    mem_write_32((rnContent+immNum+4), rtl);

    NEXT_STATE.PC  += 4;
    printf("Stored 0x%lx at simulated memory address 0x%lx\n", rtr, rtl);
    return ;
}

void Sturb(char * restOfInstruction){
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

void Sturh(char * restOfInstruction){
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

void Ldur(char * restOfInstruction){
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

void Ldurb(char * restOfInstruction){
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


void Ldurh(char * restOfInstruction){
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