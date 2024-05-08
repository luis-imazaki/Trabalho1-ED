#ifndef __ABB__
#define __ABB__
#include <stdint.h>

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
 
void heapsort(theap v[], int tam);
int insere_elemento(theap v[], int *tam, int max, void * reg, double dist);
void abb_constroi(tarv *parv, double (*cmplat)(void *, void *), double(*cmplong)(void *, void *),double (*calcula_dist)(void *, void *));
int abb_insere(tarv * parv,  void * reg);
void abb_busca_prox(tarv * parv,tnode *pnode,  void * reg, theap * heap,int  qtd_vizinhos, int n, int nivel, int * tam);
void abb_busca(tarv * parv,  void * reg, theap * heap,int qtd_vizinhos, int n);
#endif