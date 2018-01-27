#include "../header/main.h"


int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Erreurs arguments: parse <nom_fichier_valide_osm>\n");
        return 2;
    }
    osm os;
    os.nodesHashTable = g_hash_table_new(g_int64_hash, g_int64_equal);
    int retour;
    if ((retour = parseDoc(argv[1], &os)) < 0) {
        fprintf(stderr, "%d\n", retour);
        return retour;
    }

    //Gestion de la dimension de la fenêtre
    //Longitude Haut-bas
    float minY = os.bound.minLon;
    float maxY = os.bound.maxLon;
    //Latitude Gauche-droite
    float minX = os.bound.minLat;
    float maxX = os.bound.maxLat;

    float mapLon = (maxY-minY);
    float mapLat = (maxX-minX);

    int windowWidth = WIDTH;
    int windowHeight = HEIGHT;
    if (mapLon < mapLat) {
        float coefficientFenetre = mapLat / mapLon; // Longitude/Latitude
        windowHeight = windowHeight * coefficientFenetre;
    } else {
        float coefficientFenetre = mapLon / mapLat; // Longitude/Latitude
        windowWidth = windowWidth * coefficientFenetre;
    }


    // Enlever(ou mettre) les commentaires si vous voulez vérifier(ou non) si les éléments ont été bien récupérés
    //afficher(os);

    SDL_Window *win = 0;
    SDL_Renderer *ren = 0;
    // Initialisation de la SDL, en cas de problème on affiche un message d'erreur et on renvoie -1.
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "Erreur lors de l'initialisation de la SDL\n");
        return -1;
    }
    // Création de la fenêtre, en cas de problème on affiche un message d'erreur.
    win = SDL_CreateWindow(argv[1], SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight,
                           SDL_WINDOW_SHOWN);
    if (win) {
        // Création du rendu, en cas de problème on affiche un message d'erreur.
        ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
        if (ren) {
            SDL_SetRenderDrawColor(ren, 150, 150, 150, 200);
            SDL_RenderClear(ren);
            SDL_RenderPresent(ren);
            //dessiner_way_2(ren, &os);
            //dessiner_way_3(ren, &os);
            dessiner_way_gfx(ren, &os);
            //dessiner_way(ren,os.ways,os.nodesHashTable,os.bound.maxLat,os.bound.minLon,os.bound.minLat,os.bound.maxLon);
            SDL_Delay(6000);

            SDL_DestroyRenderer(ren);
        } else {
            fprintf(stderr, "Erreur lors de la création du rendu\n");
        }
        SDL_DestroyWindow(win);
    }
    else {
        fprintf(stderr, "Erreur lors de la création de la fenêtre\n");
    }
    SDL_Quit();
    return 0;
}
