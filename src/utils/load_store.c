#include "load_store.h"
void LslImm(char * restOfInstruction){
    puts("LslImm");
    //ADDS Xn + imm to Xd
    //bit 22 es N 
    //immr del 21 al 16 inclusives 
    //imms del 15 al 10 
    //Rn del 9 al 5 
    //Rd 4 al 0
    

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

void LsrImm(char 
* restOfInstruction){
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
    printf("Stored 0x%llx at simulated memory address 0x%llx\n", data, effectiveAddress);
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
    printf("Stored byte 0x%02x at simulated memory address 0x%llx\n", data, effectiveAddress);
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
    printf("Stored byte 0x%02x at simulated memory address 0x%llx\n", data, effectiveAddress);
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
    printf("Loaded 0x%llx into X%d from simulated memory address 0x%llx\n", data, RtNum, effectiveAddress);
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
    printf("Loaded byte 0x%02x into W%d from simulated memory address 0x%llx\n", byteData, RnNum, effectiveAddress);
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
    printf("Loaded half-data 0x%04x into W%d from simulated memory address 0x%llx\n", halfdata, RnNum, effectiveAddress);
    return ;
}