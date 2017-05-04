#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define P     16              // 1/2^P, P=16 precision
#define Z     1000           // iteraciones, deberían ser 27000
#define N     4             // lado de la red simulada
#define PS    100          // cantidad de valores de p para un barrido
#define PCRIT 5.612907619259259739e-01 // P_critico para L= 4
void  llenar(int *red,int n,float prob); 
int   hoshen(int *red,int n, int *clusters, int *respuesta); 
void  imprimir(int* red, int n, int m); 
int   randomvalue(float p); 
int   actualizar(int *red,int *clase,int s,int frag, int i, int j);
void  etiqueta_falsa(int *red,int *clase,int s1,int s2, int i, int j);
void  corregir_etiqueta(int *red,int *clase,int *clusters, int n, int *respuesta); 
int   percola(int *red,int n);
 
void  exportar(int *z, int n, int m);
void  imprimir_vector(int* vector, int n, char *nombre);
void  exportar_vector(double* vector, int n, char *nombre);
void  armar_red(int *red, int n);
void  exportar_probabilidades(float prob, char *file);
int   donde_percola(int *red, int n);
void  exportar_matriz(double *matriz, int n, int m);
float promedio_int(int *vector, int n);
double promedio_float(double *vector, int n);
double intensidad(int *clusters, int dondepercola, int n);
int clasificar(int *clusters, int cant_clusters, int *tamanios, int n);
int percolantes(int *red, int n, int *respuesta);
int indice_max(int *ns_i, int ps);



