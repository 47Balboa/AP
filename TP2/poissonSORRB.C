#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>


#define M_PI 3.14159265358979323846264338327950288 

float** w;
float** u;

//número de pontos da grelha
int N;

void initialize_matrices(){
    
    int i, j;
    
    w = (float**) malloc(sizeof(float*) * N);
    u = (float**) malloc(sizeof(float*) * N);
   
    //inicializar a matriz u a zero
    for(i=0;i<N;i++){
        u[i] = (float*) malloc(sizeof(float) * N);
        for(j=0;j<N;j++){
            u[i][j]=0;
            printf("%f",u[i][j]);
        }
        
    }
     
    // Preencher a matriz com 100 nos limites inferiores e laterais e 50 nos interiores
    for(i = 0; i < N-1; i++){
          
        w[i] = (float*) malloc(sizeof(float) * N);

        for(j = 0; j < N; j++){ 
            
            if(i == 0 || j == 0 || j == N-1) //fronteiras inferiores e laterais

                w[i][j] = 100;
                
            else  w[i][j] = 50; //valores iniciais dos pontos interiores 
        }
    }

    w[i] = (float*) malloc(sizeof(float) * N);
    
    // fronteira de cima preenchida a 0;
    for(j = 0; j < N; j++){ 
      
        w[i][j] = 0;
        
    }
    
    printf("get fucked\n");
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

//funcao que calcula a diferenca entre 2 vetores e guarda o resultado noutro vetor
float** diferenca(float** a, float** b, int N){
    float** result = (float**)malloc(N* sizeof(float));
   
    for(int i=0;i<N;i++){
        result[i] = (float*)malloc(N*sizeof(float*));
    }

    for(int i= 0; i<N; i++){
        for(int j=0;j<N; j++){
            result[i][j] = 0;
        }
    }

    for(int i=0; i<N; i++){
        for(int j=0;j<N; j++){
            result[i][j] = a[i][j] - b[i][j];
        }
    }
    return result;
}

//funcao que retorna um vetor bi-dimensional com o valor absoluto de cada elemento
float** absol(float** vetor, int N){
    for(int i;i<N; i++){
        for(int j=0;j<N; j++){
            vetor[i][j] = fabs(vetor[i][j]);
        }
    }
    return vetor;
}

//funcao que retorna o maior elemento de um vetor
float maximum(float** vetor, int N){
    float max = 0;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(vetor[i][j]>max) max= vetor[i][j];
        }
    }
    return max;
}

void iguala(float **a, float**b,int N){
    int i,j;

    for(i =0;i<N;i++){
        for(j=0;j<N;j++){
            a[i][j] = b[i][j];
        }
    }

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
    
    print_matrix();
    // parâmetro de relaxamento
    p=2/(1+sin(M_PI/(N-1)));

    
    float tol = 1/ (N^2);
    float diff = tol + 1;

    printf("N: %d,P: %d, TOL: %f, DIFF: %f\n", N,p,tol, diff);

    iter = 0;    
    
    while(diff > tol){
        iguala(u,w,N);
        
        printf("olaaa");
        // 
        for(i = 1; i < N-1; i++){
            for(j= 1+ (i%2); j < N-1; j+=2){
                w[i][j]=(1-p)* w[i][j]+p*(w[i-1][j]+w[i][j-1]+w[i][j+1]+w[i+1][j])/4;
            }
        }

        for(i = 1; i < N-1; i++){
            
            for(j= 1+((i+1)%2); j < N-1; j+=2){
                 w[i][j]=(1-p)* w[i][j]+p*(w[i-1][j]+w[i][j-1]+w[i][j+1]+w[i+1][j])/4;
            }
        }

        // Aumentar o número de iterações
        iter++;

        // Buscar ,em valor absoluto, o máximo da diferença das componentes em 2 iterações sucessivas
        //Em matlab o 1ºmax retorna um vetor linha com os maximos de cada linha da "matrix" e o segundo max retorna o valor maximo desse vetor
        //diff=max(max(abs(w-u)));
        diff=maximum(absol(diferenca(w,u,N),N),N);
    }


    
    print_matrix();

    printf("Número de Iterações: %d\n", iter);

    // Free das matrizes
    free_matrices();
}