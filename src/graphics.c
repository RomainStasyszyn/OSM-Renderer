#include "../header/graphics.h"

/*
 * dessiner_way : utilise sdl pour obtenir un affichage en ligne brisées de la carte
 * @param *ren : pointer vers la fenetre où sera affiché la carte
 * @param *way_array : array de way contenant toute les informations de way
 * @param *nodesHashTable : GHashTable contenant tous les nodes, leurs id servent de clé
 * @param minlat : flaot limite de latitude minimale
 * @param minlon : float limite de longitude minimale
 * @param maxlat : float limite de latitude maximale
 * @param maxlon : float limite de longitude maximale
 */
void dessiner_way(SDL_Renderer *ren, way *way_array, GHashTable *nodesHashTable, float minlat, float minlon,
                  float maxlat, float maxlon) {
    //initialisation du compteur pour parcourir le tableau de ways et celui du tableau de nodes liés à chaque way.
    int compteur = 0;
    //initialisation de nos coordonnées des nodes.
    float nodeX1 = 0, nodeY1 = 0, nodeX2 = 0, nodeY2 = 0;
    //initialisation des rapports entre les limites de la map et les dimensions de notre fenêtre.
    float rapportX = 0, rapportY = 0;
    //valeur absolue de nos limites au niveau des coordonnées.
    float MINLAT = fabs(minlat);
    float MINLON = fabs(minlon);
    float MAXLAT = fabs(maxlat);
    float MAXLON = fabs(maxlon);
    printf("coord : %f %f %f %f\n", MINLAT, MINLON, MAXLAT, MAXLON);
    printf("sub : %f %f\n", (MAXLAT - MINLAT), (MAXLON - MINLON));
    //initialisation de nos pointeurs pour avoir accès aux nodes lors de leur tracage.
    rapportX = (WIDTH / (MAXLAT - MINLAT));
    rapportY = (HEIGHT / (MAXLON - MINLON));
    printf("Rapport et compteur\n");
    printf("%f\n", rapportX);
    printf("%f\n", rapportY);
    printf("%d\n", compteur);

    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

    //On parcourt le tableau de ways.
    //printf("TAILLE DE LA HASHMAP : %lu \n", sizeof(nodesHashTable));
    while (way_array[compteur].id != '\0') {
        int compteur2 = 0;
        //printf("premier while\n");
        //printf("%d", way_array[compteur].visible);
        //Si c'est visible on s'occupe d'afficher la way sinon pas besoin.
        if (way_array[compteur].visible == 0) {
            //printf("premier if\n");
            //On parcourt le tableau de nodes constituant la way.
            while (way_array[compteur].ref[compteur2 + 1] != '\0') {
                // printf("deuxième while\n");
                //On récupère les coordonnées de deux nodes afin de tracer le segment entre les deux.
                unsigned long int key1 = way_array[compteur].ref[compteur2];
                unsigned long int key2 = way_array[compteur].ref[compteur2 + 1];

                //unsigned long int *cle1 = &key1;
                //unsigned long int *cle2 = &key2;
                // printf("Les clés sont ok\n");
                node *node1 = g_hash_table_lookup(nodesHashTable, &key1);
                node *node2 = g_hash_table_lookup(nodesHashTable, &key2);

                //printf("IS NOT NULL")
                nodeX1 = node1->latitude;
                nodeY1 = node1->longitude;
                nodeX2 = node2->latitude;
                nodeY2 = node2->longitude;

                nodeX1 = fabs(nodeX1);
                nodeY1 = fabs(nodeY1);
                nodeX2 = fabs(nodeX2);
                nodeY2 = fabs(nodeY2);

                SDL_SetRenderDrawColor(ren, 200, 200, 200, 90);
                //printf("tracage\n");
                //SDL_RenderDrawLine(ren, (nodeX1-MINLAT)*rapportX, (nodeY1-MINLON)*rapportY,(nodeX2-MINLAT)*rapportX, (nodeY2-MINLON)*rapportY);
                SDL_RenderDrawLine(ren, (nodeY1 - MINLON) * rapportY, (nodeX1 - MINLAT) * rapportX,
                                   (nodeY2 - MINLON) * rapportY, (nodeX2 - MINLAT) * rapportX);
                //SDL_RenderDrawLine(ren, );
                //SDL_RenderDrawLine(ren, (nodeX1-MINLAT)*rapportX, MAXLON-((nodeY1-MINLON)*rapportY),(nodeX2-MINLAT)*rapportX, MAXLON-((nodeY2-MINLON)*rapportY));
                //fprintf(stdout, "%f\n%f\n%f\n%f\n", nodeX1, nodeY1, nodeX2, nodeY2);

                compteur2++;
            }
        }
        compteur++;
    }
    //On actualise notre Renderer.
    SDL_RenderPresent(ren);
}

