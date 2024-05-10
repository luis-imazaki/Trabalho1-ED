#ifndef __LIBTRAB__
#define __LIBTRAB__
#include <stdint.h>
typedef struct{
    char codigo_ibge[15];
    char nome[40];
    double latitude;
    double longitude;
    int capital;
    int codigo_uf;
    int siafi_id;
    int ddd;
    char fuso_horario[60];
}tmunicipio;


typedef struct {
	uintptr_t * table;
	int size;
	int max;
	uintptr_t deleted;
	char* (*get_key)(void *);
} thash;

typedef struct _tnode{
    void * reg;
    struct _tnode *esq;
    struct _tnode *dir;
}tnode;

typedef struct _abb{
    tnode * raiz;
    double (*cmplat)(void* , void *);
    double (*cmplong)(void* , void *);
    double (*calcula_dist)(void *, void *);
}tarv;

typedef struct _heap{
    void * reg;
    double dist;
}theap;

int hash_insere(thash * h, void * bucket);
int hash_constroi(thash * h,int nbuckets, char * (*get_key)(void *) );
void * hash_busca(thash h, const char * key);
void hash_busca_cidades(thash, const char *key, int * cidades, int * num_cidades);
int hash_remove(thash * h, const char * key);
void hash_apaga(thash *h);
void heapsort(theap v[], int tam);
int insere_elemento(theap v[], int *tam, int max, void * reg, double dist);
void kdtree_constroi(tarv *parv, double (*cmplat)(void *, void *), double(*cmplong)(void *, void *),double (*calcula_dist)(void *, void *));
int kdtree_insere(tarv * parv,  void * reg);
void kdtree_vizinhos(tarv * parv,tnode *pnode,  void * reg, theap * heap,int  qtd_vizinhos, int n, int nivel, int * tam);
void kdtree_busca(tarv * parv,  void * reg, theap * heap,int qtd_vizinhos, int n);
#endif