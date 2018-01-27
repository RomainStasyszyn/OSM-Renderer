#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

void pause();

int main(int argc, char* argv[]){

	SDL_Surface *ecran = NULL, *rectangle = NULL;
	SDL_Rect position;

	//Seul le mode video nous interesse
	SDL_Init(SDL_INIT_VIDEO);

	ecran = SDL_SetVideoMode(640, 256, 32, SDL_HWSURFACE | SDL_RESIZABLE);

	rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, 220, 180, 32, 0, 0, 0, 0);
	SDL_WM_SetCaption("Ma super fenetre test", NULL);

	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 17, 206, 112));

	position.x = (640/2) - (220/2);
	position.y = (480/2) - (180/2);

	SDL_FillRect(rectangle, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
	SDL_BlitSurface(rectangle, NULL, ecran, &position);

	SDL_Flip(ecran);

	pause();

	SDL_FreeSurface(rectangle);
	
	SDL_Quit();

	return EXIT_SUCCESS;
}

void pause(){
	int continuer = 1;
	SDL_Event event;

	while(continuer){
		SDL_WaitEvent(&event);
		switch(event.type){
			case SDL_QUIT: continuer = 0;
		}
	}	
}