/*
 * dessiner_way_gfx : utilise gfx pour obtenir un affichage colorisé de la carte
 * @param *ren : pointer vers la fenetre où sera affiché la carte
 * @param *map : structure de données contenant toutes informations extraites du xml .osm
 */
void dessiner_way_gfx(SDL_Renderer *ren, osm *map) {
//Fonction de dessin(routes,rivières,batiments, parcs, terrains herbeux, fleuves)
//intégrant un ordre d'affichage afin d'éviter d'avoir
//de l'eau par dessus une île, un pont etc.


    //Gestion de la dimension de la fenêtre
    //Longitude Haut-bas
    float minY = map->bound.minLon;
    float maxY = map->bound.maxLon;
    //Latitude Gauche-droite
    float minX = map->bound.minLat;
    float maxX = map->bound.maxLat;

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

    //compteur pour nos parcours consécutifs permettant de dessiner d'abord les étendues d'eau, puis les batiments, parcs etc puis les routes.
    int compteur_ordre = 0;
    //les coordonnées des deux points formants chaque segments composants les ways
    float nodeX1 = 0, nodeY1 = 0, nodeX2 = 0, nodeY2 = 0;
    //le rapport multipliant chaque coordonnée afin d'être à l'échelle de notre fenêtre
    float rapportX = 0, rapportY = 0;
    //on calcule nos rapports liés à l'axe des abscisses et à l'axe des ordonnées
    rapportX = (windowWidth / (map->bound.maxLon - map->bound.minLon));
    rapportY = (windowHeight / (map->bound.maxLat - map->bound.minLat));

    for (compteur_ordre = 0; compteur_ordre < 4; compteur_ordre++) {
        //premier compteur pour parcourir notre tableau de ways
        int compteur = 0;
        //tant qu'on a encore une way dans notre tableau
        while (map->ways[compteur].id != '\0') {
            int compteur2 = 0;
            //si la way doit être visible alors on continue
            if (map->ways[compteur].visible == 0) {
                if (compteur_ordre == 0 && (strcmp(map->ways[compteur].type, (char *) "river_area") == 0 ||
                                            strcmp(map->ways[compteur].type, (char *) "water_area") == 0)) {
                    int lengthTab = 50;
                    Sint16 *xArray = malloc(lengthTab * sizeof(float));
                    Sint16 *yArray = malloc(lengthTab * sizeof(float));
                    while (map->ways[compteur].ref[compteur2] != '\0') {
                        if (compteur2 == lengthTab) {
                            xArray = realloc(xArray, (lengthTab + 50) * sizeof(float));
                            yArray = realloc(yArray, (lengthTab + 50) * sizeof(float));
                            lengthTab += 50;
                        }
                        unsigned long int key1 = map->ways[compteur].ref[compteur2];
                        node *node1 = g_hash_table_lookup(map->nodesHashTable, &key1);
                        nodeX1 = (node1->longitude) - (map->bound.minLon);
                        nodeY1 = (node1->latitude) - (map->bound.minLat);
                        nodeX1 = nodeX1 * rapportX;
                        nodeY1 = windowHeight - (nodeY1 * rapportY);
                        xArray[compteur2] = nodeX1;
                        yArray[compteur2] = nodeY1;
                        compteur2++;
                    }
                    filledPolygonRGBA(ren, xArray, yArray, compteur2, 0, 51, 255, 200);
                }
                else if (compteur_ordre == 0 && (strcmp(map->ways[compteur].type, (char *) "water_way") == 0 ||
                                                 strcmp(map->ways[compteur].type, (char *) "water_border") == 0)) {
                    while (map->ways[compteur].ref[compteur2 + 1] != '\0') {
                        unsigned long int key1 = map->ways[compteur].ref[compteur2];
                        unsigned long int key2 = map->ways[compteur].ref[compteur2 + 1];
                        node *node1 = g_hash_table_lookup(map->nodesHashTable, &key1);
                        node *node2 = g_hash_table_lookup(map->nodesHashTable, &key2);
                        //on calcule la distance entre l'origine et nos points
                        nodeX1 = (node1->longitude) - map->bound.minLon;
                        nodeY1 = (node1->latitude) - map->bound.minLat;
                        nodeX2 = (node2->longitude) - map->bound.minLon;
                        nodeY2 = (node2->latitude) - map->bound.minLat;
                        if (strcmp(map->ways[compteur].type, (char *) "water_way") == 0) {
                            thickLineRGBA(ren, nodeX1 * rapportX, windowHeight - nodeY1 * rapportY, nodeX2 * rapportX,
                                          windowHeight - nodeY2 * rapportY, 1, 0, 51, 255, 200);
                        }
                        else if (strcmp(map->ways[compteur].type, (char *) "water_border") == 0) {
                            thickLineRGBA(ren, nodeX1 * rapportX, windowHeight - nodeY1 * rapportY, nodeX2 * rapportX,
                                          windowHeight - nodeY2 * rapportY, 2, 255, 0, 0, 200);
                        }
                        compteur2++;
                    }
                }
                else if (compteur_ordre == 1 && (strcmp(map->ways[compteur].type, (char *) "park_area") == 0 ||
                                                 strcmp(map->ways[compteur].type, (char *) "grass_area") == 0)) {
                    int lengthTab = 50;
                    Sint16 *xArray = malloc(lengthTab * sizeof(float));
                    Sint16 *yArray = malloc(lengthTab * sizeof(float));
                    while (map->ways[compteur].ref[compteur2] != '\0') {
                        if (compteur2 == lengthTab) {
                            xArray = realloc(xArray, (lengthTab + 50) * sizeof(float));
                            yArray = realloc(yArray, (lengthTab + 50) * sizeof(float));
                            lengthTab += 50;
                        }
                        unsigned long int key1 = map->ways[compteur].ref[compteur2];
                        node *node1 = g_hash_table_lookup(map->nodesHashTable, &key1);
                        nodeX1 = (node1->longitude) - (map->bound.minLon);
                        nodeY1 = (node1->latitude) - (map->bound.minLat);
                        nodeX1 = nodeX1 * rapportX;
                        nodeY1 = windowHeight - (nodeY1 * rapportY);
                        xArray[compteur2] = nodeX1;
                        yArray[compteur2] = nodeY1;
                        compteur2++;
                    }
                    if (strcmp(map->ways[compteur].type, (char *) "grass_area") == 0) {
                        filledPolygonRGBA(ren, xArray, yArray, compteur2, 0, 180, 0, 200);
                        aapolygonRGBA(ren, xArray, yArray, compteur2, 0, 0, 0, 200);
                    }
                    else if (strcmp(map->ways[compteur].type, (char *) "park_area") == 0) {
                        filledPolygonRGBA(ren, xArray, yArray, compteur2, 0, 100, 0, 200);
                        aapolygonRGBA(ren, xArray, yArray, compteur2, 0, 0, 0, 200);
                    }
                }
                else if (compteur_ordre == 2 && strcmp(map->ways[compteur].type, (char *) "building_way") == 0) {
                    int lengthTab = 50;
                    Sint16 *xArray = malloc(lengthTab * sizeof(float));
                    Sint16 *yArray = malloc(lengthTab * sizeof(float));
                    while (map->ways[compteur].ref[compteur2] != '\0') {
                        if (compteur2 == lengthTab) {
                            xArray = realloc(xArray, (lengthTab + 50) * sizeof(float));
                            yArray = realloc(yArray, (lengthTab + 50) * sizeof(float));
                            lengthTab += 50;
                        }
                        unsigned long int key1 = map->ways[compteur].ref[compteur2];
                        node *node1 = g_hash_table_lookup(map->nodesHashTable, &key1);
                        nodeX1 = (node1->longitude) - (map->bound.minLon);
                        nodeY1 = (node1->latitude) - (map->bound.minLat);
                        nodeX1 = nodeX1 * rapportX;
                        nodeY1 = windowHeight - (nodeY1 * rapportY);
                        xArray[compteur2] = nodeX1;
                        yArray[compteur2] = nodeY1;
                        compteur2++;
                    }
                    filledPolygonRGBA(ren, xArray, yArray, compteur2, 100, 100, 100, 200);
                    aapolygonRGBA(ren, xArray, yArray, compteur2, 0, 0, 0, 200);
                }
                else if (compteur_ordre == 3 && (strcmp(map->ways[compteur].type, (char *) "normal_way") == 0 ||
                                                 strcmp(map->ways[compteur].type, (char *) "motor_way") == 0)) {
                    while (map->ways[compteur].ref[compteur2 + 1] != '\0') {
                        unsigned long int key1 = map->ways[compteur].ref[compteur2];
                        unsigned long int key2 = map->ways[compteur].ref[compteur2 + 1];
                        node *node1 = g_hash_table_lookup(map->nodesHashTable, &key1);
                        node *node2 = g_hash_table_lookup(map->nodesHashTable, &key2);
                        //on calcule la distance entre l'origine et nos points
                        nodeX1 = (node1->longitude) - map->bound.minLon;
                        nodeY1 = (node1->latitude) - map->bound.minLat;
                        nodeX2 = (node2->longitude) - map->bound.minLon;
                        nodeY2 = (node2->latitude) - map->bound.minLat;
                        if (strcmp(map->ways[compteur].type, (char *) "normal_way") == 0) {
                            thickLineRGBA(ren, nodeX1 * rapportX, windowHeight - nodeY1 * rapportY, nodeX2 * rapportX,
                                          windowHeight - nodeY2 * rapportY, 2, 200, 200, 200, 200);
                        }
                        else if (strcmp(map->ways[compteur].type, (char *) "motor_way") == 0) {
                            thickLineRGBA(ren, nodeX1 * rapportX, windowHeight - nodeY1 * rapportY, nodeX2 * rapportX,
                                          windowHeight - nodeY2 * rapportY, 2, 100, 0, 250, 250);
                        }
                        compteur2++;
                    }
                }
            }
            compteur++;
        }
    }
    SDL_RenderPresent(ren);
}

