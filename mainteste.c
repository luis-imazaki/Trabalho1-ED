#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include "./include/hashduplo.h"
#include "./include/abb.h"
#define TAM 10891

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

double cmplong(void *t1, void *t2){
    return ((tmunicipio *) t1)->longitude - ((tmunicipio *) t2)->longitude;
}

double cmplat(void *t1, void *t2){
    return ((tmunicipio *) t1)->latitude - ((tmunicipio *) t2)->latitude;
}

double calcula_dist(void *t1, void *t2){
    return pow((((tmunicipio*) t1)->latitude - ((tmunicipio*) t2)->latitude),2)+pow((((tmunicipio*) t1)->longitude - ((tmunicipio*) t2)->longitude),2);
}

char * get_key_cod(void * reg){
    return (*((tmunicipio *)reg)).codigo_ibge;
}

char * get_key_nome(void * reg){
    return (*((tmunicipio *)reg)).nome;
}

void print_vizinhos(theap *pheap,int n){
    int cont = 1;
    heapsort(pheap,n);
    printf("Vizinho(s) mais próximo(s):\n");
    for(int i=0;i<n;i++){
        printf("%d - codigo_ibge: %s\n",cont,((tmunicipio *)pheap[i].reg)->codigo_ibge);
        printf("    nome: %s\n",((tmunicipio *)(pheap)[i].reg)->nome);
        printf("    latitude: %f\n",((tmunicipio *)(pheap)[i].reg)->latitude);
        printf("    longitude: %f\n",((tmunicipio *)(pheap)[i].reg)->longitude);
        printf("    capital: %d\n",((tmunicipio *)(pheap)[i].reg)->capital);
        printf("    codigo_uf: %d\n",((tmunicipio *)(pheap)[i].reg)->codigo_uf);
        printf("    siafi_id: %d\n",((tmunicipio *)(pheap)[i].reg)->siafi_id);
        printf("    ddd: %d\n",((tmunicipio *)(pheap)[i].reg)->ddd);
        printf("    fuso_horario: %s\n",((tmunicipio *)(pheap)[i].reg)->fuso_horario);
        printf("---------------------------------------\n");
        ++cont;
    }
}

void interface(thash cod_hash,thash nome_hash, tarv * kdtree){
    int aux, n;
    char codigo[7],nome[40];
    tmunicipio *p,*q, *reg;
    do{
        printf("------------------------------\nDigite\n1 - Para buscar por codigo_ibge\n2 - Para buscar por nome\n3 - Para buscar os n vizinhos mais proximos\n4 - Para encerrar\n------------------------------\n");
        scanf("%d",&aux);   
        switch (aux)
        {
        case 1:
            printf("Digite o código da cidade\n");
            scanf("%s", codigo);
            p = (tmunicipio *)hash_busca(cod_hash,codigo);
            printf("codigo_ibge: %s,\nnome: %s,\nlatitude: %f,\nlongitude: %f,\ncapital: %d,\ncodigo_uf: %d,\nsiafi_id: %d,\nddd: %d,\nfuso_horario: %s\n", p->codigo_ibge,p->nome,p->latitude,p->longitude,p->capital,p->codigo_uf,p->siafi_id,p->ddd,p->fuso_horario);
            break;

        case 2:
            printf("Digite o nome da cidade\n");
            scanf(" %[^\n]", nome);
            q = (tmunicipio *)hash_busca(nome_hash,nome);
            printf("codigo_ibge: %s,\nnome: %s,\nlatitude: %f,\nlongitude: %f,\ncapital: %d,\ncodigo_uf: %d,\nsiafi_id: %d,\nddd: %d,\nfuso_horario: %s\n", q->codigo_ibge,q->nome,q->latitude,q->longitude,q->capital,q->codigo_uf,q->siafi_id,q->ddd,q->fuso_horario);
            break;
        case 3:
            printf("Qual o código ibge da cidade você deseja procurar?\n");
            scanf("%s",codigo);
            reg = (tmunicipio *)hash_busca(cod_hash,codigo);
            printf("Quantas cidades você quer buscar?\n");
            scanf("%d", &n);
            theap *pheap = (theap *)malloc(sizeof(theap)*n);
            abb_busca(kdtree,reg,pheap,0,n);
            print_vizinhos(pheap,n);
            break;

        default:
            printf("Opção inválida");
            break;
        }
    }while(aux!=4);


}

