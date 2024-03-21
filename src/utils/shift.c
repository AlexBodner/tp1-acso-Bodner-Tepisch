#include "shift.h"

void LsImm(char * restOfInstruction){
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
    immr = 64 - immr;
    free(immrStr);

    // guardamos el inmediatoS 
    char * immsStr = malloc(6); 
    strncpy(immsStr, restOfInstruction + 7, 6);
    int imms = (int) strtol(immsStr, NULL, 2);
    free(immsStr);

    // extraemos Rn
    char * RnStr = malloc(5);
    strncpy(RnStr, restOfInstruction + 13, 5);
    int RnNum = (int) strtol(RnStr, NULL, 2);
    uint64_t rnContent = CURRENT_STATE.REGS[RnNum];
    free(RnStr);
    
    // extraemos Rd, el índice del registro destino
    char * RdStr = malloc(5);
    strncpy(RdStr, restOfInstruction + 18, 5);
    int RdNum = (int) strtol(RdStr, NULL, 2);
    uint64_t rdContent = CURRENT_STATE.REGS[RdNum];
    free(RdStr);

    //Hacemos la operacion
    if (imms!=31 && N==1){
        uint64_t result = rnContent << immr;
        NEXT_STATE.REGS[RdNum]  = result;
    }
    else if (imms==31 && N==1){
        uint64_t result = rnContent >> immr;
        NEXT_STATE.REGS[RdNum]  = result;
    }


    NEXT_STATE.PC  += 4;
    return ;
}
