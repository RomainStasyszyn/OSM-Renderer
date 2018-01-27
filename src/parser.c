#include "../header/parser.h"

/*
 * parseBounds : récupère les attributs de l'élément bounds
 * @param node : pointer vers le node contenant les coordonnées de la carte
 * @param out : structure bounds contenant les coordonnées gps des limites de la carte
 */
bounds parseBounds(xmlNodePtr current) {

    bounds bound;
    //Récupération des dimension GPS du .osm
    bound.minLat = atof((char *) xmlGetProp(current, (const xmlChar *) "minlat"));
    bound.minLon = atof((char *) xmlGetProp(current, (const xmlChar *) "minlon"));
    bound.maxLat = atof((char *) xmlGetProp(current, (const xmlChar *) "maxlat"));
    bound.maxLon = atof((char *) xmlGetProp(current, (const xmlChar *) "maxlon"));
    return bound;
}

/*
 * parseNode: récupère les éléments node du document
 * @param cur : pointer vers le node contenant les informations d'un node
 * @param out : structure node contenant les coordonnées GPS d'un point et ses autres attributs
 */
node parseNode(xmlNodePtr cur) {
    node nodes;

    nodes.id = atoi((char *) xmlGetProp(cur, (const xmlChar *) "id"));
    nodes.longitude = atof((char *) xmlGetProp(cur, (const xmlChar *) "lon"));
    nodes.latitude = atof((char *) xmlGetProp(cur, (const xmlChar *) "lat"));
    if (xmlStrcmp(xmlGetProp(cur, (const xmlChar *) "visible"), (const xmlChar *) "true") == 1) {
        nodes.visible = true;
    } else {
        nodes.visible = false;
    }

    nodes.tags = NULL;

    /* tag pour ne pas allouer plusieurs fois le tableau tags de la structure */
    int tag = 1;
    /* n permet de contrôler les réallocations */
    int n = LENGTH_TAG_ARRAY;
    /* i sert  de curseur */
    int i = 1;

    xmlNodePtr curs = cur->xmlChildrenNode;
    while (curs != NULL) {
        if ((!xmlStrcmp(curs->name, (const xmlChar *) "tag"))) {
            if (tag) {
                nodes.tags = (struct tag *) malloc(LENGTH_TAG_ARRAY * sizeof(struct tag));
                tag = 0;
            }
            (nodes.tags[i]).key = (char *) xmlGetProp(curs, (const xmlChar *) "k");
            nodes.tags[i].value = (char *) xmlGetProp(curs, (const xmlChar *) "v");
            i++;

            /*On verifie si le tableau  tags de  la structure node est plein, si oui on le réalloue*/
            if (i == n) {
                nodes.tags = (struct tag *) realloc(nodes.tags, (LENGTH_TAG_ARRAY + n) * sizeof(struct tag));
                n += LENGTH_TAG_ARRAY;
            }
        }
        curs = curs->next;
    }

    /*Marquer la fin du tableau tags*/
    if (nodes.tags != NULL) {
        nodes.tags[i].key = NULL;
    }
    return nodes;
}

/*
 * parseWay : récupère les attributs de l'élément way
 * @param cur : pointer vers le node contenant les informations d'une way
 * @param out : structure way contenant les informations indiquant quelles nodes la constitue
 */