void aloca(tmunicipio ** node, char * cod_ibge, char * nome, char * fuso, double lat, double longi, int cap, int cod_uf, int siafi, int ddd){
    strcpy((*node)->codigo_ibge,cod_ibge);
    strcpy((*node)->nome,nome);
    strcpy((*node)->fuso_horario, fuso);
    (*node)->latitude = lat;
    (*node)->longitude = longi;
    (*node)->capital = cap;
    (*node)->codigo_uf = cod_uf;
    (*node)->siafi_id = siafi;
    (*node)->ddd = ddd;
}

int main(){
    //declaracões
    char codigo_ibge_temp[15], nome_temp[70],fuso_horario_temp[60];
    double latitude_temp,longitude_temp;
    int capital_temp,codigo_uf_temp,siafi_id_temp,ddd_temp;
    FILE *fptr;
    thash cod_hash,nome_hash;
    tmunicipio *reg;
    tarv kdtree;
    char linha[100], *token;
    hash_constroi(&cod_hash, TAM, get_key_cod);
    hash_constroi(&nome_hash, TAM, get_key_nome);
    abb_constroi(&kdtree,cmplat,cmplong,calcula_dist);
    fptr = fopen("municipios.json","r");
    token = strtok(linha,":\t\n\r");
    if(fptr == NULL){  //verifica se o arquivo não foi encontrado
        printf("arquivo não encontrado");
    }
    //"parser"

    while(fgets(linha,100,fptr)){
        token = strtok(linha, ":\", ");
        if(strcmp(token,"codigo_ibge")==0){
            token = strtok(NULL, ":\", ");
            strcpy(codigo_ibge_temp,token);
        }
        if(strcmp(token,"nome")==0){
            token = strtok(NULL, ":,");
            strcpy(nome_temp,++token);  //++token para tirar o espaço que teria no começo
            int tam=strlen(nome_temp);
            for(int k=0; k<tam-1;k++){
                nome_temp[k] = nome_temp[k+1];
            }
            nome_temp[tam-2]='\0';
        }
        if(strcmp(token,"latitude")==0){
            token = strtok(NULL, ":\", ");
            latitude_temp = atof(token);
        }
        if(strcmp(token,"longitude")==0){
            token = strtok(NULL, ":\", ");
            longitude_temp = atof(token);
        }
        if(strcmp(token,"capital")==0){
            token = strtok(NULL, ":\", ");
            capital_temp = atoi(token);
        }
        if(strcmp(token,"codigo_uf")==0){
            token = strtok(NULL, ":\", ");
            codigo_uf_temp = atoi(token);
        }
        if(strcmp(token,"siafi_id")==0){
            token = strtok(NULL, ":\", ");
            siafi_id_temp = atoi(token);
        }
        if(strcmp(token,"ddd")==0){
            token = strtok(NULL, ":\", ");
            ddd_temp = atoi(token);
        }
        if(strcmp(token,"fuso_horario")==0){
            token = strtok(NULL, ":\", ");
            strcpy(fuso_horario_temp,token);
            reg = (tmunicipio *) malloc(sizeof(tmunicipio));
            //cria uma struct temporaria para inserir nas hashs
            aloca(&reg,codigo_ibge_temp,nome_temp,fuso_horario_temp,latitude_temp,longitude_temp,capital_temp,codigo_uf_temp,siafi_id_temp,ddd_temp);
            hash_insere(&cod_hash,reg);
            hash_insere(&nome_hash,reg);
            abb_insere(&kdtree,reg);
        }   

    }
    interface(cod_hash,nome_hash, &kdtree);
    hash_apaga(&cod_hash);
    hash_apaga(&nome_hash);
    fclose(fptr);
    return 0;
}