#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <glib.h>
#include <math.h>
#include "dataStructures.h"

#define WIDTH 400
#define HEIGHT 533

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
                  float maxlat, float maxlon);

/*
 * dessiner_way_gfx : utilise gfx pour obtenir un affichage colorisé de la carte
 * @param *ren : pointer vers la fenetre où sera affiché la carte
 * @param *map : structure de données contenant toutes informations extraites du xml .osm
 */
void dessiner_way_gfx(SDL_Renderer *ren, osm *map);

/*
 * dessiner_way_3 : utilise gfx pour obtenir un affichage partiellement colorisé de la carte
 * @param *ren : pointer vers la fenetre où sera affiché la carte
 * @param *map : structure de données contenant toutes informations extraites du xml .osm
 */
void dessiner_way_3(SDL_Renderer *ren, osm *map);

/*
 * dessiner_way_2 : utilise gfx pour obtenir un affichage des routes de la carte
 * @param *ren : pointer vers la fenetre où sera affiché la carte
 * @param *map : structure de données contenant toutes informations extraites du xml .osm
 */
void dessiner_way_2(SDL_Renderer *ren, osm *map);


//TODO the 4 bellow
void dessiner_highway(SDL_Renderer *ren, osm *map, float rapportX, float rapportY, int positionCurseur);

void dessiner_building(SDL_Renderer *ren, osm *map, float rapportX, float rapportY, int positionCurseur);

void dessiner_waterway(SDL_Renderer *ren, osm *map, float rapportX, float rapportY, int positionCurseur);

void dessiner_basic(SDL_Renderer *ren, osm *map, float rapportX, float rapportY, int positionCurseur);

#endif