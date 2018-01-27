#ifndef PARSER_H
#define PARSER_H

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
#include <math.h>
#include "error.h"
#include "dataStructures.h"

/*
 * parseBounds : récupère les attributs de l'élément bounds
 * @param node : pointer vers le node contenant les coordonnées de la carte
 * @param out : structure bounds contenant les coordonnées gps des limites de la carte
 */
bounds parseBounds(xmlNodePtr node);

/*
 * parseNode: récupère les éléments node du document
 * @param cur : pointer vers le node contenant les informations d'un node
 * @param out : structure node contenant les coordonnées GPS d'un point et ses autres attributs
 */
node parseNode(xmlNodePtr cur);

/*
 * parseWay : récupère les attributs de l'élément way
 * @param cur : pointer vers le node contenant les informations d'une way
 * @param out : structure way contenant les informations indiquant quelles nodes la constitue
 */
way parseWay(xmlNodePtr cur);

/*
 * Parse le document donné en argument
 * @param *docname : Path du documents dont on cherche à lire les informations
 * @param *os : pointer vers la structure osm où seront stockées toutes les informations
 * @param out : 0 ou un code d'erreur, cf error.h
 */
error parseDoc(char *docname, osm *os);


#endif