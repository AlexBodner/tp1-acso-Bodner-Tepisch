#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "shell.h"
#include "HashMap/HashMap.h"


 #include "utils/branch.h"
#include "utils/load_store.h"
#include "./utils/mathOps.h"
#include "utils/shift.h"

int opLengths[] ={0,4+1, 5+1, 6+1,7+1,8+1,9+1,10+1,11+1,23} ;

void HLT(char * restOfInstruction){
    puts("hlt");
    RUN_BIT = 0;
    NEXT_STATE.PC+= 4;
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

// void Cbz(char * restOfInstruction){
//     puts("Cbz");    
//     char * immStr = malloc(sizeof(char) * (19));
//     strncpy(immStr, restOfInstruction, 19);
//     int64_t immNum = strtol(immStr, NULL, 2);
//     immNum = signExtendNto64(immNum,19);
//     free(immStr);

//     //guardamos Rm
//     char * RtStr = malloc(sizeof(char) * (5));
//     strncpy(RtStr, restOfInstruction+19, 5);
//     int RtNum= (int) strtol(RtStr, NULL, 2);
//     int64_t rtContent = CURRENT_STATE.REGS[RtNum];
//     free(RtStr);
//     if (rtContent == 00){
//         NEXT_STATE.PC += immNum* 4;

//     }   
//     else {
//         // Simplemente avanzar al siguiente conjunto de instrucciones si Z no es 1.
//         NEXT_STATE.PC += 4;
//     }
//     return ;
// }
// void mul(char * restOfInstruction){
//     //ADDS Xn + Xm to Xd

//     //guardamos Rm
//     char * RmStr = malloc(sizeof(char) * (5));
//     strncpy(RmStr, restOfInstruction, 5);
//     int RmNum= (int) strtol(RmStr, NULL, 2);
//     int64_t rmContent = CURRENT_STATE.REGS[RmNum];


//     //guardamos Rn
//     char * RnStr = malloc(sizeof(char) * (5));
//     strncpy(RnStr, restOfInstruction+5+6 , 5);
//     int RnNum= (int) strtol(RnStr, NULL, 2);
//     int64_t rnContent = CURRENT_STATE.REGS[RnNum];

//     //guardamos Rd
//     char * RdStr = malloc(sizeof(char) * (5));
//     strncpy(RdStr, restOfInstruction+5+6+5 , 5);
//     int RdNum= (int) strtol(RdStr, NULL, 2);

//     //Hacemos la operacion
//     int64_t result = rnContent * rmContent;

//     NEXT_STATE.REGS[RdNum]  = result;
//     NEXT_STATE.PC+= 4;
//     free(RmStr);
//     free(RnStr);
//     free(RdStr);

//     return ;
// }
// void Cbnz(char * restOfInstruction){
//     puts("Cbz");    
//     char * immStr = malloc(sizeof(char) * (19));
//     strncpy(immStr, restOfInstruction, 19);
//     int64_t immNum = strtol(immStr, NULL, 2);
//     immNum = signExtendNto64(immNum,19);
//     free(immStr);

//     //guardamos Rm
//     char * RtStr = malloc(sizeof(char) * (5));
//     strncpy(RtStr, restOfInstruction+19, 5);
//     int RtNum= (int) strtol(RtStr, NULL, 2);
//     int64_t rtContent = CURRENT_STATE.REGS[RtNum];
//     free(RtStr);
//     if (rtContent != 00){
//         NEXT_STATE.PC += immNum* 4;

//     }   
//     else {
//         // Simplemente avanzar al siguiente conjunto de instrucciones si Z no es 1.
//         NEXT_STATE.PC += 4;
//     }
//     return ;
// }

char *  decode(void (**fill_func_prt) ){
    dictionary_t *  opcodesMap = NULL; //SE DESTRUYE AL FINAL DE LA FUNCION Y SE DECLARA SIEMPRE

    if (opcodesMap == NULL){ 
        opcodesMap = dictionary_create(NULL);
        // chequear los numeros extended
         dictionary_put(opcodesMap, "10101011000", &addsExtendedReg); //ya con el 1 de sf agregado 
        dictionary_put(opcodesMap, "10110001", &addsImm); // pag. 531
        dictionary_put(opcodesMap, "11101011000", &subs_compExtendedReg); // pag. 934
        dictionary_put(opcodesMap, "11110001", &subs_compImm); // pag. 936
        dictionary_put(opcodesMap, "11010100", &HLT); // 
        dictionary_put(opcodesMap, "11101010", &AndsShiftedReg); // pag.542 - Que hago con el 1 de N? - no se usa el inmediato?
        dictionary_put(opcodesMap, "11001010", &EorShiftedReg); // pag. 620 - Aca no se updatean las flags?
        dictionary_put(opcodesMap, "10101010", &OrrShiftedReg); // 
        dictionary_put(opcodesMap, "000101", &B); // pag. 550 - Nose si esta bien
        // dictionary_put(opcodesMap, "", &Imm26); // Nose si esta explicando el caso anterior o es uno nuevo
        dictionary_put(opcodesMap, "1101011000111111000000", &Br); // pag. 562 - Revisar: llamado de funcion y NEXT_STATE.PC = CURRENT_STATE.REGS[RnNum];
        dictionary_put(opcodesMap, "01010100", &Bcond); // pag. 549
        // dictionary_put(opcodesMap, "110100110", &LslImm); // pag. 754 Revisar: shiftAmount
        // dictionary_put(opcodesMap, "110100110", &LsrImm); // pag. 757
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
         dictionary_put(opcodesMap, "10011011000", &mul); // ya con el 1 de sf agregaB
        //  dictionary_put(opcodesMap, "10110100", &Cbz); // ya con el 1 de sf agregaB
        //  dictionary_put(opcodesMap, "10110101", &Cbnz); // ya con el 1 de sf agregaB
        puts("crea dict");

    } 
    
    char * pcContentAsString = toBinaryString(mem_read_32(CURRENT_STATE.PC));
    //printf(" pcContentAsString %s", pcContentAsString );
    puts("lee pc");
    int opcodeSize;
    for (int i = 0;i<sizeof(opLengths)/ sizeof(opLengths[0]);i++){
        char * opCodeString = malloc(sizeof(char) * (opLengths[i]));
        strncpy(opCodeString, pcContentAsString , opLengths[i]);
        printf("opcode string %s", opCodeString);
        puts("flush dns");
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
    printf("opcodesize %i \n",opcodeSize);
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
        //free(instructionParams);
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
