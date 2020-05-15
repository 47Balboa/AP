#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>

#define TIME_RES 1000

double** w;
double** u;
double initial_time;
double clearcache [30000000];
int iter;
double p;
double tol;
double diff;

//número de pontos da grelha
int N;

void clearCache (){

   int i;
   for(i=0; i<30000000;i++)
    clearcache[i]=i;
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

void initialize_matrices(){
    int i, j;

    u = (double**) malloc(sizeof(double*) * N);
    w = (double**) malloc(sizeof(double*) * N);
    
    // Preencher a matriz com 100 nos limites inferiores e laterais e 50 nos interiores
  for(i = 0; i < N; i++){
          
        w[i] = (double*) malloc(sizeof(double) * N);
        u[i] = (double*) malloc(sizeof(double) * N);

        for(j = 0; j < N; j++){ 

            u[i][j]=0; //Inicializar a matriz u a zeros
           
            if(i == N-1 || (j == 0 && i!=0) || (j == N-1 && i != 0)){ //fronteiras inferiores e laterais

                w[i][j] = 100;
                
            }else{

                if(i ==0){
                    w[i][j] = 0; //valores do limite superior
                }else {

                    w[i][j] = 50; //valores iniciais dos pontos interiores 
                }
            
            }
        }
    }
}

// Dar print da matriz w
void print_matrix(){

    int i, j;

    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            printf("%f ", w[i][j]);
       }
        printf("\n");
    }
}

void free_matrices(){
    free(u);
    free(w);
}

//funcao que calcula a diferenca entre 2 vetores e guarda o resultado noutro vetor
double** diferenca(double** a, double** b){

    double** result = (double**) malloc(N * sizeof(double));
    int i, j;

    for(i = 0; i < N; i++){
        result[i] = (double*) malloc(N * sizeof(double*));
        for(j = 0; j < N; j++){
            result[i][j] = a[i][j] - b[i][j];
        }
    }
    return result;
}

//funcao que retorna um vetor bi-dimensional com o valor absoluto de cada elemento
double** absol(double** vetor){

    int i, j;

    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            vetor[i][j] = fabs(vetor[i][j]);
        }
    }
    return vetor;
}
    

//funcao que retorna o maior elemento de um vetor
double maximum(double** vetor){

    double max = 0;
    int i, j;

    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            if(vetor[i][j] > max)
                max = vetor[i][j];
        }
    }
    return max;
}

void iguala(double **a, double**b){

    int i, j;

    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            a[i][j] = b[i][j];
        }
    }
}

void parallel(int threads){

    int i,j;
	
    while(diff > tol){
        iguala(u,w);

        #pragma omp parallel for private(i,j) schedule(static) num_threads(threads)
        
        for(i = 1; i < N-1; i++){
            
            for(j = 1 + (i%2); j < N-1; j += 2){

                #pragma omp atomic write
                w[i][j] = (1-p) * w[i][j] + p * (w[i-1][j] + w[i][j-1] + w[i][j+1] + w[i+1][j])/4;
                
            }
        }
         
        #pragma omp parallel for private(i,j) schedule(static) num_threads(threads)
        
        for(i = 1; i < N-1; i++){
            
            for(j = 1 + ((i+1)%2); j < N-1; j += 2){

                #pragma omp atomic write
                w[i][j] = (1-p) * w[i][j] + p * (w[i-1][j] + w[i][j-1] + w[i][j+1] + w[i+1][j])/4;                
            }
        }
        iter++;
        diff = maximum(absol(diferenca(w,u)));
    }
}

int main(int argc, char* argv[]){
	
    if(argc != 3){
        printf ("Usage : ./ poissonSORRB <nr pontos da grelha> <numero threads>\n");
        return 0;
    }

    N = atoi(argv[1]);

    int threads = atoi(argv[2]);
    
    int i, j;

    clearCache();

    // Preparar as matrizes para aplicar o algoritmo
    initialize_matrices();
   
    // print_matrix();

    // parâmetro de relaxamento
    p = 2/(1 + sin(M_PI/(N-1)));
    
    tol = 1/(double)(N*N);
    
    diff = (tol + 1);
   
    iter = 0;

    FILE* fp = fopen("results/resultados_poisson.csv","a");

    start();

    parallel(threads);

    double tempo = stop();
    
    //print_matrix();

    fprintf(fp,"%d,%d,%2f,%d,%2f\n", N, threads, tol, iter, tempo);
    
    fclose(fp);

    free_matrices();

    return 0;
}
