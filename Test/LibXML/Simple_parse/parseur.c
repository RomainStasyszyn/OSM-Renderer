#include <stdio.h>
#include <stdlib.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <string.h>
#include "file.h"

void bfs(xmlNodePtr root);
void print_node(xmlNodePtr node);

void bfs(xmlNodePtr root) {
	element *poped;
	xmlNodePtr n;
	queue *q = (queue *) malloc(sizeof(queue));
	init_queue(q);
	enqueue(q, root);
	while(!is_empty(q)) {
		poped = pop(q);
		print_node(poped->node);
		for(n = poped->node->children; n != NULL; n = n->next) {
			enqueue(q, n);
		}		
	}

	free(q);
}

void print_node(xmlNodePtr node) {
	xmlAttrPtr attr;
	xmlChar *content;
	if(node->type == XML_ELEMENT_NODE) {
		xmlChar *path = xmlGetNodePath(node);
		printf("%s: \n", path);
		for(attr = node->properties; attr != NULL; attr = attr->next) {
			content = xmlNodeListGetString(node->doc, attr->children, 1);
			printf("-> name: %s, value: %s \n",attr->name, content);
			xmlFree(content);
		}

		printf("\n");
		xmlFree(path);
	}
}


int main(int argc, char *argv[]) {
	if(argc != 2) {
		fprintf(stderr, "Usage: ./parseur <fichier .osm>\n");
		return EXIT_FAILURE;
	}

	char *ext = strchr(argv[1], '.');
	if(ext != NULL) {
		if(strcmp(ext, ".osm") != 0) {
			fprintf(stderr, "Le fichier n'est pas un .osm\n");
			return EXIT_FAILURE;
		
		}
	}
	else {
		fprintf(stderr, "Erreur fichier incorrect\n");
		return EXIT_FAILURE;
	}

	xmlDocPtr doc;
	xmlNodePtr racine;

	doc = xmlParseFile(argv[1]);
	if(doc == NULL) {
		fprintf(stderr, "Document .osm invalide\n");
		return EXIT_FAILURE;
	}

	racine = xmlDocGetRootElement(doc);
	if(racine == NULL) {
		fprintf(stderr, "Document .osm vide\n");
		xmlFreeDoc(doc);
		return EXIT_FAILURE;
	}

	bfs(racine);
	xmlFreeDoc(doc);

	return EXIT_SUCCESS;

}
