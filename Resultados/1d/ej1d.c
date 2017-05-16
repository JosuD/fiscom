#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define N     20
#define Z     100           // iteraciones, deberían ser 27000
#define PS    100          // cantidad de valores de p para un barrido
#define PCRIT 5.925807619259259739e-01 // P_critico para L= 64

void  llenar(int *red,int n,float prob); 
int   hoshen(int *red,int n, int *clusters); 
void  imprimir(int* red, int n, int m); 
int   randomvalue(float p); 
int   actualizar(int *red,int *clase,int s,int frag, int i, int j);
void  etiqueta_falsa(int *red,int *clase,int s1,int s2, int i, int j);
void  corregir_etiqueta(int *red,int *clase,int *clusters, int n); 
int   percola(int *red,int n);
 
void   exportar(int *z, int n, int m);
void   imprimir_vector(int* vector, int n, char *nombre);
void   exportar_vector(double* vector, int n, char *nombre);
int    donde_percola(int *red, int n);
double promedio_float(double *vector, int n);
double intensidad(int *clusters, int dondepercola, int n);
int    clasificar(int *clusters, int cant_clusters, int *tamanios, int n);
double masa(int *clusters, int dondepercola, int n);
void   exportar_vector_int(int* vector, int n, char *nombre);

int main(int argc,char *argv[]){

int    i,j, l, n, k, z, ps, *red, *clusters, *tamanios, dondepercola, cant_clusters;
float  prob, paso, *linspace_prob, pcrit;
time_t t_inicial, t_final;
double *masa_l;
char   file[10];

n=N;
z=Z;
ps = PS;
pcrit = PCRIT;

if (argc==4) 
   {
     sscanf(argv[1],"%d",&n);
     sscanf(argv[2],"%d",&z);
     //sscanf(argv[3],"%s",&file);
     strcpy(file,argv[3]);
     }

clusters = (int *)malloc(n*n*sizeof(int)); // Defino un vector que guarda el tamaño de cada cluster, donde la posición coincide con la etiqueta

 // dondepercola = (int *)malloc(sizeof(int)); //Guardo en memoria dinámica (por ser cómodo) la etiqueda del primer cluster percolante (desprecio otros percolantes de la red)
tamanios = (int *)malloc(n*n*sizeof(int)); // vector que guarda en el indice i la cantidad de clusters de tamaño i

    
srand(time(NULL));
time(&t_inicial);

red=(int *)malloc(n * n *sizeof(int));

cant_clusters = 0;
printf("Entro en el loop de tamanios\n");
for(i=0;i<z;i++){
	for(l=0; l<n*n; l++) *(clusters+l) = 0;
	llenar(red, n, pcrit);
	printf("llene, i = %d\n", i);
	hoshen(red, n, clusters);
	printf("etiquete\n");
	cant_clusters = clasificar(clusters, cant_clusters, tamanios, n); //esta función me arma el n_s = tamaños		
	printf("clasifique\n");
	printf("termine interacion %d\n", i);
	}

  exportar_vector_int(tamanios, n*n, "tamanios.txt");
  time(&t_final);
  printf("Tiempo transcurrido en segundos: %.0f\n", difftime(t_final, t_inicial));
  return 0;
}




int hoshen(int *red,int n, int *clusters)
{
  /*
    Esta funcion implementa en algoritmo de Hoshen-Kopelman.
    Recibe el puntero que apunta a la red y asigna etiquetas 
    a cada fragmento de red. 
  */

  int i,j,k,s1,s2,frag;
  int *clase;

  frag=0;
  i = 0;
  j = 0;
  clase=(int *)malloc(n*n*sizeof(int));
  //printf("abri clase\n");

  for(k=0;k<n*n;k++) *(clase+k)=frag; // Lleno de ceros el vector clase, que recién sólo estaba definido su espacio en memoria
  
  // primer elemento de la red

  s1=0;
  frag=2; // última etiqueta conocida
  if (*red) frag=actualizar(red,clase,s1,frag, i, j);
  //imprimir(red, n,n);
  
  // primera fila de la red

  for(j=1;j<n;j++) 
    {
      if (*(red+j)) 
         {
           s1=*(red+j-1);  
           frag=actualizar(red+j,clase,s1,frag, i, j); // asigna una etiqueta o copia la "verdadera" etiqueta del vecino. No resuelve conflictos
           
	}
    //imprimir(red,n,n);
    }
  

  // el resto de las filas 
    
  for(i=n;i<n*n;i=i+n)
    {
	j = 0;
      // primer elemento de cada fila

      if (*(red+i)) 
         {
           s1=*(red+i-n); 
           frag=actualizar(red+i,clase,s1,frag, i, j);

         }
      //imprimir(red,n,n);
      for(j=1;j<n;j++)
	if (*(red+i+j))
	  {
	    s1=*(red+i+j-1); 
            s2=*(red+i+j-n);

	    if (s1*s2>0)
	      {
		etiqueta_falsa(red+i+j,clase,s1,s2, i, j); // Este si resuelve conflictos. S1 y S2 son las etiquetas de el anterior y el de arriba. 
	      }
	    else 
	      { if (s1!=0) frag=actualizar(red+i+j,clase,s1,frag, i, j);
	        else       frag=actualizar(red+i+j,clase,s2,frag, i, j);
	      }
	  
	  }
	//imprimir_vector(clase,n*n);
	//imprimir(red,n,n);
    }


  corregir_etiqueta(red,clase,clusters,n);

  free(clase);

  return 0;
}

