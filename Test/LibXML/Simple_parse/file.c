#include <stdio.h>
#include <stdlib.h>
#include <libxml/parser.h>
#include "file.h"

void init_queue(queue *q) {
	q->first = NULL;
	q->last = NULL;
}

int is_empty(queue *q) {
	if(q == NULL)
		return EXIT_FAILURE;
	if(q->first == NULL) {
		//printf("queue is empty\n");
		return 1;
	}

	return 0;
}

int enqueue(queue *q, xmlNodePtr node) {
	element *new_node = (element *) malloc(sizeof(element));

	if(new_node == NULL) {
		perror("malloc()");
		return EXIT_FAILURE;
	}
	if(q == NULL) 
		 return EXIT_FAILURE;

	new_node->node = node;
	new_node->next = NULL;

	if(q->first == NULL) {
		q->first = new_node;
		q->last = new_node;
	}
	else {
		q->last->next = new_node;
		q->last = new_node;
	}

	return EXIT_SUCCESS;
}

element *pop(queue *q) {
	element *elem;
	if(q == NULL) 
		exit(EXIT_FAILURE);
	if(q->first == NULL) {
		fprintf(stderr, "queue is empty\n");
		exit(EXIT_FAILURE);
	}
	else {
		elem = q->first;
		q->first = q->first->next;
		//free(elem->node);
		//free(elem);
	}
	return elem;
}

void print_queue(queue *q) {
	element *elem;
	elem = q->first;
	while(elem != NULL) {
		printf("%s\n", elem->node->name);
		elem = elem->next;
	}
	printf("\n");
}
