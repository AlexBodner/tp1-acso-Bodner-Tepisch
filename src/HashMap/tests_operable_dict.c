#include "test_malloc.h"
#include "testing.h"
#include "operable_dict.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
bool test_dic_update(){
  printf("========== %s ==========\n", __PRETTY_FUNCTION__);
  bool tests_result = true;
  dictionary_t *dict = dictionary_create(free);
  int* one = malloc(sizeof(int)); *one = 1;
  int * two = malloc(sizeof(int));*two = 2;
  int* four = malloc(sizeof(int)); *four = 4;
  int* five = malloc(sizeof(int)); *five = 5;


   dictionary_put(dict, "key1", one);
   dictionary_put(dict, "key2", two);

 dictionary_t *dict2 = dictionary_create(free);
  dictionary_put(dict2, "key1", four);
  dictionary_put(dict2, "key3", five);
  tests_result&= dictionary_update(dict, dict2);
  bool err;
  tests_result &= dictionary_get(dict,"key1",&err) == four;
  tests_result &= err==false;

  tests_result &= dictionary_get(dict,"key2",&err) == two;
  tests_result &= err==false;
  
  tests_result &= dictionary_get(dict,"key3",&err) == five;
  tests_result &= err==false;
  dictionary_destroy(dict);
  special_dictionary_destroy(dict2);

  test_assert("Se hizo bien el UPDATE", tests_result);

  return tests_result;
}
bool test_dic_and(){
    printf("========== %s ==========\n", __PRETTY_FUNCTION__);
    bool tests_result = true;
    dictionary_t *dict = dictionary_create(free);
    int* one = malloc(sizeof(int)); *one = 1;
    int * two = malloc(sizeof(int));*two = 2;
    int * three =malloc(sizeof(int));*three = 3;

    dictionary_put(dict, "key1", one);
    dictionary_put(dict, "key2", two);
    dictionary_put(dict, "key3", three);

    dictionary_t *dict2 = dictionary_create(free);
    int* four = malloc(sizeof(int)); *four = 4;
    int* five = malloc(sizeof(int)); *five = 5;

    
    dictionary_put(dict2, "key1", four);
    dictionary_put(dict2, "key3", five);
    bool err;
    dictionary_t* dictAnd  = dictionary_and(dict, dict2);
    tests_result &= dictAnd!=NULL;

    tests_result &= dictionary_size(dictAnd)==2;
    tests_result &= dictionary_get(dictAnd,"key1",&err) == one; 
    tests_result &= err==false;

    //tests_result &= (*a)  ==three;
    //tests_result &= err==false;

    tests_result &= dictionary_get(dictAnd,"key3",&err) == three;
    tests_result &= err==false;
    tests_result &= dictionary_get(dictAnd,"key2",&err) == NULL;
    tests_result &= err==true;
    tests_result &= dictionary_size(dictAnd) == 2;
    test_assert("El tamaño del diccionario despues del and es 2",  (dictionary_size(dictAnd) == 2));

    dictionary_destroy(dict);
    dictionary_destroy(dict2);
    special_dictionary_destroy(dictAnd);
      test_assert("Se hizo bien el AND", tests_result);
    return tests_result;
}
bool test_dic_or(){
    printf("========== %s ==========\n", __PRETTY_FUNCTION__);
    bool tests_result = true;
    dictionary_t *dict = dictionary_create(free);
    int* one = malloc(sizeof(int)); *one = 1;
    int * two = malloc(sizeof(int)); *two = 2;
    int * three =malloc(sizeof(int)); *three = 3;

    dictionary_put(dict, "key1", one);
    dictionary_put(dict, "key2", two);
    dictionary_put(dict, "key3", three);

    dictionary_t *dict2 = dictionary_create(free);
    int* four = malloc(sizeof(int)); *four = 4;
    int* five = malloc(sizeof(int)); *five = 5;
    int* six = malloc(sizeof(int)); *six = 6;

    
    dictionary_put(dict2, "key1", four);
    dictionary_put(dict2, "key3", five);
    dictionary_put(dict2, "key6", six);

    bool err;
    dictionary_t* dictOR  = dictionary_or(dict, dict2);
    tests_result &= dictOR!=NULL;

    tests_result &= dictionary_get(dictOR,"key1",&err) == one; 
    tests_result &= err==false;

    //tests_result &= (*a)  ==three;
    //tests_result &= err==false;

    tests_result &= dictionary_get(dictOR,"key3",&err) == three;
    tests_result &= err==false;
    tests_result &= dictionary_get(dictOR,"key2",&err) == two;
    tests_result &= err==false;
    tests_result &= dictionary_get(dictOR,"key6",&err) == six;
    tests_result &= err==false;
    tests_result &= dictionary_size(dictOR) == 4;
    test_assert("El tamaño del diccionario despues del OR es el correcto",  dictionary_size(dictOR) == 4);
    special_dictionary_destroy(dictOR);

    dictionary_destroy(dict);

    dictionary_destroy(dict2);

    test_assert("Se hizo bien el OR", tests_result);
    
    return tests_result;
}
bool test_dic_equals(){
    printf("========== %s ==========\n", __PRETTY_FUNCTION__);
    bool tests_result = true;
    dictionary_t *dict = dictionary_create(free);
    int* one = malloc(sizeof(int)); *one = 1;
    int * two = malloc(sizeof(int));*two = 2;
    int * three =malloc(sizeof(int));*three = 3;

    dictionary_put(dict, "key1", one);
    dictionary_put(dict, "key2", two);
    dictionary_put(dict, "key3", three);

    dictionary_t *dict2 = dictionary_create(free);
    
    
    dictionary_put(dict2, "key2", two);
    dictionary_put(dict2, "key3", three);
    dictionary_put(dict2, "key1", one);
    tests_result &= dictionary_equals(dict,dict2) == true;
    test_assert("Son iguales",  dictionary_equals(dict,dict2) == true);

    special_dictionary_destroy(dict2);
    dict2 = dictionary_create(free);
 
    dictionary_put(dict2, "key2", one);
    dictionary_put(dict2, "key3", three);
    dictionary_put(dict2, "key1", two);
    test_assert("No son iguales",  dictionary_equals(dict,dict2) == false);
    tests_result &= dictionary_equals(dict,dict2) == false;
    dictionary_destroy(dict);
    special_dictionary_destroy(dict2);
    
    return tests_result;
    }  
// Acá implementa tus tests

int main(void) {
  srand(117);
  int return_code = 0;
  return_code+=test_dic_update();
  return_code+=test_dic_and();
  return_code+=test_dic_or();
  return_code+=test_dic_equals();
   // Agrega tus propios tests acá
  exit(return_code);
}