/*
 * dessiner_way_3 : utilise gfx pour obtenir un affichage partiellement colorisé de la carte
 * @param *ren : pointer vers la fenetre où sera affiché la carte
 * @param *map : structure de données contenant toutes informations extraites du xml .osm
 */
void dessiner_way_3(SDL_Renderer *ren, osm *map) {

    //Gestion de la dimension de la fenêtre
    //Longitude Haut-bas
    float minY = map->bound.minLon;
    float maxY = map->bound.maxLon;
    //Latitude Gauche-droite
    float minX = map->bound.minLat;
    float maxX = map->bound.maxLat;

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

    //premier compteur pour parcourir notre tableau de ways
    int compteur = 0;
    //les coordonnées des deux points formants chaque segments composants les ways
    float nodeX1 = 0, nodeY1 = 0, nodeX2 = 0, nodeY2 = 0;
    //le rapport multipliant chaque coordonnée afin d'être à l'échelle de notre fenêtre
    float rapportX = 0, rapportY = 0;
    //on calcule nos rapports liés à l'axe des abscisses et à l'axe des ordonnées
    rapportX = (windowWidth / (map->bound.maxLon - map->bound.minLon));
    rapportY = (windowHeight / (map->bound.maxLat - map->bound.minLat));
    //tant qu'on a encore une way dans notre tableau
    while (map->ways[compteur].id != '\0') {
        int compteur2 = 0;
        //si la way doit être visible alors on continue
        if (map->ways[compteur].visible == 0) {
            //fprintf(stdout, "la way est visible\n");
            //On va dessiner différemment selon la nature de notre objet
            //si on a affaire à un batiment on va récupérer l'abscisse et l'ordonnée de chaque node de notre batiment, on va les mettre
            //dans un tableau d'abscisses et et dans un tableau d'ordonnées afin de les passer en paramètre de notre fonction de gfx
            //dessinant un polygone au nombre quelconque de points
            if (strcmp(map->ways[compteur].type, (char *) "building_way") == 0 ||
                strcmp(map->ways[compteur].type, (char *) "grass_area") == 0 ||
                strcmp(map->ways[compteur].type, (char *) "water_area") == 0 ||
                strcmp(map->ways[compteur].type, (char *) "river_area") == 0 ||
                strcmp(map->ways[compteur].type, (char *) "park_area") == 0) {
                //fprintf(stdout, "on a un batiment\n");
                int lengthTab = 50;
                Sint16 *xArray = malloc(lengthTab * sizeof(float));
                Sint16 *yArray = malloc(lengthTab * sizeof(float));
                while (map->ways[compteur].ref[compteur2] != '\0') {
                    if (compteur2 == lengthTab) {
                        xArray = realloc(xArray, (lengthTab + 50) * sizeof(float));
                        yArray = realloc(yArray, (lengthTab + 50) * sizeof(float));
                        lengthTab += 50;
                    }
                    unsigned long int key1 = map->ways[compteur].ref[compteur2];
                    //unsigned long int key2 = map->ways[compteur].ref[compteur2 + 1];
                    node *node1 = g_hash_table_lookup(map->nodesHashTable, &key1);
                    //node *node2 = g_hash_table_lookup(map->nodesHashTable, &key2);
                    //on calcule la distance entre l'origine et nos points
                    nodeX1 = (node1->longitude) - (map->bound.minLon);
                    nodeY1 = (node1->latitude) - (map->bound.minLat);
                    nodeX1 = nodeX1 * rapportX;
                    nodeY1 = windowHeight - (nodeY1 * rapportY);
                    xArray[compteur2] = nodeX1;
                    yArray[compteur2] = nodeY1;
                    //fprintf(stdout, "%f\t%f\n", node1->longitude, node1->latitude);
                    compteur2++;
                }
                /* compteur_test = 0;
                for(compteur_test = 0 ; compteur_test < compteur2 ; compteur_test++){
                    fprintf(stdout, "%f\t%f\n", xArray[compteur_test], yArray[compteur_test]);
                }*/
                //fprintf(stdout, "%d\n", compteur2);
                if (strcmp(map->ways[compteur].type, (char *) "building_way") == 0) {
                    filledPolygonRGBA(ren, xArray, yArray, compteur2, 100, 100, 100, 200);
                    aapolygonRGBA(ren, xArray, yArray, compteur2, 0, 0, 0, 200);
                }
                else if (strcmp(map->ways[compteur].type, (char *) "grass_area") == 0) {
                    filledPolygonRGBA(ren, xArray, yArray, compteur2, 0, 180, 0, 200);
                }
                else if (strcmp(map->ways[compteur].type, (char *) "river_area") == 0) {
                    filledPolygonRGBA(ren, xArray, yArray, compteur2, 0, 51, 255, 200);
                }
                else if (strcmp(map->ways[compteur].type, (char *) "water_area") == 0) {
                    filledPolygonRGBA(ren, xArray, yArray, compteur2, 0, 51, 255, 200);
                }
                else if (strcmp(map->ways[compteur].type, (char *) "park_area") == 0) {
                    filledPolygonRGBA(ren, xArray, yArray, compteur2, 0, 100, 0, 200);
                }
            }

                //si on a affaire à une way
            else if (strcmp(map->ways[compteur].type, (char *) "water_way") == 0 ||
                     strcmp(map->ways[compteur].type, (char *) "normal_way") == 0) {
                //fprintf(stdout, "on a une waterway\n");
                while (map->ways[compteur].ref[compteur2 + 1] != '\0') {
                    unsigned long int key1 = map->ways[compteur].ref[compteur2];
                    unsigned long int key2 = map->ways[compteur].ref[compteur2 + 1];
                    node *node1 = g_hash_table_lookup(map->nodesHashTable, &key1);
                    node *node2 = g_hash_table_lookup(map->nodesHashTable, &key2);
                    //on calcule la distance entre l'origine et nos points
                    nodeX1 = (node1->longitude) - map->bound.minLon;
                    nodeY1 = (node1->latitude) - map->bound.minLat;
                    nodeX2 = (node2->longitude) - map->bound.minLon;
                    nodeY2 = (node2->latitude) - map->bound.minLat;
                    //fprintf(stdout,"%f\t%f\t%f\t%f\n",nodeX1, nodeY1, nodeX2, nodeY2);
                    if (strcmp(map->ways[compteur].type, (char *) "water_way") == 0) {
                        thickLineRGBA(ren, nodeX1 * rapportX, windowHeight - nodeY1 * rapportY, nodeX2 * rapportX,
                                      windowHeight - nodeY2 * rapportY, 1, 0, 51, 255, 200);
                    }
                    else if (strcmp(map->ways[compteur].type, (char *) "normal_way") == 0) {
                        thickLineRGBA(ren, nodeX1 * rapportX, windowHeight - nodeY1 * rapportY, nodeX2 * rapportX,
                                      windowHeight - nodeY2 * rapportY, 2, 200, 200, 200, 200);
                    }

                    compteur2++;
                }
            }
            //si on a affaire à une highway ou autre routes basiques
            /*else if(strcmp(map->ways[compteur].type, (char *)"normal_way") == 0){
                //fprintf(stdout, "on a une highway\n");
                while(map->ways[compteur].ref[compteur2 + 1] != '\0') {
                    unsigned long int key1 = map->ways[compteur].ref[compteur2];
                    unsigned long int key2 = map->ways[compteur].ref[compteur2 + 1];
                    node *node1 = g_hash_table_lookup(map->nodesHashTable, &key1);
                    node *node2 = g_hash_table_lookup(map->nodesHashTable, &key2);
                    //on calcule la distance entre l'origine et nos points
                    nodeX1 = (node1->longitude) - map->bound.minLon;
                    nodeY1 = (node1->latitude) - map->bound.minLat;
                    nodeX2 = (node2->longitude) - map->bound.minLon;
                    nodeY2 = (node2->latitude) - map->bound.minLat;
                    //fprintf(stdout,"%f\t%f\t%f\t%f\n",nodeX1, nodeY1, nodeX2, nodeY2);
                    thickLineRGBA(ren, nodeX1*rapportX, HEIGHT-nodeY1*rapportY, nodeX2*rapportX, HEIGHT-nodeY2*rapportY, 2, 200, 200, 200, 200);
                    //fprintf(stdout, "%f\t%f\t%f\t%f\n", node1->latitude, node1->longitude, node2->latitude, node2->longitude);
                    compteur2++;
                }
            }*/
            //tant qu'il y a un noeud appartenant à notre way
            /*while(map->ways[compteur].ref[compteur2 + 1] != '\0') {
                unsigned long int key1 = map->ways[compteur].ref[compteur2];
                unsigned long int key2 = map->ways[compteur].ref[compteur2 + 1];
                node *node1 = g_hash_table_lookup(map->nodesHashTable, &key1);
                node *node2 = g_hash_table_lookup(map->nodesHashTable, &key2);
                //on calcule la distance entre l'origine et nos points
                nodeX1 = (node1->longitude) - map->bound.minLon;
                nodeY1 = (node1->latitude) - map->bound.minLat;
                nodeX2 = (node2->longitude) - map->bound.minLon;
                nodeY2 = (node2->latitude) - map->bound.minLat;
                fprintf(stdout,"%f\t%f\t%f\t%f\n",nodeX1, nodeY1, nodeX2, nodeY2);

                compteur2++;
            }*/
        }
        compteur++;
    }
    //fprintf(stdout, "%f\t%f\t%f\t%f\n",map->bound.minLat, map->bound.minLon, map->bound.maxLat, map->bound.maxLon);
    SDL_RenderPresent(ren);
}

