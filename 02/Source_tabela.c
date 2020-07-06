#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#define MAX 1200
#define INFINITO 9999
#define TRUE 1
#define FALSE 0

/*Calculo a area de cada ponto e depois nas linhas seguintes em que aumento o numero de pontos da solução, faço a soma da área que foi calculada antes com a area de um novo ponto.
Guardo o y que foi adicionado anteriormente de forma a que ao calcular a area do ponto a adicionar esta nao contenha a area da interseção entre ambos os retangulos.*/

struct coordenadas {
    double x;
    double y;
};



struct coordenadas pontos[MAX];
int n, k;

void read_file() {
    int i = 1;

    scanf("%d %d", &n, &k);
    k = k + 1;
    n = n + 1;
    pontos[0].x = 0;
    pontos[0].y = 0;
    while (i < n) {
        scanf("%lf %lf", &pontos[i].x, &pontos[i].y);
        i++;
    }

}


void verifica_leitura() {
    int i;
    for (i = 0; i < n; i++) {
        printf("%.12lf %.12lf\n", pontos[i].x, pontos[i].y);
    }
}

/* Merges two subarrays of arr[].
 First subarray is arr[l..m]
 Second subarray is arr[m+1..r]
 Seen on www.geeksforgeeks.org/merge-sort */
void merge(struct coordenadas arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    /* create temp arrays */
    struct coordenadas L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; /* Initial index of first subarray  */
    j = 0; /* Initial index of second subarray */
    k = l; /* Initial index of merged subarray */
    while (i < n1 && j < n2)
    {
        if (L[i].x <= R[j].x)
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void mergeSort(struct coordenadas arr[], int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

/*Utility function*/
void swap(struct coordenadas* a, struct coordenadas* b)
{
    struct coordenadas t = *a;
    *a = *b;
    *b = t;
}


/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
    array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
int partition(struct coordenadas arr[], int low, int high)
{
    double pivot = arr[high].x;    // pivot 
    int i = (low - 1);  // Index of smaller element 
    int j;

    for (j = low; j <= high - 1; j++)
    {
        // If current element is smaller than the pivot 
        if (arr[j].x < pivot)
        {
            i++;    // increment index of smaller element 
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

/* The main function that implements QuickSort
 arr[] --> Array to be sorted,
  low  --> Starting index,
  high  --> Ending index */
void quickSort(struct coordenadas arr[], int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(arr, low, high);

        // Separately sort elements before 
        // partition and after partition 
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
/*End of www.geeksforgeeks.org/merge-sort && https://www.geeksforgeeks.org/quick-sort/ */


/* P(x1,y1) ponto principal
   x2 - coordenada x do ponto anterior/origem(caso seja o primeiro)
*/
double calcula_area(double x1, double y1, double x2, double y2) {
    double comprimento, largura, area;
    comprimento = x1 - x2;
    largura = y1 - y2;
    area = comprimento * largura;
    return area;

}


void print_tabela(double tabela[k][n]) {
    int row, columns;
    int numberOfLines = k;
    int numberColumns = n;
    for (row = 0; row < numberOfLines; row++)
    {
        for (columns = 0; columns < numberColumns; columns++)
        {
            printf("%.12lf     ", tabela[row][columns]);
        }
        printf("\n");
    }

}

void print_tabela_int(int tabela[k][n]) {
    int row, columns;
    int numberOfLines = k;
    int numberColumns = n;
    for (row = 0; row < numberOfLines; row++)
    {
        for (columns = 0; columns < numberColumns; columns++)
        {
            printf("%d     ", tabela[row][columns]);
        }
        printf("\n");
    }

}

void preenche_tabela() {
    int i, j, l;
    double area_atual, maximo;
    double tabela[k][n]; /*[n][k]*/
    memset(tabela, 0.0, n * k * sizeof(double));
    double resposta_final = 0.0;
    /* CASO BASE já implementado com memset();
    for (j = 0; j < n; j++) {
        tabela[0][j] = 0;
    }
    for (i = 1; i < k; i++) {
        tabela[i][0] = 0;
    }*/
    for (j = 1; j < n; j++) {
        tabela[1][j] = calcula_area(pontos[j].x, pontos[j].y, 0, 0);
    }
    for (i = 2; i < k; i++) {
        for (j = 1; j < n; j++) {
            maximo = 0.0;
            for (l = j + 1; l < n; l++) {
                /*printf("%d [%d][%d]\n",ponto_x,i,j);*/
                area_atual = calcula_area(pontos[j].x, pontos[j].y, 0, pontos[l].y) + tabela[i - 1][l];
                /*printf("Area:%12lf [%d][%d]\n", area_atual, j, l);*/
                if (maximo < area_atual) {
                    maximo = area_atual;
                    if (resposta_final < maximo) {
                        resposta_final = maximo;
                    }
                }
            }
            tabela[i][j] = maximo; /*Caso o valor maximo da area somada esteja relacionada com um ponto anterior*/
        }
    }
    /*
    print_tabela(tabela);
    print_tabela_int(aux);
    */
    printf("%.12lf\n", resposta_final);

}




int main() {
    read_file();
    /*
    mergeSort(pontos, 0, n - 1);
    */
    quickSort(pontos, 0, n - 1);
    preenche_tabela();
    return 0;

}