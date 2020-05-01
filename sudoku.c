#include <pthread.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define num_hilos 27

//Variables de validéz en la sección crítica.
int valido[num_hilos];
pthread_mutex_t  mutex;
int acabaron;
int sudoku[9][9];

//Estructura 

typedef struct {
	int row;
	int col;
	int p_index;
}parametros;


int sudoku[9][9];


void *colVal(void* param){
	parametros *params = (parametros*) param;
	int col = params ->col;
	int ind = params ->p_index;
	free(param);
	int num;
	int arre[9] = {0};
	for(int i = 0 ; i < 9 ; i++){
		//printf("%i ",ind);
		num = sudoku[i][col];
		if(num > 0 && num < 10 && arre[num-1] == 0){
			arre[num-1] = 1;
		}else{
			pthread_mutex_lock(&mutex);
			acabaron++;
			pthread_mutex_unlock(&mutex);
			pthread_exit(NULL);
		}
	}
	valido[col] = 1;
	pthread_mutex_lock(&mutex);
	acabaron++;
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}


void *filVal(void* param){
	parametros *params = (parametros*) param;
	int fil = params -> row;
	int ind = params ->p_index;
	free(param);
	int num;
	int arre[9] = {0};
	for(int i = 0 ; i < 9 ; i++){
		//printf("%i ",ind);
		num = sudoku[fil][i];
		if(num > 0 && num < 10 && arre[num-1] == 0){
			arre[num-1] = 1;
		}else{
			valido[9 + fil] = 0;
			pthread_mutex_lock(&mutex);
			acabaron++;
			pthread_mutex_unlock(&mutex);
			pthread_exit(NULL);
		}
	}
	valido[9 + fil] = 1;
	pthread_mutex_lock(&mutex);
	acabaron++;
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

void *secVal(void* param){
	parametros *params = (parametros*) param;
	int fil = params ->row;
	int col = params ->col;
	int ind = params ->p_index;
	free(param);
	int num;
	int arre[9] = {0};
	for(int i = 0 ; i < 3 ; i++){
		for(int j = 0 ; j < 3 ; j++){
			num = sudoku[fil + i][col + j];
			//printf("%i ",ind);
			if(num > 0 && num < 10 && arre[num-1] == 0){
				arre[num-1] = 1;
			}else{
				valido[18 + fil + col/3] = 0;
				pthread_mutex_lock(&mutex);
				acabaron++;
				pthread_mutex_unlock(&mutex);
				pthread_exit(NULL);
			}

		}
	
	}
	valido[18 + fil + col/3] = 1;	
	pthread_mutex_lock(&mutex);
	acabaron++;
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	char aux[6];
	for(int i = 0 ; i < 9 ; i++){
		for(int j = 0 ; j < 9 ; j++){
			scanf("%s",aux);
			sudoku[i][j] = atoi(aux);
			printf("%d " , sudoku[i][j]);
			if(j == 2 || j == 5){
				printf("| ");
			}
		}
		printf("\n");
		if(i == 2 || i == 5){
			printf("---------------------\n");
		}
	}
	pthread_t hilos[num_hilos];
	int numHilo = 0;
	for(int i = 0 ; i < 9 ; i++){
		for(int j = 0 ; j < 9 ; j++){
			if(i == 0){
				//Manda columna j
				parametros *params = (parametros *)malloc(sizeof(parametros));
				params -> row = i;
				params -> col = j;
				params -> p_index = numHilo;
				pthread_create(&hilos[numHilo++],NULL,colVal,params);
			}
			if(j == 0){
				//Manda fila i 
				parametros *params = (parametros *)malloc(sizeof(parametros));
				params -> row = i;
				params -> col = j;
				params -> p_index = numHilo;
				pthread_create(&hilos[numHilo++],NULL,filVal,params);
			}
			if(i%3 == 0 && j%3 == 0){
				//manda sección i,j
				parametros *params = (parametros *)malloc(sizeof(parametros));
				params -> row = i;
				params -> col = j;
				params -> p_index = numHilo;
				pthread_create(&hilos[numHilo++],NULL,secVal,params);
			}
		}
	} 
	//for(int i = 0; i < num_hilos; i++){
	//	pthread_join(hilos[i],NULL);
	//}
	while(acabaron < 27){
		sleep(0.1);
	}

	for(int i = 0; i < num_hilos; i++){
		if(valido[i] == 0){
			printf("\nSudoku no es válido\n\n");
			return 1;
		}
		//printf("%d " , valido[i]);

	}
	printf("\nSudoku SI es válido\n\n");
	return 1;

}

