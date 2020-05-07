#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

//#define M_PI 3.14159265358979323846264338327950288

double** w;
double** u;

//número de pontos da grelha
int N;

void initialize_matrices(){
    int i, j;

    u = (double**) malloc(sizeof(double*) * N);
    w = (double**) malloc(sizeof(double*) * N);

    // Preencher a matriz com 100 nos limites inferiores e laterais e 50 nos interiores
    for(i = 0; i < N; i++){

        u[i] = (double*) malloc(sizeof(double) * N);
        w[i] = (double*) malloc(sizeof(double) * N);

        for(j = 0; j < N; j++){

            u[i][j] = 0; //inicializar a matriz u a zero

            if(i == N-1)
                w[i][j] = 0; // fronteira de cima preenchida a 0;

            else if(i == 0 || j == 0 || j == N-1)
                w[i][j] = 100; //fronteiras inferiores e laterais

            else
                w[i][j] = 50; //valores iniciais dos pontos interiores 
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

int main(int argc, char* argv[]){

    if(argc != 2){
        printf ("Usage : ./ poissonSORRB <nr pontos da grelha>\n");
    }

    N = atoi(argv[1]);
    
    int i, j;

    // Preparar as matrizes para aplicar o algoritmo
    initialize_matrices();

    print_matrix();

    // parâmetro de relaxamento
    double p = 2/(1 + sin(M_PI/(N-1)));
    
    double tol = 1/(double)(N*N);
    
    double diff = (tol + 1);

    printf("N: %d,P: %f, TOL: %f, DIFF: %f\n", N,p,tol, diff);
    
    int iter = 0;

    while(diff > tol){
        iguala(u,w);

        for(i = 1; i < N-1; i++){
            for(j = 1 + (i%2); j < N-1; j += 2){
                w[i][j] = (1-p) * w[i][j] + p * (w[i-1][j] + w[i][j-1] + w[i][j+1] + w[i+1][j])/4;
            }
        }

        for(i = 1; i < N-1; i++){
            for(j = 1 + ((i+1)%2); j < N-1; j += 2){
                w[i][j] = (1-p) * w[i][j] + p * (w[i-1][j] + w[i][j-1] + w[i][j+1] + w[i+1][j])/4;
            }
        }

        // Aumentar o número de iterações
        iter++;

        // Buscar ,em valor absoluto, o máximo da diferença das componentes em 2 iterações sucessivas
        //Em matlab o 1ºmax retorna um vetor linha com os maximos de cada linha da "matrix" e o segundo max retorna o valor maximo desse vetor
        //diff=max(max(abs(w-u)));
        diff = maximum(absol(diferenca(w,u)));
    }

    printf("Número de Iterações: %d\n", iter);

    // Free das matrizes
    free_matrices();

    return 0;
}