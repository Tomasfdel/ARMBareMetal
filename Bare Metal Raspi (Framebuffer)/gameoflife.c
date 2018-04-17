#include <ctype.h>
#include <stdint.h>
#include "gameoflife.h"
#include "mailbox.h"
#define RGB32(red, green, blue) (red | (green << 8) | (blue << 16) | 0xFF000000)

uint32_t readMB0 (MB0_CHANNEL channel)
{
  uint32_t response = 0; 
  while (1) {
    while (*MB0_STATUS & MAIL_EMPTY) {};
    response = *MB0_READ;
    if ((response & CHANNEL_MASK) == channel) {
      return (response & (~CHANNEL_MASK));
    }
  }
}

void writeMB0 (fb_info_t * data, MB0_CHANNEL channel)
{
  uint32_t intData = (uint32_t) data | BUFF_ADDR_MASK | channel;
  while (*MB0_STATUS & MAIL_FULL) {};
  *MB0_WRITE = intData;
}

const int HDIR[] = {-1, -1, -1, 0, 0, 1, 1, 1};
const int WDIR[] = {-1, 0, 1, -1, 1, -1, 0, 1};
fb_info_t fbInfo;
volatile uint32_t * pix;

char T[2][H+2][W+2];

void ingresarTablero(){
	int i;
	for(i = 0; i < sizeof(coor)/sizeof(int); i += 2){
		T[0][coor[i+1]][coor[i]] = 1;
	}
}

void initTablero(){
	int i, j;
	for(i = 0; i < H + 2; ++i)
		for(j = 0; j < W + 2; ++j)
			T[0][i][j] = T[1][i][j] = 0;
			
	ingresarTablero();
}


void setCasilla(int x, int y, int R, int G, int B){
	int tamanioCasilla = fbInfo.vWidth/W;
	int offsetX = (x - 1) * tamanioCasilla;
	int offsetY = (y - 1) * tamanioCasilla;
	int i, j;
	for(i = 0; i < tamanioCasilla; i++)
		for(j = 0; j < tamanioCasilla; j++)
			pix[(offsetY + i) * fbInfo.vWidth + (offsetX + j)] = RGB32(R,G,B);
}

//Funcion implementada para testeo
void clearTablero(){
	int x, y;
	for (x = 1; x < W+1; x++) 
     		for(y = 1; y < H+1; y++)
			setCasilla(x, y, 0, 0, 0);
    
}

void printTablero(int act){
	int i, j;
	for(i = 1; i < H+1; ++i){
		for(j = 1; j < W+1; ++j){
			if(T[act][i][j])
				setCasilla(j, i, 255, 255, 255);
			else
				setCasilla(j, i, 0, 0, 0);
			}
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

			if(T[act][i][j] != T[sig][i][j]){
				if(T[sig][i][j])
					setCasilla(j, i, 255, 255, 255);
				else
					setCasilla(j, i, 0, 0, 0);
			}
		}
	}
}


void notmain(){
	//inicializamos el framebuffer  
	fbInfo.width  = 1920;
	fbInfo.height = 1080;
	fbInfo.vWidth = 1920;
	fbInfo.vHeight = 1080;
	fbInfo.pitch = 0;
	fbInfo.depth = 32;
	fbInfo.yOffset = 0;
	fbInfo.xOffset = 0;
	fbInfo.fb = 0;
	fbInfo.fbSize = 0;
	
	//mailbox negocia el tamanio del framebuffer,junto a otros parametros
	writeMB0(&fbInfo, FRAMEBUFFER);

	int x = readMB0(FRAMEBUFFER);

	pix = (volatile uint32_t *) (fbInfo.fb & (~BUFF_ADDR_MASK));


	int p = 0;

	initTablero();
	printTablero(0);
	while(1){
		evolTablero(p);
		p = (p ^ 1) & 1;
	}
}
