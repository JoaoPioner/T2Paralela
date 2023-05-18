#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define TAREFAS 100

int checkQueen(int **queens, int linha, int col, int n);
void play(int **queens, int col, int n, int *sol, int *maxQueens, int *count, int x, int y);


//MAIN
int main(){

    int my_rank;
    int proc_n;
    //linha de comando (np)
    int message[2];
    int saco[2*TAREFAS];
    MPI_Init(&argc , &argv);
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

    int n = 8, sol = 0, maxQueens = 0, count = 0, x = 0, y = 0;

    int **queens = (int**) malloc(n  * sizeof(int *));
    for(int i = 0; i < n; ++i)
            queens[i] = (int*) malloc(n  * sizeof(int));

    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            queens[i][j] = 0;
        }
    }

    if(my_rank == 0){ //MESTRE
        //alocando rainhas
        for (size_t z = 0; z < 2*TAREFAS; z=z+2) {
            for (size_t i = 0; i < n; i++) {
                for (size_t j = 0; j < n-1; j++) {
                    saco[z] = i;
                    saco[z+1] = j;
                }   
            }
        }
        int index = 0;
        for (size_t i = 0; i < TAREFAS; i++)
        {
            message[0] = saco[i];
            message[0] = saco[i+1];
            //mandar para os escravos
            MPI_Send(message, 2, MPI_INT, index+1, index+1, MPI_COMM_WORLD);
        }
        
    } else { //ESCRAVO
        //recebe o tabuleiro e executa o metodo play
        play(queens, 0, n, &sol, &maxQueens, &count);
    }
    printf("Rank: %d Message %d", my_rank, message);
    MPI_Finalize();    

    //imprime resultados
    printf("Maximo de rainhas: %d \n", maxQueens);
    printf("Numero de solucoes: %d \n", sol);


    return 0;
}

//testando se é possivel colocar rainha em determinada posição

int checkQueen(int **queens, int linha, int col, int n){

    int a = 0;

    if(queens[linha][col] == 1)
        return 1;
    {
        for(int i = linha, j = col; j >= 0 && i >= 0; --i, --j){
            if(queens[i][j] == 1){
                
                a = 1;
            }
        }


        if(!a){
            for(int i = linha, j = col; j >= 0 && i < n ; ++i, --j){
                if(queens[i][j] == 1){
                    
                    a = 1;
                }
            }
        }


        if(!a){
            for(int i = linha, j = col; j < n && i >= 0 ; --i, ++j){
                if(queens[i][j] == 1){
                    
                    a = 1;
                }
            }
        }


        if(!a){
            for(int i = linha, j = col; j < n && i < n ; ++i, ++j){
                if(queens[i][j] == 1){
                    
                    a = 1;
                }
            }
        }


        if(!a){
            for(int i = linha; i < n ; ++i){
                if(queens[linha][i] == 1){
                    
                    a = 1;
                }
            }
        }


        if(!a){
            for(int i = linha; i >= 0 ; --i){
                if(queens[linha][i] == 1){
                    
                    a = 1;
                }
            }
        }


        if(!a){
            for(int i = col; i < n ; ++i){
                if(queens[i][col] == 1){
                    
                    a = 1;
                }
            }
        }


        if(!a){
            for(int i = col; i >= 0 ; --i){
                if(queens[i][col] == 1){
                    
                    a = 1;
                }
            }
        }

    }

    return a;
}
//executa testes
void play(int **queens, int col, int n, int *sol, int *maxQueens, int *count){

    if (col == n){
        ++*sol;
        return;
    }
    for(int i = 0; i < n; ++i){
        if(!checkQueen(queens, i, col, n)){
            queens[i][col] = 1;
            ++*count;
            play(queens, col+1, n, sol, maxQueens, count);

            if(*count > *maxQueens){
                *maxQueens = *count;
            }
            --*count;
            queens[i][col] = 0;
           
        }
    }

}