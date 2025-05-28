#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Element
{
    int val[2];  //(l,c) coordonate
    struct Element *next;
}List;

typedef struct StackNode
{
    List *head;
    struct StackNode *next;
}SNode;

void push(SNode **top, List *head)
{
    SNode* newNode = (SNode*)malloc(sizeof(SNode));
    if (newNode == NULL) 
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->head = head;
    newNode->next = *top;
    *top = newNode;
}

void HelpRows(int *n, char **middle, char **previous, char **next)
{
    *n = *n + 3;
    *previous = (char *) malloc(sizeof(char)*(*n));
    *middle = (char *) malloc(sizeof(char)*(*n));
    *next = (char *) malloc(sizeof(char)*(*n));

    if (*middle == NULL || *previous == NULL || *next == NULL) 
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    for (int i=0; i<*n-1; i++)
        (*previous)[i] = '+';
    (*previous)[strlen(*previous)] = '\0';
}

void Concat(char **string, int len)
{
    int i;
    for (i = len-2; i > 0; i--) 
    {
        (*string)[i] = (*string)[i-1]; 
    }
    (*string)[0]='+';
    (*string)[strlen((*string))]='+';
    (*string)[strlen((*string))+1]='\0';
}

int NumberOfLiveCells(const char *previous, const char *middle, const char *next, int j, int i)
{
    int count = 0;
    if (previous[j] == 'X') count ++; 
    if (previous[j-1] == 'X') count ++;
    if (previous[j+1] == 'X') count ++;

    if (middle[j-1] == 'X') count ++;
    if (middle[j+1] == 'X') count ++;
      
    if (next[j] == 'X') count ++;
    if (next[j-1] == 'X') count ++;
    if (next[j+1] == 'X') count ++;
    return count;
}

void ReadValues(int *n, int *m, const char *filename)
{
    int gen, test;
    FILE *f = fopen(filename, "r"); 
    if (f == NULL)
    {
        printf("Cannot open file %s\n", filename);
        exit(1);
    }
    fscanf(f, "%d %d %d %d", &test, m, n, &gen); //I am reading the important dates out of the input
    if (test != 2)
    {
        printf("It is not the good file");
        exit(1);
    }
    fclose(f);
}

int IsCellAliveOrNot(int linie, int coloana, SNode *top, char cell)
{

    int count = 0;
    SNode *temp = top;
    //how many times does it appear in the stack?
    while(temp) 
    {
        List *p = temp->head;
        while(p) 
        {
            if (p->val[0] + 1 == linie && p->val[1] + 1 == coloana)
                count++;
            p = p->next;
        }
        temp = temp->next;
    }

    if(cell == '+')
        return(count%2? 1:0);
    else if (cell == 'X')
        return(count%2? 0:1);
    else {
            printf("Unknown character (%d,%d): %c\n", linie, coloana, cell);
            return -1;
        }
}

