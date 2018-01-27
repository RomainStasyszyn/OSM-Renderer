#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <glib.h>


/*====================================================================================================================================*
 *    Première version du renderer utilisant les données d'une carte .osm extraite par un parseur s'occupant pour le moment juste     *
 *    des ways avec leur liste de nodes. Les batiments ainsi que les rivières viendront sous peu. Les ways sont stockées dans un      *
 *    tableau et les nodes les constituant sont stockés dans une table de hashage dont la clé est leur id.                            *
 *====================================================================================================================================*/


const int WIDTH = 800;
const int HEIGHT = 600;

//Il faut récupérer les limites de la cartes, ici c'est en dur mais ça ne peut pas rester comme ça.

const double MINLAT = 48.8192800;
const double MINLON = 2.3814700;
const double MAXLAT = 48.8236500;
const double MAXLON = 2.3916700;

int maint(int argc, char **argv)
{
	SDL_Window *win = 0;
	SDL_Renderer *ren = 0;

	//Initialisation de la SDL, en cas de problème on affiche un message d'erreur et on renvoie -1.
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		fprintf(stderr, "Erreur lors de l'initialisation de la SDL\n");
		return -1;
	}

	//Création de la fenêtre, en cas de problème on affiche un message d'erreur.
	win = SDL_CreateWindow("Paris Diderot map", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

	if(win)
	{
		//Création du rendu, en cas de problème on affiche un message d'erreur.
		ren = SDL_CreateRender(win, -1, SDL_RENDERER_ACCELERATED);

		if(ren)
		{


			SDL_DestroyRenderer(ren);
		}
		else
		{
			fprintf(stderr, "Erreur lors de la création du rendu: %s\n", SDL_GetError());
		}

		SDL_DestroyWindow(win);
	}
	else
	{
		fprintf(stderr, "Erreur lors de la création de la fenêtre : %s\n", SDL_GetError());
	}

	SDL_Quit();
	return 0;
}


/* Fonction s'occupant du dessin des ways de notre carte. Elle prend en paramètre notre Renderer, le tableau contenant les ways ainsi
   que les limites de la carte (latitude minimum et maximum et longitude minimum et maximum). Elle parcourt le tableau de ways et 
   récupère de cette manière la liste des nodes de chacune des way pour ensuite chercher pour chaque node, dans la table de hashage
   les coordonnées de celui-ci avant de tracer segment par segment la way associée. Le paramètre taille_tableau serait appréciable. */

void dessiner_way(Renderer *ren, ways tableau_ways [], int taille_tableau, GHashTable nodesHashTable, double MINLAT, double MAXLON, double MAXLAT, double MAXLON)
{
	int compteur;
	double rapportX = 0, rapportY = 0;
	rapportX = (WIDTH/(latMax-latMin));
	rapportY = (HEIGHT/(lonMax-lonMin));

	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

    //On parcourt le tableau de ways
	for(compteur = 0, void *ptr = tableau_ways [compteur].listeNode ; compteur < taille_tableau ; compteur++)
	{	
		//Si la way est visible alors on cherche à afficher les nodes qui la constituent.
		if(way.visible)
		{
			//On parcourt la liste de nodes associée à la way.
			while(ptr.suivant != NULL)
			{
				void *node1 = g_hash_table_lookup(*nodesHashTable, *ptr);
				void *node2 = g_hash_table_lookup(*nodesHashTable, *ptr.suivant;
				nodeX1 = *node1.lat;
				nodeY1 = *node1.lon;
				nodeX2 = *node2.lat;
				nodeY2 = *node2.lon;

				SDL_SetRenderDrawColor(ren, 253, 253, 253, 90);
				SDL_DrawLine(ren, (nodeX1-MINLAT)*rapportX, (nodeY1-MINLON)*rapportY, (nodeX2-MINLAT)*rapportX, (nodeY2-MINLON)*rapportY);

				ptr++;
			}
		}
	}
	//On actualise notre Renderer.
	SDL_RenderPresent(ren);
}