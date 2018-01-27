#include "parse1.h"

const float WIDTH = 800;
const float HEIGHT = 600;

void dessiner_way(SDL_Renderer *ren, way *way_array, GHashTable *nodesHashTable, float minlat, float minlon, float maxlat, float maxlon);
void dessiner_way_2(SDL_Renderer *ren, osm *map);
void dessiner_way_3(SDL_Renderer *ren, osm *map);
void dessiner_way_4(SDL_Renderer *ren, osm *map);
void dessiner_highway(SDL_Renderer *ren, osm *map, float rapportX, float rapportY, int positionCurseur);
void dessiner_building(SDL_Renderer *ren, osm *map, float rapportX, float rapportY, int positionCurseur);
void dessiner_waterway(SDL_Renderer *ren, osm *map, float rapportX, float rapportY, int positionCurseur);
void dessiner_basic(SDL_Renderer *ren, osm *map, float rapportX, float rapportY, int positionCurseur);

/*
* parseBounds : récupère les attributs de l'élément bounds
*/
bounds parseBounds(xmlNodePtr current){

    bounds bound ;
	//Récupération des dimension GPS du .osm
    bound.minLat = atof((char*)xmlGetProp(current,(const xmlChar *)"minlat"));
    bound.minLon = atof((char*)xmlGetProp(current,(const xmlChar *)"minlon"));
    bound.maxLat = atof((char*)xmlGetProp(current,(const xmlChar*)"maxlat"));
    bound.maxLon = atof((char*)xmlGetProp(current,(const xmlChar *)"maxlon"));
    return bound;
}

