#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define M 2 // Número de linhas da matriz
#define N 3 // Número de colunas da matriz e linhas do vetor

int matrix[M][N] = {
        {1, -1, 2},
        {0, -3, 1}
};
int vector[N] = {2, 1, 0};
int result[M];

typedef struct {
    int row;
} ThreadData;

void* computeElement(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int row = data->row;
    result[row] = 0;
    
    for (int j = 0; j < N; j++) {
        result[row] += matrix[row][j] * vector[j];
    }
    
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[M];
    ThreadData threadData[M];
    
    for (int i = 0; i < M; i++) {
        threadData[i].row = i;
        int rc = pthread_create(&threads[i], NULL, computeElement, (void*)&threadData[i]);
        if (rc) {
            printf("erro: %d\n", rc);
            exit(-1);
        }
    }

    for (int i = 0; i < M; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Resulting vector:\n");
    for (int i = 0; i < M; i++) {
        printf("%d\n", result[i]);
    }

    return 0;
}
