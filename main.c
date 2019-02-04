#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <windows.h>
#include <SDL2\SDL.h>
#include <SDL2\SDL_ttf.h>

bool quit = false;
size_t cols = 40;
size_t rows = 20;
char ** buffer;
int sw = 800;
int sh = 600;
int swd2 = 400;
int shd2 = 300;
int ppcx;
int ppcy;
char offset = 0x20;

SDL_Window * window;
SDL_Renderer * renderer;
SDL_Event event;
SDL_Surface * surface;
SDL_Texture * characters[('~' - ' ') + 1];
SDL_Color color;
SDL_Rect source;
SDL_Rect dest;
TTF_Font * font;
const char fontpath[] = "Proggy.ttf";

char * insults[5];



void cleanup(void){
	if(window != NULL){
		SDL_DestroyWindow(window);
	}
	if(renderer != NULL){
		SDL_DestroyRenderer(renderer);
	}
	if(font != NULL){
		TTF_CloseFont(font);
	}
	SDL_Quit();
	TTF_Quit();
}

int end(void){
	printf("Program has ended");
	return 0;
}

int fatalerror(const char * errormessage){
	printf("FATAL ERROR: %s\n", errormessage);
	return 1;	
}

void error(const char * errormessage){
	printf("ERROR: %s\n", errormessage);	
}

int createbuffer(void){
	buffer = (char **) malloc(sizeof(char *) * cols);
	if(buffer == NULL){
		return -1;
	}
	for(int i = 0; i < cols; i++){
		buffer[i] = (char *) malloc(sizeof(char) * rows);
		if(buffer[i] == NULL){
			return -1;
		}
	}
	return 0;
}

int resizebuffer(size_t newcols, size_t newrows){
	for(int i = 0; i < cols; i++){
		free(buffer[i]);
	}
	free(buffer);
	cols = newcols;
	rows = newrows;
	return createbuffer();
}
	
void ap_putchar(char putc, int x, int y){
	if(x < cols && y < rows){
		if(x < 0){
			x = (cols >> 1);
		}
		if(y < 0){
			y = (rows >> 1);
		}
		buffer[x][y] = putc;
	}else{
		error("Invalid index (putchar)");
		return;
	}
}

void ap_putstring(char * puts, size_t len, int x, int y){
	if(x < (int)cols && y < (int)rows){
		if(x < 0){
			if(len / 2 <= cols / 2)
				x = (cols / 2) - (len / 2);
		}
		
		if(y < 0)
			y = (rows >> 1);
		
		
		for(int i = 0; (i + x) < cols && i < len; i++)
			buffer[x + i][y] = puts[i];
			
	}else{
		error("Invalid index (putstring)");
		return;
	}
}

void ap_clear(void){
	for(int y = 0; y < rows; y++){
		for(int x = 0; x < cols; x++){
			buffer[x][y] = ' ';
		}
	}
}

int main(int argc, char ** argv){
	
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		return fatalerror("Could not initialize SDL!");
	}
	
	if(TTF_Init() < 0){
		return fatalerror("Could not initialize SDL_ttf");
	}
	
	window = SDL_CreateWindow("ASCIIPUNK", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, sw, sh, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if(window == NULL){
		return fatalerror("Could not create SDL window");
	}
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL){
		return fatalerror("Could not create SDL renderer");
	}
	
	font = TTF_OpenFont(fontpath, 200);
	if(font == NULL){
		return fatalerror("Could not load font");
	}
	
	int checkw;
	int checkh;
	TTF_SizeText(font, "d", &checkw, &checkh);
	printf("%d, %d \n", checkw, checkh);
	
	char cc[2] = {' ', 0};
	int i = 0;
	color = (SDL_Color){.r = 255, .g = 255, .b = 255, .a = 255};
	for(cc[0] = ' '; cc[0] <= '~'; cc[0]++, i++){
		surface = TTF_RenderText_Solid(font, cc, color);
		characters[i] = SDL_CreateTextureFromSurface(renderer, surface);
		if(SDL_SetTextureColorMod(characters[i], 0, 255, 0) < 0){
			return fatalerror("Color modulation not supported");
		}
		SDL_FreeSurface(surface);
	}
	
	
	createbuffer();
	ap_clear();
	source = (SDL_Rect){.x = 0, .y = 0, .w = surface->w, .h = surface->h};
	ppcx = sw / cols;
	ppcy = sh / rows;
	
	int tx = 0;
	int ty = 0;
	int txv = 1;
	int tyv = 1;
	int hits = 0;
	int hitcount = 0;
	int cornercount = 0;
	int kek;
	insults[0] = "fuck you faggot";
	insults[1] = "go eat a bag of dicks";
	insults[2] = "ur dumber than a bag of hammers";
	insults[3] = "kiss my piss";
	insults[4] = "go die in a hole";
	char mystring1[] = "coding with ethanol";
	
	color = (SDL_Color){.r = 0, .g = 255, .b = 0, .a = 255};
	
	while(!quit){
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_WINDOWEVENT:
					switch(event.window.event){
						case SDL_WINDOWEVENT_RESIZED:
        				case SDL_WINDOWEVENT_SIZE_CHANGED:
        					SDL_GetWindowSize(window, &sw, &sh);
        					swd2 = sw >> 1;
        					shd2 = sh >> 1;
        					ppcx = sw / cols;
							ppcy = sh / rows;
						break;
					}
					break;
			}
		}	
		
		SDL_RenderClear(renderer);
	
		tx += txv;
		ty += tyv;
		if(tx > (cols - strlen(mystring1))){
			hits++;
			tx = cols - strlen(mystring1);
			txv = -txv;
		}
		if(tx <= 0){
			hits++;
			tx = 0;
			txv = -txv;
		}
		if(ty >= rows - 1){
			hits++;
			ty = rows - 1;
			tyv = -tyv;
		}
		if(ty <= 0){
			hits++;
			ty = 0;
			tyv = -tyv;
		}
		
		if(hits >= 2){
			color.r = 255 - color.r;
			color.g = 255 - color.g;
			cornercount++;
			hits = 0;
		}else{
			hitcount++;
			hits = 0;
		}
		
		ap_clear();
		ap_putstring(mystring1, strlen(mystring1), tx, ty);
		if(rand() % 20 == 13){
			kek = rand() % 5;
			ap_putstring(insults[kek], strlen(insults[kek]), -1, -1);
		}
		for(int ay = 0; ay < rows; ay++){
			for(int ax = 0; ax < cols; ax++){
				dest = (SDL_Rect){.x = ax * ppcx, .y = ay * ppcy, .w = ppcx, .h = ppcy};
				SDL_SetTextureColorMod(characters[buffer[ax][ay] - offset], color.r, color.g, color.b);
				SDL_RenderCopy(renderer, characters[buffer[ax][ay] - offset], &source, &dest);
			}
		}
		
		SDL_RenderPresent(renderer);
		SDL_Delay(100);
	}
	
	printf("corner hits: %d\n", cornercount);
	printf("wall hits: %d\n", hitcount);
	cleanup();
	return end();
}
