#define LENGTH_NODE_ARRAY 150
#define LENGTH_WAY_ARRAY 20
#define LENGTH_TAG_ARRAY 4
#define LENGTH_REF_ARRAY 10
#define LENGTH_XML_STRING 15

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <glib.h>
#include <SDL2/SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <math.h>
#include "error.h"
#include "../../dataStructurePrototype.h"

/*parseBounds : récupère les attributs de l'élément bounds*/
bounds parseBounds(xmlNodePtr node);

/*parseNode: récupère les éléments node du document*/
node parseNode(xmlNodePtr cur);

/* Parse le document donné en argument*/
error parseDoc(char * docname, osm * os);

/*parseWay : récupère les attributs de l'élément way*/
way parseWay(xmlNodePtr cur);

/*Afficher les valeurs de la structure osm*/
void afficher(osm os );