/*
* parseWay : récupère les attributs de l'élément way
*/
way parseWay(xmlNodePtr cur) {
    way ways ;
    ways.id = atoi((char*)xmlGetProp(cur,(const xmlChar*)"id"));

    if (xmlStrcmp(xmlGetProp(cur,(const xmlChar*)"visible"),(const xmlChar *)"true")==1) {
        ways.visible = true;
    } else {
        ways.visible = false;
    }
    ways.ref = NULL ;
    ways.tags = NULL ;
    ways.type = (char *)"way_par_defaut";

    /* tag pour ne pas allouer plusieurs fois le tableau tags de la structure */
    int tag = 1;
    /* ref pour ne pas allouer plusieurs fois le tableau ref de la structure */
    int ref = 1;
    /* n_tag permet de contrôler les réallocations  du tableau tags*/
    int n_tag = LENGTH_TAG_ARRAY ;
    /* n_ref permet de contrôler les réallocations  du tableau ref*/
    int n_ref = LENGTH_REF_ARRAY ;
    /* i_tag et i_ref servent   de curseur  aux tableaux tags et ref*/
    int i_tag = 0 ;
    int i_ref = 0 ;

    xmlNodePtr curs = cur->xmlChildrenNode ;

    while(curs != NULL)
    {   
        //suppression du ! pour la comparaison
        if (!(xmlStrcmp(curs->name,(const xmlChar *) "tag"))){
            if(tag) {
                ways.tags =(struct tag*) malloc(LENGTH_TAG_ARRAY * sizeof( struct tag));
                tag = 0 ;
            }
            (ways.tags[i_tag]).key = (char*)xmlGetProp(curs,(const xmlChar*)"k");
            ways.tags[i_tag].value = (char*)xmlGetProp(curs,(const xmlChar*)"v");

            if(strcmp(ways.type,(char *)"way_par_defaut") == 0){
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
                if(!xmlStrcmp(xmlGetProp(curs,(const xmlChar*)"k"),(const xmlChar*)"highway") && (!xmlStrcmp(xmlGetProp(curs,(const xmlChar*)"v"),(const xmlChar*)"motorway") || !xmlStrcmp(xmlGetProp(curs,(const xmlChar*)"v"),(const xmlChar*)"trunk"))){
                    ways.type = (char *)"motor_way";
                }
                else if(!xmlStrcmp(xmlGetProp(curs,(const xmlChar*)"k"),(const xmlChar*)"highway")){
                    ways.type = (char *)"normal_way";
                }
                else if(!xmlStrcmp(xmlGetProp(curs,(const xmlChar*)"k"),(const xmlChar*)"building")){
                    ways.type = (char *)"building_way";
                }
                else if(!xmlStrcmp(xmlGetProp(curs,(const xmlChar*)"k"),(const xmlChar*)"waterway") && !xmlStrcmp(xmlGetProp(curs,(const xmlChar*)"v"),(const xmlChar*)"riverbank")){
                	ways.type = (char *)"river_area";
                }
                else if(!xmlStrcmp(xmlGetProp(curs,(const xmlChar*)"k"),(const xmlChar*)"waterway")){
                    ways.type = (char *)"water_way";
                }
                else if(!xmlStrcmp(xmlGetProp(curs,(const xmlChar*)"k"),(const xmlChar*)"natural") && !xmlStrcmp(xmlGetProp(curs,(const xmlChar*)"v"),(const xmlChar*)"water")){
                	ways.type = (char *)"water_area";
                }
                else if(!xmlStrcmp(xmlGetProp(curs,(const xmlChar*)"k"),(const xmlChar*)"landuse") && !xmlStrcmp(xmlGetProp(curs,(const xmlChar*)"v"),(const xmlChar*)"grass")){
                	ways.type = (char *)"grass_area";
                }
                else if(!xmlStrcmp(xmlGetProp(curs,(const xmlChar*)"k"),(const xmlChar*)"leisure") && !xmlStrcmp(xmlGetProp(curs,(const xmlChar*)"v"),(const xmlChar*)"park")){
                	ways.type = (char *)"park_area";
                }
                else if(!xmlStrcmp(xmlGetProp(curs,(const xmlChar*)"k"),(const xmlChar*)"natural") && !xmlStrcmp(xmlGetProp(curs,(const xmlChar*)"v"),(const xmlChar*)"coastline")){
                	ways.type = (char *)"water_border";
                }
            }
            i_tag++;

            /*On verifie si le tableau  tags de  la structure node est plein, si oui on le réalloue*/
            if(i_tag==n_tag)
            {
                ways.tags = ( struct tag *)realloc( ways.tags,(LENGTH_TAG_ARRAY + n_tag)*sizeof( struct tag) );
                n_tag += LENGTH_TAG_ARRAY ;
            }
        }

        if ((!xmlStrcmp(curs->name,(const xmlChar *)"nd"))) {
            if(ref) {
                ways.ref = malloc(LENGTH_REF_ARRAY * sizeof(unsigned long int));
                ref = 0;
            }
            ways.ref[i_ref]=atoi((char*)xmlGetProp(curs,(const xmlChar*)"ref"));
            i_ref++;

            /*On verifie si le tableau  ref de  la structure node est plein, si oui on le réalloue*/
            if(i_ref==n_ref) {
                ways.ref = realloc( ways.ref,(LENGTH_REF_ARRAY + n_ref)*sizeof(unsigned long int)) ;
                n_ref += LENGTH_REF_ARRAY ;
            }
        }
        curs = curs->next ;
    }

    if(ways.tags!=NULL) {
        ways.tags[i_tag].key=NULL;
    }

    if(ways.ref!=NULL) {
        ways.ref[i_ref]='\0';
    }
    return ways;
}

