#pragma once
#include <string.h>
#include <stdint.h>
//#include "t1ha-master/t1ha-master/src/t1ha1.c"
#include <stdlib.h>
#include <stdbool.h>
#define PERTURB_SHIFT 5



typedef void (*destroy_f)(void *);

typedef struct registro{
  char *key;
  size_t keyLength;
  char *value;
  bool isDeleted;
} registro_t;

typedef struct table{
    registro_t * registros;
    float cargaMax;
    float cargaMin;
    destroy_f destroy_func;
    int size;
    int n_elements;
    float carga;
} table_t;

bool tableInsert(table_t * tabla,const char * key, void* value);
bool tableInsertRehash(table_t * tabla,char* key, void *value, size_t keyLength);
long int hashear(const void *obj, size_t len, int mod);
bool registerIsEmpty(registro_t * reg);
table_t *table_create_size(destroy_f destroy, int size);
void tableReduceElem(table_t *table);
bool table_delete_item(table_t *table, const char *key);
long int table_search(table_t * table, const char * key);
void table_destroy(table_t *table);
void table_free(table_t *table);
long int mod(long int a,long  int b);
char*  getKey(registro_t * reg);
void*  getValue(registro_t * reg);