int main(int argc,char *argv[])
{
  int    i,j, l, n, k, z, ps, *red, *clusters, *tamanios, dondepercola, cant_clusters, *respuesta, *ns_iter, smax, *ns_i, indice, clusters_percolantes;
  float  prob,denominador, paso, linspace_prob[PS], pcrit;
  time_t t_inicial, t_final, t_parcial;
  double *intensidades_iter, *intensidades_prob, hist[2*PS], *p_max, *ns_prob, *m2;
  char   file[2*10];

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
    
  red=(int *)malloc(2*n*n*sizeof(int)); // Definimos el espacio de memoria para meter la red
  clusters = (int *)malloc(2*n*n*sizeof(int)); // Defino un vector que guarda el tamaño de cada cluster, donde la posición coincide con la etiqueta

 // dondepercola = (int *)malloc(sizeof(int)); //Guardo en memoria dinámica (por ser cómodo) la etiqueda del primer cluster percolante (desprecio otros percolantes de la red)
  tamanios = (int *)malloc(n*n*sizeof(int)); // vector que guarda en el indice i la cantidad de clusters de tamaño i
  for(i=0; i<n*n; i++) *(tamanios+i) = 0;
  intensidades_iter = (double *)malloc(2*z*sizeof(double));

  intensidades_prob = (double *)malloc(2*ps*sizeof(double));
  for(i=0; i<2*ps; i++) *(intensidades_prob+i) = 0.0;

  respuesta = (int *)malloc(n*sizeof(int));
  for(i=0; i<n; i++) *(respuesta+i)=0;

// Linspace de probabilidades
  paso = 1.0/ps;
  *hist = 0.0;
  *linspace_prob = 0;
  for(i=1; i<ps; i++){
	*(linspace_prob + i) = *(linspace_prob + i - 1) + paso;
	*(hist+i) = 0.0;} //de paso inicializo el histograma en el mismo for
  
  srand(time(NULL));
  time(&t_inicial);

for(k=0; k<ps; k++){
  printf("probabilidad = %d/100\n", k);
  for(l=0; l<2*z; l++) *(intensidades_iter+l) = 0.0;

  for(i=0;i<z;i++)
    {
	  for(l=0; l<n*n; l++) *(clusters+l) = 0;

          //printf("Iteracion numero %d\n", i);
          llenar(red,n,*(linspace_prob + k));
  	  hoshen(red,n, clusters, respuesta);
	  if(percola(red,n)){
		*(hist+k)+=1.0;
		 dondepercola = donde_percola(red, n);
	  //printf("percolo en %d\n", dondepercola);
		*(intensidades_iter+i) = intensidad(clusters, dondepercola, n);
		//printf("intensidades_iter = %g\n", *(intensidades_iter+i));
	  }
	  
    }
         //printf("Cambio de iteracion ---------\n");
	*(intensidades_prob+k) = promedio_float(intensidades_iter, z); //descarto las que no percolaron?
	//printf("intensidad_prob = %g en k = %d\n", *(intensidades_prob+k), k);
}
  //exportar(red,n,n);
  //free(red);
  //free(intensidades_iter);
  for(k=0; k<ps; k++){
	*(hist+k) = *(hist+k)/(double)z;}
  exportar_vector(hist, ps, "histograma.txt");
  exportar_vector(intensidades_prob, ps, "intensidades.txt");

  if(n==36){
	cant_clusters = 0.0;
	printf("Entro en el loop de tamanios\n");
	for(i=0;i<z;i++){
		//red=(int *)malloc(n*n*sizeof(int));
		for(l=0; l<n*n; l++) *(clusters+l) = 0;
		llenar(red, n, pcrit);
		printf("llene, i = %d\n", i);
		imprimir(red, n, n);
		hoshen(red, n, clusters, respuesta);
		printf("etiquete\n");
		cant_clusters = clasificar(clusters, cant_clusters, tamanios, n); //esta función me arma el n_s = tamaños
//		imprimir(tamanios, n,n);		
		printf("clasifique\n");
		printf("termine interacion %d\n", i);
		//free(red);
	}
  }
  //free(red);
  imprimir_vector(tamanios, n*n, "tamanios.txt");
  
  if(n == 64){
	  smax = 15; // cantidad de s usamos
	// Linspace fino alrededor de pc
	  paso = 0.2/ps; // voy a ir de 0.4 a 0.6
	  *linspace_prob = 0.4; // valor inicial
	  for(i=1; i<ps; i++){
		*(linspace_prob + i) = *(linspace_prob + i - 1) + paso;
	  }

	  for(i=0; i<n*n; i++) *(tamanios+i) = 0;	  // inicializo tamaños
	  cant_clusters = 0.0;

	  ns_iter = (int *)malloc(n*n*sizeof(int));	// vector donde acumulo los ns para luego dividir por z
	  for(i=0; i<n*n; i++) *(ns_iter+i) = 0;

	  ns_prob = (double *)malloc(smax*ps*sizeof(double)); // aquí guardo los vectores ns para cada probabilidad del intervalo
	  for(i=0; i<smax*ps; i++) *(ns_prob+i) = 0.0;

	  p_max = (double *)malloc(smax*sizeof(double));  // acá guardo los p donde se produce la mayor cantidad de cluster de tamaño s
	  for(i=0; i<smax; i++) *(p_max+i) = 0.0;

	  ns_i = (int *)malloc(ps*sizeof(int));
	  for(i=0; i<ps; i++) *(ns_i+i) = 0;

	  for(i=0; i<ps; i++){
		for(j=0; j<z; j++){

			for(l=0; l<n*n; l++) *(clusters+l) = 0;
			llenar(red, n, *(linspace_prob+i));
			hoshen(red, n, clusters, respuesta);
			cant_clusters = clasificar(clusters, cant_clusters, tamanios, n);
			for(k=0; k<smax; k++){
				*(ns_iter+k) += *(tamanios+smax+k);
			}
		}
//		printf("imprimo vector tamanios\n");
//		imprimir(tamanios, n, n);
		for(j=0; j<smax; j++){
			*(ns_prob+smax*i+j) = (double)(*(ns_iter+j))/(double)z;
//			printf("valores promediados en posicion j= %d da %f\n", j, *(ns_prob+smax*i+j));
//			printf("dividi %f con %f\n", (double)(*(ns_iter+j)), (double)z);
		}
		printf("nueva probabilidad = %d/100\n", i);
	  }
	  for(i=0; i<smax; i++){
		for(j=0; j<ps; j++){
//			printf("estoy por guardar ns_i por vez %d de 100\n", j);
			*(ns_i+j) = *(ns_prob+i+j*smax);
			printf("ns_i+j guardado = %d\n", *(ns_i+j));
		}
//		printf("estoy por calcular indice_max por vez %d/%d\n", i, smax);
		indice = indice_max(ns_i, ps);
		printf("indice = %d\n", indice);
	  	*(p_max+i)= *(linspace_prob+indice);
//		printf("prob exportada = %f\n", *(linspace_prob+indice));
	  }
	  exportar_vector(p_max, smax, "pmax.txt");
	  exportar_matriz(ns_prob, smax, ps);
  }

  if(n==33){
	  clusters_percolantes = 0;
	  smax = 15; // cantidad de s usamos
	// Linspace fino alrededor de pc
	  paso = 0.2/ps; // voy a ir de 0.4 a 0.6
	  *linspace_prob = 0.45; // valor inicial
	  for(i=1; i<ps; i++){
		*(linspace_prob + i) = *(linspace_prob + i - 1) + paso;
	  }

	  for(i=0; i<n*n; i++) *(tamanios+i) = 0;	  // inicializo tamaños
	  cant_clusters = 0.0;

	  ns_iter = (int *)malloc(n*n*sizeof(int));	// vector donde acumulo los ns para luego dividir por z
	  for(i=0; i<n*n; i++) *(ns_iter+i) = 0;

	  ns_prob = (double *)malloc(smax*ps*sizeof(double)); // aquí guardo los vectores ns para cada probabilidad del intervalo
	  for(i=0; i<smax*ps; i++) *(ns_prob+i) = 0.0;

	  m2 = (double *)malloc(ps*sizeof(double));
          for(l=0; l<ps; l++) *(m2+l) = 0.0;
	  
	  for(i=0; i<ps; i++){
	  for(l=0; l<n*n; l++) *(ns_iter+l) = 0;
		for(j=0; j<z; j++){

			for(l=0; l<n*n; l++) *(clusters+l) = 0;
			for(l=0; i<n; i++) *(respuesta+i)=0;
			llenar(red, n, *(linspace_prob+i));
			clusters_percolantes = percolantes(red, n, respuesta);
			hoshen(red, n, clusters, respuesta); //falta discriminar los que son percolantes
			cant_clusters = clasificar(clusters, cant_clusters, tamanios, n);
			for(k=0; k<n*n; k++){
				*(ns_iter+k) += *(tamanios+k);
			}
		}
		for(j=1; j<n*n; j++){
			*(m2+i) += (double)(*(ns_iter+j))/(double)(z)*j*j;
			printf("para p = %f y s = %d, m2 = %f\n", *(linspace_prob+i),j, *(m2+i));
		}
	  printf("nueva probabilidad %d/100\n", i);
	  }
	  exportar_vector(m2, ps, "m2.txt");



  }

  time(&t_final);
  printf("Tiempo transcurrido en segundos: %.0f\n", difftime(t_final, t_inicial));
  return 0;
}


