#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define P     1              // 1/2^P, P=16
#define Z     1              // iteraciones, deberían ser 27000
#define N     6             // lado de la red simulada


void  llenar(int *red,int n,float prob); // Esta está
int   hoshen(int *red,int n); // Esta parece que está
void  imprimir(int* red, int n, int m); // Esta está
int   randomvalue(float p); // Esta está
int   actualizar(int *red,int *clase,int s,int frag, int i, int j); // Ya está
void  etiqueta_falsa(int *red,int *clase,int s1,int s2, int i, int j); // Ya está
void  corregir_etiqueta(int *red,int *clase,int n); // Ya está
int   percola(int *red,int n); // ya está
void  exportar(int *z, int n, int m);
void  imprimir_vector(int* vector, int n);
void  armar_red(int *red, int n);


int main(int argc,char *argv[])
{
  int    i,j, n, z,*red;
  float  prob,denominador;

  n=N;
  z=Z;

  if (argc==3) 
     {
       sscanf(argv[1],"%d",&n);
       sscanf(argv[2],"%d",&z);
     }
    
  red=(int *)malloc(n*n*sizeof(int)); // Definimos el espacio de memoria para meter la red

  for(i=0;i<z;i++)
    {
      prob=0.5;
      denominador=2.0;
 
      srand(time(NULL));

      for(j=0;j<P;j++)
        {
          llenar(red,n,prob);
 
	  //armar_red(red, n); // armo una red a mano para testear hoshen
          imprimir(red,n,n);
  	  hoshen(red,n);
	  imprimir(red,n,n);
	  exportar(red,n,n);
        
          denominador=2.0*denominador;

          if (percola(red,n)) 
             {prob+=(-1.0/denominador);
	     printf("percolo!\n");} 
          else{ 
	      prob+=(1.0/denominador);
	      printf("no pecolo :(\n");}
        }
    }

  free(red);

  return 0;
}



int hoshen(int *red,int n)
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

  for(k=0;k<n*n;k++) *(clase+k)=frag; // Lleno de ceros el vector clase, que recién sólo estaba definido su espacio en memoria
  
  // primer elemento de la red

  s1=0;
  frag=2; // última etiqueta conocida
  if (*red) frag=actualizar(red,clase,s1,frag, i, j);
  imprimir(red, n,n);
  
  // primera fila de la red

  for(j=1;j<n;j++) 
    {
      if (*(red+j)) 
         {
           s1=*(red+j-1);  
           frag=actualizar(red+j,clase,s1,frag, i, j); // asigna una etiqueta o copia la "verdadera" etiqueta del vecino. No resuelve conflictos
           //printf("la segunda direccion es %p\n", (void *) (red+j));  
	}
    imprimir(red,n,n);
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
      imprimir(red,n,n);
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
	imprimir(red,n,n);
    }


  corregir_etiqueta(red,clase,n);

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
		val = 0;
	else
		val = 1;
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
	printf("actualizar en posicion (%d, %d)\n", i, j);	
	printf("entro frag = %d y s = %d \n", frag, s);
	//printf("la direccion que entra es %p\n", (void *) red);
	if(s>0){
		while(*(clase+s)<0)
			s = - *(clase+s);		
		*(red) = s;
		*(clase+s)=s;
		printf("copio s = %d\n", s);
	}
	else{
		*(red) = frag;
		*(clase+frag) = frag;
		frag ++;
		printf("nuevo fragmento\n");
	}
	return frag;	
}

void corregir_etiqueta(int *red, int *clase, int n){
	int i,s;
	printf("actua corregir etiqueta\n");	
	for(i=0; i<n*n; i++){
		s = *(red+i);
		while(*(clase+s)<0)
			s = - *(clase+s);
		*(red+i) = s;
		
	}
}


void etiqueta_falsa(int *red,int *clase,int s1,int s2, int i, int j){	
	printf("actuo etiqueta falsa en posicion (%d, %d)\n", i, j);	
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
	imprimir_vector(red, n);
	imprimir_vector(red+n*(n-1), n);
	return out;
}

void exportar(int *z, int n, int m){
	int i,j;
	FILE *fp;
	fp = fopen("myfile.txt", "a");
	for(i = 0; i< n; i++){ //fprintf(fp, "%d\t", *(z+i));
		for(j=0; j<m-1; j++){
			fprintf(fp, "%d\t", *(z+n*i+j));		
		}
		fprintf(fp, "%d\n", *(z+n*i+j));
	}
	fclose(fp);
}

void imprimir_vector(int* vector, int n){	// función para debbugear
	int i;
	FILE *fp;
	fp = fopen("percola.txt","a");	
	for (i=0; i<n; i++){
		fprintf(fp, "%d\t     ", *(vector+i));
		}
    	fprintf(fp, "\n");
	fclose(fp);
	
}

void armar_red(int *red, int n){ // pienso en una red 3x3
	//int i,j;
	*red = 0;
	*(red+1)=0;
	*(red+2)=1;
	*(red+3)=1;
	*(red+4)=1;
	*(red+5)=1;
	*(red+6)=0;
	*(red+7)=1;
	*(red+8)=0;
//	*(red+9)=1;

}


