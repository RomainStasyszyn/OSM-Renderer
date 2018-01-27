#include "parse1.h"


/*parseBounds : récupère les attributs de l'élément bounds*/


bounds parseBounds(xmlNodePtr cur)
{

    bounds bound ;

    /*Récupérer minlat */
    bound.minlat=xmlGetProp(cur,(const xmlChar *)"minlat");

    /*Récupérer minlon */
    bound.minlon=xmlGetProp(cur,(const xmlChar *)"minlon");

    /*Récupérer maxlat*/
    bound.maxlat = xmlGetProp(cur,(const xmlChar*)"maxlat");

    /*Récupérer maxlon */
    bound.maxlon=xmlGetProp(cur,(const xmlChar *)"maxlon") ;

    return bound;

}


/*parseWay : récupère les attributs de l'élément way*/

way parseWay(xmlNodePtr cur)
{
  way ways ;

  ways.id = xmlGetProp(cur,(const xmlChar*)"id");

  ways.visible= xmlStrcmp(xmlGetProp(cur,(const xmlChar*)"visible"),(const xmlChar *)"true");

  ways.ref = NULL ;

  ways.tags = NULL ;

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

  while(curs!= NULL)
  {

    if ( ( !xmlStrcmp(curs->name,(const xmlChar *) "tag" ) ) )
    {

      if(tag)
      {
        ways.tags =(struct tag*) malloc(LENGTH_TAG_ARRAY * sizeof( struct tag));
        tag = 0 ;

      }
      (ways.tags[i_tag]).key=xmlGetProp(curs,(const xmlChar*)"k");

      ways.tags[i_tag].value=xmlGetProp(curs,(const xmlChar*)"v");

      i_tag++;

      /*On verifie si le tableau  tags de  la structure node est plein */
      /*, si oui on le réalloue*/

      if(i_tag==n_tag)
      {

        ways.tags = ( struct tag *)realloc( ways.tags,(LENGTH_TAG_ARRAY + n_tag)*sizeof( struct tag) ) ;

        n_tag += LENGTH_TAG_ARRAY ;

      }

    }

    if ( ( !xmlStrcmp(curs->name,(const xmlChar *) "nd" ) ) )
    {

      if(ref)
      {
        ways.ref =(xmlChar **) malloc(LENGTH_REF_ARRAY * sizeof(xmlChar*));
        ref = 0 ;

      }
      ways.ref[i_ref]=xmlGetProp(curs,(const xmlChar*)"ref");

      i_ref++;

      /*On verifie si le tableau  ref de  la structure node est plein */
      /*, si oui on le réalloue*/

      if(i_ref==n_ref)
      {

        ways.ref = ( xmlChar **)realloc( ways.ref,(LENGTH_REF_ARRAY + n_ref)*sizeof(xmlChar *) ) ;

        n_ref += LENGTH_REF_ARRAY ;

      }

    }

    curs = curs->next ;

  }

  if(ways.tags!=NULL)
  {

    ways.tags[i_tag].key=NULL;

  }

  if(ways.ref!=NULL)
  {

    ways.ref[i_ref]=NULL ;

  }
  return ways ;

}


/*parseNode: récupère les éléments node du document*/


node parseNode(xmlNodePtr cur)
{
  node nodes ;

  nodes.id=xmlGetProp(cur,(const xmlChar*)"id");

  nodes.lon=xmlGetProp(cur,(const xmlChar*)"lon");

  nodes.lat=xmlGetProp(cur,(const xmlChar*)"lat") ;

  nodes.visible = xmlStrcmp(xmlGetProp(cur,(const xmlChar*)"visible"),(const xmlChar *)"true");


  nodes.tags = NULL ;

  /* tag pour ne pas allouer plusieurs fois le tableau tags de la structure */
  int tag = 1;

  /* n permet de contrôler les réallocations */

  int n = LENGTH_TAG_ARRAY ;

  /* i sert  de curseur */

  int i = 1 ;

  xmlNodePtr curs = cur->xmlChildrenNode ;


  while(curs!= NULL)
  {

    if ( ( !xmlStrcmp(curs->name,(const xmlChar *) "tag" ) ) )
    {

      if(tag)
      {
        nodes.tags =(struct tag*) malloc(LENGTH_TAG_ARRAY * sizeof( struct tag));
        tag = 0 ;

      }
      (nodes.tags[i]).key=xmlGetProp(curs,(const xmlChar*)"k");

      nodes.tags[i].value=xmlGetProp(curs,(const xmlChar*)"v");

      i++;

      /*On verifie si le tableau  tags de  la structure node est plein */
      /*, si oui on le réalloue ****************************************/

      if(i==n)
      {

        nodes.tags = ( struct tag *)realloc( nodes.tags,(LENGTH_TAG_ARRAY + n)*sizeof( struct tag) ) ;

        n += LENGTH_TAG_ARRAY ;

      }

    }

    curs = curs->next ;

  }

  /*Marquer la fin du tableau tags*/
  if(nodes.tags!=NULL)
  {
    nodes.tags[i].key=NULL;

  }

  return nodes ;

}




/* Parse le document donné en argument*/


