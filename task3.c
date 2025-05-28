#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Elem {
	int coordinates[2];   // coordinates of the live cell
	struct Elem *next;  
}List;

typedef struct Tree {
    List *node;
    struct Tree *right, *left;
}Node;

void BuildList(List **head, int i, int j) 
{
    List *new = malloc(sizeof(List));
    if (new == NULL){
        printf("Memory allocation failed!\n");
        exit(1);
    }
    new->coordinates[0] = i;
    new->coordinates[1] = j;
    new->next = NULL;
    if (*head == NULL){
        *head = new;
    }
    else{
        List *tail = *head;
        while(tail->next){
            tail = tail->next;
        }
        tail->next = new;
    }
}

List* MainInfo(const char *filename, int *n, int *m, int *generations)
{
    int test, i, j;
    List *living_cells= NULL;
    FILE *input = fopen(filename, "r");
    if (input == NULL){
        printf("Nu este fisierul bun");
        exit(1);
    }
    fscanf(input, "%d %d %d %d", &test, n, m, generations);
    if (test != 3){
        printf("Nu este fisierul bun");
        fclose(input);
        exit(1);
    }
    fgetc(input); //for the new line
    //in the next lines i am searching for the live cells only to add them in the list
    i = 0;
    j = 0;
    do{
        char cell;
        cell = fgetc(input);
        if(cell == EOF) break;
        if(cell == 'X') {   
            BuildList(&living_cells, i, j);
            j++;
        }
        else if(cell == '\n'){   
            i++;
            j=0;
        }
        else j++;
    } while (i<*n);    
    fclose(input);
    return living_cells;
}

int FindCoord(List *node, int i, int j, int n, int m)
{
    if (i < 0 || i >= n || j < 0 || j >= m) return 0;
    List *temp = node;
    while(temp){
        if(temp->coordinates[0] == i && temp->coordinates[1] == j)
            return 1;
        temp = temp->next;
    }
    return 0;
}
List* RuleA(List *root, int n, int m)
{
    int i, j, live_neighbours, current_cell;
    List* head = NULL, *tail = NULL;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            live_neighbours = 0;
            live_neighbours += FindCoord(root, i-1, j-1, n, m);
            live_neighbours += FindCoord(root, i-1, j, n, m);
            live_neighbours += FindCoord(root, i-1, j+1, n, m);

            live_neighbours += FindCoord(root, i+1, j-1, n, m);
            live_neighbours += FindCoord(root, i+1, j+1, n, m);
            live_neighbours += FindCoord(root, i+1, j, n, m);

            live_neighbours += FindCoord (root, i, j+1, n, m);
            live_neighbours += FindCoord (root, i, j-1, n, m);

            //what happens to my cell ? 
            //well let's see
            current_cell = FindCoord(root, i, j, n, m);
            if (current_cell == 0 && live_neighbours == 3) // it's alive!!!
            {
                BuildList(head, i, j);
            }
            if ( current_cell == 1 && (live_neighbours == 2 || live_neighbours == 3) ) // still livin'
            {
                BuildList(head, i, j);
            }
        }
    } 
    return head;
}
List* RuleB(List *root, int n, int m)
{
    int i, j, live_neighbours, current_cell;
    List* head = NULL, *tail = NULL;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            live_neighbours = 0;
            current_cell = FindCoord(root, i, j, n, m);
            if(current_cell == 0) //dead...
            {
                live_neighbours += FindCoord(root, i-1, j-1, n, m);
                live_neighbours += FindCoord(root, i-1, j, n, m);
                live_neighbours += FindCoord(root, i-1, j+1, n, m);

                live_neighbours += FindCoord(root, i+1, j-1, n, m);
                live_neighbours += FindCoord(root, i+1, j+1, n, m);
                live_neighbours += FindCoord(root, i+1, j, n, m);

                live_neighbours += FindCoord (root, i, j+1, n, m);
                live_neighbours += FindCoord (root, i, j-1, n, m);
                if (live_neighbours == 2) // it's alive!!!!!!!
                {
                    BuildList(head, i, j);
                }
            }
            else if (current_cell == 1)
            {
               BuildList(head, i, j);
            }
        }
    } 
    return head;
}
//for generations
void BuildTree(Node **root, int n, int m, int generations)
{
    if(generations == 0 || *root == NULL) return;
    Node *leftChild = malloc(sizeof(Node));
    Node *rightChild = malloc(sizeof(Node));
    if(!rightChild || !leftChild)
    {
        perror("Memory allocation failed");
        exit(1);
    }

    leftChild->node = RuleB((*root)->node, n, m);
    leftChild->left = leftChild->right = NULL;

    rightChild->node = RuleA((*root)->node, n, m);
    rightChild->left = rightChild->right = NULL;

    (*root)->left = leftChild;
    (*root)->right = rightChild;

    BuildTree(&leftChild, n, m, generations - 1);
    BuildTree(&rightChild, n, m, generations - 1);

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

int main(int argc, char *argv[])
{
    Node* root = malloc(sizeof(Node));
    if(root == NULL)
    {
        perror("Memory allocation failed\n");
        exit(1);
    }
    int n, m, generations;
    root->left = root->right = NULL;
    root->node = MainInfo(argv[1], &n, &m, &generations);
    BuildTree(&root, n, m, generations);

    FILE* output = fopen(argv[2], "w");
    PrintTreePreorder(root, output, n, m);
    fclose(output);
    FreeTree(root);
    return 0;
}