/*
* parseNode: récupère les éléments node du document
*/
node parseNode(xmlNodePtr cur) {
    node nodes;

    nodes.id = atoi((char*)xmlGetProp(cur,(const xmlChar*)"id"));
    nodes.longitude = atof((char*)xmlGetProp(cur,(const xmlChar*)"lon"));
    nodes.latitude = atof((char*)xmlGetProp(cur,(const xmlChar*)"lat"));
    if (xmlStrcmp(xmlGetProp(cur,(const xmlChar*)"visible"),(const xmlChar *)"true")==1){
        nodes.visible = true;
    } else {
        nodes.visible = false;
    }

    nodes.tags = NULL;

    /* tag pour ne pas allouer plusieurs fois le tableau tags de la structure */
    int tag = 1;
    /* n permet de contrôler les réallocations */
    int n = LENGTH_TAG_ARRAY ;
    /* i sert  de curseur */
    int i = 1 ;

    xmlNodePtr curs = cur->xmlChildrenNode ;
    while(curs!= NULL) {
        if ((!xmlStrcmp(curs->name,(const xmlChar *)"tag"))) {
            if(tag) {
                nodes.tags =(struct tag*) malloc(LENGTH_TAG_ARRAY * sizeof( struct tag));
                tag = 0 ;
            }
            (nodes.tags[i]).key=(char*)xmlGetProp(curs,(const xmlChar*)"k");
            nodes.tags[i].value=(char*)xmlGetProp(curs,(const xmlChar*)"v");
            i++;

            /*On verifie si le tableau  tags de  la structure node est plein, si oui on le réalloue*/
            if(i==n) {
                nodes.tags = ( struct tag *)realloc( nodes.tags,(LENGTH_TAG_ARRAY + n)*sizeof( struct tag) ) ;
                n += LENGTH_TAG_ARRAY ;
            }
        }
        curs = curs->next ;
    }

    /*Marquer la fin du tableau tags*/
    if(nodes.tags!=NULL) {
        nodes.tags[i].key=NULL;
    }
    return nodes ;
}

/*
* parseDoc : Parse le document donné en argument
*/
error parseDoc(char * docname,osm *os) {

    xmlDocPtr doc ;
    xmlNodePtr cur ;
    doc = xmlParseFile(docname);
    if(doc==NULL) {
        fprintf(stderr,"Error parsing document :( \n)");
        return ERROR_PARSING ;
    }
    cur = xmlDocGetRootElement(doc);
    if(cur==NULL) {
        fprintf(stderr,"Empty document \n");
        xmlFreeDoc(doc);
        return EMPTY_DOC ;
    }

    if (xmlStrcmp(cur->name, (const xmlChar*)"osm")) {
        fprintf(stderr,"Wrong format root node != osm \n");
        xmlFreeDoc(doc);
        return WRONG_FORMAT ;
    }

    cur = (cur->xmlChildrenNode)->next ;

    if ((xmlStrcmp((cur->name),(const xmlChar*)"bounds"))) {
        fprintf(stderr, "Wrong format first child of osm != bounds");
        xmlFreeDoc(doc);
        return WRONG_FORMAT ;
    }

    os->bound= parseBounds(cur);
    cur = cur->next ;
    /*Tableau pour stocker les ways */
    way* way_array = (way *)malloc(LENGTH_WAY_ARRAY*sizeof(way));
    /* n_node et et n_way permettent  de contrôler les réallocations */
    int n_way = LENGTH_WAY_ARRAY ;

    /* i_node sert  de curseur aux nodes */
    int i_way = 0 ;


while(cur!=NULL) {
    if((!xmlStrcmp(cur->name,(const xmlChar*)"node"))) {
        node* nodeTemp = malloc(sizeof(node));
        *nodeTemp = parseNode(cur);
        g_hash_table_insert(os->nodesHashTable, &nodeTemp->id, nodeTemp);
    }

    if((!xmlStrcmp(cur->name,(const xmlChar*)"way"))) {
        way_array[i_way] = parseWay(cur) ;
        i_way ++ ;

        /*On verifie si le tableau est plein, si oui on le réalloue*/
        if(i_way==n_way) {
            way_array = (way *)realloc(way_array,(LENGTH_WAY_ARRAY + n_way)*sizeof(way)) ;
            n_way += LENGTH_WAY_ARRAY ;
        }
    }
    cur = cur->next ;
}

    /*Marquer la fin des tableaux */
    way_array[i_way].id='\0';

    os->ways = way_array;

    /* Free */
    xmlFreeDoc(doc) ;
    return 0;
}

