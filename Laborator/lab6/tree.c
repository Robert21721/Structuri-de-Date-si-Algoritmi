/*
*	Created by Nan Mihai on 05.05.2021
*	Copyright (c) 2020 Nan Mihai. All rights reserved.
*	Laborator 6 - Structuri de date
*	Grupa 314CD
*	Facultatea de Automatica si Calculatoare
*	Anul Universitar 2020-2021, Seria CD
*/
#include "tree.h"

/*
*	Funcție care creează un arbore cu un singur nod
*/
Tree createTree(Tree parent, Item value) {
	Tree root = malloc(sizeof(TreeNode));
	root->parent = parent;
	root->value = value;
	root->left = NULL;
	root->right = NULL;
	return root;
}

/*
*	Funcție care inițializează un nod de arbore
*		- îi alocă memorie
*		- îi setează câmpul valoare
*		- setează left și right să pointeze către NULL
*/
void init(Tree *root, Tree parent, Item value) {
	*root = malloc(sizeof(TreeNode));
	(*root)->parent = parent;
	(*root)->value = value;
	(*root)->left = NULL;
	(*root)->right = NULL;
}


/*
*	Funcție care verifică dacă un arbore este vid sau nu
*		- întoarce 1 dacă arborele este vid
*		- întoarce 0 dacă arborele nu este vid
*/
int isEmpty(Tree root) {
	return root == NULL;
}

/*
*	Funcție care inserează o valoare într-un arbore binar, respectând
* proprietățile unui arbore binar de căutare
*/
Tree insertHelper(Tree root, Tree parent, Item value) {
	if (root == NULL)
		return createTree(parent, value);
	if (root->value == value) {
		return root;
	}
	if (value < root->value)
		root->left = insertHelper(root->left, root, value);
	else
		root->right = insertHelper(root->right, root, value);
	return root;
}

Tree insert(Tree root, Item value) {
	return insertHelper(root, NULL, value);
}

/*
*	Funcție care verifică dacă o valoare există sau nu în arbore
*		- 1 dacă value există în arbore
*		- 0 dacă value nu există în arbore
*/
int contains(Tree tree, Item value) {
	// TODO 5
	if (tree == NULL)
		return 0;
	if (tree->value == value)
		return 1;
	if (value < tree->value)
		return contains(tree->left, value);
	return contains(tree->right, value);
}

/*
*	Funcție care determină elementul minim dintr-un arbore binar de căutare
*		- NULL pentru arbore vid
*/
Tree minimum(Tree tree) {
	// TODO 6
	if (tree == NULL)
		return NULL;
	if (tree->left == NULL)
		return tree;
	return minimum(tree->left);
}

/*
*	Funcție care determină elementul maxim dintr-un arbore binar de căutare
*		- NULL pentru arbore vid
*/
Tree maximum(Tree tree) {
	// TODO 7
	if (tree == NULL)
		return NULL;
	if (tree->right == NULL)
		return tree;
	return maximum(tree->right);
}

/*
*	Funcție care dealocă întreaga memorie alocată pentru un arbore binar
*		- root va pointa către NULL după ce se va apela funcția
*/
void destroyTree(Tree *root) {
	if (*root == NULL)
		return;
	destroyTree(&((*root)->left));
	destroyTree(&((*root)->right));
	free(*root);
	*root = NULL;
}

/*
*	Funcție care șterge un nod cu o anumită valoare din arbore
*/
Tree delete(Tree root, Item value) {
	if (!root)
		return root;
	if (root->value > value)
		root->left = delete(root->left, value);
	else if (root->value < value)
		root->right = delete(root->right, value);
	else {
		if (root->left != NULL && root->right != NULL) {
			Tree tmp = minimum(root->right);
			root->value = tmp->value;
			root->right = delete(root->right, tmp->value);
		} else {
			Tree tmp = root;
			if(root->left)
				root = root->left;
			else
				root = root->right;
			free(tmp);
		}
	}
	return root;
}

/*
*	Funcție care determină cel mai apropiat strămoș comun pentru
* două noduri având cheile value1 și value2
*/
Tree lowestCommonAncestor(Tree root, Item value1, Item value2) {
	if (root == NULL)
		return NULL;
	if (value1 < root->value && value2 < root->value)
		return lowestCommonAncestor(root->left, value1, value2);
	if (value1 > root->value && value2 > root->value)
		return lowestCommonAncestor(root->right, value1, value2);
	return root;
}

