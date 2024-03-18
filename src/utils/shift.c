#include "shift.h"
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