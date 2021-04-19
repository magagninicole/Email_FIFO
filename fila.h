#ifndef FILA_H_INCLUDED
#define FILA_H_INCLUDED
#define MAX_STRING_SIZE 40
#define NUMBER_OF_STRING 5

typedef struct{
	char email[MAX_STRING_SIZE];
}EMAILS;

typedef struct{
	EMAILS dados[NUMBER_OF_STRING];
	int tam;
}FILA;

void inicializa(FILA *fila){
	for (int i = 0; i < NUMBER_OF_STRING; i++){
		for(int j = 0; j < MAX_STRING_SIZE; j++){
			fila->dados[i].email[j] = ' ';
		}
    }
	fila->tam = 0;
}

int entrada(FILA *fila, char item[]){
	if(fila->tam >= NUMBER_OF_STRING){
		return 0;
	}
	for(int j = 0; j < MAX_STRING_SIZE; j++){
		fila->dados[fila->tam].email[j] = item[j];
	}
	fila->tam++;
	return 1;
}

char* leitura(FILA *fila){
	char *lido = fila->dados[0].email;
	for (int i = 1; i < fila->tam; i++){
		for(int j = 0; j<MAX_STRING_SIZE; j++){
			fila->dados[i-1].email[j] = fila->dados[i].email[j];
		}
	}
	fila->tam--;
	return lido;
}

void imprime(FILA *fila){
        for (int i = 0; i < fila->tam; i++){
            printf("(%s) |---|", fila->dados[i].email);
	}
	printf("\n\n");
}


#endif // FILA_H_INCLUDED
