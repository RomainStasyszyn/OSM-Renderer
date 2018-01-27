#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

const int WIDTH = 800;
const int HEIGHT = 600;
/*const float MINLAT = 48.8192800;
const float MINLON = 2.3814700;
const float MAXLAT = 48.8236500;
const float MAXLON = 2.3916700;*/

//void dessin_ligne_brisee(SDL_Renderer *ren, int latMin, int lonMin, int latMax, int lonMax);
void dessin_ligne_simple(SDL_Renderer *ren, int tableau_coordonnees [6][4], int latMin, int lonMin, int latMax, int lonMax);
void dessin_ligne_brisee(SDL_Renderer *ren, int tab [], int latMin, int lonMin, int latMax, int lonMax);

int main(int argc, char** argv)
{
	//Notre fenetre de dessin
	SDL_Window *win = 0;

	//Notre Renderer pour le rendu du dessin dans la fenetre
	SDL_Renderer *ren = 0;

	//Un tableau exemple contenant les coordonnees de six segments (ways)
	int tableau_coordonnees [6][4] = {{37,64,80,75},{64,89,110,100},{89,57,100,56},{40,58,120,108},{63,98,69,78},{27,69,130,102}};

	//Un tableau exemple contenant les coordonnees d'une ligne brisee composee de 5 points
	int tab [10] = {50,50,100,110,300,200,450,260,500,350};

	int tab1 [14] = {60, 60, 150, 150, 200, 200, 300, 300, 450, 450, 550, 550, 600, 600};

	fprintf(stdout, "\n%d\n", (int)sizeof(tableau_coordonnees)/16);
	//Initialisation de la SDL (en cas de probleme on quitte)
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		fprintf(stderr, "Erreur lors de l'initialisation\n");
		return -1;
	}

	//Creation de la fenetre et du renderer
	SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &win, &ren);
	if(!win || !ren)
	{
		fprintf(stderr, "Erreur lors de la creation de la fenetre ou du rendu\n");
		SDL_Quit();
		return -1;
	}

	//dessin_ligne_simple(ren, tableau_coordonnees, 25, 50, 150, 120);
	dessin_ligne_brisee(ren, tab, 25, 50, 150, 120);
	dessin_ligne_brisee(ren, tab1, 25, 50, 150, 120);
	SDL_Delay(6000);

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}

/* Fonction dessinant toutes les ways contenues dans la hashmap passee en parametre */
/*void dessin_ligne_brisee(SDL_Renderer *ren, int latMin, int lonMin, int latMax, int lonMax)
{
	int x = 0, y = 0;
	x = (WIDTH/(latMax-latMin));
	y = (HEIGHT/(lonMax-lonMin));

	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

	while(ways->id != NULL && ways->visible)
	{
		SDL_SetRenderDrawColor(ren, 253, 253, 253, 20);
		SDL_RenderDrawLine();
	}
	SDL_RenderPresent(ren);
}*/

/* Fonction dessinant des segments en recuperant leurs coordonnees depuis un tableau bidimensionnel */
void dessin_ligne_simple(SDL_Renderer *ren, int tableau_coordonnees [6][4], int latMin, int lonMin, int latMax, int lonMax)
{
	int compteur;
	int x = 0, y = 0;
	x = (WIDTH/(latMax-latMin));
	y = (HEIGHT/(lonMax-lonMin));

	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);


	for(compteur = 0 ; compteur < 6 ; compteur++)
	{
		//dessin_bis(ren, tableau_coordonnees[compteur][0], tableau_coordonnees[compteur][1], tableau_coordonnees[compteur][2], tableau_coordonnees[compteur][3], 25, 50, 150, 120);
		//printf("%d", compteur);
		SDL_SetRenderDrawColor(ren, 253, 253, 253, 90);
		SDL_RenderDrawLine(ren, (((tableau_coordonnees[compteur][0])-lonMin)*x),(((tableau_coordonnees[compteur][1])-latMin)*y),((tableau_coordonnees[compteur][2])*x),((tableau_coordonnees[compteur][3])*y));
	}
	SDL_RenderPresent(ren);
}

/* Fonction dessinant une ligne brisee (segment compose d'au minimum deux points) */
void dessin_ligne_brisee(SDL_Renderer *ren, int tab [], int latMin, int lonMin, int latMax, int lonMax)
{	
	int compteur;
	int x = 0, y = 0;
	x = (WIDTH/(latMax-latMin));
	y = (HEIGHT/(lonMax-lonMin));

	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

	for(compteur = 2 ; compteur <= 5 ; compteur++)	
	{
		SDL_SetRenderDrawColor(ren, 253, 253, 253, 90);
		//SDL_RenderDrawLine(ren, ((tab[(compteur*2)-4]-lonMin)*x), ((tab[(compteur*2)-3]-latMin)*y), ((tab[(compteur*2)-2])*x), ((tab[(compteur*2)-1])*y));
		SDL_RenderDrawLine(ren, tab[compteur*2-4], tab[compteur*2-3], tab[compteur*2-2], tab[compteur*2-1]);
	}
	//SDL_SetRenderDrawColor(ren, 253, 253, 253, 90);
	//SDL_RenderDrawLines(ren, , 5)
	SDL_RenderPresent(ren);
}
