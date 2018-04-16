#include <ctype.h>
#include <stdint.h>
#define H 10
#define W 20

const int HDIR[] = {-1, -1, -1, 0, 0, 1, 1, 1};
const int WDIR[] = {-1, 0, 1, -1, 1, -1, 0, 1};

char T[2][H+2][W+2];

void clear();
void printTablero(int);

typedef volatile struct {
 uint32_t DR;
 uint32_t RSR_ECR;
 uint8_t reserved1[0x10];
 const uint32_t FR;
 uint8_t reserved2[0x4];
 uint32_t LPR;
 uint32_t IBRD;
 uint32_t FBRD;
 uint32_t LCR_H;
 uint32_t CR;
 uint32_t IFLS;
 uint32_t IMSC;
 const uint32_t RIS;
 const uint32_t MIS;
 uint32_t ICR;
 uint32_t DMACR;
} pl011_T;
 
enum {
 RXFE = 0x10,
 TXFF = 0x20,
};
 
pl011_T * const UART0 = (pl011_T *)0x101f1000;

static void uart_puts(pl011_T *uart, char *s){
	while(*s != '\0') {
		uart->DR = (unsigned int)(*s);
		s++;
	}
}

static void uart_gets(pl011_T *uart, char *s){
	char* posicionActual = s;
	int lineend = 0;
	char c;
	while(!lineend){
		//Entra al if cuando el recieve FIFO no esta vacio
		if ((uart->FR & RXFE) == 0) {
			//Se queda esperando mientras el transmit FIFO este lleno
			while(uart->FR & TXFF)
				;
			c  = uart->DR;
			switch(c){
				//Carriage return
				case(13):
					lineend = 1;
					break;
				//Backspace
				case(8):
					if(posicionActual > s)
						--posicionActual;
					break;
				//Delete
				case(127):
					if(posicionActual > s)
						--posicionActual;
					break;
				default:
					*posicionActual = uart->DR = c;
					++posicionActual;
			}
		}
	}
	
	*posicionActual = 0;
}

static void uart_getint(pl011_T *uart, int* n){
	char s[9];
	uart_gets(UART0, s);
	*n = 0;
	int i;
	for(i = 0;;++i){
		if(s[i] == 0)
			break;
		(*n) *= 10;
		(*n) += s[i]-'0';
	}
}

/*
 * Press F to pay respects
static void uart_getc(pl011_T *uart, char *c){
	int read = 0;
	while(!read){
		if ((uart->FR & RXFE) == 0) {
			while(uart->FR & TXFF);
			*c  = uart->DR = uart->DR;
			read = 1;	
		}
	}
}*/

static void uart_getc(pl011_T *uart, char *c){
	char s[20];
	uart_gets(uart, s);
	*c = s[0];
}

static inline char upperchar(char c) {
	if((c >= 'a') && (c <= 'z')) {
		return c - 'a' + 'A';
	} else {
		return c;
	}
}

void ingresarCelda(){
	int x, y;
	while(1){
		uart_puts(UART0, "\nIngrese la coordenada horizontal: ");
		uart_getint(UART0, &x);
		if(x >= 1 && x <= W){
			uart_puts(UART0, "\nIngrese la coordenada vertical: ");
			uart_getint(UART0, &y);
			if(y >= 1 && y <= H){
				T[0][y][x] = (T[0][y][x] ^ 1) & 1;
				break;
			}
				
			else
				uart_puts(UART0, "\nERROR. Ingrese un valor valido entre 1 y el alto.\n\n");
				
			}
		
		else
			uart_puts(UART0, "\nERROR. Ingrese un valor valido entre 1 y el ancho.\n\n");
		
	}
}



void ingresarTablero(){
	char opcion, dummy;
	while(1){
		uart_puts(UART0, "\n¿Desea encender o apagar otra celda? (S/N) : ");
		uart_getc(UART0, &opcion);
		opcion = upperchar(opcion);
		if (opcion == 'S'){
			ingresarCelda();
			uart_puts(UART0, "\nEstado actual del tablero:\n"); 
			printTablero(0);			
		}
		else if (opcion == 'N')	
			break;
		else 
			uart_puts(UART0, "\nERROR. Ingrese una opcion valida.\n");
		
	}
	
	uart_puts(UART0, "\nEmpezando simulacion. Presione una tecla para continuar.\n");
	uart_getc(UART0, &dummy);
}

void initTablero(){
	int i, j;
	for(i = 0; i < H + 2; ++i)
		for(j = 0; j < W + 2; ++j)
			T[0][i][j] = 0;
			
	ingresarTablero();
}

void clear(){
	int i;
	for(i = 0; i < 10; ++i)
		uart_puts(UART0, "\n");
}

void printTablero(int act){
	clear();
	
	int i, j;
	for(i = 1; i < H+1; ++i){
		for(j = 1; j < W+1; ++j){
			if(T[act][i][j])
				uart_puts(UART0, "*");
			else
				uart_puts(UART0, " ");
			}
		
		uart_puts(UART0, "\n");
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

void c_entry(){
	
	initTablero();
	
	int i;
	int p = 0;
	char dummy;
	
	for(i=0; i<100; ++i){
		evolTablero(p);
		p = (p ^ 1) & 1;
		printTablero(p);
		uart_getc(UART0, &dummy);
	}
}
