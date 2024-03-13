#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "shell.h"
#include "HashMap/HashMap.h"

int opLengths[] ={7+1, 10+1} ;
dictionary_t *  opcodesMap = NULL; //FALTA LIBERARLO AL FINAL DE TODO
//Insert OPCode, func *
//habria que sumarle un 1 al principio a todos los opcodes
void addsExtendedReg(){
    puts("extended");
    return ;
}
void addsImm(){
    puts("Imm");
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

void decode(void (**fill_func_prt) ){
 if (opcodesMap == NULL){ 
        puts("crea dict");
        opcodesMap = dictionary_create(NULL);
        dictionary_put(opcodesMap, "101011001", &addsExtendedReg); // ya con el 1 de sf agregado
        dictionary_put(opcodesMap, "10110001", &addsImm); // ya con el 1 de sf agregado
    } 
    
    char * pcContentAsString = toBinaryString(mem_read_32(CURRENT_STATE.PC));
    printf(" pcContentAsString %s", pcContentAsString );

    for (int i = 0;i<sizeof(opLengths)/ sizeof(int);i++){
        puts("aca");
        char * opCodeString = malloc(sizeof(char) * (opLengths[i]));
        strncpy(opCodeString, pcContentAsString , opLengths[i]);
        puts("aca x2");
        printf("opcode string %s", opCodeString);

        if (dictionary_contains(opcodesMap, opCodeString)){
            puts("aca x3");
            *fill_func_prt = dictionary_get(opcodesMap,opCodeString, NULL);

        }
        puts("sale");
        free(opCodeString);
    }
    free(pcContentAsString);
    return;
}
void process_instruction(){
    void (*func_ptr)() = NULL;
    decode(&func_ptr );
    if (func_ptr == NULL){
        //la instruccion no fue encontrada
    }
    else{
        (*func_ptr)();
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
