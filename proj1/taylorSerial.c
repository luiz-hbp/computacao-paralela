#include <stdio.h>
long double calculoE(int n){
        long double eEstimado = 2.0;
        long double fat = 1.0;
        for (int i = 2; i < n; i++){
                fat = fat*i;
                eEstimado += 1.0/fat;
                printf("%d %0.70Lf\n", i, eEstimado);
        }
        return eEstimado;
}

int main(void){
        printf("%0.70Lf\n",calculoE(21));
}
