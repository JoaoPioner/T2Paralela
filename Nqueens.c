#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define TAREFAS 100

int checkQueen(int **queens, int linha, int col, int n);
void play(int **queens, int col, int n, int *sol, int *maxQueens, int *count, int x, int y);


//MAIN
int main(){

    int numSlave = 3;
    int my_rank;
    int proc_n;
    //linha de comando (np)
    int message[2];
    int respostas[2*TAREFAS];
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

        for (size_t i = 0; i < 2*TAREFAS; i+=2)//tem que ser i até n escravos
        {
            message[0] = saco[i];
            message[1] = saco[i+1];
            //mandar para os escravos
            MPI_Send(message, 2, MPI_INT, i, i, MPI_COMM_WORLD);
            
        }

        int accKill = 0;
        int pos = 0; // pos = ultima posicao do saco
        int aux = 0;// preciso mudar este nome
        while(accKill < numSlave){
            MPI_Recv(message, 2, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, status);
            respostas[aux] = message[0];
            respostas[aux+1] = message[1];
            aux+=2;
            if(pos < 2*TAREFAS){
                message[0] = pos;
                message[1] = pos+1;
                MPI_Send(message, 2, MPI_INT, status.MPI_SOURCE, status.MPI_SOURCE, MPI_COMM_WORLD);
            } else {
                accKill++;
                MPI_Send(0, 1, MPI_INT, status.MPI_SOURCE, "Kill", MPI_COMM_WORLD);// se o escravo receber kill ele vai direto para o finalize
            }
        }

        for ( i=0 ; i < TAREFAS ; i++)
        {
            // recebo mensagens de qualquer emissor e com qualquer etiqueta (TAG)

            MPI_Recv(message, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, status);  // recebo por ordem de chegada com any_source

            saco[status.MPI_SOURCE-1] = message;   // coloco mensagem no saco na posição do escravo emissor
        }
        
    } else { //ESCRAVO
        //MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status)
        //recebe o tabuleiro e executa o metodo play
        MPI_Recv(message, 2, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        message[0] = play(queens, 0, n, &sol, &maxQueens, &count, message[0], message[1]);
        MPI_Send(message[0], 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
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
int play(int **queens, int col, int n, int *sol, int *maxQueens, int *count, int x, int y){

    if (col == n){
        printf("\nSolucao #%d\n", ++*sol);
        printTabuleiro(queens, n);
        return *sol;
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
            *sol += play(queens, col+1, n, sol, maxQueens, count, x, y);

            if(*count > *maxQueens){
                *maxQueens = *count;
            }
            --*count;
            queens[i][col] = 0;
           
        }
    }
    if (col != n){
        return 0;
    }

}