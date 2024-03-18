#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "shell.h"
#include "HashMap/HashMap.h"

#include "utils/branch.h"
#include "utils/load_store.h"
#include "utils/mathOps.h"
#include "utils/shift.h"

int opLengths[] ={7+1, 10+1} ;

void HLT(char * restOfInstruction){
    puts("hlt");
    RUN_BIT = 0;
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

char *  decode(void (**fill_func_prt) ){
    dictionary_t *  opcodesMap = NULL; //SE DESTRUYE AL FINAL DE LA FUNCION Y SE DECLARA SIEMPRE

    if (opcodesMap == NULL){ 
        puts("crea dict");
        opcodesMap = dictionary_create(NULL);
        // chequear los numeros extended
         dictionary_put(opcodesMap, "10101011001", &addsExtendedReg); //ya con el 1 de sf agregado 
        dictionary_put(opcodesMap, "10110001", &addsImm); // pag. 531
        // dictionary_put(opcodesMap, "1101011001", &subsExtendedReg); // 
        dictionary_put(opcodesMap, "11110001", &subsImm); // pag. 936
        dictionary_put(opcodesMap, "11010100", &HLT); // 
        // dictionary_put(opcodesMap, "", &compExtendedReg); // 
        dictionary_put(opcodesMap, "11110001", &compImm); // Revisar como se guarda la resta
        dictionary_put(opcodesMap, "11101010", &AndsShiftedReg); // pag.542 - Que hago con el 1 de N? - no se usa el inmediato?
        dictionary_put(opcodesMap, "11001010", &EorShiftedReg); // pag. 620 - Aca no se updatean las flags?
        dictionary_put(opcodesMap, "10101010", &OrrShiftedReg); // 
        dictionary_put(opcodesMap, "000101", &B); // pag. 550 - Nose si esta bien
        // dictionary_put(opcodesMap, "", &Imm26); // Nose si esta explicando el caso anterior o es uno nuevo
        dictionary_put(opcodesMap, "11010110001111110000001111110000", &Br); // pag. 562 - Revisar: llamado de funcion y NEXT_STATE.PC = CURRENT_STATE.REGS[RnNum];
        dictionary_put(opcodesMap, "", &Beq); // pag. 138 - Revisar: llamado de funcion y funcionamiento
        dictionary_put(opcodesMap, "", &Bne); // 
        dictionary_put(opcodesMap, "", &Bgt); // 
        dictionary_put(opcodesMap, "", &Blt); // 
        dictionary_put(opcodesMap, "", &Bge); // 
        dictionary_put(opcodesMap, "", &Ble); // 
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
        // dictionary_put(opcodesMap, "", &Mul); // ya con el 1 de sf agregaB
        // dictionary_put(opcodesMap, "", &Cbz); // ya con el 1 de sf agregaB
        // dictionary_put(opcodesMap, "", &Cbnz); // ya con el 1 de sf agregaB
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
