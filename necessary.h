#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct List 
{
        int i, j;  // coordinates livin' cell
        struct List *next;
}List;

typedef struct Tree
{
    List *node;
    struct Tree *right, *left;
}Node;

typedef struct Graph 
{
    int  n; //number of vertices
    List **adj; // adjacency list
}Graph;

void BuildList(List **head, int i, int j);
List* MainInfo(const char *filename, int *n, int *m, int *generations);
int FindCoord(List *node, int i, int j, int n, int m);
int CountNeighbors(List *node, int i, int j, int n, int m);
List* RuleA(List *root, int n, int m);
List* RuleB(List *root, int n, int m);  
Graph* CreateGraph(List *head, int n, int m);
void DFS(Graph *g, int u, int *visited, int *comp_nodes, int *comp_size, int m_cols);
void BacktrackHamiltonian(Graph *graph, int entry, int *visited_path, int depth, int *current_path, int *max_len, int *best_path, int m_cols);
// this function searches for the hamiltonian path
void HamiltonianPath(Graph *graph, List *cells, int n_rows, int m_cols, int *max_path_len, int **best_path_nodes);
// this functions builds the binary tree with rule A and B and then initiates the hamiltonian search 
void BuildTreeAndPrintHamiltonianPath(Node **root, int n, int m, int gens, FILE *output_file);
void PrintTreePreorder(Node *root, FILE *output, int n, int m);
void FreeList(List *head);
void FreeTree(Node *root);
void FreeGraph(Graph *graph);
