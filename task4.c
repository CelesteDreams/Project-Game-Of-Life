#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "necessary.h"

int main(int argc, char *argv[])
{
    int n, m, generations;
    List *head = MainInfo(argv[1], &n, &m, &generations);  // read input file and init the root node;
    if (head == NULL)
    {
        printf("No live cells found in the input file.\n");
        return 1;
    }
    Node *root = malloc(sizeof(Node));
    if (root == NULL)
    {
        perror("Memory allocation failed\n");
        FreeList(head);
        return 1;
    }
    root->left = root->right = NULL;
    root->node = head;
    int max_path = 0;
    int *ham_path = NULL;
    Graph *graph = CreateGraph(head, n, m);
    HamiltonianPath(graph, head, n, m, &max_path, &ham_path);
    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        perror("Failed to open output file\n");
        FreeTree(root);
        FreeList(head);
        return 1;
    }
    BuildTreeAndPrintHamiltonianPath(&root, n, m, generations, output);
    fclose(output);
    FreeTree(root);
    return 0;
}