#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>
#include <unistd.h>
#include <limits.h>

#define TIME_RES 1000000
#define FLT_MAX 3.402823466e+38F /* max value */
	
double initial_time;
double clear_cache [30000000];
float** matrix_distances;
int * destino;
int cities;
int nodo;// calculado com a funcao rand

void clearCache (){
   int i;
   for(i=0; i<30000000;i++)
	clear_cache[i]=i;
}

void start(){
  double time= omp_get_wtime();
  initial_time= time* TIME_RES;

}

double stop(){
   double time = omp_get_wtime();
   double final = time * TIME_RES;

   return final - initial_time;
}


// funcao para alocar e inicializar a matriz de distancias e o array com as cidades
void initialize_matrix(){
	int i,j;

        matrix_distances = (float**) malloc(sizeof(float*) * cities );
	destino = (int*) malloc(sizeof(int) * cities);
	

	for (i =0;i<cities;i++){
		
		destino[i] = -1;
		  matrix_distances[i] =malloc(sizeof(float) * cities);
		//  matrix_distances[i] = 0;
		
	}
	for(i=0;i<cities;i++){
	  for(j=0;j<cities;j++){
		matrix_distances[i][j] = 0;
	  }
	}
	

}

// funcao que consoante as posicoes(x,y)  calcula as distancias entre as varias cidades
void find_distances(int* x, int* y, int n){
	int i,j;

	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
		    if(i!=j){
			matrix_distances[i][j] = sqrt(pow(x[i]-x[j],2) + pow(y[i]-y[j],2));
		    }
		}
	//printf("O valor de cada distancia : %f",  matrix_distances[i][j]);
	}
}

// funcao principal que calcula,sucessivamente  o caminho mais curto de uma cidade inicial ate a seguinte
// anda nao visitada
void greedy(int num_proc){

	int i,j,pos;
	float  min= FLT_MAX;
	float count=0;
	int iter= 0;

	i = nodo; // cidade inicial calculado pelo rand
	
      #pragma omp parallel num_threads(num_proc)
      {	
	while(iter<(cities-1)){

		for(j=0;j<cities;j++){
			if(matrix_distances[nodo][j]<min && (matrix_distances[nodo][j]!=0) && (destino[j]==-1)){
				min = matrix_distances[nodo][j];
				pos = j;
			}
				
		}

		destino[nodo] = pos;
		printf("Sou o nodo: %d \n", nodo);
		nodo = pos;
		iter++;
		count+=min ;
	 	min = FLT_MAX;

	}

	for(j=0;j<cities;j++){
		if(destino[j]<0){
			destino[j] = i;
			#pragma omp atomic 
			count+= matrix_distances[j][i];
			
		} 
	}
      }
 	printf("A distancia total e: %f\n", count);
}



int main(int argc, char** argv){
	

		
	srand((unsigned) time(NULL));
	cities=atoi(argv[1]);
	int num_proc = atoi(argv[2]);
	int* posX;
	int* posY;
	int j,i;
	nodo = rand()% cities;

	posX = (int*) malloc(sizeof(int) * cities);
	posY = (int*) malloc(sizeof(int)  *cities);

	for(i = 0; i<cities;i++){
		posX[i] = rand() % 50;
		posY[i] = rand() % 50;
		//printf("%d ",posY[i]);
        	//printf("%d ",posX[i]);
	}
	
	initialize_matrix();
	
	start();	
	find_distances(posX,posY,cities);
	/*
	for(i=0;i<cities;i++){
	   for(j=0;j<cities;j++){
              printf("%f ", matrix_distances[i][j]);
	      if(j== cities-1) printf("\n");
	   }
	}
	*/
	
	greedy(num_proc);
	double tempo = stop();
	printf("Demorou %f  segundos\n  ",tempo);

}
