#include "HashMap.h"

bool rehash(dictionary_t *dict, int newSize){
    //printf("entra con carga %f", dict->tabla->carga);
    //puts("flush");
    table_t* newTable = table_create_size(dict->tabla->destroy_func, newSize);
    if(newTable ==NULL){
      return false;
    }
    for (int  i = 0; i < dict->tabla->size; i++){
        registro_t * r= & dict->tabla->registros[i];
        if (!registerIsEmpty(r)){
            if(tableInsertRehash(newTable,getKey(r), getValue(r), r->keyLength) == false){
                table_free(newTable);
                return false;
            }
        }
    }
    
    table_free(dict->tabla);
    newTable->carga =(float) newTable-> n_elements / (float) newTable->size;

    dict->tabla = newTable;
    return true;
}

bool augment(dictionary_t *dict){
    int newSize = 4* dict->tabla->size;
    return rehash(dict, newSize);
}

bool decrement(dictionary_t *dict){
    int newSize = dict->tabla->size /2;
    return  rehash(dict, newSize);
}



dictionary_t *dictionary_create_size(destroy_f destroy, int size){
  dictionary_t *dict = malloc(sizeof(dictionary_t));
  if (dict == NULL)
  {
    return NULL;
  }
  
  table_t * newTabla = table_create_size(destroy,  size);
  if (newTabla == NULL)
  {
    free(dict);
    return NULL;
  }
  dict->tabla = newTabla;
  return dict;
}

dictionary_t *dictionary_create(destroy_f destroy){
    return dictionary_create_size(destroy, 256);
}

bool dictionary_put(dictionary_t *dictionary, const char *key, void *value){
    // chech factor de carga

    if (dictionary->tabla->carga > dictionary->tabla->cargaMax   ){
         // ☠️
        if (!augment(dictionary)){
            return false; //si falla rehash no puede asegurar O(1) entonces no inserto
        }
    }

    bool ok = tableInsert(dictionary->tabla,key,value);

    if (!ok){//si no pude insertar pruebo rehashear 
      if(augment(dictionary)){
          if(tableInsert(dictionary->tabla,key,value)){
              return true; //rehashee bien y pude insertar en la nueva tabla
          }
      }
      return false;
    }
    else{
      return true; //pude insertar sin tener que rehashear
    }
}

void *dictionary_get(dictionary_t *dictionary, const char *key, bool *err){
  long int position = table_search(dictionary->tabla,key);
  if(position == -1){
    *err = true;
    return NULL;
  }
  *err = false;
  
  return getValue(&dictionary->tabla->registros[position]);
}

bool dictionary_delete(dictionary_t *dictionary, const char *key){
  
    bool ok = table_delete_item(dictionary->tabla, key);
    if (dictionary->tabla->carga < dictionary->tabla->cargaMin && dictionary->tabla->size>260){
        decrement(dictionary);
    }
    return ok;
}

void *dictionary_pop(dictionary_t *dictionary, const char *key, bool *err)
{
    long int pos = table_search(dictionary->tabla, key);
    if(pos==-1){
        *err = true;
        return NULL;
    }
    registro_t * aux = &dictionary->tabla->registros[pos];
    void  * val= getValue(aux);
    free(getKey(aux));
    *err = false;
    aux->key=NULL;
    aux->value = NULL;
    aux->keyLength = 0;
    aux->isDeleted = true;
    tableReduceElem(dictionary->tabla);
    if (dictionary->tabla->carga < dictionary->tabla->cargaMin && dictionary->tabla->size>260){
        decrement(dictionary);
    }
    return val;
}

bool dictionary_contains(dictionary_t *dictionary, const char *key)
{
  return table_search(dictionary->tabla, key)!=-1;
}

size_t dictionary_size(dictionary_t *dictionary) {
  return dictionary->tabla->n_elements;
}
void dictionary_destroy(dictionary_t *dictionary){
  table_destroy(dictionary->tabla);
  free(dictionary);
}
