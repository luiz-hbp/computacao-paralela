#include <stdio.h>
long double calculoE(int n){ //Funcão calculoE recebe número de séries e retorna o número de Euler estimado.
        long double eEstimado = 2.0; //ínicia valor estimado como 2 e já poupa o calculo das duas primeiras séries
        long double fat = 1.0;
        for (int i = 2; i < n; i++){//Partindo da terceira série parte por todos os números de séries
                fat = fat*i;//Calcula o falor do fatorial baseando-se no valor anterior
                eEstimado += 1.0/fat;//Incrementa o valor estimado com o valor da série atual
                //printf("%d %0.70Lf\n", i, eEstimado);
        }
        return eEstimado;
}

int main(void){
        printf("%0.70Lf\n",calculoE(5000));
}
