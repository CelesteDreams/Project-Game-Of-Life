#include "necessary.h"

void BuildList(List **head, int i, int j) 
{
    List *newNode = (List*)malloc(sizeof(List));
    if (newNode == NULL)
    {
        printf("Memory allocation failed in BuildList!"); 
        exit(1);
    }
    newNode->i = i;
    newNode->j = j;
    newNode->next = *head; 
    *head = newNode;       
}
List* MainInfo(const char *filename, int *n, int *m, int *generations)
{
    int test, i, j;
    List *head = NULL;
    FILE *input = fopen(filename, "r");
    if (input == NULL)
    {
        printf("Nu este fisierul bun");
        exit(1);
    }
    fscanf(input, "%d %d %d %d", &test, n, m, generations);
    if (test != 3)
    {
        printf("Nu este fisierul bun");
        fclose(input);
        exit(1);
    }
    fgetc(input); //pentru newline
    //parcurg matricea initiala in cautarea celulelor vii
    i = 0;
    j = 0;
    do{
        char cell;
        cell = fgetc(input);
        if(cell == EOF) break;
        if(cell == 'X') 
        {
            BuildList(&head, i, j);
            j++;
        }
        else if(cell == '\n') 
        {
            i++;
            j=0;
        }
        else j++;
    } while (i<*n);
    fclose(input);
    return head;
}
int FindCoord(List *node, int i, int j, int n, int m)
{
    if (i < 0 || i >= n || j < 0 || j >= m) return 0;
    List *temp = node;
    while(temp)
    {
        if(temp->i == i && temp->j == j)
            return 1;
        temp = temp->next;
    }
    return 0;
}
int CountNeighbours(List *node, int i, int j, int n, int m)
{
    int count = 0;
    count += FindCoord(node, i-1, j-1, n, m);
    count += FindCoord(node, i-1, j, n, m);
    count += FindCoord(node, i-1, j+1, n, m);
    count += FindCoord(node, i+1, j-1, n, m);
    count += FindCoord(node, i+1, j+1, n, m);
    count += FindCoord(node, i+1, j, n, m);
    count += FindCoord(node, i, j+1, n, m);
    count += FindCoord(node, i, j-1, n, m);
    return count;
}
List* RuleA(List *root, int n, int m)
{
    int i, j, live_neighbours, current_cell;
    List* head = NULL;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            live_neighbours = CountNeighbours(root, i, j, n, m);
            //what happens to my cell ?
            //well let's see
            current_cell = FindCoord(root, i, j, n, m);
            if ( current_cell == 0 && live_neighbours == 3) // it's alive!!!
            {
                //add in the list, it's in our interest
                BuildList(&head, i, j);
            }
            if (current_cell == 1 && (live_neighbours == 2 || live_neighbours == 3)) // still livin'
            {
                //add in the list, it's in our interest
                BuildList(&head, i, j);
            }
        }
    }
    return head;
}
List* RuleB(List *root, int n, int m)
{
    int i, j, live_neighbours, current_cell;
    List* head = NULL;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            current_cell = FindCoord(root, i, j, n, m);
            live_neighbours = CountNeighbours(root, i, j, n, m);
            if((current_cell == 0 && live_neighbours == 2) || current_cell == 1)
            {
                BuildList(&head, i, j);
            }
        }
    }
    return head;
}
void PrintTreePreorder(Node *root, FILE *output, int n, int m)
{
    int i, j, cell;
    if (root == NULL)
        return;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            cell = FindCoord(root->node, i, j, n, m);
            if(cell == 0) fprintf(output, "+");
            else fprintf (output, "X");
        }
        fprintf(output, "\n");
    }

    fprintf(output, "\n");
    PrintTreePreorder(root->left, output, n, m);
    PrintTreePreorder(root->right, output, n, m);
}

//the graph part
Graph* CreateGraph(List *head, int n, int m)
{
    int i, j;
    Graph *graph = malloc(sizeof(Graph));
    if (graph == NULL)
    {
        printf("Memory allocation failed");
        exit(1);
    }
    graph->n = n*m; // number of possible nodes 
    graph->adj = (List **)calloc(graph->n, sizeof(List *));
    if (graph->adj == NULL)
    {
        printf("Memory allocation failed");
        exit(1);
    }
    List *temp = head;
   while(temp != NULL)
    {
        i = temp->i;
        j = temp->j;
        int index = i * m + j; // index in the adjacency list
        if(FindCoord(head, i-1, j-1, n, m))
            BuildList(&graph->adj[index], i-1, j-1);
        if(FindCoord(head, i-1, j, n, m))
            BuildList(&graph->adj[index], i-1, j);
        if(FindCoord(head, i-1, j+1, n, m))
            BuildList(&graph->adj[index], i-1, j+1);
        if(FindCoord(head, i+1, j-1, n, m))
            BuildList(&graph->adj[index], i+1, j-1);
        if(FindCoord(head, i+1, j+1, n, m))
            BuildList(&graph->adj[index], i+1, j+1);
        if(FindCoord(head, i+1, j, n, m))
            BuildList(&graph->adj[index], i+1, j);
        if(FindCoord(head, i, j+1, n, m))
            BuildList(&graph->adj[index], i, j+1);
        if(FindCoord(head, i, j-1, n, m))
            BuildList(&graph->adj[index], i, j-1);
        temp = temp->next;
    }
    return graph;
}

