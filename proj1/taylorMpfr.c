#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <mpfr.h>
#include <gmp.h>
#define N 100000 //Número de iterações da série de Taylor
pthread_mutex_t m; //Declaração de variáveis globais que as threads acessam
mpfr_t eEstimado;
typedef struct {//Estrutura ThreadData usada para gerenciar as threads
    int fracaoInicial;
    int fracaoFinal;
    int threadId;
} ThreadData;


void fat(int n, mpfr_t result_fat){//Função de cálculo fatorial iterativo
    mpfr_set_ui(result_fat, 1.0, MPFR_RNDU);
    for (int i = n; i > 0; i--){
        mpfr_mul_ui(result_fat, result_fat, i, MPFR_RNDU);
    }
}




void* calculoEParcial(void* arg) {//Função executada pelas threads para estimar intervalo de séries que a thread foi atribuida
    ThreadData* data = (ThreadData*)arg;
    int i = data->fracaoInicial;//Determina série inicial a ser calculada pela thread
    mpfr_t eParcial, quociente, numeroUm, fatAtual;
    mpfr_init2(eParcial, 1000000);
    mpfr_set_d(eParcial, 0.0, MPFR_RNDU);
    mpfr_init2(quociente, 1000000);
    mpfr_set_d(quociente, 1.0, MPFR_RNDU);
    mpfr_init2(numeroUm, 1000000);
    mpfr_set_d(numeroUm, 1.0, MPFR_RNDU);
    mpfr_init2(fatAtual, 1000000);
    fat(i,fatAtual);
    mpfr_div(quociente, numeroUm, fatAtual, MPFR_RNDU);
    mpfr_add(eParcial, eParcial, quociente, MPFR_RNDU);
    i++;
    for (i; i <= data->fracaoFinal; i++) {//Calcula valores das séries da thread
        mpfr_mul_ui(fatAtual, fatAtual, i, MPFR_RNDU);
        mpfr_div(quociente, numeroUm, fatAtual, MPFR_RNDU);
        mpfr_add(eParcial, eParcial, quociente, MPFR_RNDU);
    }
    pthread_mutex_lock(&m);//Thread tenta acessar recurso protegido pelo mutex
    mpfr_add(eEstimado, eEstimado, eParcial, MPFR_RNDU);//incrementa valor global do E estimado
    pthread_mutex_unlock(&m);//Libera mutex para outras threads acessarem
    mpfr_clear(quociente);
    mpfr_clear(numeroUm);
    mpfr_clear(eParcial);
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
    mpfr_init2(eEstimado, 1000000);
    mpfr_set_d(eEstimado, 0.0, MPFR_RNDU);
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
    mpfr_out_str(stdout, 10, 0, eEstimado, MPFR_RNDU);
}
