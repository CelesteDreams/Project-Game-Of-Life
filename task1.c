#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void readMatrix(const char *filename, char ***matrix, int *n, int *m, int *nrgen)
{
    int test, i , j;
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Cannot open file %s\n", filename);
        exit(1);
    }

    fscanf(file, "%d", &test);
    fscanf(file, "%d %d %d", n, m, nrgen);


    *n = *n + 2;
    *m = *m + 2;

    *matrix = (char **)malloc(*n * sizeof(char *));
    if (*matrix == NULL) 
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    for (i = 0; i < *n; i++)
    {
        (*matrix)[i] = (char *)malloc(*m * sizeof(char));
        if ((*matrix)[i] == NULL) 
        {
            printf("Memory allocation failed for row %d!\n", i);
            exit(1);
        }
        
        for (j = 0; j < *m; j++)
        {
            if (i == 0 || i == (*n) - 1 || j == 0 || j == (*m) - 1)
            {
                (*matrix)[i][j] = '+';
            }
            else 
            {
                fscanf(file, " %c", &(*matrix)[i][j]);
            }
            printf("%c", (*matrix)[i][j]);
        }
        printf("\n");
    }
    fclose(file);
}

void CopyMatrix(char **matrix, char ***new_mat, int n, int m)
{
    int i, j;
    for (i=0; i<n; i++)
        for (j=0; j<m; j++)
        {
            (*new_mat)[i][j] = matrix[i][j];
        }
}

int NrOfNeighbours(char **matrix, int i, int j)
{
    int nr = 0;
    if (matrix[i-1][j-1] == 'X') nr++;
    if (matrix[i-1][j] == 'X') nr++;
    if (matrix[i-1][j+1] == 'X') nr++;
    if (matrix[i][j-1] == 'X') nr++;
    if (matrix[i][j+1] == 'X') nr++;
    if (matrix[i+1][j] == 'X') nr++;
    if (matrix[i+1][j+1] == 'X') nr++;
    if (matrix[i+1][j-1] == 'X') nr++;
    return nr;
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
    char **matrix = NULL, **new = NULL;
    int n=0, m=0, NR_GEN=0, i, j, vecini;
    readMatrix(argv[1], &matrix, &n, &m, &NR_GEN);
    FILE* g = fopen(argv[2], "w");
    if (g == NULL) 
    {
        printf("Nu s-a putut deschide fisierul!");
        freeMatrix(&matrix, n);
        exit(1);
    }

    printf("%s", argv[1]);

    new = (char **)malloc(n * sizeof(char *));
    if (new == NULL) 
    {
        printf("Nu s-a alocat bine memoria!");
        freeMatrix(&matrix, n);
        fclose(g);
        exit(1);
    }

    for (i = 0; i < n; i++) 
    {
        new[i] = (char *)malloc(m * sizeof(char));
        if (new[i] == NULL) 
        {
            printf("Nu s-a alocat bine memoria pe linie!");
            freeMatrix(&matrix, n);
            freeMatrix(&new, i); 
            fclose(g);
            exit(1);
        }
    }
    
    for (i = 1; i < n-1; i++) 
    {
        for (j = 1; j < m-1; j++) 
        {
            fprintf(g, "%c", matrix[i][j]);
        }
        fprintf(g, "\n");
    }
    fprintf(g, "\n");

    CopyMatrix(matrix, &new, n, m);

    //Game of Life pe generatii
    for (int k = 0; k < NR_GEN; k++)
    {
        for (i = 1; i < n-1; i++) {
            for (j = 1; j < m-1; j++) 
            {
                vecini = NrOfNeighbours(matrix, i, j);
                if (vecini < 2 || vecini > 3)
                    new[i][j] = '+';
                else if (vecini == 3)
                    new[i][j] = 'X';            
            }
        }
        CopyMatrix(new, &matrix, n, m);
        for (i = 1; i < n-1; i++) 
        {
            for (j = 1; j < m-1; j++) 
            {
               fprintf(g, "%c", matrix[i][j]);
            }
            fprintf(g, "\n");
        }
        fprintf(g, "\n");
    }
    fclose(g);
    freeMatrix(&matrix, n);
    freeMatrix(&new, n);
    return 0;
}