/*
 * dessiner_way_2 : utilise gfx pour obtenir un affichage des routes de la carte
 * @param *ren : pointer vers la fenetre où sera affiché la carte
 * @param *map : structure de données contenant toutes informations extraites du xml .osm
 */
void dessiner_way_2(SDL_Renderer *ren, osm *map) {


    //Gestion de la dimension de la fenêtre
    //Longitude Haut-bas
    float minY = map->bound.minLon;
    float maxY = map->bound.maxLon;
    //Latitude Gauche-droite
    float minX = map->bound.minLat;
    float maxX = map->bound.maxLat;

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

    //premier compteur pour parcourir notre tableau de ways
    int compteur = 0;
    //les coordonnées des deux poinnts formants chaque segments composants les ways
    float nodeX1 = 0, nodeY1 = 0, nodeX2 = 0, nodeY2 = 0;
    //le rapport multipliant chaque coordonnée afin d'être à l'échelle de notre fenêtre
    float rapportX = 0, rapportY = 0;
    //on calcule nos rapports liés à l'axe des abscisses et à l'axe des ordonnées
    rapportX = (windowWidth / (map->bound.maxLon - map->bound.minLon));
    rapportY = (windowHeight / (map->bound.maxLat - map->bound.minLat));
    //tant qu'on a encore une way dans notre tableau
    while (map->ways[compteur].id != '\0') {
        int compteur2 = 0;
        //si la way doit être visible alors on continue
        if (map->ways[compteur].visible == 0) {
            //tant qu'il y a un noeud appartenant à notre way
            while (map->ways[compteur].ref[compteur2 + 1] != '\0') {
                unsigned long int key1 = map->ways[compteur].ref[compteur2];
                unsigned long int key2 = map->ways[compteur].ref[compteur2 + 1];
                node *node1 = g_hash_table_lookup(map->nodesHashTable, &key1);
                node *node2 = g_hash_table_lookup(map->nodesHashTable, &key2);
                //on calcule la distance entre l'origine et nos points
                nodeX1 = (node1->longitude) - map->bound.minLon;
                nodeY1 = (node1->latitude) - map->bound.minLat;
                nodeX2 = (node2->longitude) - map->bound.minLon;
                nodeY2 = (node2->latitude) - map->bound.minLat;

                //SDL_SetRenderDrawColor(ren, 200, 200, 200, 90);

                //SDL_RenderDrawLine(ren, nodeX1*rapportX, HEIGHT-nodeY1*rapportY, nodeX2*rapportX, HEIGHT-nodeY2*rapportY);

                /*if(strcmp(map->ways[compteur].type, (char*)"highway") == 0){
                    fprintf(stdout,"HIGHWAY !\n");
                    thickLineRGBA(ren, nodeX1*rapportX, HEIGHT-nodeY1*rapportY, nodeX2*rapportX, HEIGHT-nodeY2*rapportY, 2, 200, 200, 200, 90);
                    break;
                }
                else if(strcmp(map->ways[compteur].type, (char *)"waterway") == 0){
                    fprintf(stdout,"WATERWAY !\n");
                    thickLineRGBA(ren, nodeX1*rapportX, HEIGHT-nodeY1*rapportY, nodeX2*rapportX, HEIGHT-nodeY2*rapportY, 1, 51, 100, 250, 90);
                    break;
                }
                else if(strcmp(map->ways[compteur].type, (char *)"building") == 0){
                    //dessiner_building();
                    break;
                }
                else if(strcmp(map->ways[compteur].type, (char *)"coastline") == 0){
                    fprintf(stdout,"COASTLINE !\n");
                    thickLineRGBA(ren, nodeX1*rapportX, HEIGHT-nodeY1*rapportY, nodeX2*rapportX, HEIGHT-nodeY2*rapportY, 3, 51, 100, 250, 90);
                    break;
                }
                else{
                    fprintf(stdout,"BASIC !\n");
                    thickLineRGBA(ren, nodeX1*rapportX, HEIGHT-nodeY1*rapportY, nodeX2*rapportX, HEIGHT-nodeY2*rapportY, 1, 150, 150, 150, 90);
                    break;
                }*/
                if (strcmp(map->ways[compteur].type, (char *) "normal_way") == 0) {
                    //fprintf(stdout, "%s\n", map->ways[compteur].type);
                    //fprintf(stdout, "%f\t%f\t%f\t%f\n", nodeX1 * rapportX, HEIGHT - nodeY1 * rapportY,
                    //       nodeX2 * rapportX, HEIGHT - nodeY2 * rapportY);
                    thickLineRGBA(ren, nodeX1 * rapportX, windowHeight - nodeY1 * rapportY, nodeX2 * rapportX,
                                  windowHeight - nodeY2 * rapportY, 1, 200, 200, 200, 90);
                }
                else if (strcmp(map->ways[compteur].type, (char *) "water_way") == 0) {
                    //fprintf(stdout, "%s\n", map->ways[compteur].type);
                    thickLineRGBA(ren, nodeX1 * rapportX, windowHeight - nodeY1 * rapportY, nodeX2 * rapportX,
                                  windowHeight - nodeY2 * rapportY, 2, 50, 100, 200, 90);
                }
                compteur2++;
            }
        }
        compteur++;
    }
    SDL_RenderPresent(ren);
}