Tree successor(Tree root, Item value) {
	if (root == NULL)
		return root;
	if (root->value == value) {
		if (root->right != NULL) {
			Tree tmp = root->right;
			while (tmp->left != NULL)
				tmp = tmp->left;
			return tmp;
		} else {
			Tree tmp = root->parent;
			while (tmp != NULL && root == tmp->right) {
				root = tmp;
				tmp = tmp->parent;
			}
			return tmp;
		}
	}
	if (value < root->value)
		return successor(root->left, value);
	else
		return successor(root->right, value);
}

Tree predecessor(Tree root, Item value) {
	if (root == NULL)
		return root;
	if (root->value == value) {
		if (root->left != NULL) {
			Tree tmp = root->left;
			while (tmp->right != NULL)
				tmp = tmp->right;
			return tmp;
		} else {
			Tree tmp = root->parent;
			while (tmp != NULL && root == tmp->left) {
				root = tmp;
				tmp = tmp->parent;
			}
			return tmp;
		}
	}
	if (value < root->value)
		return predecessor(root->left, value);
	else
		return predecessor(root->right, value);
}

/* Solutiile iterative ale interfetei cu arbori binari de cautare */
Tree insert_iterative(Tree root, Item value){
	Tree x = root;
	Tree y = NULL;

	while (x != NULL){
		y = x;
		if(x->value == value)
			return root;

		if(value < x->value)
			x = x->left;
		else
			x = x->right;
	}

	TreeNode *node = createTree(y, value);

	if(y == NULL)
		root = node;
	else if(node->value < y->value)
			y->left = node;
	else
			y->right = node;

	return root;
}

int contains_iterative(Tree tree, Item value)
{
	TreeNode* x = tree;

	while(x != NULL){
		if(value == x->value)
			return 1;
		else if(value < x->value)
			x = x->left;
		else
			x = x->right;
	}
	return 0;
}

Tree minimum_iterative(Tree tree){
	TreeNode *x = tree;
	TreeNode *y = NULL;
	while(x != NULL){
		y = x;
		x = x->left;
	}
	return y;
}

Tree maximum_iterative(Tree tree){
	TreeNode *x = tree;
	TreeNode *y = NULL;
	while(x != NULL){
		y = x;
		x = x->right;
	}
	return y;
}

TreeNode* search(Tree tree, Item value)
{
	TreeNode* x = tree;
	while(x != NULL){
		if(value == x->value)
			return x;
		else if(value < x->value)
			x = x->left;
		else
			x = x->right;
	}
	return NULL;
}

Tree successor_iterative(Tree root, Item value) {
	// TODO 5
	if(root == NULL) return NULL;

	TreeNode* x = search(root, value);
	if(x == NULL) return NULL;

	//
	if(x->right != NULL)
		return minimum(x->right);

	TreeNode* y = x->parent;

	while(y != NULL && x == y->right){
		x = y;
		y = y->parent;
	}

	return y;
}

/*
*	Funcție care determină predecesorul în inordine pentru nodul care conține
* valoarea value.
*/
Tree predecessor_iterative(Tree root, Item value) {
	// TODO 6
	if(root == NULL) return NULL;

	TreeNode* x = search(root, value);
	if(x == NULL) return NULL;

	//
	if(x->left != NULL)
		return maximum(x->left);

	TreeNode* y = x->parent;

	while(y != NULL && x == y->left){
		x = y;
		y = y->parent;
	}

	return y;
}

Tree delete_iterative(Tree tree, Item value){
	TreeNode *z; // node to be deleted
	TreeNode *y; // node to replace z
	TreeNode *x; // child of node y

	if (tree == NULL)
		return NULL;

	z = search(tree, value);
	if(z == NULL)	return NULL;

	y = ((z->left == NULL) || (z->right == NULL)) ? z : successor(tree, z->value);
	x = (y->left != NULL) ? y->left : y->right;

	if(y->parent == NULL){
			tree = x;
	}else{
		if(y == y->parent->left){
			y->parent->left = x;
		}else{
			y->parent->right = x;
		}
	}
	if(y != z)
		z->value = y->value;
	free(y);
	return tree;
}
