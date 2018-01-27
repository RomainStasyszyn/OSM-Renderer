#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <stdbool.h>
#include <glib.h>

typedef struct LinkedRef {
	unsigned long int ref;
	struct node* next;
} linkedRef;

typedef struct LinkedRelation {
	unsigned long int ref;
	struct relation* next;
} linkedRelation;

typedef struct Bounds {
	float minLat;
	float minLon;
	float maxLat;
	float maxLon;
} bounds;

typedef struct tag{
	char* key;
	char* value;
} tag;

typedef struct Node {
	unsigned long int id;
	double latitude;
	double longitude;
	bool visible; // if false -> /ignore
	tag *tags; // tableau de tag
} node;

typedef struct Way {
	unsigned long int id;
	char* type; //building / highway
	char* name;
	unsigned long int* ref; // les attributs ref de l'élément nd dans way
	bool visible;  // if false -> /ignore
	linkedRef nodes;
	tag *tags; // tableau de tag
} way;

typedef struct Relation {
	unsigned long int id;
	char* type;
	linkedRef members;
	linkedRelation relations;
	bool inner; //if false then is outer
	tag *tags; // tableau de tag
} relation;

typedef struct {
	bounds bound;
	GHashTable* nodesHashTable;
	way* ways;
	relation* relations;
}osm;

#endif