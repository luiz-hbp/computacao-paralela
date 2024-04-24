#include <stdio.h>
#include <math.h>

float calculaArea(double a, double b, int n){
	double h = (b-a)/n;
	double approx = (sin(a) + sin(b))/2.0;
	for(int i = 1; i < n; i++){
		double x_i = a + i*h;
		approx += sin(x_i);
	}
	approx = h*approx;
}

int main(void){
	double inicio = 0.75;
	double fim = 4.25;
	int n = 8;
	printf("Área Calculada: %f", calculaArea(inicio, fim, n));
	return 0;
}
