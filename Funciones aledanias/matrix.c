#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int randomvalue(float p);
int prob(float x, float p);

int main()
{
        srand(time(NULL));
	float p = 0.5;
        int a[10][10];
        int i,j;
        for(i=0;i<10;i=i+1){
                for(j=0; j<10; j= j+1){
                        a[i][j] = randomvalue(p);
                        printf("En la posicion (%i , %i) esta el valor %i \n",i,j, a[i][j]);
                        }
                }
        printf("Yo queria una matriz :) \n");
        return 0;
}

int randomvalue(float p){
        int val;
        val = prob((float)rand()/((float)RAND_MAX),p);
        return val;
}

int prob(float x,float p){

	if(x < p) 
		return 0;
	else 	
		return 1;

}

