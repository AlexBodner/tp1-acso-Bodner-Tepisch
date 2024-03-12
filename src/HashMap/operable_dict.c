#include <stdbool.h>
#include "operable_dict.h"
#include "hashTable.h"
/*
 * Punto extra de operable dict, suma 3 puntos como máximo.
 */

/*
 * Inserta o pisa en dictionary1 todas las claves y valores que están en dictionary2.
 * Las claves se mantienen independientes entre ambos diccionarios, pero los valores no.
 * (las claves nuevas de agregar dictionary2 en dictionary1 deben ser punteros distintos
 * pero los valores son el mismo puntero).
 * La operación es atómica, si falla dictionary1 queda en el mismo estado que estaba antes del update.
 * Devuelve true si funcionó, false si fayó.
 */
/*Precondicion para todos los métodos: los diccionario existen, los punteros no son NULL*/


void special_table_destroy(table_t *table){  //borra todo de la tabla menos los values, es como si la funcion de destruccion fuera null
    for (int i =0; i<table->size;i++){
        registro_t * aux=& table->registros[i];
        if (!registerIsEmpty(aux)){     
            free(getKey(aux));
        }
        //free(aux);
    }
    free(table->registros);

    free(table);
    return;
}

void special_dictionary_destroy(dictionary_t *dictionary){ //es un dict destroy pero que no borra los values, lo uso para tratar la destruccion en los tests en vez de crear el dict con func de destruccion NULL 
    special_table_destroy(dictionary->tabla);
    free(dictionary);
}

bool dictionary_update(dictionary_t *dictionary1, dictionary_t *dictionary2){
    dictionary_t  * auxDict = dictionary_create(dictionary1->tabla->destroy_func);
    if (auxDict == NULL){
        return false;
    }
    for(int i=0; i<dictionary1->tabla->size;i++){
        registro_t * reg=  &dictionary1->tabla->registros[i];
        if(!registerIsEmpty(reg)){
            if(!dictionary_put(auxDict,getKey(reg),getValue(reg))){
                special_dictionary_destroy(auxDict);
                return false;
                }            
            }
    }
    for(int i=0; i<dictionary2->tabla->size;i++){
    registro_t * reg =  &dictionary2->tabla->registros[i];
    if(!registerIsEmpty(reg)){
        if(!dictionary_put(auxDict,getKey(reg),getValue(reg))){
            special_dictionary_destroy(auxDict);
            return false;
            }            
        }
    }

    special_table_destroy(dictionary1->tabla);
    dictionary1->tabla = auxDict->tabla;
    free(auxDict);
    return true;
}

/*
 * Combina ambos diccionarios en uno nuevo que contiene solo las claves que están presentes en ambos.
 * En todos los casos se conservan los valores del dictionary1.
 * Devuelve NULL si falla.
 */

dictionary_t* dictionary_and(dictionary_t *dictionary1, dictionary_t *dictionary2){
    dictionary_t *newDict = dictionary_create(dictionary1->tabla->destroy_func);
    if (newDict == NULL){
        return NULL;
    }
    for(int i=0; i<dictionary1->tabla->size; i++){
        registro_t * reg = &dictionary1->tabla->registros[i];
        if(!registerIsEmpty(reg)){

            if(dictionary_contains(dictionary2,getKey(reg))){
                
                if(!dictionary_put(newDict,getKey(reg), getValue(reg))){
                    special_dictionary_destroy(newDict);
                    return NULL;
                }
            }
        }
    }
    return newDict;
}

/*
 * Combina ambos diccionarios en uno nuevo que contiene todas las claves de ambos.
 * En caso de que ambos tengan una misma clave se conserva el valor de dictionary1.
 * Devuelve NULL si falla.
 */
dictionary_t* dictionary_or(dictionary_t *dictionary1, dictionary_t *dictionary2){
    dictionary_t *newDict = dictionary_create(NULL);
    if(newDict == NULL){
        return NULL;
    }
    for(int i=0; i<dictionary2->tabla->size;i++){
        registro_t * reg=  &dictionary2->tabla->registros[i];
        if(!registerIsEmpty(reg)){
            if(!dictionary_put(newDict,getKey(reg),getValue(reg))){
                special_dictionary_destroy(newDict);
                return NULL;
            }            
        }
    }

    for(int i=0; i<dictionary1->tabla->size;i++){
        registro_t * reg=  &dictionary1->tabla->registros[i];
        if(!registerIsEmpty(reg)){
            if(!dictionary_put(newDict,getKey(reg),getValue(reg))){
                special_dictionary_destroy(newDict);
                return NULL;
            }            
        }
    }
    newDict->tabla->destroy_func=dictionary1->tabla->destroy_func;
    return newDict;
}

/*
 * Devuelve true si ambos diccionarios son iguales. Dos diccionarios son iguales sí:
 *  - Las claves son iguales aunque puedan tener distinta posición en memoria.
 *  - Los values son los mismos punteros para cada clave
 *  - Tienen la misma cantidad de claves
 */
bool dictionary_equals(dictionary_t *dictionary1, dictionary_t *dictionary2){
    if(dictionary_size(dictionary1)!= dictionary_size(dictionary2)){
        return false;
    }
    for (int i=0; i<dictionary1->tabla->size; i++){
        registro_t * reg = &dictionary1->tabla->registros[i];
        registro_t * reg2 = &dictionary2->tabla->registros[i];
        bool err1,err2;
        if (!registerIsEmpty(reg)){
            if(dictionary_get(dictionary1,getKey(reg),&err1) != dictionary_get(dictionary2,getKey(reg2),&err2 ) || err2 ==true){
                return false;
            }
        }
    }//tecnicamente lo hacemos en O(n) con la garantía de que la tabla es O(1) en promedio
    return true;
}
