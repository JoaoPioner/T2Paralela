#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define TAREFAS 100

int checkQueen(int **queens, int linha, int col, int n);
void play(int **queens, int col, int n, int *sol, int *maxQueens, int *count, int x, int y);


//MAIN
int main(int argc, char **argv){

    int numSlave = 3; // numero de escravos
    int my_rank;
    int proc_n;
    //linha de comando (np)
    int message[2];
    int saco[2*TAREFAS];
    int resposta = 0;
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
        int index = 0;
        while (index<2*TAREFAS) {
            for (size_t i = 0; i < 10; i++) {
                for (size_t j = 0; j < 10; j++) {
                    saco[index] = i;
                    index++;
                    saco[index]= j;
                    index++;
                }
            }   
        }

        index = 0;
        for (size_t i = 1; i <= numSlave; i++)//tem que ser i até n escravos
        {
            message[0] = saco[index];
            message[1] = saco[index+1];
            index+=2;
            //mandar para os escravos
            MPI_Send(message, 2, MPI_INT, i, i, MPI_COMM_WORLD);
            
        }

        int accKill = 0;
        int pos = index; // pos = ultima posicao do saco
        while(accKill < numSlave){
            MPI_Recv(&sol, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            resposta += sol;// verificar a linha
            if(pos < 2*TAREFAS){
                message[0] = pos;
                message[1] = pos+1;
                MPI_Send(message, 2, MPI_INT, status.MPI_SOURCE, status.MPI_SOURCE, MPI_COMM_WORLD);
            } else {
                accKill++;
            }
        }
        printf("Rank: %d Message %d", my_rank, message);
    } else { //ESCRAVO
        //MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status)
        //recebe o tabuleiro e executa o metodo play
        MPI_Recv(message, 2, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        play(queens, 0, n, &sol, &maxQueens, &count, message[0], message[1]);
        MPI_Send(&sol, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

   MPI_Finalize();
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
void play(int **queens, int col, int n, int *sol, int *maxQueens, int *count, int x, int y){

    if (col == n){
        return ;
    }
    for(int i = 0; i < n; ++i){
        if (col == y && y<n-1){
            col++;
        }
        if(i == x){
            continue;
        }
        if(!checkQueen(queens, i, col, n)){
            queens[i][col] = 1;
            ++*count;
            play(queens, col+1, n, sol, maxQueens, count, x, y);

            if(*count > *maxQueens){
                *maxQueens = *count;
            }
            --*count;
            queens[i][col] = 0;
           
        }
    }

}
