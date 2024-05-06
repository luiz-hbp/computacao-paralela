#include <stdio.h>
#include <pthread.h>
typedef struct {
    int startRow;
    int endRow;
} ThreadData;

long double fat(int n){
	long double result = 1.0;
	for (int i = n; i > 0; i--){
		result = result*i;
	}
	return result:
}


void* calculoEParcial(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int i = data->startRow;
    long double fat = fat(i);
    long double eParcial = 1.0/mod;
    i++;
    for (i; i <= data->endRow; i++) {
	    fat = fat*i;
	    eParcial += 1.0/fat;
    }
    pthread_exit(NULL);
}

long double calculoE(int n){
        long double eEstimado = 2.0;
        long double fat = 1.0;
        for (int i = 2; i < n; i++){
                fat = fat*i;
                eEstimado += 1.0/fat;
                printf("%0.100Lf\n", eEstimado);
        }
        return eEstimado;
}

int main(int argc){
        printf("%0.100Lf\n",calculoE(200));
}