/*
* afficher : Affiche le les nodes stockés dans la structure osm
*/
/*
void afficher(osm os) {

//Afficher bounds
    printf("bound minLon %f\n", os.bound.minLon);
    printf("bound minLat %f\n", os.bound.minLat);
    printf("bound maxLon %f\n", os.bound.maxLon);
    printf("bound maxLat %f\n", os.bound.maxLat);
    printf("\n\n\n");

//Afficher nodes
    node * nodes = os.nodes;

    while(nodes->id != '\0') {
        printf(" id node %lu\n",nodes->id );
        printf(" lon node%f\n", nodes->longitude);
        printf(" lat node%f\n", nodes->latitude);
        printf(" visible node%d\n", nodes->visible);
        printf("\n\n\n");

        struct tag* tags = nodes->tags ;

        while(tags!=NULL && tags->key != NULL) {
            printf("\t key%s\n", tags->key);
            printf("\t value%s\n", tags->value);
            tags++ ;
        }

        nodes++;
    }

    way* ways = os.ways ;

    while(ways->id != '\0') {

        printf(" id way %lu\n",ways->id );
        printf(" visible way %d\n",ways->visible );

        struct tag* tags = ways->tags ;

        while(tags!=NULL && tags->key != NULL) {
            printf("\t key%s\n", tags->key);
            printf("\t value%s\n", tags->value);
            tags++ ;
        }

        unsigned long int* ref = ways->ref ;

        int i = 0 ;
        while(ref[i]!='\0') {
            printf("ref: %lu\n", ref[i]);
            i++ ;
        }

        ways++;
    }


}*/

