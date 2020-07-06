#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#define MAX 1000
#define INFINITO 9999
#define TRUE 1
#define FALSE 0


/*Criar lista de adjacencia tem O(n)*/
/*Criar um grafo tem O(a)*/

/*Floyd Warshall para calcular o custo minimo entre cada vertice*/
/*Kruskall para criar a Minimum Spanning Tree*/
/*Depth First Search para percorrer ambos e calcular o custo associado*/

/*======================DATA STRUCTURES========================*/
struct node {
    int vertex;
    int value;
    struct node* next;
};


struct edge {
    int node1;
    int node2;
    int value;
    struct edge* next;
};


struct Graph {
    int numVertices;
    struct node** adjLists;
};



/*===========GLOBAL VARIABLES===================*/
/*Lista de servidores*/
int server[MAX];
/*Ordem pela qual os servidores sao visitados em cada grafo conexo*/
int serverAUX[MAX];
/*Numero de servidores*/
int n_servers = 0;
/*Custo de percorrer os servidores com MST*/
int custoTopologico = 0;
/*Custo de percorrer os servidores com pairwise*/
int custoConnected = 0;
/*Numero de servidores em cada grafo conexo*/
int nr_nodes = 0;
/*Tabela de custo entre cada vertice do grafo*/
int D[MAX][MAX];


/*===================FUNCTIONS============================*/
struct node* createNode(int, int);
struct Graph* createGraph(int);
struct edge* addList(struct edge*, int, int, int);
void displayList(struct edge*);
void deleteList(struct edge**);
void deleteList2(struct node**);
void deleteGraph(struct Graph*, int);
void addEdge(struct Graph*, int, int, int);
void printGraph(struct Graph*);
void MergeSort(struct edge**);
struct edge* SortedMerge(struct edge*, struct edge*);
void FrontBackSplit(struct edge*, struct edge**, struct edge**);
int isServer(int);
void APaux(struct Graph*, int, int*, int*, int*, int*, int);
int AP(struct Graph*);
void make_set(struct Graph*, int*, int*);
void unionUtil(int*, int*, int, int);
int find(int*, int);
void linkUtil(int*, int*, int, int);
struct Graph* Kruskall(struct Graph*, struct edge*);
void FloidWarshall(struct Graph*, int);
void read_file(void);
void DFSaux(struct Graph*, int, int*, int, int, int);
void DFS(struct Graph*, int);


/*===================UTILITY FUNCTIONS===========================================*/


/*Criar um no*/
struct node* createNode(int v, int value) {
    struct node* newNode = malloc(sizeof(struct node));
    newNode->vertex = v;
    newNode->value = value;
    newNode->next = NULL;
    return newNode;
}

/*Criar uma aresta*/
struct edge* createEdge(int node1, int node2, int value) {
    struct edge* newEdge = malloc(sizeof(struct edge));
    newEdge->node1 = node1;
    newEdge->node2 = node2;
    newEdge->value = value;
    newEdge->next = NULL;
    return newEdge;
}

/* Criar um grafo*/
struct Graph* createGraph(int vertices) {
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->numVertices = vertices;

    graph->adjLists = malloc(vertices * sizeof(struct node*));

    int i;
    for (i = 0; i < vertices; i++)
        graph->adjLists[i] = NULL;

    return graph;
}


void displayList(struct edge* root)
{
    printf("Ligacoes:\n");
    while (root != NULL) {
        printf("%d %d %d\n", root->node1, root->node2, root->value);
        root = root->next;
    }
}

void deleteList(struct edge** head_ref)
{

    struct edge* current = *head_ref;
    struct edge* next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }

    *head_ref = NULL;
}

void deleteList2(struct node** head_ref)
{

    struct node* current = *head_ref;
    struct node* next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }

    *head_ref = NULL;
}

void deleteGraph(struct Graph* graph, int numVertices) {
    int i;
    for (i = 0; i < numVertices; i++)
        deleteList2(&graph->adjLists[i]);
    free(graph->adjLists);
    free(graph);
}


struct edge* addList(struct edge* root, int node1, int node2, int value) {
    struct edge* p;
    struct edge* newEdge = createEdge(node1, node2, value);
    if(root!=NULL) {
        p = root;
        newEdge->next = p;
    }
    return newEdge;
}

/* Adicionar aresta */
void addEdge(struct Graph* graph, int node1, int node2, int value) {
    /*edge node1->node2*/
    struct node* newNode = createNode(node2, value);
    newNode->next = graph->adjLists[node1];
    graph->adjLists[node1] = newNode;

    /*edge node2->node1 */
    newNode = createNode(node1, value);
    newNode->next = graph->adjLists[node2];
    graph->adjLists[node2] = newNode;
}

