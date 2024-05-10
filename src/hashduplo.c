#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/libtrab.h"
#define SEED 0x12345678
#define TAM 10891

int hash1(const char* str, uint32_t h){
    for (; *str; ++str) {
        h ^= *str;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}

int hash2(const char * str){
    int tam = strlen(str);
	int total = 0;

	for (int i = 0; i < tam; i++) {
		total += str[i];
    }
	return total;
}

int hashduplo(const char *key, uint32_t h,int i){
    return abs(hash1(key,h) + i * hash2(key));
}

int hash_insere(thash * h, void * bucket){
    int i=0;
    int pos = hashduplo(h->get_key(bucket),SEED, i)%TAM;
    //se esta cheio
    if (h->max == (h->size)+1) {
        free(bucket);
        return EXIT_FAILURE;    
    }

    //inserindo
    while (h->table[pos] != 0) {
        if (h->table[pos] == h->deleted) break;
        pos = hashduplo(h->get_key(bucket),SEED, ++i)%TAM;
    }
    h->table[pos] = (uintptr_t)bucket;
    h->size += 1;
    return EXIT_SUCCESS;
}



int hash_constroi(thash * h,int nbuckets, char * (*get_key)(void *) ){
    h->table = calloc(sizeof(void *), nbuckets + 1); // pois para caber 10 elementos precisamos de 11 espaços
    
    if (h->table == NULL)
        return EXIT_FAILURE;

    h->max = nbuckets + 1;
    h->size = 0;
    h->deleted = (uintptr_t)&(h->size);
    h->get_key = get_key;
    return EXIT_SUCCESS;

}


void * hash_busca(thash  h, const char * key){//tirar num_cidades e cidades
    int i = 0;
    int pos = hashduplo(key, SEED, i)%TAM;
    void * ret = NULL;
    while(h.table[pos]!=0 && ret == NULL){
        if (strcmp(h.get_key((void*)h.table[pos]),key) == 0){
            ret = (void *) h.table[pos];
        }else{
            pos = hashduplo(key, SEED, ++i)%TAM;
        }
    }
    return ret;

}

void hash_busca_cidades(thash h, const char *key, int * cidades, int *num_cidades){
    int i = 0;
    int pos = hashduplo(key, SEED, i)%TAM;
    while(h.table[pos]!=0){
        if (strcmp(h.get_key((void*)h.table[pos]),key) == 0){
            printf("entrou na busca\n");
            cidades[*num_cidades] = pos; //isso ta dando errado pq?
            printf("dps da busca\n");
            pos = hashduplo(key, SEED, ++i)%TAM;
            *num_cidades+=1;
        }else{
            pos = hashduplo(key, SEED, ++i)%TAM;
        }
    }

}

int hash_remove(thash * h, const char * key){
    int i = 0;
    int pos = hashduplo(key, SEED, i)%TAM;;
    while(h->table[pos]!=0){
        if (strcmp(h->get_key((void*)h->table[pos]),key) == 0){ /* se achei remove*/
            free((void *)h->table[pos]);
            h->table[pos] = h->deleted;
            h->size -= 1;
            return EXIT_SUCCESS;
        }else{
            pos = hashduplo(key, SEED, ++i)%TAM;
        }
    }
    return EXIT_FAILURE;
}

void hash_apaga(thash *h){
    int pos;
    for (pos = 0; pos < h->max; pos++){
        if (h->table[pos] != 0 && h->table[pos] != h->deleted)
            free((void*) h->table[pos]);

    }
    free(h->table);
}