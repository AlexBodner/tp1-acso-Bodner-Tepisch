#include "hashTable.h"
#define MAX_SEARCHES 64
long int pythonHash(const char* a,const size_t len) {
    unsigned char* p = (unsigned char*)a;
    long x = *p << 7;
    long int len2= (long int)len;
    while (--len2 >= 0)
        x = (1000003 * x) ^ *p++;
    
    x ^= len;
    
    if (x == -1)
        x =(long ) -2;

    return x;
}

char*  getKey(registro_t * reg){
    return reg->key;
}
void*  getValue(registro_t * reg){
    return reg->value;
}

long int mod(long int a,long  int b){//modulo con b una potencia de 2
    return a &(b-1);
}
long int getNextIndex(long int j, int table_size, unsigned long int * perturb) { // como table size es potencia de 2, el modulo se puede calcular con el and 
    (*perturb) >>= PERTURB_SHIFT;
    return mod((j<<2)+j+1+*perturb, table_size);//esta es la actualizacion de python = (5*j+1 +perturb) mod table_size
    //return mod(j+1, table_size); //esta es la actualizacion del lineal
}

bool registerIsEmpty(registro_t * reg){//precondicion: no es NULL
    return reg->key==NULL;
}

long int hashear(const void *obj, size_t len, int modul){
    return mod(pythonHash(obj,len),modul);
}

bool tableInsert(table_t * tabla,const char * key, void* value){
    size_t len = strlen(key);

    long int index  = hashear(key, len, tabla->size);
    unsigned long int  perturb = index;
    // probamos insertar
    long int putPos= -1;
        int i = 0;
    while(i<MAX_SEARCHES){//o se puede hacer con i<tabla->size, pero si ya tengo que hacer + de 64 que es un poco mas que log n lo corto para hacer rehash
        if(registerIsEmpty(&tabla->registros[index])){
            if (putPos ==-1){
                putPos=index;
            }
            if(tabla->registros[index].isDeleted ==false){
                break;
            }
        }
        else{
            if(strcmp(key, getKey(&tabla->registros[index])) == 0){
                //update
                if ( tabla->destroy_func !=NULL){
                    (tabla->destroy_func)(getValue(&tabla->registros[index]));
                }
                tabla->registros[index].value = value;
                return true;
            }
        }
        i++;
        index = getNextIndex(index,  tabla->size,&perturb);
    }

    if (putPos==-1){
        return false;
    }

    char * newKey= malloc(sizeof(char)*(len + 1));
    if(newKey == NULL){
        return false;
    }
    strcpy(newKey,key);
    tabla->registros[putPos].key = newKey;
    tabla->registros[putPos].value = value;
    tabla->registros[putPos].keyLength = len;
    tabla->registros[putPos].isDeleted = false;
    (tabla->n_elements) ++;
    tabla->carga =(float) tabla-> n_elements / (float) tabla->size;
    return true;

}

bool tableInsertRehash(table_t * tabla,char* key, void *value, size_t keyLength){//funcion para el rehash, de esta forma no hago una copia de la key y se que no voy a tener borrados ni repetidos, por lo que inserto apenas encuentro lugar
    int i = 0;
    long int index = hashear(key, keyLength, tabla->size);
    unsigned long int  perturb = index;

    while(i<tabla->size){
        if(registerIsEmpty(&tabla->registros[index])){
            tabla->registros[index].key = key;
            tabla->registros[index].keyLength = keyLength;
            tabla->registros[index].value = value;
            (tabla->n_elements) ++;
            return true;
        }

        i++;
        index = getNextIndex(index,  tabla->size,&perturb);
        }
    return false;

}


table_t *table_create_size(destroy_f destroy, int size){
    table_t *tabla = malloc(sizeof(table_t));
    if (tabla == NULL)
    {
        return NULL;
    }
    tabla->size = size;
    tabla->cargaMin = (float) 0.1;
    tabla->n_elements = 0;
    tabla->carga = 0;
    tabla->cargaMax =(float)0.85 ; //python usa 2/3 o 1/2 segun su doc oficial,pero con mis experimentos obtuve mejores resultados asi
    registro_t * newRegistros = calloc(size, sizeof(registro_t));
    if (newRegistros == NULL)
    {
        free(tabla);
        return NULL;
    }
    tabla->destroy_func = destroy;
    tabla->registros = newRegistros;
    return tabla;
    }

void tableReduceElem(table_t *table){
    (table->n_elements)--;
    if (table->n_elements == 0){
        table->carga=0;
    }
    else{
        table->carga = ((float)table->n_elements) / (float)table->size;
    }
    return;
}

bool table_delete_item(table_t *table, const char *key){
    long int pos = table_search(table, key);
    if(pos==-1){
        return false;
    }
    registro_t * aux=  &table->registros[pos];
    if ( table->destroy_func !=NULL ){
        (table->destroy_func)(aux->value);
    }

    free(aux->key);
    aux->key=NULL;
    aux->value=NULL;
    aux->keyLength=0; //aunque esto solo lo voy a ver si está la key asi que podría no hacerse 
    aux->isDeleted = true;
    tableReduceElem(table);
    return true;
}

void table_destroy(table_t *table){
    for (int i =0; i<table->size;i++){
        registro_t * aux=& table->registros[i];
        if (!registerIsEmpty(aux)){
            if ( table->destroy_func !=NULL ){
                (table->destroy_func)(getValue(aux));
            }
            free(getKey(aux));    
        }
    }
    free(table->registros);

    free(table);
    return;
}
/*
funcion para liberar una tabla sin borrar keys y values
 rehasheada (xq el rehash usa los mismos registros_t *, key y value)
*/
void table_free(table_t *table){//

    free(table->registros);
    free(table);
    return;
}
/*Si encuentra devuelve el indice de la tabla en el que está el elemento y sino -1*/
long int table_search(table_t * table,const char * key){
    //long int perturb  = hashear(key, strlen(key), table->size);

    int i = 0;
    long int index =  hashear(key, strlen(key), table->size) ;
    unsigned long int  perturb = index;

    while(i<table->size){

        if(registerIsEmpty(&table->registros[index])){
            if(table->registros[index].isDeleted==false){
                return -1;
            }
        }
        else if(strcmp(key, getKey(&table->registros[index])) == 0){
            return index;
        }
        index = getNextIndex(index,  table->size,&perturb);
        i++;
    }
    return -1;
}