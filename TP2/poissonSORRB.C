#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

float** w;

//número de pontos da grelha
int N;

void initialize_matrices(){
    
    int i, j;
    
    w = (float**) malloc(sizeof(float*) * N);
    
    // Preencher a matriz com 100 nos limites superior e laterais e 50 nos interiores
        for(i = 0; i < N-1; i++){
          
            w[i] = (float*) malloc(sizeof(float) * N);

            for(j = 0; j < N; j++){ 
            
                if(i == 0 || j == 0 || j == N-1) //fronteiras superior e laterais

                    w[i][j] = 100;
                
                 else  w[i][j] = 50; //valores iniciais dos pontos interiores   
           }
    }

    w[i] = (float*) malloc(sizeof(float) * N);
    
    // fronteira de baixo preenchida a 0;
    for(j = 0; j < N; j++){ 
      
        w[i][j] = 0;
    }
    
}

void print_matrix(){
    int i, j;

    // Dar print da matriz w
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            printf("%f ", w[i][j]);
        }
        printf("\n");
    }
}

void free_matrices(){

    free(w);
}


int main(int argc, char* argv[]){

    if(argc != 2){
        
        printf ("Usage : ./ poissonSORRB <nr pontos da grelha>\n");
         _exit(0);
    }

    int N = atoi(argv[1]);
    int i, j, p , iter;

    // Preparar as matrizes para aplicar o algoritmo
    initialize_matrices();
    
    // parâmetro de relaxamento
    p=2/(1+sin(M_PI/(N-1)));

    // COMO É QUE SEI O VALOR INICIAL de tol
    float tol;
    float diff = tol + 1;

    printf("N: %d,P: %d, TOL: %f, DIFF: %f\n", N,p,tol, diff);

    iter = 0;    
    
    while(diff > tol){
        //cópia da ultima iteração. MAS O QUE É O u?
        //u=w 
      printf("olaaa");
        // 
        for(i = 2; i < N-1; i++){
            for(j= 2+(i%2); j < N-1; j++){
                w[i][j]=(1-p)* w[i][j]+p*(w[i-1][j]+w[i][j-1]+w[i][j+1]+w[i+1][j])/4;
            }
        }

        for(i = 2; i < N-1; i++){
            
            for(j= 2+((i+1)%2); j < N-1; j+=2){
                 w[i][j]=(1-p)* w[i][j]+p*(w[i-1][j]+w[i][j-1]+w[i][j+1]+w[i+1][j])/4;
            }
        }

        // Aumentar o número de iterações
        iter++;

        // Buscar o maior valor do máximo absoluto da diferença entre w-u
        //diff=max(max(abs(w-u)));
    }
    
    print_matrix();

    printf("Número de Iterações: %d\n", iter);

    // Free das matrizes
    free_matrices();
}