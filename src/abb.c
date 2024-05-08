#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include"../include/abb.h"

int pai(int n){
    return (n-1)/2;
}

int filho_esq(int n){
    return n*2 + 1;
}

int filho_dir(int n){
    return n*2 + 2;
}

void troca(theap *a,theap *b){
    theap aux;
    aux = *a;
    *a = *b;
    *b = aux;
}

void desce(theap V[],int n, int tam){
    int maior=n,esq=filho_esq(n),dir=filho_dir(n);
    if(esq<tam && V[esq].dist>V[maior].dist){
        maior = esq;
    }
    if(dir<tam&& V[dir].dist>V[maior].dist){
        maior = dir;
    }
    if(maior!=n){
        troca(&V[n],&V[maior]);
        desce(V,maior,tam);
    }

}

void sobe(theap v[], int n){
    printf("%f -  %d - pai\n",v[n].dist,pai(n) );
    if(v[n].dist>v[pai(n)].dist){
        troca(&v[n],&v[pai(n)]);
        sobe(v,pai(n));
    }
}

int insere_elemento(theap v[], int *tam, int max, void * reg, double dist) {
    printf("inserindo heap\n");
    
    if (*tam >= max) //nao sei se é >= ou ==
        return EXIT_FAILURE;

    v[*tam].reg =  reg;
    v[*tam].dist = dist;
    sobe(v, *tam);
    *tam += 1;
    return EXIT_SUCCESS;
}

void altera_prioridade(theap v[], int tam, int n, int dist, void * reg) {
	theap anterior = v[n];
    v[n].reg = reg;
	v[n].dist = dist;

	if (dist > anterior.dist)
		sobe(v, n);
	if (dist < anterior.dist)
		desce(v, tam, n);
}

void heapsort(theap v[], int tam){
    for(int i = tam-1;i>=0;i--){
        troca(&v[0],&v[i]);
        desce(v,0,i);
    }
}
void abb_constroi(tarv *parv, double (*cmplat)(void *, void *),double (*cmplong)(void *, void *), double (*calcula_dist)(void *, void *)){
    printf("construiu abb!\n");
    parv->raiz = NULL;
    parv->cmplat  = cmplat;
    parv->cmplong = cmplong;
    parv->calcula_dist = calcula_dist;
}

int abb_insere_node(tarv * parv,tnode ** ppnode,void *reg, int nivel){
    if (*ppnode == NULL){
        *ppnode = malloc(sizeof(tnode));
        (*ppnode)->reg = reg;
        (*ppnode)->esq = NULL;
        (*ppnode)->dir = NULL;
        return EXIT_SUCCESS;
    }else{  
        if(nivel%2 == 0){
            if(parv->cmplat((*ppnode)->reg,reg)>0){

                return abb_insere_node(parv,&((*ppnode)->esq),reg,
                ++nivel);
            }else{
    
                return abb_insere_node(parv,&((*ppnode)->dir),reg,++nivel);
            }
        }else{
            if(parv->cmplong((*ppnode)->reg,reg)>0){
        
                return abb_insere_node(parv,&((*ppnode)->esq),reg,++nivel);
            }else{
            
                return abb_insere_node(parv,&((*ppnode)->dir),reg,++nivel);
            }
        }
    }
    return EXIT_FAILURE;
}

int   abb_insere(tarv * parv,  void * reg){
    return abb_insere_node(parv,&parv->raiz,reg,0);
}

void abb_busca_prox(tarv * parv,tnode * pnode,void *reg, theap * heap,int qtd_vizinhos, int n, int nivel, int * tam){
    if (pnode == NULL){
        return;
    }
    double dist = parv->calcula_dist(pnode->reg,reg);
    ++qtd_vizinhos;
    if(dist>0){
        int ret = insere_elemento(heap,tam,n,pnode->reg,dist);
        (*tam)++; //nao sei se tenq somar
        if(ret==EXIT_FAILURE&&dist<heap[0].dist){
            altera_prioridade(heap,*tam,n,dist,reg);
        }
    }
    tnode * predecessor = NULL;
    tnode * sucessor = NULL;
    if(nivel%2 == 0){
        if(parv->cmplat(pnode->reg,reg)>0){
            sucessor = pnode->esq;
            predecessor = pnode->dir;
        }else{
            sucessor = pnode->dir;
            predecessor = pnode->esq;
        }
    }
    if(nivel%2 == 1){
        if(parv->cmplong(pnode->reg,reg)>0){
            sucessor = pnode->esq;
            predecessor = pnode->dir;
        }else{
            sucessor = pnode->dir;
            predecessor = pnode->esq;
        }
    }
    abb_busca_prox(parv,sucessor,reg,heap,qtd_vizinhos,n,nivel+1,tam);
    abb_busca_prox(parv,predecessor,reg,heap,qtd_vizinhos,n,nivel+1,tam);
}

void abb_busca(tarv * parv,  void * reg, theap * heap, int qtd_vizinhos, int n){
    int tam=0;
    abb_busca_prox(parv,parv->raiz,reg,heap,qtd_vizinhos,n,0,&tam);
}