/* Imprimir graph */
void printGraph(struct Graph* graph) {
    int v;
    for (v = 1; v < graph->numVertices; v++) {
        struct node* temp = graph->adjLists[v];
        printf("\n Adjacency list of vertex %d\n ", v);
        while (temp) {
            printf("%d -> ", temp->vertex);
            temp = temp->next;
        }
        printf("\n");
    }
    printf("\n\n\n");
}


/*Sort has O(n Log n)*/
/*Seen on https://www.geeksforgeeks.org/merge-sort-for-linked-list/ */
/* sorts the linked list by changing next pointers (not data) */
void MergeSort(struct edge** headRef)
{
    struct edge* head = *headRef;
    struct edge* a;
    struct edge* b;

    /* Base case -- length 0 or 1 */
    if ((head == NULL) || (head->next == NULL)) {
        return;
    }

    /* Split head into 'a' and 'b' sublists */
    FrontBackSplit(head, &a, &b);

    /* Recursively sort the sublists */
    MergeSort(&a);
    MergeSort(&b);

    /* answer = merge the two sorted lists together */
    *headRef = SortedMerge(a, b);
}

/* See https:// www.geeksforgeeks.org/?p=3622 for details of this
function */
struct edge* SortedMerge(struct edge* a, struct edge* b)
{
    struct edge* result = NULL;

    /* Base cases */
    if (a == NULL)
        return (b);
    else if (b == NULL)
        return (a);

    /* Pick either a or b, and recur */
    if (a->value <= b->value) {
        result = a;
        result->next = SortedMerge(a->next, b);
    }
    else {
        result = b;
        result->next = SortedMerge(a, b->next);
    }
    return (result);
}


/* Split the nodes of the given list into front and back halves,
    and return the two lists using the reference parameters.
    If the length is odd, the extra node should go in the front list.
    Uses the fast/slow pointer strategy. */
void FrontBackSplit(struct edge* source,
    struct edge** frontRef, struct edge** backRef)
{
    struct edge* fast;
    struct edge* slow;
    slow = source;
    fast = source->next;

    /* Advance 'fast' two nodes, and advance 'slow' one node */
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    /* 'slow' is before the midpoint in the list, so split it in two
    at that point. */
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}
/*End*/

/*Check if it is a server*/
int isServer(int number) {
    int i;
    for (i = 0; i < n_servers; i++) {
        if (number == server[i]) {
            return 1;
        }
    }
    return 0;
}






/*========================ARTICULATION POINT ALGORITHM============================================*/

/*It has complexity O(|V| + |E|)*/
void APaux(struct Graph* G, int v, int* dfs, int* parent, int* low, int* ap, int time) {
    struct node* nodeAUX = G->adjLists[v];
    int children = 0;
    int w;
    low[v] = dfs[v] = time;
    time++;

    while (nodeAUX != NULL)
    {
        w = nodeAUX->vertex;
        if (!dfs[w])
        {
            children++;
            parent[w] = v;
            APaux(G, w, dfs, parent, low, ap, time);
            low[v] = fmin(low[v], low[w]);
            /*1. it is the root with two or more children*/
            if (dfs[v] == 1 && children > 1)
                ap[v] = 1;
            /*2. otherwise, it has a child w for which there is no backedge
            between w (or descendants) and a predecessor of v.*/
            if (dfs[v] != 1 && low[w] >= dfs[v])
                ap[v] = 1;
        }
        else if (parent[v] != w)
        {
            low[v] = fmin(low[v], dfs[w]);
        }
        nodeAUX = nodeAUX->next;
    }



    /*

    Function DFS(G; v)
    mark v
    for each arc (v; u) 2 A do
        if u is not marked then
            DFS(G; u)



    Function AP(v)
        low[v] = dfs[v] = t
        t = t + 1
        for each edge {v;w} 2 E do
            if dfs[w] has no value then
                parent[w] = v
                AP(w)
                low[v] = min(low[v]; low[w])
                if dfs[v] = 1 and dfs[w] != 2 then
                    v is a (root) AP
                if dfs[v] != 1 and low[w] >= dfs[v] then
                    v is a (non-root) AP
            else if parent[v] != w then
                low[v] = min(low[v]; dfs[w])
    */
}



int AP(struct Graph* G)
{
    int i;
    int j = 0;
    int time = 1;
    int* dfs = (int*)malloc(sizeof(int) * G->numVertices);
    int* parent = (int*)malloc(sizeof(int) * G->numVertices);
    int* low = (int*)malloc(sizeof(int) * G->numVertices);
    int* ap = (int*)malloc(sizeof(int) * G->numVertices);

    for (i = 1; i < G->numVertices; i++) {
        parent[i] = 0;
        ap[i] = 0;
        dfs[i] = 0;
    }

    for (i = 1; i < G->numVertices; i++) {
        if (dfs[i] == 0)
            APaux(G, i, dfs, parent, low, ap, time);
    }

    for (i = 1; i < G->numVertices; i++) {
        if (ap[i] == 1) {
            n_servers++;
            server[j] = i;
            j++;
        }
    }
    free(dfs);
    free(parent);
    free(low);
    free(ap);
    return n_servers;
}