way parseWay(xmlNodePtr cur) {
    way ways;
    ways.id = atoi((char *) xmlGetProp(cur, (const xmlChar *) "id"));

    if (xmlStrcmp(xmlGetProp(cur, (const xmlChar *) "visible"), (const xmlChar *) "true") == 1) {
        ways.visible = true;
    } else {
        ways.visible = false;
    }
    ways.ref = NULL;
    ways.tags = NULL;
    ways.type = (char *) "way_par_defaut";

    /* tag pour ne pas allouer plusieurs fois le tableau tags de la structure */
    int tag = 1;
    /* ref pour ne pas allouer plusieurs fois le tableau ref de la structure */
    int ref = 1;
    /* n_tag permet de contrôler les réallocations  du tableau tags*/
    int n_tag = LENGTH_TAG_ARRAY;
    /* n_ref permet de contrôler les réallocations  du tableau ref*/
    int n_ref = LENGTH_REF_ARRAY;
    /* i_tag et i_ref servent   de curseur  aux tableaux tags et ref*/
    int i_tag = 0;
    int i_ref = 0;

    xmlNodePtr curs = cur->xmlChildrenNode;

    while (curs != NULL) {
        //suppression du ! pour la comparaison
        if (!(xmlStrcmp(curs->name, (const xmlChar *) "tag"))) {
            if (tag) {
                ways.tags = (struct tag *) malloc(LENGTH_TAG_ARRAY * sizeof(struct tag));
                tag = 0;
            }
            (ways.tags[i_tag]).key = (char *) xmlGetProp(curs, (const xmlChar *) "k");
            ways.tags[i_tag].value = (char *) xmlGetProp(curs, (const xmlChar *) "v");

            if (strcmp(ways.type, (char *) "way_par_defaut") == 0) {
                /*if(xmlStrcmp(xmlGetProp(curs,(const xmlChar*)"k"),(const xmlChar*)"highway") == 1){
                ways.type = (char*)"highway";
                }
                else if(xmlStrcmp(xmlGetProp(curs,(const xmlChar*)"k"),(const xmlChar*)"building") == 1){
                  ways.type = (char*)"building";
                }
                else if(xmlStrcmp(xmlGetProp(curs,(const xmlChar*)"k"),(const xmlChar*)"waterway") == 1){
                    ways.type = (char*)"waterway";
                }
                else if(xmlStrcmp(xmlGetProp(curs,(const xmlChar*)"k"),(const xmlChar*)"coastline") == 1){
                   ways.type = (char*)"coastline";
                }*/
                if (!xmlStrcmp(xmlGetProp(curs, (const xmlChar *) "k"), (const xmlChar *) "highway") &&
                    (!xmlStrcmp(xmlGetProp(curs, (const xmlChar *) "v"), (const xmlChar *) "motorway") ||
                     !xmlStrcmp(xmlGetProp(curs, (const xmlChar *) "v"), (const xmlChar *) "trunk"))) {
                    ways.type = (char *) "motor_way";
                }
                else if (!xmlStrcmp(xmlGetProp(curs, (const xmlChar *) "k"), (const xmlChar *) "highway")) {
                    ways.type = (char *) "normal_way";
                }
                else if (!xmlStrcmp(xmlGetProp(curs, (const xmlChar *) "k"), (const xmlChar *) "building")) {
                    ways.type = (char *) "building_way";
                }
                else if (!xmlStrcmp(xmlGetProp(curs, (const xmlChar *) "k"), (const xmlChar *) "waterway") &&
                         !xmlStrcmp(xmlGetProp(curs, (const xmlChar *) "v"), (const xmlChar *) "riverbank")) {
                    ways.type = (char *) "river_area";
                }
                else if (!xmlStrcmp(xmlGetProp(curs, (const xmlChar *) "k"), (const xmlChar *) "waterway")) {
                    ways.type = (char *) "water_way";
                }
                else if (!xmlStrcmp(xmlGetProp(curs, (const xmlChar *) "k"), (const xmlChar *) "natural") &&
                         !xmlStrcmp(xmlGetProp(curs, (const xmlChar *) "v"), (const xmlChar *) "water")) {
                    ways.type = (char *) "water_area";
                }
                else if (!xmlStrcmp(xmlGetProp(curs, (const xmlChar *) "k"), (const xmlChar *) "landuse") &&
                         !xmlStrcmp(xmlGetProp(curs, (const xmlChar *) "v"), (const xmlChar *) "grass")) {
                    ways.type = (char *) "grass_area";
                }
                else if (!xmlStrcmp(xmlGetProp(curs, (const xmlChar *) "k"), (const xmlChar *) "leisure") &&
                         !xmlStrcmp(xmlGetProp(curs, (const xmlChar *) "v"), (const xmlChar *) "park")) {
                    ways.type = (char *) "park_area";
                }
                else if (!xmlStrcmp(xmlGetProp(curs, (const xmlChar *) "k"), (const xmlChar *) "natural") &&
                         !xmlStrcmp(xmlGetProp(curs, (const xmlChar *) "v"), (const xmlChar *) "coastline")) {
                    ways.type = (char *) "water_border";
                }
            }
            i_tag++;

            /*On verifie si le tableau  tags de  la structure node est plein, si oui on le réalloue*/
            if (i_tag == n_tag) {
                ways.tags = (struct tag *) realloc(ways.tags, (LENGTH_TAG_ARRAY + n_tag) * sizeof(struct tag));
                n_tag += LENGTH_TAG_ARRAY;
            }
        }

        if ((!xmlStrcmp(curs->name, (const xmlChar *) "nd"))) {
            if (ref) {
                ways.ref = malloc(LENGTH_REF_ARRAY * sizeof(unsigned long int));
                ref = 0;
            }
            ways.ref[i_ref] = atoi((char *) xmlGetProp(curs, (const xmlChar *) "ref"));
            i_ref++;

            /*On verifie si le tableau  ref de  la structure node est plein, si oui on le réalloue*/
            if (i_ref == n_ref) {
                ways.ref = realloc(ways.ref, (LENGTH_REF_ARRAY + n_ref) * sizeof(unsigned long int));
                n_ref += LENGTH_REF_ARRAY;
            }
        }
        curs = curs->next;
    }

    if (ways.tags != NULL) {
        ways.tags[i_tag].key = NULL;
    }

    if (ways.ref != NULL) {
        ways.ref[i_ref] = '\0';
    }
    return ways;
}