int main(int argc, char* argv[]) {

    if(argc!=2) {
        fprintf(stderr,"Erreurs arguments: parse <nom_fichier_valide_osm>\n");
        return 2;
    }
    osm os;
    os.nodesHashTable = g_hash_table_new(g_int64_hash ,g_int64_equal);
    int retour;
    if((retour=parseDoc(argv[1],&os))<0) {
        fprintf(stderr, "%d\n",retour );
        return retour;
    }

    //Enlever(ou mettre) les commentaires si vous voulez vérifier(ou non) si les éléments ont été bien récupérés
    //afficher(os);

	SDL_Window *win = 0;
    SDL_Renderer *ren = 0;
    //Initialisation de la SDL, en cas de problème on affiche un message d'erreur et on renvoie -1.
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "Erreur lors de l'initialisation de la SDL\n");
        return -1;
    }
    //Création de la fenêtre, en cas de problème on affiche un message d'erreur.
    win = SDL_CreateWindow("Renderer basique", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if(win){
        //Création du rendu, en cas de problème on affiche un message d'erreur.
        ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
        if(ren) {
        	SDL_SetRenderDrawColor(ren, 150, 150, 150, 200);
        	SDL_RenderClear(ren);
        	SDL_RenderPresent(ren);
            dessiner_way_4(ren, &os);
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

void dessiner_way(SDL_Renderer *ren, way* way_array, GHashTable * nodesHashTable, float minlat, float minlon, float maxlat, float maxlon) {
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
    rapportX = (WIDTH/(MAXLAT-MINLAT));
    rapportY = (HEIGHT/(MAXLON-MINLON));
    printf("Rapport et compteur\n");
    printf("%f\n", rapportX);
    printf("%f\n", rapportY);
    printf("%d\n", compteur);

	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

    //On parcourt le tableau de ways.
    //printf("TAILLE DE LA HASHMAP : %lu \n", sizeof(nodesHashTable));
    while(way_array[compteur].id != '\0') {
        int compteur2 = 0;
        //printf("premier while\n");
        //printf("%d", way_array[compteur].visible);
        //Si c'est visible on s'occupe d'afficher la way sinon pas besoin.
        if(way_array[compteur].visible == 0) {
            //printf("premier if\n");
            //On parcourt le tableau de nodes constituant la way.
            while(way_array[compteur].ref[compteur2 + 1] != '\0') {
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
                SDL_RenderDrawLine(ren,(nodeY1-MINLON)*rapportY,(nodeX1-MINLAT)*rapportX,(nodeY2-MINLON)*rapportY,(nodeX2-MINLAT)*rapportX);
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
//Fonction de dessin(routes,rivières,batiments, parcs, terrains herbeux, fleuves) intégrant un ordre d'affichage afin d'éviter d'avoir
//de l'eau par dessus une île, un pont etc.
void dessiner_way_4(SDL_Renderer *ren, osm *map){
    //compteur pour nos parcours consécutifs permettant de dessiner d'abord les étendues d'eau, puis les batiments, parcs etc puis les routes.
    int compteur_ordre = 0;
    //les coordonnées des deux points formants chaque segments composants les ways
    float nodeX1 = 0, nodeY1 = 0, nodeX2 = 0, nodeY2 = 0;
    //le rapport multipliant chaque coordonnée afin d'être à l'échelle de notre fenêtre
    float rapportX = 0, rapportY = 0;
    //on calcule nos rapports liés à l'axe des abscisses et à l'axe des ordonnées
    rapportX = (WIDTH / (map->bound.maxLon - map->bound.minLon));
    rapportY = (HEIGHT / (map->bound.maxLat - map->bound.minLat));

    for(compteur_ordre = 0 ; compteur_ordre < 4 ; compteur_ordre++){
    	//premier compteur pour parcourir notre tableau de ways
    	int compteur = 0;
    	//tant qu'on a encore une way dans notre tableau
    	while(map->ways[compteur].id != '\0') {
        	int compteur2 = 0;
        	//si la way doit être visible alors on continue
        	if(map->ways[compteur].visible == 0) {
        		if(compteur_ordre == 0 && (strcmp(map->ways[compteur].type, (char *)"river_area") == 0 || strcmp(map->ways[compteur].type, (char *)"water_area") == 0)){
        			int lengthTab = 50;
                	Sint16* xArray = malloc(lengthTab*sizeof(float));
                	Sint16* yArray = malloc(lengthTab*sizeof(float));
                	while(map->ways[compteur].ref[compteur2] != '\0') {
                    	if(compteur2 == lengthTab) {
                      	  xArray = realloc(xArray,(lengthTab + 50)*sizeof(float));
                      	  yArray = realloc(yArray,(lengthTab + 50)*sizeof(float));
                      	  lengthTab += 50;
                    	}
                    	unsigned long int key1 = map->ways[compteur].ref[compteur2];
                    	node *node1 = g_hash_table_lookup(map->nodesHashTable, &key1);
                    	nodeX1 = (node1->longitude) - (map->bound.minLon);
                    	nodeY1 = (node1->latitude) - (map->bound.minLat);
                    	nodeX1 = nodeX1*rapportX;
                    	nodeY1 = HEIGHT-(nodeY1*rapportY);
                    	xArray[compteur2] = nodeX1;
                   		yArray[compteur2] = nodeY1;
                    	compteur2++;
                	}
                	filledPolygonRGBA(ren, xArray, yArray, compteur2, 0, 51, 255, 200);
        		}
        		else if(compteur_ordre == 0 && (strcmp(map->ways[compteur].type, (char *)"water_way") == 0 || strcmp(map->ways[compteur].type, (char *)"water_border") == 0)){
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
                    	if(strcmp(map->ways[compteur].type, (char *)"water_way") == 0){
                    		thickLineRGBA(ren, nodeX1*rapportX, HEIGHT-nodeY1*rapportY, nodeX2*rapportX, HEIGHT-nodeY2*rapportY, 1, 0, 51, 255, 200);
                    	}
                    	else if(strcmp(map->ways[compteur].type, (char *)"water_border") == 0){
                    		thickLineRGBA(ren, nodeX1*rapportX, HEIGHT-nodeY1*rapportY, nodeX2*rapportX, HEIGHT-nodeY2*rapportY, 2, 255, 0, 0, 200);
                    	}
                   	 	compteur2++;
                	}
        		}
        		else if(compteur_ordre == 1 && (strcmp(map->ways[compteur].type, (char *)"park_area") == 0 || strcmp(map->ways[compteur].type, (char *)"grass_area") == 0)){
        			int lengthTab = 50;
                	Sint16* xArray = malloc(lengthTab*sizeof(float));
                	Sint16* yArray = malloc(lengthTab*sizeof(float));
                	while(map->ways[compteur].ref[compteur2] != '\0') {
                    	if(compteur2 == lengthTab) {
                      	  xArray = realloc(xArray,(lengthTab + 50)*sizeof(float));
                      	  yArray = realloc(yArray,(lengthTab + 50)*sizeof(float));
                      	  lengthTab += 50;
                    	}
                    	unsigned long int key1 = map->ways[compteur].ref[compteur2];
                    	node *node1 = g_hash_table_lookup(map->nodesHashTable, &key1);
                    	nodeX1 = (node1->longitude) - (map->bound.minLon);
                    	nodeY1 = (node1->latitude) - (map->bound.minLat);
                    	nodeX1 = nodeX1*rapportX;
                    	nodeY1 = HEIGHT-(nodeY1*rapportY);
                    	xArray[compteur2] = nodeX1;
                   		yArray[compteur2] = nodeY1;
                    	compteur2++;
                	}
                	if(strcmp(map->ways[compteur].type, (char *)"grass_area") == 0){
                		filledPolygonRGBA(ren, xArray, yArray, compteur2, 0, 180, 0, 200);
                		aapolygonRGBA(ren, xArray, yArray, compteur2, 0, 0, 0, 200);
                	}
                	else if(strcmp(map->ways[compteur].type, (char *)"park_area") == 0){
                		filledPolygonRGBA(ren, xArray, yArray, compteur2, 0, 100, 0, 200);
                		aapolygonRGBA(ren, xArray, yArray, compteur2, 0, 0, 0, 200);
                	}
        		}
        		else if(compteur_ordre == 2 && strcmp(map->ways[compteur].type, (char *)"building_way") == 0){
        			int lengthTab = 50;
                	Sint16* xArray = malloc(lengthTab*sizeof(float));
                	Sint16* yArray = malloc(lengthTab*sizeof(float));
                	while(map->ways[compteur].ref[compteur2] != '\0') {
                    	if(compteur2 == lengthTab) {
                      		xArray = realloc(xArray,(lengthTab + 50)*sizeof(float));
                      	  	yArray = realloc(yArray,(lengthTab + 50)*sizeof(float));
                      	  	lengthTab += 50;
                    	}
                    	unsigned long int key1 = map->ways[compteur].ref[compteur2];
                    	node *node1 = g_hash_table_lookup(map->nodesHashTable, &key1);
                    	nodeX1 = (node1->longitude) - (map->bound.minLon);
                    	nodeY1 = (node1->latitude) - (map->bound.minLat);
                    	nodeX1 = nodeX1*rapportX;
                    	nodeY1 = HEIGHT-(nodeY1*rapportY);
                    	xArray[compteur2] = nodeX1;
                   		yArray[compteur2] = nodeY1;
                    	compteur2++;
                	}
                	filledPolygonRGBA(ren, xArray, yArray, compteur2, 100, 100, 100, 200);
                	aapolygonRGBA(ren, xArray, yArray, compteur2, 0, 0, 0, 200);
        		}
        		else if(compteur_ordre == 3 && (strcmp(map->ways[compteur].type, (char *)"normal_way") == 0 || strcmp(map->ways[compteur].type, (char *)"motor_way") == 0)){
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
                    	if(strcmp(map->ways[compteur].type, (char *)"normal_way") == 0){
                    		thickLineRGBA(ren, nodeX1*rapportX, HEIGHT-nodeY1*rapportY, nodeX2*rapportX, HEIGHT-nodeY2*rapportY, 2, 200, 200, 200, 200);
                    	}
                    	else if(strcmp(map->ways[compteur].type, (char *)"motor_way") == 0){
                    		thickLineRGBA(ren, nodeX1*rapportX, HEIGHT-nodeY1*rapportY, nodeX2*rapportX, HEIGHT-nodeY2*rapportY, 2, 100, 0, 250, 250);
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

void dessiner_way_3(SDL_Renderer *ren, osm *map){
    //premier compteur pour parcourir notre tableau de ways
    int compteur = 0;
    //les coordonnées des deux points formants chaque segments composants les ways
    float nodeX1 = 0, nodeY1 = 0, nodeX2 = 0, nodeY2 = 0;
    //le rapport multipliant chaque coordonnée afin d'être à l'échelle de notre fenêtre
    float rapportX = 0, rapportY = 0;
    //on calcule nos rapports liés à l'axe des abscisses et à l'axe des ordonnées
    rapportX = (WIDTH / (map->bound.maxLon - map->bound.minLon));
    rapportY = (HEIGHT / (map->bound.maxLat - map->bound.minLat));
    //tant qu'on a encore une way dans notre tableau
    while(map->ways[compteur].id != '\0') {
        int compteur2 = 0;
        //si la way doit être visible alors on continue
        if(map->ways[compteur].visible == 0) {
            //fprintf(stdout, "la way est visible\n");
            //On va dessiner différemment selon la nature de notre objet
            //si on a affaire à un batiment on va récupérer l'abscisse et l'ordonnée de chaque node de notre batiment, on va les mettre
            //dans un tableau d'abscisses et et dans un tableau d'ordonnées afin de les passer en paramètre de notre fonction de gfx
            //dessinant un polygone au nombre quelconque de points
            if(strcmp(map->ways[compteur].type, (char *)"building_way") == 0 || strcmp(map->ways[compteur].type, (char *)"grass_area") == 0 || strcmp(map->ways[compteur].type, (char *)"water_area") == 0 || strcmp(map->ways[compteur].type, (char *)"river_area") == 0 || strcmp(map->ways[compteur].type, (char *)"park_area") == 0){
                //fprintf(stdout, "on a un batiment\n");
                int lengthTab = 50;
                Sint16* xArray = malloc(lengthTab*sizeof(float));
                Sint16* yArray = malloc(lengthTab*sizeof(float));
                while(map->ways[compteur].ref[compteur2] != '\0') {
                    if(compteur2 == lengthTab) {
                        xArray = realloc(xArray,(lengthTab + 50)*sizeof(float));
                        yArray = realloc(yArray,(lengthTab + 50)*sizeof(float));
                        lengthTab += 50;
                    }
                    unsigned long int key1 = map->ways[compteur].ref[compteur2];
                    //unsigned long int key2 = map->ways[compteur].ref[compteur2 + 1];
                    node *node1 = g_hash_table_lookup(map->nodesHashTable, &key1);
                    //node *node2 = g_hash_table_lookup(map->nodesHashTable, &key2);
                    //on calcule la distance entre l'origine et nos points
                    nodeX1 = (node1->longitude) - (map->bound.minLon);
                    nodeY1 = (node1->latitude) - (map->bound.minLat);
                    nodeX1 = nodeX1*rapportX;
                    nodeY1 = HEIGHT-(nodeY1*rapportY);
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
                if(strcmp(map->ways[compteur].type, (char *)"building_way") == 0){
                	filledPolygonRGBA(ren, xArray, yArray, compteur2, 100, 100, 100, 200);
                	aapolygonRGBA(ren, xArray, yArray, compteur2, 0, 0, 0, 200);
                }
                else if(strcmp(map->ways[compteur].type, (char *)"grass_area") == 0){
                	filledPolygonRGBA(ren, xArray, yArray, compteur2, 0, 180, 0, 200);
                }
                else if(strcmp(map->ways[compteur].type, (char *)"river_area") == 0){
                	filledPolygonRGBA(ren, xArray, yArray, compteur2, 0, 51, 255, 200);
                }
                else if(strcmp(map->ways[compteur].type, (char *)"water_area") == 0){
                	filledPolygonRGBA(ren, xArray, yArray, compteur2, 0, 51, 255, 200);
                }
                else if(strcmp(map->ways[compteur].type, (char *)"park_area") == 0){
                	filledPolygonRGBA(ren, xArray, yArray, compteur2, 0, 100, 0, 200);
                }
            }
                
            //si on a affaire à une way 
            else if(strcmp(map->ways[compteur].type, (char *)"water_way") == 0 || strcmp(map->ways[compteur].type, (char *)"normal_way") == 0){
                //fprintf(stdout, "on a une waterway\n");
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
                    if(strcmp(map->ways[compteur].type, (char *)"water_way") == 0){
                    	thickLineRGBA(ren, nodeX1*rapportX, HEIGHT-nodeY1*rapportY, nodeX2*rapportX, HEIGHT-nodeY2*rapportY, 1, 0, 51, 255, 200);
                    }
                    else if(strcmp(map->ways[compteur].type, (char *)"normal_way") == 0){
                    	thickLineRGBA(ren, nodeX1*rapportX, HEIGHT-nodeY1*rapportY, nodeX2*rapportX, HEIGHT-nodeY2*rapportY, 2, 200, 200, 200, 200);
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

void dessiner_way_2(SDL_Renderer *ren, osm *map) {
	//premier compteur pour parcourir notre tableau de ways
	int compteur = 0;
	//les coordonnées des deux poinnts formants chaque segments composants les ways
	float nodeX1 = 0, nodeY1 = 0, nodeX2 = 0, nodeY2 = 0;
	//le rapport multipliant chaque coordonnée afin d'être à l'échelle de notre fenêtre
    float rapportX = 0, rapportY = 0;
    //on calcule nos rapports liés à l'axe des abscisses et à l'axe des ordonnées
    rapportX = (WIDTH / (map->bound.maxLon - map->bound.minLon));
    rapportY = (HEIGHT / (map->bound.maxLat - map->bound.minLat));
    //tant qu'on a encore une way dans notre tableau
    while(map->ways[compteur].id != '\0') {
        int compteur2 = 0;
        //si la way doit être visible alors on continue
        if(map->ways[compteur].visible == 0) {
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
                if(strcmp(map->ways[compteur].type, (char *)"normal_way") == 0){
                    fprintf(stdout,"%s\n",map->ways[compteur].type);
                    fprintf(stdout,"%f\t%f\t%f\t%f\n", nodeX1*rapportX, HEIGHT-nodeY1*rapportY, nodeX2*rapportX, HEIGHT-nodeY2*rapportY);
                    thickLineRGBA(ren, nodeX1*rapportX, HEIGHT-nodeY1*rapportY, nodeX2*rapportX, HEIGHT-nodeY2*rapportY, 1, 200, 200, 200, 90);
                }
                else if(strcmp(map->ways[compteur].type, (char *)"water_way") == 0){
                    fprintf(stdout,"%s\n",map->ways[compteur].type);
                    thickLineRGBA(ren, nodeX1*rapportX, HEIGHT-nodeY1*rapportY, nodeX2*rapportX, HEIGHT-nodeY2*rapportY, 2, 50, 100, 200, 90);
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