/*==========================DFS ALGORITHM ====================================*/

/*   FLAG=1 -> Custo Pairwise            FLAG=2 -> Custo Tree Topology  */
void DFSaux(struct Graph* G, int v, int* visited, int flag, int help, int node_aux) {
    struct node* nodeAUX2;
    nodeAUX2 = G->adjLists[v];
    visited[v] = 1;
    /*printf("%d->",v);*/
    if (flag == 1) {
        if (isServer(v) == 1) {
            serverAUX[nr_nodes] = v;
            nr_nodes++;
        }
        while (nodeAUX2 != NULL) {
            if (visited[nodeAUX2->vertex] == 0) {
                DFSaux(G, nodeAUX2->vertex, visited, flag, help, node_aux);
            }
            nodeAUX2 = nodeAUX2->next;
        }
    }
    else if (flag == 2) {
        /*printf("%d->",v);*/
        if (isServer(v) == 1) {
            if (help == 1) {
                custoTopologico += D[node_aux][v];
            }
            node_aux = v;
            help = 1;
        }
        while (nodeAUX2 != NULL) {
            if (visited[nodeAUX2->vertex] == 0) {
                DFSaux(G, nodeAUX2->vertex, visited, flag, help, node_aux);
            }
            nodeAUX2 = nodeAUX2->next;
        }
    }

}



void DFS(struct Graph* G, int flag) {
    int* visited = (int*)malloc(sizeof(int) * G->numVertices);
    int help;
    int node_aux=0;
    int i, j;
    nr_nodes = 0;
    if (flag == 2) {
        custoTopologico = 0;
    }
    if (flag == 1) {
        custoConnected = 0;
    }
    for (i = 1; i < G->numVertices; i++) {
        visited[i] = 0;
    }
    for (i = 1; i < G->numVertices; i++) {
        if (visited[i] == 0) {
            help = 0;
            node_aux = 0;
            for (j = 0; j < n_servers; j++) {
                serverAUX[j] = 0;
            }
            nr_nodes = 0;
            DFSaux(G, i, visited, flag,help,node_aux);
            if (flag == 1) {
                if (nr_nodes == 1) {
                    custoConnected += 0;
                }
                else if (nr_nodes == 2) {
                    /*printf("[%d][%d]:%d", serverAUX[0], serverAUX[1], D[serverAUX[0]][serverAUX[1]]);*/
                    custoConnected += D[serverAUX[0]][serverAUX[1]];
                }
                else {
                    for (i = 0; i < nr_nodes; i++) {
                        for (j = i; j < nr_nodes; j++) {
                            if (serverAUX[j] != 0) {
                                custoConnected += D[serverAUX[i]][serverAUX[j]];
                            }
                        }
                    }
                }
            }
        }

    }
    free(visited);

    /*
    Function DFS(G; v)
    mark v
    for each arc (v; u) 2 A do
        if u is not marked then
            DFS(G; u)
    push(v; S)
    */

}





/*============================KRUSKALL ALGORITHM=============================================*/

void make_set(struct Graph* G, int* set, int* rank) {
    int i;
    for (i = 1; i < G->numVertices; i++) {
        set[i] = i;
        rank[i] = 0;
    }

    /*
    make set
    for each vertex i E V do
        set[i] = i
        rank[i] = 0
*/
}

void unionUtil(int* set, int* rank, int a, int b) {
    linkUtil(set, rank, find(set, a), find(set, b));

    /*
    union(a,b)
    link(find(a); find(b))
    */
}

int find(int* set, int a) {
    if (set[a] != a) {
        set[a] = find(set, set[a]);
    }
    return set[a];

    /*
    find(a)
    if set[a] != a then
        set[a] = find(set[a])
    return set[a]
    */
}


void linkUtil(int* set, int* rank, int a, int b) {
    if (rank[a] > rank[b]) {
        set[b] = a;
    }
    else {
        set[a] = b;
    }
    if (rank[a] == rank[b]) {
        rank[b]++;
    }


    /*
    link(a,b)
    if rank[a] > rank[b] then
        set[b] = a
    else
        set[a] = b
    if rank[a] = rank[b] then
        rank[b] + +
    */
}



