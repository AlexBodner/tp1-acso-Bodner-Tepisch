#include "test_malloc.h"
#include "testing.h"
#include "internal_iterator.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Acá implementa tus tests
bool square(const char* key, void* value, void* extra){
  int * p = value;
  *p = *p* (*p);
  return true;
}
bool test_square(){
    printf("========== %s ==========\n", __PRETTY_FUNCTION__);
    bool tests_result = true;
    dictionary_t *dict = dictionary_create(free);
    int* one = malloc(sizeof(int)); *one = 1;
    int * two = malloc(sizeof(int));*two = 2;
    int * three =malloc(sizeof(int));*three = 3;

    dictionary_put(dict, "key1", one);
    dictionary_put(dict, "key2", two);
    dictionary_put(dict, "key3", three);


    void* extra = malloc(1);
    iterate( dict, square,  extra);
    bool err = false;
    tests_result &= err==false;

    //tests_result &= (*a)  ==three;
    //tests_result &= err==false;

    tests_result &= *(int * )dictionary_get(dict,"key1",&err) ==1 ;
    test_assert("Se obtuvo el valor esperado con enteros", *(int * )dictionary_get(dict,"key1",&err) ==1);
    tests_result &= err==false;
    tests_result &= *(int * )dictionary_get(dict,"key2",&err) ==4;
        test_assert("Se obtuvo el valor esperado con enteros", *(int * )dictionary_get(dict,"key2",&err) ==4);
    tests_result &= err==false;
    tests_result &= *(int * )dictionary_get(dict,"key3",&err) ==9;
        test_assert("Se obtuvo el valor esperado con enteros", *(int * )dictionary_get(dict,"key3",&err) ==9);
    tests_result &= err==false;
    dictionary_destroy(dict);
    free(extra);
    return tests_result;
}

int main(void) {
  srand(117);
  int return_code = 0;
  return_code += test_square(); // Agrega tus propios tests acá
  exit(return_code);
}
