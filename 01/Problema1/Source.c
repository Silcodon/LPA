#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#define MAX 99999
#define MAX_LISTENERS 200
#define MAX_ANTENNAS 100
#define MAX_TYPE_ANTENNAS 10
#define TRUE 1
#define FALSE 0

struct coordenadas {
    int x;
    int y;
};


struct antena {
    int custo;
    int raio;
};

struct coordenadas ouvintes[MAX_LISTENERS];
struct coordenadas posicao_antenas[MAX_ANTENNAS];
struct antena antenas[MAX_TYPE_ANTENNAS+1];
int custos_antenas[MAX_TYPE_ANTENNAS+1];
int custo_minimo = MAX;
int numero_ouvintes;
int numero_antenas;
int tipos_antenas;

void read_file() {
    int j = 0;


    scanf("%d", &numero_ouvintes);
    while (j < numero_ouvintes)
    {
        scanf("%d %d", &ouvintes[j].x, &ouvintes[j].y);
        j++;
    }

    
    scanf("%d", &numero_antenas);
    j = 0;
    while (j < numero_antenas)
    {
        scanf("%d %d", &posicao_antenas[j].x, &posicao_antenas[j].y);
        j++;
    }

    
    scanf("%d", &tipos_antenas);
    j = 0;
    while (j < tipos_antenas)
    {
        scanf("%d %d", &antenas[j].raio, &antenas[j].custo);
        custos_antenas[j] = antenas[j].custo;
        j++;
    }
    custos_antenas[tipos_antenas] = 0; 
    tipos_antenas += 1; 

}



/*Percorre as antenas para verificar a cobertura dos ouvintes
Verifica se todos os ouvintes tem cobertura
Retorna TRUE caso todos tenham, FALSE caso contrario*/
int verifica_cobertura_solucao(int solucao[]) {
    int i,index_posicao,raio,j;
    int contador = 0;
    int cobertura[numero_ouvintes];
    memset(cobertura, FALSE, sizeof(cobertura));
    /*Percorre as antenas todas de uma solucao*/
    for (index_posicao = 0; index_posicao < numero_antenas; index_posicao++) {
        /*Percorre os tipos de antenas para saber qual o raio associado ao custo*/
        if (solucao[index_posicao] == 0) {
            continue;
        }
        else {
            for (j = 0; j < tipos_antenas; j++) {
                if (solucao[index_posicao] == antenas[j].custo) {
                    raio = antenas[j].raio;
                    break;
                }
            }
        }
        /*Uma antena percorre os ouvintes todos*/
        for (i = 0; i < numero_ouvintes; i++) {
            /*Verifica se o ouvinte esta dentro da cobertura da antena 
            Distancia entre dois pontos=sqrt(pow(x2-x1,2)+pow(y2-y1,2))== pow(raio,2)=pow(x2-x1,2)+pow(y2-y1,2)*/
            if (cobertura[i] != TRUE) {
                if ((pow((ouvintes[i].x - posicao_antenas[index_posicao].x), 2) + pow((ouvintes[i].y - posicao_antenas[index_posicao].y), 2)) <= raio*raio) {
                    contador++;
                    /*Se contador==numero_ouvintes entao todos tem cobertura*/
                    if (contador == numero_ouvintes) {
                        return TRUE;
                    }
                    cobertura[i] = TRUE;
                }
            }
        }
    }
    return FALSE;
}



void buscar_numero(int solucao[], int indice, int n, int custo) {
    solucao[indice] = custos_antenas[n % tipos_antenas];
    custo += solucao[indice];
    if (custo > custo_minimo) {
        return;
    }
    if (indice == numero_antenas - 1) {
        if (verifica_cobertura_solucao(solucao) == TRUE) {
            custo_minimo = custo;      
        }
        return;
    }
    buscar_numero(solucao, indice + 1, n / tipos_antenas, custo);

}


/*Gerar vetores com possiveis soluçoes para o problema*/
/*
-Cada elemento i do vetor solucao pode ser preenchido de (tipos_antenas) maneiras, e o vetor solucao terá comprimento (num_antenas), logo haverá (tipos_antenas^{num_antenas}) sequências.
-Um loop desde 0 até (tipos_antenas^{num_antenas} - 1) irá gerar as sequencias todas, e em cada i converte-se o i de base 10 para base (tipos_antenas). Este numero convertido vai representar o indice do array a ser adicionado ao vetor solucao.
*/
void gera_solucoes(int n,int maximo) {
    int i;
    int custo_total = 0;
    int solucao[numero_antenas];
    buscar_numero(solucao, 0, n, custo_total);
    

    if (n < maximo) {
        gera_solucoes(n + 1,maximo);
    }
}



int main() {
    int i;
    int cobertura;
    read_file();
    int maximo = (int)pow(tipos_antenas, numero_antenas);
    gera_solucoes(0, maximo);
    if (custo_minimo != MAX) {
        printf("%d\n", custo_minimo);
    }
    else {
        printf("no solution\n");
    }
    return 0;

}