struct Graph* Kruskall(struct Graph* G, struct edge* root) {
    int* set = (int*)malloc(sizeof(int) * G->numVertices);
    int* rank = (int*)malloc(sizeof(int) * G->numVertices);
    struct Graph* Tree2;
    struct edge* aux;
    Tree2 = createGraph(G->numVertices);
    make_set(G, set, rank);
    MergeSort(&root);
    aux = root;
    while (aux != NULL) {
        if (find(set, aux->node1) != find(set, aux->node2)) {
            addEdge(Tree2, aux->node1, aux->node2, aux->value);
            unionUtil(set, rank, aux->node1, aux->node2);
        }
        aux = aux->next;
    }
    free(set);
    free(rank);
    return Tree2;



    /*
    Function Kruskal(G)
    T = NULL;
    for each vertex v E V do
        make set(v)
    sort edges in E into nondecreasing order by weight
    for each edge {u; v} E E do
        if find_set(u) != find_set(v) then
            T = T U {u; v}
            union(u; v)
    return T
    */

}




/*======================FLOIDWARSHALL ALGORITHM=======================================*/

void FloidWarshall(struct Graph* G, int n_servers) {
    int i, j, k;
    custoConnected = 0;
    struct node* nodeAUX;
    for (i = 1; i < G->numVertices; i++) {
        for (j = 1; j < G->numVertices; j++) {
            if (j == i) {
                D[i][j] = 0;
            }
            else {
                D[i][j] = INFINITO;
            }
        }
    }
    for (i = 1; i < G->numVertices; i++) {
        nodeAUX = G->adjLists[i];
        while (nodeAUX) {
            j = nodeAUX->vertex;
            D[i][j] = nodeAUX->value;
            nodeAUX = nodeAUX->next;
        }

    }
    for (k = 1; k < G->numVertices; k++) {
        for (i = 1; i < G->numVertices; i++) {
            for (j = 1; j < G->numVertices; j++) {
                D[i][j] = fmin(D[i][j], D[i][k] + D[k][j]);
            }
        }
    }
    /* Negative cycle
    for (i = 1; i < G->numVertices; i++) {
        if (D[i][i] < 0) {
            return;
        }
    }*/
    /*
    for (i = 1; i < G->numVertices; i++) {
        for (j = 1; j < G->numVertices; j++) {
            printf("Tabela[%d][%d]:%d\n",i,j,D[i][j]);
        }
    }
    */

    DFS(G, 1);

    /*Calcula pairwise connected*/
    /* if (n_servers == 1) {
        custoConnected = 0;
    }
    else if (n_servers == 2) {
        if (D[server[0]][server[1]] != INFINITO)
            custoConnected += D[server[0]][server[1]];
    }
    else {
        for (i = 0; i < n_servers; i++) {
            if (server[i + 1] != 0) {
                if (D[server[i]][server[i + 1]] != INFINITO)
                    custoConnected += D[server[i]][server[i + 1]];
            }
            else {
            if (D[server[i]][server[0]] != INFINITO)
                custoConnected += D[server[i]][server[0]];
            }
        }
    }*/



    /*
    Function FW(G; D)
        for i = 1 to numVertices do
            D[i; i] = 0
        for each arc(i; j) 2 A do
            D[i; j] = w(i; j)
        for k = 1 to numVertices do
            for i = 1 to numVertices do
                for j = 1 to numVertices do
                    D[i; j] = min(D[i; j]; D[i; k] + D[k; j])
        for i = 1 to numVertices do
            if D[i; i] < 0 then           {check for negative cycle}
               return negative cycle found
        return D
        */
}




/*=============================================================================*/

void read_file() {
    int n, graph_number = 0;
    int servidores;
    int node1, node2, value;
    struct Graph* graph;
    struct edge* root;
    struct Graph* tree_aux;
    while (scanf("%d", &n) != EOF) {
        graph = createGraph(n + 1);
        memset(server, 0, MAX*sizeof(int));
        root = NULL;
        n_servers = 0;
        graph_number += 1;

        while (1) {
            scanf("%d ", &node1);
            if (node1 == 0) {
                /*Grafo esta completo*/
                break;
            }
            scanf("%d ", &node2);
            scanf("%d", &value);
            addEdge(graph, node1, node2, value);
            root=addList(root, node1, node2, value);
        }
        servidores = AP(graph);
        if (servidores == 0) {
            printf("no server\n");
            deleteGraph(graph, n + 1);
            deleteList(&root);
            continue;
        }
        else if (servidores == 1) {
            printf("%d 0 0\n", servidores);
            deleteGraph(graph, n + 1);
            deleteList(&root);
            continue;
        }
        FloidWarshall(graph, servidores);
        tree_aux = Kruskall(graph, root);
        DFS(tree_aux, 2);
        printf("%d %d %d\n", servidores, custoConnected, custoTopologico);
        deleteList(&root);
        deleteGraph(graph,n+1);
        deleteGraph(tree_aux,n+1);

    }

}






int main() {
    read_file();
    return 0;

}