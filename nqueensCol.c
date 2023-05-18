#include <stdio.h>
#include <stdlib.h>


int checkQueen(int **queens, int linha, int col, int n);
void play(int **queens, int col, int n, int *sol, int *maxQueens, int *count, int x, int y);


//MAIN
int main(){
    int n = 8, sol = 0, maxQueens = 0, count = 1;
    int x = 5;
    int y = 0;


    //alocando rainhas
    int **queens = (int**) malloc(n  * sizeof(int *));
    for(int i = 0; i < n; ++i)
        queens[i] = (int*) malloc(n  * sizeof(int));

    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            queens[i][j] = 0;
        }
    }   
    queens[x][y] = 1;
    play(queens, 0, n, &sol, &maxQueens, &count, x, y);

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

void printTabuleiro(int **queens, int n){
    for(int i = 0; i < n; ++i){
        printf("\n");
        for(int j = 0; j < n; ++j){
            printf("%d - ", queens[i][j]);
        }
    }
    printf("\n");
}

//executa testes
void play(int **queens, int col, int n, int *sol, int *maxQueens, int *count, int x, int y){

    if (col == n){
        printf("\nSolucao #%d\n", ++*sol);
        printTabuleiro(queens, n);
        return;
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