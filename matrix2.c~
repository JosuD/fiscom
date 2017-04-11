/* Esta version tiene otra forma de visualizar la matriz
Además, arma la matriz dinámica habilitando el espacio en la memoria */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int randomvalue(float p);
void imprimirmatriz(int** a, int n, int m);
int main()
{
        srand(time(NULL));
	float p = 0.5;
	int n = 15; // filas 
	int m = 15; // columnas 
	int** a = (int**)malloc(n * sizeof(int*)); // defino el espacio para la matriz
	int index;
	for (index=0;index<n;++index){
		a[index] = (int*)malloc(m * sizeof(int));
		}
        int i,j;
        for(i=0;i<n;i=i+1){ 	// Armo la matriz con 0 y 1
                for(j=0; j<m; j= j+1){
                        a[i][j] = randomvalue(p);
                        }
                }
	imprimirmatriz(a,n,m);
        printf("Yo queria una matriz :) \n");
        return 0;
}

int randomvalue(float p){
        int val;
	float x;
	x = (float)rand()/((float)RAND_MAX); // Valor de distribución uniforme entre 0 y 1
	if(x<p)
		val = 0;
	else
		val = 1;
        return val;
}

void imprimirmatriz(int** a, int n, int m){	
	int row, columns;
	for (row=0; row<n; row++){
		for(columns=0; columns<m; columns++)
			printf("%d     ", a[row][columns]);
    		printf("\n");
	}
}
