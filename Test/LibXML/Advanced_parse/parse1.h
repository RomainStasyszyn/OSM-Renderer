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
#include "error.h"




/*Structure pour stocker les attributs k et v d'un élément tag*/


struct tag
{

  xmlChar *key ;
  xmlChar *value ;

};


/*Structure pour stocker les attributs des noeuds*/


typedef struct
{
  xmlChar* id ;/*id d'un noeud */ ;
  xmlChar *lon ;/* lon d'un noeud */ ;
  xmlChar *lat ; /* lat d'un noeud */ ;
  struct tag *tags ; /* tableau de tag */ ;
  int visible ;

}node ;


/*Structure pour stocker les attributs de l'élément bounds*/

typedef struct
{
  xmlChar *minlon ;
  xmlChar *minlat;
  xmlChar *maxlat ;
  xmlChar *maxlon ;

}bounds ;


/*Structure pour stocker les attributs d'un élément way*/

typedef struct
{
  xmlChar* id; /* id d'un élément way */
  int visible ; /*la visibilté d'un chemin */
  xmlChar** ref ; /* les attributs ref de l'élément nd dans way */
  struct tag *tags ;/*tableau de tags */

}way ;



/*Structure pour stocker tous les éléments d'une carte*/

typedef struct
{
  bounds bound ;
  node* nodes ;
  way* ways ;

}osm ;

/*parseBounds : récupère les attributs de l'élément bounds*/

bounds parseBounds(xmlNodePtr node) ;


/*parseNode: récupère les éléments node du document*/

node parseNode(xmlNodePtr cur) ;


/* Parse le document donné en argument*/


error parseDoc(char * docname, osm * os);


/*parseWay : récupère les attributs de l'élément way*/

way parseWay(xmlNodePtr cur) ;


/*Afficher les valeurs de la structure osm*/

void afficher(osm os );
