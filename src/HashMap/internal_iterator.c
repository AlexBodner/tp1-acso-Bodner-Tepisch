#include "internal_iterator.h"

/*
 * Punto extra de internal iterator, suma 1 punto como máximo.
 */

/*
 * Itera cada par clave-valor del diccionario pasandoselo a la función de iteración.
 * Recibe un parámetro extra que puede contener cualquier cosa para permitirle a la función guardar resultados.
 */
void iterate(dictionary_t* dict, iterate_f f, void* extra){
    if (dict ==NULL){
        return;
    }
    for(int i=0; i<dict->tabla->size; i++){
        registro_t * reg=  &dict->tabla->registros[i];
        if(!registerIsEmpty(reg)){
            if(f!=NULL){
                f(getKey(reg),  getValue(reg),  extra);
            }
        }
    }
    return;
}