/*
 * Parse le document donné en argument
 * @param *docname : Path du documents dont on cherche à lire les informations
 * @param *os : pointer vers la structure osm où seront stockées toutes les informations
 * @param out : 0 ou un code d'erreur, cf error.h
 */
error parseDoc(char *docname, osm *os) {

    xmlDocPtr doc;
    xmlNodePtr cur;
    doc = xmlParseFile(docname);
    if (doc == NULL) {
        fprintf(stderr, "Error parsing document :( \n)");
        return ERROR_PARSING;
    }
    cur = xmlDocGetRootElement(doc);
    if (cur == NULL) {
        fprintf(stderr, "Empty document \n");
        xmlFreeDoc(doc);
        return EMPTY_DOC;
    }

    if (xmlStrcmp(cur->name, (const xmlChar *) "osm")) {
        fprintf(stderr, "Wrong format root node != osm \n");
        xmlFreeDoc(doc);
        return WRONG_FORMAT;
    }

    cur = (cur->xmlChildrenNode)->next;

    if ((xmlStrcmp((cur->name), (const xmlChar *) "bounds"))) {
        fprintf(stderr, "Wrong format first child of osm != bounds");
        xmlFreeDoc(doc);
        return WRONG_FORMAT;
    }

    os->bound = parseBounds(cur);
    cur = cur->next;
    /*Tableau pour stocker les ways */
    way *way_array = (way *) malloc(LENGTH_WAY_ARRAY * sizeof(way));
    /* n_node et et n_way permettent  de contrôler les réallocations */
    int n_way = LENGTH_WAY_ARRAY;

    /* i_node sert  de curseur aux nodes */
    int i_way = 0;


    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "node"))) {
            node *nodeTemp = malloc(sizeof(node));
            *nodeTemp = parseNode(cur);
            g_hash_table_insert(os->nodesHashTable, &nodeTemp->id, nodeTemp);
        }

        if ((!xmlStrcmp(cur->name, (const xmlChar *) "way"))) {
            way_array[i_way] = parseWay(cur);
            i_way++;

            /*On verifie si le tableau est plein, si oui on le réalloue*/
            if (i_way == n_way) {
                way_array = (way *) realloc(way_array, (LENGTH_WAY_ARRAY + n_way) * sizeof(way));
                n_way += LENGTH_WAY_ARRAY;
            }
        }
        cur = cur->next;
    }

    /*Marquer la fin des tableaux */
    way_array[i_way].id = '\0';

    os->ways = way_array;

    /* Free */
    xmlFreeDoc(doc);
    return 0;
}
