#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define N 5000 //Número de iterações da série de Taylor
pthread_mutex_t m; //Declaração de variáveis globais que as threads acessam
long double eEstimado = 0;
typedef struct {//Estrutura ThreadData usada para gerenciar as threads
    int fracaoInicial;
    int fracaoFinal;
    int threadId;
} ThreadData;

long double fat(int n){//Função de cálculo fatorial iterativo
	long double result = 1.0;
	for (int i = n; i > 0; i--){
		result = result*i;
	}
	return result;
}


void* calculoEParcial(void* arg) {//Função executada pelas threads para estimar intervalo de séries que a thread foi atribuida
    ThreadData* data = (ThreadData*)arg;
    int i = data->fracaoInicial;//Determina série incial a ser calculada pela thread
    long double fatAtual = fat(i);//Calcula fatorial da primeira série atribuida
    long double eParcial = 1.0/fatAtual;//Calculo do valor da primeira série da thread
    i++;
    for (i; i <= data->fracaoFinal; i++) {//Calcula valores das séries da thread
	    fatAtual = fatAtual*i;
	    eParcial += 1.0/fatAtual;//Incrementa valor parcial das séries calculadas pela thread
    }
    //printf("E parcial para a thread %d: %0.70Lf\n",data->threadId, eParcial);
    pthread_mutex_lock(&m);//Thread tenta acessar recurso protegido pelo mutex 
    eEstimado += eParcial;//incrementa valor global do E estimado
    pthread_mutex_unlock(&m);//Libera mutex para outras threads acessarem
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	pthread_mutex_init(&m, NULL);//Inicia mutex usada para garantir que o valor global do E estimado não seja comprometido
	int numThreads = atoi(argv[1]);//Busca número de threads a ser usado declarado na execução
	if (numThreads <= 0) {//Indica que o usuário inseriu número inválido de threads
		printf("Número de threads deve ser maior que 0.\n");
	       	return 1;
	}
    if (numThreads > N) numThreads = N;//Caso o número de threads declarado seja maior que o número de séries a ser calculado, o número de threads considerado é o número de séries
    pthread_t threads[numThreads];//Declara estrutura para gerenciar o número de threads definido
    ThreadData threadData[numThreads];

    int fracoesPorThread = N / numThreads;//Divide o número de series igualmente pelas threads
    int fracoesExtras = N % numThreads;//Calcula resto da divisão para atribuir as séries que sobraram

    int fracaoAtual = 0;
    for (int i = 0; i < numThreads; i++) {//Distribui séries entre as threads
        threadData[i].fracaoInicial = fracaoAtual;//Atribui valor da primeira série da thread na estrutura da thread
        fracaoAtual += fracoesPorThread + (fracoesExtras > 0 ? 1 : 0); // Distribui frações extras
        threadData[i].fracaoFinal = fracaoAtual - 1;
	threadData[i].threadId = i;
        fracoesExtras--;

        if (pthread_create(&threads[i], NULL, calculoEParcial, (void*)&threadData[i])) {//Cria threads com os intervalos de séries definidas
            printf("Erro ao criar thread\n");//Indica erro na criação de thread
            exit(-1);
        }
    }

    for (int i = 0; i < numThreads; i++) {//Sincroniza threads criadas
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&m);
    printf("e Estimado: %0.70Lf\n", eEstimado);//Mostra número de euler estimado
}
