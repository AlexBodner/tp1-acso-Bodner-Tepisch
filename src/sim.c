#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"
#include "HashMap/HashMap.h"


 #include "utils/branch.h"
#include "utils/load_store.h"
#include "./utils/mathOps.h"
#include "utils/shift.h"

int opLengths[] ={0,4+1, 5+1, 6+1,7+1,8+1,9+1,10+1,11+1,23} ;

void HLT(char * restOfInstruction){
    RUN_BIT = 0;
    NEXT_STATE.PC+= 4;
    return ;
}


void Movz(char * restOfInstruction) {

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
        opcodesMap = dictionary_create(NULL);
        dictionary_put(opcodesMap, "10101011000", &addsExtendedReg); //ya con el 1 de sf agregado 
        dictionary_put(opcodesMap, "10110001", &addsImm); 
        dictionary_put(opcodesMap, "11101011000", &subs_compExtendedReg); 
        dictionary_put(opcodesMap, "11110001", &subs_compImm); 
        dictionary_put(opcodesMap, "11010100", &HLT);  
        dictionary_put(opcodesMap, "11101010", &AndsShiftedReg); 
        dictionary_put(opcodesMap, "11001010", &EorShiftedReg); 
        dictionary_put(opcodesMap, "10101010", &OrrShiftedReg); 
        dictionary_put(opcodesMap, "000101", &B); 
        dictionary_put(opcodesMap, "1101011000111111000000", &Br); 
        dictionary_put(opcodesMap, "01010100", &Bcond); 
        dictionary_put(opcodesMap, "110100110", &LsImm); 
        dictionary_put(opcodesMap, "11111000000", &Stur);
        dictionary_put(opcodesMap, "00111000000", &Sturb); 
        dictionary_put(opcodesMap, "01111000000", &Sturh); 
        dictionary_put(opcodesMap, "11111000010", &Ldur);
        dictionary_put(opcodesMap, "00111000010", &Ldurb); 
        dictionary_put(opcodesMap, "01111000010", &Ldurh); 
        dictionary_put(opcodesMap, "110100101", &Movz); 

        // -----------------------extras----------------------

        dictionary_put(opcodesMap, "10001011000", &AddExtendedReg); 
        dictionary_put(opcodesMap, "10010001", &AddImmediate); 
        dictionary_put(opcodesMap, "10011011000", &mul); 
        dictionary_put(opcodesMap, "10110100", &Cbz); 
        dictionary_put(opcodesMap, "10110101", &Cbnz); 
    } 
    
    char * pcContentAsString = toBinaryString(mem_read_32(CURRENT_STATE.PC));
    int opcodeSize;
    for (int i = 0;i<sizeof(opLengths)/ sizeof(opLengths[0]);i++){
        char * opCodeString = malloc(sizeof(char) * (opLengths[i]));
        strncpy(opCodeString, pcContentAsString , opLengths[i]);
        if (dictionary_contains(opcodesMap, opCodeString)){
            *fill_func_prt = dictionary_get(opcodesMap,opCodeString, NULL);
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
        puts("no se encontro el opcode");
    }
    else{
        (*func_ptr)(instructionParams);
        free(instructionParams);
    }
    
}
  