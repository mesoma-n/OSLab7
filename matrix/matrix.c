#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 20
int matA[MAX][MAX];
int matB[MAX][MAX];
int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX];
int matProductResult[MAX][MAX];
int num_threads = 10;  // Total number of threads

typedef struct {
    int row_start;
    int row_end;
    int col_start;
    int col_end;
} ThreadData;

void fillMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i < MAX; i++) {
        for(int j = 0; j < MAX; j++) {
            matrix[i][j] = rand() % 10 + 1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i < MAX; i++) {
        for(int j = 0; j < MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void* computeSum(void* args) {
    ThreadData *data = (ThreadData*) args;
    for (int i = data->row_start; i < data->row_end; i++) {
        for (int j = data->col_start; j < data->col_end; j++) {
            matSumResult[i][j] = matA[i][j] + matB[i][j];
        }
    }
    free(data); // Free allocated memory for the thread data
    return NULL;
}

void* computeDiff(void* args) {
    ThreadData *data = (ThreadData*) args;
    for (int i = data->row_start; i < data->row_end; i++) {
        for (int j = data->col_start; j < data->col_end; j++) {
            matDiffResult[i][j] = matA[i][j] - matB[i][j];
        }
    }
    free(data);
    return NULL;
}

void* computeProduct(void* args) {
    ThreadData *data = (ThreadData*) args;
    for (int i = data->row_start; i < data->row_end; i++) {
        for (int j = data->col_start; j < data->col_end; j++) {
            matProductResult[i][j] = 0;
            for (int k = 0; k < MAX; k++) {
                matProductResult[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }
    free(data);
    return NULL;
}

void divide_workload_and_execute(void* (*function)(void*)) {
    pthread_t threads[num_threads];
    int rows_per_thread = MAX / num_threads;
    
    for (int i = 0; i < num_threads; i++) {
        ThreadData *data = (ThreadData*) malloc(sizeof(ThreadData));
        data->row_start = i * rows_per_thread;
        data->row_end = (i == num_threads - 1) ? MAX : data->row_start + rows_per_thread;
        data->col_start = 0;
        data->col_end = MAX;
        
        pthread_create(&threads[i], NULL, function, data);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    srand(time(0));

    fillMatrix(matA);
    fillMatrix(matB);

    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);

    // Perform parallel operations
    divide_workload_and_execute(computeSum);
    divide_workload_and_execute(computeDiff);
    divide_workload_and_execute(computeProduct);

    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);

    return 0;
}
