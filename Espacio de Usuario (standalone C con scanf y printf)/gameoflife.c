#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define H 10
#define W 20

const int HDIR[] = {-1, -1, -1, 0, 0, 1, 1, 1};
const int WDIR[] = {-1, 0, 1, -1, 1, -1, 0, 1};

char T[2][H+2][W+2];

void clearDeMentirita();
void printTablero(int);


	
void ingresarCelda(){
	int x, y;
	while(1){
		printf("Ingrese la coordenada horizontal: ");
		scanf("%d", &x);
		if(x >= 1 && x <= W){
			printf("Ingrese la coordenada vertical: ");
			scanf("%d", &y);
			if(y >= 1 && y <= H){
				T[0][y][x] = (T[0][y][x] ^ 1) & 1;
				break;
			}
				
			else
				printf("\nERROR. Ingrese un valor valido entre 1 y el alto.\n\n");
				
			}
		
		else
			printf("\nERROR. Ingrese un valor valido entre 1 y el ancho.\n\n");
		
	}
}



void ingresarTablero(){
	char opcion;
	while(1){
		printf("¿Desea encender o apagar otra celda? (S/N) : ");
		scanf("%c", &opcion);
		opcion = toupper(opcion);
		if (opcion == 'S'){
			ingresarCelda();
			getchar();
			printf("Estado actual del tablero:\n"); 
			printTablero(0);			
		}
		else if (opcion == 'N')	
			break;
		else 
			printf("\nERROR. Ingrese una opcion valida.\n");
		
	}
	
	printf("Empezando simulacion. Presione una tecla para continuar.\n");
	getchar();
	getchar();
}

void initTablero(){
	int i, j;
	for(i = 0; i < H + 2; ++i)
		for(j = 0; j < W + 2; ++j)
			T[0][i][j] = 0;
			
	ingresarTablero();
}

void clearDeMentirita(){
	int i;
	for(i = 0; i < 10; ++i)
		putchar('\n');
}

void printTablero(int act){
	clearDeMentirita();
	
	int i, j;
	for(i = 1; i < H+1; ++i){
		for(j = 1; j < W+1; ++j){
			if(T[act][i][j])
				putchar('*');
			else
				putchar(' ');
			}
		
		putchar('\n');
	}	
}

//p es el actual, p+1 el siguiente
void evolTablero(int p){
	int act = p;
	int sig = (p ^ 1) & 1;
	
	int i, j, k;
	for(i = 1; i < H+1; ++i){
		for(j = 1; j < W+1; ++j){
			int around = 0;
			for(k = 0; k < 8; ++k)
				around += T[act][ i + HDIR[k] ][ j + WDIR[k] ];
			
			if(T[act][i][j] && around <= 3 && around >= 2)
				T[sig][i][j] = 1;
			
			else{
				if(around == 3)
					T[sig][i][j] = 1;
				
				else
					T[sig][i][j] = 0;
			}
		}
	}
}

int main(){
	
	initTablero();
	
	int i;
	int p = 0;
	
	
	for(i=0; i<100; ++i){
		evolTablero(p);
		p = (p ^ 1) & 1;
		printTablero(p);
		getchar();
	}
	
	return 0;
}
