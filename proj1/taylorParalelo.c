#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define N 60 //Número de iterações da série de Taylor
pthread_mutex_t m;
long double eEstimado = 0;
typedef struct {
    int fracaoInicial;
    int fracaoFinal;
    int threadId;
} ThreadData;

long double fat(int n){
	long double result = 1.0;
	for (int i = n; i > 0; i--){
		result = result*i;
	}
	return result;
}


void* calculoEParcial(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int i = data->fracaoInicial;
    long double fatAtual = fat(i);
    long double eParcial = 1.0/fatAtual;
    i++;
    for (i; i <= data->fracaoFinal; i++) {
	    fatAtual = fatAtual*i;
	    eParcial += 1.0/fatAtual;
    }
    printf("E parcial para a thread %d: %0.70Lf\n",data->threadId, eParcial);
    pthread_mutex_lock(&m); 
    eEstimado += eParcial;
    pthread_mutex_unlock(&m);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	pthread_mutex_init(&m, NULL);
	int numThreads = atoi(argv[1]);
	if (numThreads <= 0) {
		printf("Número de threads deve ser maior que 0.\n");
	       	return 1;
	}
    if (numThreads > N) numThreads = N;
    pthread_t threads[numThreads];
    ThreadData threadData[numThreads];

    int fracoesPorThread = N / numThreads;
    int fracoesExtras = N % numThreads;

    int fracaoAtual = 0;
    for (int i = 0; i < numThreads; i++) {
        threadData[i].fracaoInicial = fracaoAtual;
        fracaoAtual += fracoesPorThread + (fracoesExtras > 0 ? 1 : 0); // Distribui frações extras
        threadData[i].fracaoFinal = fracaoAtual - 1;
	threadData[i].threadId = i;
        fracoesExtras--;

        if (pthread_create(&threads[i], NULL, calculoEParcial, (void*)&threadData[i])) {
            printf("Erro ao criar thread\n");
            exit(-1);
        }
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&m);
    printf("e Estimado: %0.70Lf\n", eEstimado);
}
