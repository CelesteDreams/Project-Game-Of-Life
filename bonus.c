#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max 2000
/*
    input files structure:
    k 
    ++++ 
    +X++
    +XX+
    ++++
    1 ...
    2 ...
    3 ...
    .
    .
    .
    k ...

    k is for number of generations
*/

typedef struct Element
{
    int i, j;  //(l,c) coordonate
    struct Element *next;
}List;

typedef struct StackNode
{
    List *head;
    struct StackNode *next;
}Stack;

void push(Stack **top, List *head)
{
    Stack* newNode = (Stack*)malloc(sizeof(Stack));
    if (newNode == NULL) 
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->head = head;
    newNode->next = *top;
    *top = newNode;
}

int FindCoord(List *living_cells, int row, int col)
{
    List *temp = living_cells;
    while(temp)
    {
        if( row == temp->i && col == temp->j )
            return 1;  //it's alive
        temp = temp->next;
    }
    return 0; //it's dead X.X
}

void BuildList(List **head, int i, int j) 
{ 
    List *new = malloc(sizeof(List));
    if (new == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    new->i = i;
    new->j = j;
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

List *LastStateAndTheChangesSuffered(const char *filename, Stack **top, int *k, int *cols, int *rows)
{
    int current_row = 0, length;
    char *line, *stack_line;
    List *living_cells = NULL;
    FILE *input = fopen(filename, "r");
    if (input == NULL) 
    {
        printf("Cannot open file %s\n", filename);
        exit(1);
    }
    fscanf(input, "%d %d %d\n", k, rows, cols); 
    
    length = *cols+2;
    line =  calloc(length, sizeof(char));

    if (line == NULL)
    {
        printf("Error allocating memory!");
        fclose(input);
        exit(1);
    }
    for(current_row = 0; current_row < *rows; current_row++) 
    {
        fgets(line, length, input);
        line[strcspn(line, "\n")] = '\0'; 
        for (int current_col = 0; current_col < *cols; current_col++) 
        {
            if (line[current_col] == 'X') 
            {
                BuildList(&living_cells, current_row, current_col);
            }
        }
    }
    //build the stack
    char *coordinate;
    int gen_number, col_coord, row_coord;
    stack_line = calloc(max, sizeof(char));
    for (int i = 0; i < *k; i++) 
    {
        fgets(stack_line, max, input);
        stack_line[strcspn(stack_line, "\n")] = '\0';  //delete the newline from fgets
        List *changes_list = NULL; 

        //the first number is for the generation number
        coordinate = strtok(stack_line, " ");
        gen_number = atoi(coordinate); //from char to integer '12' -> 12
        //i'm searching for numbers in my string ex: 2 3 10 12 12 11

       while ((coordinate = strtok(NULL, " ")) != NULL) 
        { 
            row_coord = atoi(coordinate);
            coordinate = strtok(NULL, " "); 
            col_coord = atoi(coordinate);
            BuildList(&changes_list, row_coord, col_coord);
        }
        push(top, changes_list);
    }
    free(line);
    free(stack_line);
    fclose(input);
    return living_cells;
}

void InitMatrix(Stack *top, FILE *output, int rows, int cols, List *living_cells)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int cell_is_alive = FindCoord(living_cells, i, j); // 1 if 'X'-is found in the list, 0 if '+' -it is not found in the list

            int total_changes_for_cell = 0; // with this we count for how many times this appeared changed through K generations
            for (Stack *s = top; s != NULL; s = s->next)
            {
                // we go through the list of changes for the current generation in the stack
                for (List *l = s->head; l != NULL; l = l->next)
                {
                    if (l->i == i && l->j == j)
                    {
                        total_changes_for_cell++; 
                    }
                }
            }
            // Now, using 'total_changes_for_cell', we determine the state at Generation 0.
            // If 'total_changes_for_cell' is odd 2k+1, it means that the state at Generation 0 was the opposite to the state at K
            // If 'total_changes_for_cell' is even 2k, it means that the state at Generation 0 is identical to the state at Generation K.
            if (total_changes_for_cell % 2 == 1) 
            {
                fprintf(output, "%c", cell_is_alive ? '+' : 'X'); 
            } else 
            {
                fprintf(output, "%c", cell_is_alive ? 'X' : '+'); 
            }
        }
        fprintf(output, "\n");
    }
}
void freeList(List *head)
{
    while (head != NULL) 
    {
        List *temp = head;
        head = head->next;
        free(temp);
    }
}
void freeStack(Stack *top)
{
    while (top != NULL)
    {
        Stack *temp = top;
        top = top->next;
        freeList(temp->head); 
        free(temp);
    }
}

int main(int argc, char *argv[])
{
    Stack *top = NULL; 
    List *living_cells = NULL;
    int k = 0, cols, rows;
    cols = rows = 0;
    living_cells = LastStateAndTheChangesSuffered(argv[1],  &top, &k, &cols, &rows);
    FILE *output = fopen(argv[2], "w");
    InitMatrix(top, output, rows, cols, living_cells);
    fclose(output);
    freeStack(top);
    freeList(living_cells);
    return 0;
}