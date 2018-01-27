#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

const int WIDTH = 640;
const int HEIGHT = 480;

void dessin(SDL_Renderer *ren);
void dessin1(SDL_Renderer *ren);
void dessin_bis(SDL_Renderer *ren, int xStart, int yStart, int xEnd, int yEnd, int latMin, int lonMin, int latMax, int lonMax);
void dessin_alpha(SDL_Renderer *ren, int latMin, int lonMin, int latMax, int lonMax);
void dessin_beta(SDL_Renderer *ren, int tableau_coordonnees [6][4], int latMin, int lonMin, int latMax, int lonMax);

int main(int argc, char** argv)
{
	SDL_Window *win = 0;
	SDL_Renderer *ren = 0;
	int tableau_coordonnees [6][4] = {{37,64,80,75},{64,89,110,100},{89,57,100,56},{40,58,120,108},{63,98,69,78},{27,69,130,102}};

	/* Initialisation de la SDL, en cas de probleme on quitte */

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		fprintf(stderr, "Erreur lors de l'initialisation !\n");
		return -1;
	}

	/* Creation de la fenetre et du renderer */

	SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &win, &ren);

	if(!win || !ren)
	{
		fprintf(stderr, "Erreur lors de la création des fenetres !\n");
		SDL_Quit();
		return -1;
	}

	dessin_beta(ren, tableau_coordonnees, 25, 50, 150, 120);
	//dessin_alpha(ren, 25, 50, 150, 120);
	//dessin_bis(ren, 70, 80, 90, 60, 25, 50, 150, 120);
	//dessin_1(ren);

	SDL_Delay(6000);

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}

/*  Fonction test dessinant des segments dont les coordonnes ont ete generees aleatoirement */
/*void dessin_alpha(SDL_Renderer *ren, int latMin, int lonMin, int latMax, int lonMax)
{
	int x=0, y=0, v=0, w=0;
	int compteur;

	for(compteur = 0 ; compteur < 20 ; compteur ++)
	{
		x = (rand()%(latMax-latMin))+latMin;
		y = (rand()%(lonMax-lonMin))+lonMin;
		v = (rand()%(latMax-latMin))+latMin;
		w = (rand()%(lonMax-lonMin))+lonMin;
		dessin_bis(ren, x, y, v, w, latMin, lonMin, latMax, lonMax);
	}
	SDL_RenderPresent(ren);
}*/

/* Fonction dessinant des segments en recuperant leurs coordonnees depuis un tableau bidimensionnel*/
void dessin_beta(SDL_Renderer *ren, int tableau_coordonnees [6][4], int latMin, int lonMin, int latMax, int lonMax)
{
	int compteur;
	int x = 0, y = 0;
	x = (WIDTH/(latMax-latMin));
	y = (HEIGHT/(lonMax-lonMin));

	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);


	for(compteur = 0 ; compteur < (sizeof(tableau_coordonnees)/sizeof(tableau_coordonnees[0])*4) ; compteur++)
	{
		//dessin_bis(ren, tableau_coordonnees[compteur][0], tableau_coordonnees[compteur][1], tableau_coordonnees[compteur][2], tableau_coordonnees[compteur][3], 25, 50, 150, 120);

		SDL_SetRenderDrawColor(ren, 253, 253, 253, 20);
		SDL_RenderDrawLine(ren, (((tableau_coordonnees[compteur][0])-lonMin)*x),(((tableau_coordonnees[compteur][1])-latMin)*y),((tableau_coordonnees[compteur][2])*x),((tableau_coordonnees[compteur][3])*y));
	}
	SDL_RenderPresent(ren);
}

/* Fonction dessinant un segment en fonction de ses coordonnees et de la taille de l'ecran */
void dessin_bis(SDL_Renderer *ren, int xStart, int yStart, int xEnd, int yEnd, int latMin, int lonMin, int latMax, int lonMax)
{
	int x = 0, y = 0;
	x = (WIDTH/(latMax-latMin));
	y = (HEIGHT/(lonMax-lonMin));

	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

	SDL_SetRenderDrawColor(ren, 253, 253, 253, 20);
	SDL_RenderDrawLine(ren, ((xStart-lonMin)*x),((yStart-latMin)*y),(xEnd*x),(yEnd*y));

	//SDL_RenderPresent(ren);
}
/*
void dessin1(SDL_Renderer *ren)
{
	int i;
	int x=0, y=0, v=0, w=0;

	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

	for(i=0 ; i<15 ; i++)
	{
		x = rand()%WIDTH;;
		y = rand()%HEIGHT;
		v = rand()%(WIDTH-x);
		w = rand()%(HEIGHT-y);
		SDL_SetRenderDrawColor(ren, 253, 253, 253, 20);
		SDL_RenderDrawLine(ren, x, y, v, w);
	}

	//int x=62, y=62, v=186, w=186;
	//SDL_SetRenderDrawColor(ren, 253, 253, 253, 20);
	//SDL_RenderDrawLine(ren, x, y, v, w);

	SDL_RenderPresent(ren);
}

void dessin(SDL_Renderer *ren)
{
	int colorr, colorg, colorb;
	SDL_Rect r;
	int i;

	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

	for(i=0 ; i<30 ; i++)
	{
		r.x = rand()%WIDTH;
		r.y = rand()%HEIGHT;
		r.w = rand()%(WIDTH-r.x);
		r.h = rand()%(HEIGHT-r.y);
		colorr = rand()%256;
		colorg = rand()%256;
		colorb = rand()%256;
		SDL_SetRenderDrawColor(ren, colorr, colorg, colorb, 20);
		SDL_RenderFillRect(ren, &r);
	}
	SDL_RenderPresent(ren);
}*/