void DFS(Graph *graph, int entry, int *visited, int *comp_nodes, int *comp_size, int m_cols)  //entry is for the index in the adjaceny list
{
    visited[entry] = 1;
    comp_nodes[*comp_size] = entry;
    (*comp_size)++;

    List *temp = graph->adj[entry];
    while(temp) 
    {
        int neighbour_idx = temp->i * m_cols + temp->j;
        if (visited[neighbour_idx] == 0) 
        {
            DFS(graph, neighbour_idx, visited, comp_nodes, comp_size, m_cols);
        }
        temp = temp->next;
    }
}

void BacktrackHamiltonian(Graph *graph, int entry, int *visited_path, int depth, int *curr_path, int *max_len, int *best_path, int m) 
{
    visited_path[entry] = 1;
    curr_path[depth - 1] = entry;

    if (depth > *max_len) 
        *max_len = depth;

    List *temp = graph->adj[entry];
    while (temp) 
    {
        int v_idx = temp->i * m + temp->j;
        if (!visited_path[v_idx]) 
        {
            BacktrackHamiltonian(graph, v_idx, visited_path, depth + 1, curr_path, max_len, best_path, m);
        }
        temp = temp->next;
    }
    visited_path[entry] = 0; 
}

void HamiltonianPath(Graph *graph, List *head, int n, int m, int *max_path, int **best_path_nodes) 
{
    int total_cells = n * m;

    int *visited_for_connected_parts = (int *)calloc(total_cells, sizeof(int));
    
    //this array tracks nodes already visited during connected component discovery with DFS.
    if (visited_for_connected_parts == NULL) 
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    //this array will hold the longest path's nodes.
    if (*best_path_nodes == NULL) 
    {
        *best_path_nodes = (int*)calloc(total_cells, sizeof(int));
        if (*best_path_nodes == NULL) 
        {
            printf("Memory allocation failed!\n");
            exit(1);
        }
    }
    List *temp= head;
    while(temp != NULL) 
    {
        int current_cell_idx = temp->i * m + temp->j; //i use this index as a key
        if(visited_for_connected_parts[current_cell_idx] == 0) 
        {
            int current_component_size = 0;
            //DFS(...); 
            // DFS is called to find all nodes in the connected component
        }
        temp = temp->next;
    }
    free(visited_for_connected_parts);
}


void BuildTreeAndPrintHamiltonianPath(Node **root, int n, int m, int generations, FILE *output)
{
    if(generations == 0 || *root == NULL) return;

    Node *leftChild = malloc(sizeof(Node));
    Node *rightChild = malloc(sizeof(Node));
    Graph *graph = NULL;

    if(!rightChild || !leftChild)
    {
        perror("Memory allocation failed");
        exit(1);
    }
    leftChild->left = leftChild->right = NULL;
    rightChild->left = rightChild->right = NULL;

    leftChild->node = RuleB((*root)->node, n, m);
    graph = CreateGraph(leftChild->node, n, m);

    int max_path = 0;
    int *best_path = NULL;
    HamiltonianPath(graph, leftChild->node, n, m, &max_path, &best_path);
    fprintf(output, "%d\n", max_path);
    if (max_path > 0) 
    {
        for (int k = 0; k < max_path; k++) 
        {
            int i = best_path[k] / m, j = best_path[k] % m; 
            fprintf(output, "(%d,%d) ", i, j);
        }
        fprintf(output, "\n");
    }


    FreeGraph(graph);
    graph = NULL; 
    rightChild->node = RuleA((*root)->node, n, m);
    graph = CreateGraph(rightChild->node, n, m); 
    best_path = NULL;
    HamiltonianPath(graph, rightChild->node, n, m, &max_path, &best_path);

    fprintf(output, "%d\n", max_path);
    if (max_path > 0) 
    {
        for (int k = 0; k < max_path; k++) 
        { 
            int i = best_path[k] / m, j =  best_path[k] % m;
            fprintf(output, "(%d,%d)", i, j);
        }
        fprintf(output, "\n");
    }
    FreeGraph(graph); 
    free(best_path);
    (*root)->left = leftChild;
    (*root)->right = rightChild;

    BuildTreeAndPrintHamiltonianPath(&leftChild, n, m, generations - 1, output);
    BuildTreeAndPrintHamiltonianPath(&rightChild, n, m, generations - 1, output);
}

void FreeGraph(Graph *graph)
{
    for (int i = 0; i < graph->n; i++)
        FreeList(graph->adj[i]);
    free(graph->adj);
    free(graph);
}
void FreeList(List *head)
{
    List *temp;
    while (head)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}
void FreeTree(Node *root)
{
    if (!root) return;
    FreeTree(root->left);
    FreeTree(root->right);
    FreeList(root->node);
    free(root);
}