/*
  @param data : Node to insert in the Binary Tree
  @param out : Pointer to the noeud that is to be added
*/
struct noeud* newNode(Node data) {
  struct noeud* node = new(struct noeud);
  node->data = data;
  node->left = NULL;
  node->right = NULL;
  return(node);
}

/*
  @param noeud* : pointeur to the noeud where we insert values
  @param node : Node that we want to insert
  @param out : Pointer to the node that where has been inserted
*/
struct noeud* insertion(struct noeud* noeud, Node node){
  if (noeud == NULL) {
    return(newNode(node));
  } else {
    if (node.id <= noeud->data.id) {
      noeud->left = insertion(noeud->left, node);
    } else {
      noeud->right = insertion(noeud->right, node);
    }
    return(noeud);
  }
}

/*
TODO : RETURN POINTER TO THE NODE WITH CORRECT VAL
	@param noeud* : pointer to the starting node of the search
  @param valRech : value we seek
  @param out : TO CHANGE
*/
struct noeud* recherche(struct noeud* noeud, int idRech) {
  if (noeud == NULL) {
    return(NULL);
  }
  else {
    if (idRech == noeud->data.id){
      return(noeud);
    }
    else {
      if (idRech < noeud->data.id){
      	return(recherche(noeud->left, valRech));
      }
      else{
		    return(recherche(noeud->right, valRech));
      }
    }
  }
}