/*void dessiner_way_3(SDL_Renderer *ren, osm *map){
    //premier compteur pour parcourir notre tableau de ways
    int compteur = 0;
    float nodeX1 = 0, nodeY1 = 0, nodeX2 = 0, nodeY2 = 0;
    //le rapport multipliant chaque coordonnée afin d'être à l'échelle de notre fenêtre
    float rapportX = 0, rapportY = 0;
    //on calcule nos rapports liés à l'axe des abscisses et à l'axe des ordonnées
    rapportX = (WIDTH / (map->bound.maxLon - map->bound.minLon));
    rapportY = (HEIGHT / (map->bound.maxLat - map->bound.minLat));

    //tant qu'il y a une way dans le tableau de ways de notre objet osm
    while(map->ways[compteur].id != '\0') {
        int compteur2 = 0;

        //si la way doit être visible alors on continue
        if(map->ways[compteur].visible == 0) {
            //si on a affaire à un batiment on doit créer un tableau des abscisses et un tableau des ordonnées pour chaque node
            if(strcmp(map->ways[compteur].type, "building") == 0){
                continue;
            }
            else{
                //tant qu'il y a un noeud appartenant à notre way
                while(map->ways[compteur].ref[compteur2 + 1] != '\0') {

                    unsigned long int key1 = map->ways[compteur].ref[compteur2];
                    unsigned long int key2 = map->ways[compteur].ref[compteur2 + 1];
                    node *node1 = g_hash_table_lookup(map->nodesHashTable, &key1);
                    node *node2 = g_hash_table_lookup(map->nodesHashTable, &key2);
                    //on calcule la distance entre l'origine et nos points
                    nodeX1 = (node1->longitude) - map->bound.minLon;
                    nodeY1 = (node1->latitude) - map->bound.minLat;
                    nodeX2 = (node2->longitude) - map->bound.minLon;
                    nodeY2 = (node2->latitude) - map->bound.minLat;

                    if(strcmp(map->ways[compteur].type, "highway") == 0){
                        thickLineRGBA(ren, nodeX1*rapportX, HEIGHT-nodeY1*rapportY, nodeX2*rapportX, HEIGHT-nodeY2*rapportY, 2, 200, 200, 200, 90);
                        break;
                    }
                    else if(strcmp(map->ways[compteur].type, "waterway") == 0){
                        thickLineRGBA(ren, nodeX1*rapportX, HEIGHT-nodeY1*rapportY, nodeX2*rapportX, HEIGHT-nodeY2*rapportY, 2, 51, 100, 200, 90);
                        break;
                    }
                    else{
                        thickLineRGBA(ren, nodeX1*rapportX, HEIGHT-nodeY1*rapportY, nodeX2*rapportX, HEIGHT-nodeY2*rapportY, 2, 200, 50, 50, 90);
                        break;
                    }
                    compteur2++;
                }
            }
        }
        compteur++;
    }
    SDL_RenderPresent(ren);
}*/

