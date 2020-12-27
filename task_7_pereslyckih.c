#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <omp.h>

const int dim = 1000;

void matrix_multiplier( long *matrix, long *vector, long *result_vector, int threads_amount);
void matrix_and_vector_rand( long *matrix, long *vector);

int main (void)
{
    srand( (unsigned)time(NULL) );

    int matrix_amount, threads_amount;

    printf("Enter amount of matrixes: ");
    scanf("%d", &matrix_amount);

    printf("Enter amount of threads: ");
    scanf("%d", &threads_amount);

    long *matrixes;
    long *vectors;
    long *res_vectors;

    //memory allocation for vectors and matrices
    matrixes = (long*)malloc(matrix_amount * dim * dim * sizeof(long));
    vectors = (long*)malloc(matrix_amount * dim * sizeof(long));
    res_vectors = (long*)malloc(matrix_amount * dim * dim * sizeof(long));

    //creating random matrixes and vectors
    for( int i = 0; i < matrix_amount; i++ )
    {
        matrix_and_vector_rand( matrixes + i*dim*dim , vectors + i*dim);
    }
    
    float start, end, delta;
    for(int k = 1; k <= threads_amount; k++){
        //multiply timing start
        start = omp_get_wtime();

        //matrix and vector multiplications 
        for( int i = 0; i < matrix_amount; i++ ){
            matrix_multiplier( matrixes + i*dim*dim, vectors + i*dim , res_vectors + i*dim, k);
        }

        end = omp_get_wtime();
        delta = end - start;

        printf("amount of treads: %i\n", k);
        printf("multiplication algorithm duration: %f\n\n", delta );
    }

    free(matrixes);
    free(vectors);
    free(res_vectors);
}

/*
* this function fill the matrix and vector with random numbers(parallel)
*/
void matrix_and_vector_rand( long *matrix, long *vector ){
    for (int i = 0; i < dim; i++){
        *(vector + i) = rand();
        
        for (int j = 0; j < dim; j++){
            *(matrix + i*dim + j) = rand();
        }
    }
}

/*
* this function multiplies matrix and vector(parallel)
*/
void matrix_multiplier( long *matrix, long *vector, long *result_vector , int threads_amount)
{
    //if j not privated - programm does not parallel
    #pragma omp parallel for num_threads(threads_amount)
        for (int i = 0; i < dim; i++){
            *(result_vector + i) = 0;

            for (int j = 0; j < dim; j++){
                *(result_vector + i) += *(matrix + i*dim + j) * ( *(vector + j) );
            }
        }
}