/**
 * @file multiplication.c
 * @brief Edited verion of multiplication.c from lab 3 to get results for final project assingment
 */

#include <stdio.h>
#include <stdlib.h>

#ifndef SIZE
#define SIZE 32
#endif

void multiplication(int** mat_A, int** mat_B, int** mat_product, int N, FILE *file)
{

    int i, j, k;
    //Matrix multiplication without transposing the B matrix
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (k = 0; k < N; k++) {
		        mat_product[i][j] += mat_A[i][k] * mat_B[k][j];
                fprintf(file, "%p R %d\n", &mat_A[i][k], mat_A[i][k]);
                fprintf(file, "%p R %d\n", &mat_B[k][j], mat_B[k][j]);
                fprintf(file, "%p W %d\n", &mat_product[i][j], mat_product[i][j]);
            }
        }
    }
    fclose(file);
}

int main(void)
{
    printf("Generating matrices of size %d * %d \n",SIZE,SIZE);

    /* Open file to write memory accesses to */
    FILE *file = fopen("multiplication_input.txt", "w");

    int **mat_A = (int **)malloc(SIZE * sizeof(int *)); 
    int **mat_B = (int **)malloc(SIZE * sizeof(int *));
    int **mat_product = (int **)malloc(SIZE * sizeof(int *));
  
    for (int i=0; i<SIZE; i++) { 
            mat_A[i] = (int *)malloc(SIZE * sizeof(int)); 
	    mat_B[i] = (int *)malloc(SIZE * sizeof(int));
	    mat_product[i] = (int *)malloc(SIZE * sizeof(int));
    }

    for( int i=0; i<SIZE; i++) {
        for(int j=0; j<SIZE; j++) {
            mat_A[i][j]= i-j;
            mat_B[i][j]= 1;
            mat_product[i][j] = 0;
            fprintf(file, "%p W %d\n", &mat_A[i][j], i-j);
            fprintf(file, "%p W %d\n", &mat_B[i][j], 1);
            fprintf(file, "%p W %d\n", &mat_product[i][j], 0);
        }
    }
    printf("Computing the product....\n");

    // Matrix multiplication with B
    multiplication(mat_A, mat_B, mat_product, SIZE, file);

    return 0;
}