List* BuildFirstListGeneration(const char *filename, int *gen_number, int n, int m, char **init_matrix)
{
    List *head = NULL, *tail = NULL; 
    int test, i, j, vecini, len, k;
    char *previous, *middle, *next;
    FILE *f = fopen(filename, "r"); 
    if (f == NULL)
    {
        printf("Cannot open file %s\n", filename);
        exit(1);
    }
   
    fscanf(f, "%d %d %d %d", &test, &m, &n, gen_number); //I am reading important stuff out of the input file
    HelpRows(&n, &middle, &previous, &next);
    len = n;
    strcpy(init_matrix[0], previous);

    fscanf(f, "%s", middle);
    
    /*  de  exemplu:
        ++++    <-this is prev
        ++X+    <-this is middle
        +XX+    <-this is next
    */
    Concat(&middle, len);  //concatenate dead cells
    strcpy(init_matrix[1], middle);
    for (i = 0; i < m; i++)
    {
        if (i == m-1)
        {
             for (k = 0; k < n - 1; k++)
                next[k]='+';
            next[n-1]='\0';
        }
        else 
        {
            fscanf(f, "%s", next);
            Concat(&next, len);
        }
        strcpy(init_matrix[i+2], next);
        for (j = 1; j < n-2; j++) 
        {
            vecini = NumberOfLiveCells(previous, middle, next, j, i);
            if (((vecini < 2 || vecini > 3) && middle[j]=='X') || (vecini == 3 && middle[j] == '+'))
            {    
                List *node = (List *)malloc(sizeof(List));
                if (node == NULL)
                {
                    printf("Allocation failed!");
                    exit(1);
                }
                node->val[0] = i;
                node->val[1] = j-1;
                node->next = NULL;
                if (head == NULL)
                {
                    head = tail = node;
                }
                else 
                {
                    tail->next = node;
                    tail = node;
                }
            }
        }
        if (i != m-1)
        {
            strcpy(previous, middle);
            strcpy(middle, next);
        }
    }

    fclose(f);
    free(previous);
    free(middle);
    free(next); 
    return head;
}
List* NextGenerations(const char *filename, SNode *top, int n, int m, char **matrix)
{
    int test, gen_number, i, j, live_neighbours, current_cell;
    List *head = NULL, *tail = NULL;
    FILE *f;
    f = fopen(filename, "r");
    if (f == NULL)
    {
        printf("Cannot open file %s\n", filename);
        exit(1);
    }
    fscanf(f, "%d %d %d %d", &test, &m, &n, &gen_number);
    for (i = 1; i <= m; i++)
    {
        for(j = 1; j <= n; j++)
        {
           live_neighbours = 0;
           live_neighbours += IsCellAliveOrNot(i-1, j-1, top, matrix[i-1][j-1]);
           live_neighbours += IsCellAliveOrNot(i-1, j, top, matrix[i-1][j]);
           live_neighbours += IsCellAliveOrNot(i-1, j+1, top, matrix[i-1][j+1]);
           live_neighbours += IsCellAliveOrNot(i, j-1, top, matrix[i][j-1]);
           live_neighbours += IsCellAliveOrNot(i, j+1, top, matrix[i][j+1]);
           live_neighbours += IsCellAliveOrNot(i+1, j-1, top, matrix[i+1][j-1]);
           live_neighbours += IsCellAliveOrNot(i+1, j, top, matrix[i+1][j]);
           live_neighbours += IsCellAliveOrNot(i+1, j+1, top, matrix[i+1][j+1]);
           current_cell = IsCellAliveOrNot(i, j, top, matrix[i][j]);


           if ((current_cell && (live_neighbours < 2 || live_neighbours > 3)) || (!current_cell && live_neighbours == 3))
           {
               List *node = (List *)malloc(sizeof(List));
               if (!node)
               {
                   printf("Memory allocation failed!\n");
                   exit(1);
               }
               node->val[0] = i - 1;
               node->val[1] = j - 1;
               node->next = NULL;
               if (!head)
               {
                    head = node;
                    head->next = NULL;
                    tail = head;
               }
                   
               else
               {
                   tail->next = node;
                   tail = node;
               }
           }
        }
    }
    fclose(f);
    return head;
}

//eliberare memorie
void freeList(List *head)
{
    while (head != NULL) 
    {
        List *temp = head;
        head = head->next;
        free(temp);
    }
}
void freeStack(SNode *top)
{
    while (top != NULL)
    {
        SNode *temp = top;
        top = top->next;
        freeList(temp->head); 
        free(temp);
    }
}

void freeMatrix(char ***matrix, int n)
{
    if (*matrix == NULL) return; 

    for (int i = 0; i < n; i++) 
    {
        free((*matrix)[i]); 
    }
    free(*matrix); 
    *matrix = NULL;
}

int main(int argc, char *argv[])
{
    SNode *top = NULL; //capul stivei
    List *head = NULL, *current = NULL;

    int k=1, n = 0, m = 0 , gen_number;
    ReadValues(&n, &m, argv[1]);
    char **init_matrix = (char **)calloc((m + 2), sizeof(char*)); // m + 2 linii
    if (init_matrix == NULL) 
    {
    printf("Alocare de memorie esuata");
    exit(1);
    }
    for (int i = 0; i < m + 2; i++) 
    {
        init_matrix[i] = (char *)calloc((n + 3), sizeof(char));
        if (init_matrix[i] == NULL) 
        {
            printf("Memory allocation failed for init_matrix[%d]\n", i);
            exit(1);
        }
    }
    head = BuildFirstListGeneration(argv[1], &gen_number, n, m, init_matrix);
    push(&top, head); //first list of changes in the list
    
    FILE* g = fopen(argv[2], "w"); 
    fprintf(g, "%d", k); //number of the first generation
    current = head;
    while (current != NULL)
    {
        fprintf(g, " %d %d", current->val[0], current->val[1]); 
        current = current->next;
    }
    fprintf(g, "\n");

    //next generations
    for( k = 2; k <= gen_number; k++ )
    {
        fprintf(g, "%d", k);
        head = NextGenerations(argv[1], top, n, m, init_matrix);
        push(&top, head);
        current = head;
        while (current != NULL)
        {
            fprintf(g, " %d %d", current->val[0], current->val[1]); 
            current = current->next;
        }
        fprintf(g, "\n");
    }
    freeMatrix(&init_matrix, m+2);
    freeStack(top);
    fclose(g);
    return 0;
}
