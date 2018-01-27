#include "../dataStructurePrototype.h"

struct noeud {
    Node data;
    struct noeud* left;
    struct noeud* right;
};

struct noeud* newNode(Node data);
struct noeud* insertion(struct noeud*, Node node);
struct int existe(struct noeud* noeud, int idRech);