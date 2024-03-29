
#ifndef AVLTREE_H_
#define AVLTREE_H_

#include<stdlib.h>

// An AVL tree node
typedef struct AVLNode{
	Item elem;
	int height;
	struct AVLNode *l;
	struct AVLNode *r;
	struct AVLNode *p;

}AVLNode;

typedef struct AVLTree{
	long size;
	AVLNode* root;
	AVLNode* nil;
	int (*comp)(Item a, Item b);
}AVLTree;

AVLTree* avlCreateTree(int (*comp) (Item1,Item1))
{
	AVLTree* newTree = (AVLTree*) malloc(sizeof(AVLTree));

	newTree->comp = comp;


	newTree->nil = (AVLNode*) malloc(sizeof(AVLNode));
	newTree->nil->p = newTree->nil->l = newTree->nil->r = newTree->nil;
	newTree->nil->height = 0;

	newTree->root = (AVLNode*) malloc(sizeof(AVLNode));
	newTree->root->p = newTree->root->l = newTree->root->r = newTree->nil;
	newTree->root->height = 0;

	newTree->size = 0;

	return newTree;
}

int avlIsEmpty(AVLTree* tree){
	return (tree->root->l == tree->nil);
}

AVLNode* avlNewNode(AVLTree* tree){
	AVLNode* newNode = (AVLNode*) malloc(sizeof(AVLNode));
	// Initialize the new node to be used in the tree
	newNode->p = newNode->r = newNode->l = tree->nil;
	newNode->height = 1;

	return newNode;
}

// A utility function to get maximum of two integers
int max(int a, int b){
	return (a > b)? a : b;
}

// A utility function to right rotate subtree rooted with y
// See the diagram given above.
void avlRightRotate(AVLTree *tree,  AVLNode *y){
	AVLNode *x = y->l;

	y->l = x->r;
	if(x->r != tree->nil)
		x->r->p = y;

	x->p = y->p;
	if(y->p->l == y)
		y->p->l = x;
	else
		y->p->r = x;

	x->r = y;
	y->p = x;

	// Update heights
	y->height = max(y->l->height, y->r->height) + 1;
	x->height = max(x->l->height, x->r->height) + 1;
}

// A utility function to l rotate subtree rooted with x
// See the diagram given above.
void avlLeftRotate(AVLTree *tree, AVLNode *x){
	AVLNode *y = x->r;

	x->r = y->l; // x->r = beta
	if(y->l != tree->nil)
		y->l->p = x; // the p of the y l subtree is x

	y->p = x->p; // update p for y
	if(x->p->l == x)
		x->p->l = y;
	else
		x->p->r = y;

	y->l = x; // x is to the l of y
	x->p = y; // x p is y

	// Update heights
	y->height = max(y->l->height, y->r->height) + 1;
	x->height = max(x->l->height, x->r->height) + 1;
}

// Get Balance factor of node x
int avlGetBalance(AVLNode *x){
	if (x == NULL)
		return 0;
	return x->l->height - x->r->height;
}

AVLNode * avlMinimum(AVLTree* tree, AVLNode* x){
	while (x->l != tree->nil)
		x = x->l;
	return x;
}

AVLNode* avlMaximum(AVLTree* tree, AVLNode* x){
	while(x->r != tree->nil){
		x = x->r;
	}
	return x;
}


void avlInsertFixUp(AVLTree* tree, AVLNode* y, Item elem){
	while(y != tree->root){
		y->height = max(y->l->height,y->r->height) + 1;
		int balance = avlGetBalance(y);

		// If this node becomes unbalanced, then there are 4 cases


		// Left Left Case
		if (balance > 1 && (-1 == tree->comp(elem,y->l->elem)))
			avlRightRotate(tree, y);

		// Right Right Case
		if (balance < -1 && (1 == tree->comp(elem,y->r->elem)))
			avlLeftRotate(tree,y);

		// Left Right Case
		if (balance > 1 && (1 == tree->comp(elem,y->l->elem))){
			avlLeftRotate(tree, y->l);
			avlRightRotate(tree, y);
		}

		// Right Left Case
		if (balance < -1 && (-1 == tree->comp(elem,y->r->elem))){
			avlRightRotate(tree, y->r);
			avlLeftRotate(tree, y);
		}
		y = y->p;
	}
}

void avlInsert(struct AVLTree* tree, Item elem){

	/* 1. Perform the normal BST insertion */
	AVLNode* x = tree->root->l;
	AVLNode* y = tree->root;

	AVLNode *z  = avlNewNode(tree);
	z->elem = elem;


	while(x != tree->nil){
		y = x;
		if(1 == tree->comp(x->elem,elem))
			x = x->l;
		else if(-1 == tree->comp(x->elem,elem))
			x = x->r;

	}

	z->p = y;
	//y > newNode sau newNode < y
	if(y == tree->root || (1 == tree->comp(y->elem,z->elem)))
		y->l = z;
	else
		y->r = z;

	tree->size++;

	//2. Update list links for the new node z
	avlInsertFixUp(tree, y,elem);
}




void avlDeleteNode(AVLTree *tree, AVLNode* node){
	destroyElem(node->elem);
	free(node);
}


void avlDestroyTreeHelper(AVLTree* tree, AVLNode* x) {
	if (x != tree->nil) {
		avlDestroyTreeHelper(tree,x->l);
		avlDestroyTreeHelper(tree,x->r);
		avlDeleteNode(tree,x);
	}
}

void avlDestroyTree(AVLTree* tree){
	avlDestroyTreeHelper(tree, tree->root->l);
	free(tree->root);
	free(tree->nil);
	free(tree);
}

#endif /* AVLTREE_H_ */
