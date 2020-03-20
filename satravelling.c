#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>
#include <unistd.h>
#include <math.h>


double initial_time;
double clearcache [30000000];
float** matrix_distances;
int * path;
int cities;


void clearCache () {
    int i;
    for (i = 0; i < 30000000; ++i)
        clearcache[i] = i;
}


void start (void) {
    double time = omp_get_wtime();
    initial_time = time * 1000000;
}

double stop() {
    double time = omp_get_wtime();
    double final_time = time * 1000000;

    return final_time - initial_time;
}	


//calcula as permutações aleatórias
void randperm(int* perm,int N){
	int i,j,tmp;
    for(i=0;i<N;i++)
    	perm[i] = i;

    for(i=0;i<N;i++){
    	j=rand()% (	N-i) + i;
    	tmp = perm[j];
    	perm[j] = perm [i];
    	perm[i] = tmp;
    }
}

//calcula custos do caminho calculado
float calculate_cost(int* path,int N){
	int i,cost;
	cost = matrix_distances[path[N-1]][path[0]];

	for(i=0;i<N;i++)
		cost += matrix_distances[path[i]][path[i+1]];
	return cost;
}

//calcula distancia entre cidades
void find_distances(int* x, int* y,int N){
	int i,j;
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			matrix_distances[i][j] = sqrt(pow(x[i]-x[j],2) + pow(y[i]-y[j],2));
}

void initialize_matrix(){
	int i,j;

    matrix_distances = (float**) malloc(sizeof(float*) * cities );
	path = (int*) malloc(sizeof(int) * cities);
	

	for (i =0;i<cities;i++){
		
		path[i] = -1;
		matrix_distances[i] =malloc(sizeof(float) * cities);
		
		
	}
	for(i=0;i<cities;i++){
	  for(j=0;j<cities;j++){
		matrix_distances[i][j] = 0;
	  }
	}
	

}

float tspAnnealing(float** dist, int* path, int N, int iter, int num_proc,float temperatura){

	int i,c,previous,next1,next2,priv_path[N],tmp;
	
	float cost,delta,newCost,random;
	delta = 0.0;

	cost = calculate_cost(path,N);

	#pragma omp parallel num_threads(num_proc)
	{
	newCost = cost;

	for (i = 0; i < N; i++)
	{

		priv_path[i] = path[i];
	}

	i=0;
	while(i<iter){
		c= (rand() % N);
        
		if(c==0){
			previous = N-1;
			next1=1;
			next2=2;
		}
		else{
			previous = c-1;
			next1=(c+1) % N;
			next2 = (c+2) % N;
		}
         
		delta = dist[priv_path[previous]][next1] + dist[priv_path[c]][priv_path[next2]] - dist[priv_path[previous]][priv_path[c]] - dist[priv_path[next1]][priv_path[next2]];
        
        random = (float)((double)rand()/(double)(RAND_MAX)) * 1; // nr random entre 0 e 1 para ver se aceita o caminho ou não
		if(delta < 0 || (exp(-delta/temperatura))>random){
			tmp = priv_path[c];
			priv_path[c] = priv_path[next1];
			priv_path[next1] = tmp;
			newCost += delta;
			
		}
			if (delta<0) i=0;
		else i++;
		temperatura = 0.999*temperatura;
	}


	#pragma omp critical
		{
	if(newCost < cost){
		cost = newCost;	

		for(i=0;i<iter;i++)
			path [i] = priv_path [i];
	}
       }
    }
	return cost ;
}


int main(int argc, char const *argv[])
{

	int i;
	float cost, tspCost;

	srand((unsigned) time(NULL));

	cities = atoi(argv[1]);
	int num_proc = atoi(argv[2]);
	int iter = atoi(argv[3]);
	int temp = atof(argv[4]);

    int* x_pos = malloc (cities * sizeof (int)) ;
    int* y_pos = malloc (cities * sizeof (int)) ;

    //gera coordenadas para cada cidade num quadrado de lado 50
     for ( i = 0; i < cities; i++ ) {
		x_pos [i] = rand () % 50;
		y_pos [i] = rand () % 50;
		
	}
	for ( i = 0; i < cities; i++ ) {
		//printf("%f",y_pos[i]);
	}
    
    initialize_matrix();
  
    find_distances(x_pos,y_pos,cities);

    randperm(path,cities);

    cost = calculate_cost(path,cities);
    printf("Custo Inicial :%f\n",cost);
     
     /*
    printf("Distancias:\n");
	for(i = 0; i < cities; i++){
		for(int j = 0; j < cities; j++){
			printf("%f ", matrix_distances[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	*/

    clearCache();

    start();

    tspCost = tspAnnealing(matrix_distances,path,cities,iter,num_proc,temp);
    printf("Tempo Simulated Annealing:%f\n",stop());

    printf("Custo Ann  : %f\n", tspCost);

    for(i = 0; i < cities; i++)
		printf("Permutações Ann: %d\n", path[i]);

    return 0;
}
