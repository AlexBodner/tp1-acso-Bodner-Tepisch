#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "shell.h"
#include "HashMap/HashMap.h"

int opLengths[] ={7+1, 10+1} ;
//Insert OPCode, func *
//habria que sumarle un 1 al principio a todos los opcodes
void addsExtendedReg(char * restOfInstruction){
    puts("extended");
    return ;
}
void HALT(char * restOfInstruction){
    puts("halt");
    RUN_BIT = 0;
    return ;
}
void addsImm(char * restOfInstruction){
    puts("Imm");
    //ADDS Xn + imm to Xd
    //bits 23 y 22 son de shift y deberian ser 00 
    //imm del 21 al 10 inclusives 
    //Rn del 9 al 5 
    //Rd 4 al 0
    char * immStr = malloc(sizeof(char) * (12));
    strncpy(immStr, restOfInstruction+2 , 12);
    int immNum= (int) strtol(immStr, NULL, 2);

    char * RnStr = malloc(sizeof(char) * (5));
    strncpy(RnStr, restOfInstruction+14 , 5);
    int RnNum= (int) strtol(RnStr, NULL, 2);

    char * RdStr = malloc(sizeof(char) * (5));
    strncpy(RdStr, restOfInstruction+14+5 , 5);
    int RdNum= (int) strtol(RdStr, NULL, 2);
    int rdContent = CURRENT_STATE.REGS[RdNum];
    int result = rdContent + immNum;
    if (result<0){
        //set negative flag
        NEXT_STATE.FLAG_N = 1;
    } 
    else{
        NEXT_STATE.FLAG_N = 0;
    }
    if (result == 0){
        //set 0 Flag
        NEXT_STATE.FLAG_Z = 1;
    }
    else{
        NEXT_STATE.FLAG_Z = 0;

    }

    NEXT_STATE.REGS[RdNum]  = result;
    printf("Result %i\n", result);
    NEXT_STATE.PC+= 4;
    return ;
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

char *  decode(void (**fill_func_prt) ){
    dictionary_t *  opcodesMap = NULL; //SE DESTRUYE AL FINAL DE LA FUNCION Y SE DECLARA SIEMPRE

    if (opcodesMap == NULL){ 
        puts("crea dict");
        opcodesMap = dictionary_create(NULL);
        dictionary_put(opcodesMap, "101011001", &addsExtendedReg); // ya con el 1 de sf agregado
        dictionary_put(opcodesMap, "10110001", &addsImm); // ya con el 1 de sf agregado
        dictionary_put(opcodesMap, "11010100", &HALT); // ya con el 1 de sf agregado

    } 
    
    char * pcContentAsString = toBinaryString(mem_read_32(CURRENT_STATE.PC));
    //printf(" pcContentAsString %s", pcContentAsString );
    int opcodeSize;
    for (int i = 0;i<sizeof(opLengths)/ sizeof(int);i++){
        char * opCodeString = malloc(sizeof(char) * (opLengths[i]));
        strncpy(opCodeString, pcContentAsString , opLengths[i]);
        printf("opcode string %s", opCodeString);

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
