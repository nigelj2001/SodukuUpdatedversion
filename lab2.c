#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lab2.h"


void *checkR(void* param);
void *checkC(void* param);
void *checkB(void* param);


int** read_board_from_file(char* filename){
    FILE *fp = NULL;
    int** sudoku_board = (int**)malloc(sizeof(int*)*ROW_SIZE);
    for(int row = 0; row < ROW_SIZE; row++){
	    sudoku_board[row] = (int*)malloc(sizeof(int)*COL_SIZE);
    }

    fp = fopen(filename,"r");
    for(int i = 0; i < ROW_SIZE; i++) {
        for(int j = 0; j < COL_SIZE; j++) {
            fscanf(fp, "%d%*c", &sudoku_board[i][j]);
        }
    }
    fclose(fp);
    return sudoku_board;
}


int is_board_valid(){
    pthread_t* tid; 
    pthread_attr_t attr;
    int thread_index = 0;
    worker_validation = (int*) malloc(sizeof(int) * THREADS_NUM);
    tid = (pthread_t*) malloc(sizeof(int) * THREADS_NUM);

    for(int i = 0; i < ROW_SIZE; i++)
    {
        for(int j = 0; j < COL_SIZE; j++)
        {
            if(i % 3 == 0 && j % 3 == 0){
                param_struct * checkboxes_T = (param_struct*) malloc(sizeof(param_struct));
                checkboxes_T->starting_row = i;
                checkboxes_T->starting_col = j;
                pthread_create(&tid[thread_index++], NULL, checkB, checkboxes_T);
            }

            if(i==0){
                param_struct * checkcolumns_T = (param_struct*) malloc(sizeof(param_struct));
                checkcolumns_T->starting_row = i;
                checkcolumns_T->starting_col = j;
                pthread_create(&tid[thread_index++], NULL, checkC,checkcolumns_T);
            }

            if(j==0){
                param_struct * checkrows_T = (param_struct*) malloc(sizeof(param_struct));
                checkrows_T->starting_row = i;
                checkrows_T->starting_col = j;
                pthread_create(&tid[thread_index++], NULL, checkR, checkrows_T);
            }

        }
    }

    for(int n = 0; n < THREADS_NUM; n++){
        pthread_join(tid[i], NULL);
        if(worker_validation[i] == 0){
            return 0;
        }
    }

    free(worker_validation);
    free(tid);
    return 1;
}

void *checkR(void * params){
    param_struct * param = (param_struct*) params;
    int verifyArr[9] = {0};
    int row = param -> starting_row;
    int col = param -> starting_col;

	int i;
	for (i = 0; i < 9; i++){
		int num = sudoku_board[row][i];
		if (num < 1 || num > 9 || verifyArr[num - 1] == 1) {
			pthread_exit(NULL);
		} else {
			verifyArr[num - 1] = 1;
		}
	}
	worker_validation[9 + row] = 1;
	pthread_exit(NULL);
}


void *checkC(void* params){
    param_struct *p = (param_struct*) params;
    int validation_array[9] = {0};
    int row = p -> starting_row;
    int col = p -> starting_col;

    for(int i = 0; i< 9; i++){
        int num = sudoku_board[i][col];
        if (num > 9 || num < 1 || validation_array[num - 1]==1)
        {
            pthread_exit(NULL);
        }
        else{
            validation_array[num - 1] = 1;
        }
    }

    worker_validation[18 + col] = 1;
    pthread_exit(NULL);
}

void *checkB(void* params){
    param_struct *p = (param_struct*) params;
    int validation_array[9] = {0};
    int row = p -> starting_row;
    int col = p -> starting_col;

    for(int i = row; i < row + 3; i++){
        for(int j = col; j < col + 3; j++){
            int num = sudoku_board[i][j];
            if (num > 9 || num < 1 || validation_array[num - 1]==1)
            {
                pthread_exit(NULL);
            }
            else{
                validation_array[num - 1] = 1;
            }
        }
    }
    worker_validation[row + col / 3] = 1;
    pthread_exit(NULL);

}