error parseDoc(char * docname,osm *os)
{

  xmlDocPtr doc ;

  xmlNodePtr cur ;

  doc = xmlParseFile(docname);

  if(doc==NULL)
  {

    fprintf(stderr,"Error parsing document :( \n)");
    return ERROR_PARSING ;

  }

  cur = xmlDocGetRootElement(doc);

  if(cur==NULL)
  {

    fprintf(stderr,"Empty document \n");

    xmlFreeDoc(doc);

    return EMPTY_DOC ;

  }

  if ( xmlStrcmp(cur->name, (const xmlChar*)"osm") )
  {
    fprintf(stderr,"Wrong format root node != osm \n");

    xmlFreeDoc(doc);

    return WRONG_FORMAT ;

  }

  cur = (cur->xmlChildrenNode)->next ;

  if ( ( xmlStrcmp((cur->name),(const xmlChar*)"bounds")))
  {
    fprintf(stderr, "Wrong format first child of osm != bounds");

    xmlFreeDoc(doc);

    return WRONG_FORMAT ;

  }

  os->bound= parseBounds(cur);

  cur = cur->next ;

  /* Tableau pour stocker les noeuds */
  node* node_array = ( node *)malloc(LENGTH_NODE_ARRAY*sizeof(node));

  /*Tableau pour stocker les ways */
  way* way_array = (way *)malloc(LENGTH_WAY_ARRAY*sizeof(way));

  /* n_node et et n_way permettent  de contrôler les réallocations */

  int n_node = LENGTH_NODE_ARRAY ;

  int n_way = LENGTH_WAY_ARRAY ;

  /* i_node sert  de curseur aux nodes */

  int i_node= 0 ;

  int i_way = 0 ;

  while(cur!=NULL)
  {
    if( ( !xmlStrcmp(cur->name,(const xmlChar*)"node" ) ))
    {

      node_array[i_node] = parseNode(cur) ;

      i_node ++ ;

      /*On verifie si le tableau est plein */
      /*, si oui on le réalloue ************/

      if(i_node==n_node)
      {

        node_array = (node *)realloc(node_array,(LENGTH_NODE_ARRAY + n_node)*sizeof(node)) ;

        n_node += LENGTH_NODE_ARRAY ;

      }

    }

    if( ( !xmlStrcmp(cur->name,(const xmlChar*)"way" ) ))
    {

      way_array[i_way] = parseWay(cur) ;

      i_way ++ ;

      /*On verifie si le tableau est plein */
      /*, si oui on le réalloue ************/

        if(i_way==n_way)
        {

          way_array = (way *)realloc(way_array,(LENGTH_WAY_ARRAY + n_way)*sizeof(way)) ;

          n_way += LENGTH_WAY_ARRAY ;

        }

      }
    cur = cur->next ;

  }

  /*Marquer la fin des tableaux */
  node_array[i_node].id=NULL ;
  way_array[i_way].id=NULL;

  os->nodes = node_array;

  os->ways = way_array;

  /*Free */

  xmlFreeDoc(doc) ;

  return 0;

}

void afficher(osm os )
 {

   /*Afficher bounds */
   printf("boud minlon %s\n", os.bound.minlon);
   printf("boud minlat %s\n", os.bound.minlat);
   printf("boud maxlon %s\n", os.bound.maxlon);
   printf("boud maxlat %s\n", os.bound.maxlat);
   printf("\n\n\n");
   /*Afficher nodes */
   node * nodes = os.nodes ;
   while(nodes->id!= NULL)
   {

     printf(" id node %s\n",nodes->id );
     printf(" lon node%s\n", nodes->lon);
     printf(" lat node%s\n", nodes->lat);
     printf(" visible node%d\n", nodes->visible);
     printf("\n\n\n");


     struct tag* tags = nodes->tags ;

     while(tags!=NULL && tags->key != NULL)
     {
       printf("\t key%s\n", tags->key);
       printf("\t value%s\n", tags->value);
       tags++ ;

     }

     nodes++;

   }

   way* ways = os.ways ;

   while(ways->id!=NULL)
   {

     printf(" id way %s\n",ways->id );
     printf(" visible way %d\n",ways->visible );

     struct tag* tags = ways->tags ;

     while(tags!=NULL && tags->key != NULL)
     {
       printf("\t key%s\n", tags->key);
       printf("\t value%s\n", tags->value);
       tags++ ;

     }

     xmlChar ** ref = ways->ref ;

     int i = 0 ;
     while(ref[i]!=NULL)
     {
       printf("ref: %s\n", ref[i]);
       i++ ;

     }

     ways++;

   }


 }
int main(int argc, char* argv[])
{

    if(argc!=2)
    {

      fprintf(stderr,"Erreurs arguments: parse <nom_fichier_valide_osm>\n");

      return 2 ;

    }

    osm os ;

    int retour ;

    if((retour=parseDoc(argv[1],&os))<0)
    {

      fprintf(stderr, "%d\n",retour );

      return retour ;

    }

    /*Enlever(ou mettre) les commentaires si vous voulez vérifier(ou non) si les éléments ont été bien récupérés*/
    afficher(os);


    return 0 ;
}