int hoshen(int *red,int n, int *clusters, int *respuesta)
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


  corregir_etiqueta(red,clase,clusters,n, respuesta);

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

void corregir_etiqueta(int *red, int *clase, int *clusters, int n, int *respuesta){
	int i,j,s,ok;
	//printf("actua corregir etiqueta\n");	
	for(i=0; i<n*n; i++){
		s = *(red+i);
		while(*(clase+s)<0)
			s = - *(clase+s);
		*(red+i) = s;
		ok = 1;
		for(j=0; j<n; j++){
			if(s==*(respuesta+j)){
				ok = 0;
			}
		}
		if(ok) *(clusters+s)+=1;
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


int percolantes(int *red, int n, int *respuesta){
	int *primerafila, *ultimafila, i, j, jj, out;
	primerafila = (int *)malloc(n*n*sizeof(int));
	ultimafila = (int *)malloc(n*n*sizeof(int));
	out = 0;

	for(i=0; i<n*n; i++){
		*(primerafila+i) = 0;
		*(ultimafila+i) = 0;
	}
	for(i=0; i<n; i++){
		j = *(red+i);
		jj= *(red + n*(n-1) +i);
		*(primerafila+j)+=1;
		*(ultimafila + jj)+=1;
	}
	for(i=2; i<n*n; i++){
		j= *(primerafila+i);
		jj= *(ultimafila+i);
		if(j*jj){
			*(respuesta+out) = i;
			out ++;
		}
	}
	return out;
}

int indice_max(int *ns_i, int ps){
	int indice, valor, i;
	valor = *ns_i;
	for(i=1; i<ps; i++){
		if(*(ns_i+i)>valor){
			valor = *(ns_i+i);
			indice = i;
		}
	}
	return indice;
}



float promedio_int(int *vector, int n){
	int i;
	double res;
	res = 0.0;
	for(i=0; i<n; i++){
		res += (double)(*(vector + i));
	}
	res = res/(double)n;
	return (float)res;
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

void exportar_probabilidades(float prob, char *file){
	FILE *fp;
//	fp = fopen("L4.txt", "a");
	fp = fopen(file, "a");
	fprintf(fp, "%f\n", prob);
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
    	fprintf(fp, "%g\n     ", *(vector+i));
	fclose(fp);

}

void exportar_matriz(double *matriz, int n, int m){
	int i,j;
	FILE *fp;
	fp = fopen("ns_prob.txt", "a");
	for(i = 0; i< n; i++){
		for(j=0; j<m-1; j++){
			fprintf(fp, "%f\t", *(matriz+n*i+j));		
		}
		fprintf(fp, "%f\n", *(matriz+n*i+j));
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

}


