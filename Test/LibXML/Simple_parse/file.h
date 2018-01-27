#include <stdio.h>
#include <stdlib.h>
#include <libxml/parser.h>

typedef struct element_queue {
	xmlNodePtr node;
	struct element_queue *next;
} element;

typedef struct queue {
	element *first;
	element *last;
} queue;

void init_queue(queue *q);
int is_empty(queue *q);
int enqueue(queue *q, xmlNodePtr node);
element *pop(queue *q);
void print_queue(queue *q);