/*void dessiner_highway(SDL_Renderer *ren, osm *map, float rapportX, float rapportY, int positionCurseur) {
    int compteur2 = 0;
    //tant qu'il y a un noeud appartenant à notre way
    while(map->ways[positionCurseur].ref[compteur2 + 1] != '\0') {

        unsigned long int key1 = map->ways[positionCurseur].ref[compteur2];
        unsigned long int key2 = map->ways[positionCurseur].ref[compteur2 + 1];
        node *node1 = g_hash_table_lookup(map->nodesHashTable, &key1);
        node *node2 = g_hash_table_lookup(map->nodesHashTable, &key2);
        //on calcule la distance entre l'origine et nos points
        float nodeX1 = (node1->longitude) - map->bound.minLon;
        float nodeY1 = (node1->latitude) - map->bound.minLat;
        float nodeX2 = (node2->longitude) - map->bound.minLon;
        float nodeY2 = (node2->latitude) - map->bound.minLat;

        thickLineRGBA(ren, nodeX1*rapportX, HEIGHT-nodeY1*rapportY, nodeX2*rapportX, HEIGHT-nodeY2*rapportY, 2, 200, 200, 200, 90);
    }
    SDL_RenderPresent(ren);
}

void dessiner_building(SDL_Renderer *ren, osm *map, float rapportX, float rapportY, int positionCurseur){}

void dessiner_waterway(SDL_Renderer *ren, osm *map, float rapportX, float rapportY, int positionCurseur){
    int compteur2 = 0;
    //tant qu'il y a un noeud appartenant à notre way
    while(map->ways[positionCurseur].ref[compteur2 + 1] != '\0') {

        unsigned long int key1 = map->ways[positionCurseur].ref[compteur2];
        unsigned long int key2 = map->ways[positionCurseur].ref[compteur2 + 1];
        node *node1 = g_hash_table_lookup(map->nodesHashTable, &key1);
        node *node2 = g_hash_table_lookup(map->nodesHashTable, &key2);
        //on calcule la distance entre l'origine et nos points
        float nodeX1 = (node1->longitude) - map->bound.minLon;
        float nodeY1 = (node1->latitude) - map->bound.minLat;
        float nodeX2 = (node2->longitude) - map->bound.minLon;
        float nodeY2 = (node2->latitude) - map->bound.minLat;

        thickLineRGBA(ren, nodeX1*rapportX, HEIGHT-nodeY1*rapportY, nodeX2*rapportX, HEIGHT-nodeY2*rapportY, 1, 51, 102, 153, 255);
    }
    SDL_RenderPresent(ren);
}

void dessiner_basic(SDL_Renderer *ren, osm *map, float rapportX, float rapportY, int positionCurseur){
    int compteur2 = 0;
    //tant qu'il y a un noeud appartenant à notre way
    while(map->ways[positionCurseur].ref[compteur2 + 1] != '\0') {

        unsigned long int key1 = map->ways[positionCurseur].ref[compteur2];
        unsigned long int key2 = map->ways[positionCurseur].ref[compteur2 + 1];
        node *node1 = g_hash_table_lookup(map->nodesHashTable, &key1);
        node *node2 = g_hash_table_lookup(map->nodesHashTable, &key2);
        //on calcule la distance entre l'origine et nos points
        float nodeX1 = (node1->longitude) - map->bound.minLon;
        float nodeY1 = (node1->latitude) - map->bound.minLat;
        float nodeX2 = (node2->longitude) - map->bound.minLon;
        float nodeY2 = (node2->latitude) - map->bound.minLat;

        thickLineRGBA(ren, nodeX1*rapportX, HEIGHT-nodeY1*rapportY, nodeX2*rapportX, HEIGHT-nodeY2*rapportY, 1, 200, 200, 200, 90);
    }
    SDL_RenderPresent(ren);
}
*/