void llenar(int *red,int n,float prob){
        int i,j;
        for(i=0;i<n;i=i+1){ 	// Armo la matriz con 0 y 1
                for(j=0; j<n; j= j+1){
                        red[i*n+j] = randomvalue(prob);
                        }
                }
}

int randomvalue(float p){
        int val;
	float x;
	x = (float)rand()/((float)RAND_MAX); // Valor de distribución uniforme entre 0 y 1
	if(x<p)
		val = 1;
	else
		val = 0;
        return val;
}

void imprimir(int* red, int n, int m){	
	int row, columns;
	for (row=0; row<n; row++){
		for(columns=0; columns<m; columns++)
			printf("%d     ", red[row*n+columns]);
    		printf("\n");
	}
}

int actualizar(int *red,int *clase,int s,int frag, int i, int j){
	//printf("actualizar en posicion (%d, %d)\n", i, j);	
	//printf("entro frag = %d y s = %d \n", frag, s);
	//printf("la direccion que entra es %p\n", (void *) red);
	if(s>0){
		while(*(clase+s)<0)
			s = - *(clase+s);		
		*(red) = s;
		*(clase+s)=s;
		//printf("copio s = %d\n", s);
	}
	else{
		*(red) = frag;
		*(clase+frag) = frag;
		frag ++;
		//printf("nuevo fragmento\n");
	}
	return frag;	
}

void corregir_etiqueta(int *red, int *clase, int *clusters, int n){
	int i,s;
	//printf("actua corregir etiqueta\n");	
	for(i=0; i<n*n; i++){
		s = *(red+i);
		while(*(clase+s)<0)
			s = - *(clase+s);
		*(red+i) = s;
		*(clusters+s)+=1;
	}
}


void etiqueta_falsa(int *red,int *clase,int s1,int s2, int i, int j){	
	//printf("actuo etiqueta falsa en posicion (%d, %d)\n", i, j);	
	while(*(clase+s1)<0)
		s1 = - *(clase+s1);
	while(*(clase + s2)<0)
		s2 = - *(clase+s2);
	if(s1 < s2){
		*(clase+s2) = -s1;
		*(clase+s1) = s1;
		*(red) = s1;
	}
	else{
		*(clase+s1) = -s2;
		*(clase+s2) = s2;
		*(red) = s2;
		
	}
}

int percola(int *red, int n){
	int i,j, out;
	out = 0;
	for(i=0; i<n; i++){
		for(j=0; j<n; j++){
			if((*(red+i)!=0) && (*(red +n*(n-1) + j) != 0) && (*(red + i) == *(red +n*(n-1) + j))){ //pido celdas !=0 que valgan lo mismo
				out = 1;
				break;
			}
		}	
	}
	return out;
}

int donde_percola(int *red, int n){
	int i,j, out;
	out = 0;
	for(i=0; i<n; i++){
		for(j=0; j<n; j++){
			if((*(red+i)!=0) && (*(red +n*(n-1) + j) != 0) && (*(red + i) == *(red +n*(n-1) + j))){ //pido celdas !=0 que valgan lo mismo
				out = *(red + i);
				break;
			}
		}	
	}
	return out;
}


double intensidad(int *clusters, int dondepercola, int n){
	int numerador, i;
	double res;	
	numerador = *(clusters + dondepercola); //tamaño del cluster que percoló
	res = (double)(numerador)/(double)(n*n);// lo divido por el total de celdas
	//printf("numerador=%f y denominador=%f \n. La division da %f. Percola en %d\n", (float)numerador, denominador, res, dondepercola);
	return res;
}

double masa(int *clusters, int dondepercola, int n){
	int numerador, i;
	numerador = *(clusters + dondepercola); //tamaño del cluster que percoló
	return (double)(numerador);
}


int clasificar(int *clusters, int cant_clusters, int *tamanios, int n){
	int i;	
	for(i=0; i<n*n; i++){
		cant_clusters += *(clusters + i);
		if(*(clusters+i) != 0){
			*(tamanios+*(clusters + i))+=1;
		}
	}
	return cant_clusters;
}


double promedio_float(double *vector, int n){
	int i;
	double res;
	res = 0.0;
	for(i=0; i<n; i++){
		res += *(vector + i);
	}
	res = res/(double)n;
	return (float)res;
}


void exportar(int *z, int n, int m){
	int i,j;
	FILE *fp;
	fp = fopen("myfile.txt", "a");
	for(i = 0; i< n; i++){
		for(j=0; j<m-1; j++){
			fprintf(fp, "%d\t", *(z+n*i+j));		
		}
		fprintf(fp, "%d\n", *(z+n*i+j));
	}
	fprintf(fp, "\n");
	fclose(fp);
}


void imprimir_vector(int* vector, int n, char *nombre){	// función para debbugear
	int i;
	FILE *fp;
	fp = fopen(nombre,"a");	
	for (i=0; i<n-1; i++){
		fprintf(fp, "%d\t     ", *(vector+i));
		}
    	fprintf(fp, "%d\n", *(vector+i));
	fclose(fp);
	
}

void  exportar_vector(double* vector, int n, char *nombre){
	int i;
	FILE *fp;
	fp = fopen(nombre,"a");	
	for (i=0; i<n-1; i++){
		fprintf(fp, "%g\t     ", *(vector+i));
		}
    	fprintf(fp, "%f\n     ", *(vector+i));
	fclose(fp);

}

void  exportar_vector_int(int* vector, int n, char *nombre){
	int i;
	FILE *fp;
	fp = fopen(nombre,"a");	
	for (i=0; i<n-1; i++){
		fprintf(fp, "%d\t     ", *(vector+i));
		}
    	fprintf(fp, "%d\n     ", *(vector+i));
	fclose(fp);

}
