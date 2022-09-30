/*
*	Created by Nan Mihai on 17.03.2020
*	Copyright (c) 2020 Nan Mihai. All rights reserved.
*	Laborator 5 - Structuri de date
*	Grupa 314CD
*	Facultatea de Automatica si Calculatoare
*	Anul Universitar 2019-2020, Seria CD
*/
#include "tree.h"

/*
*	Funcție care creează un arbore cu un singur nod
*/
Tree createTree(Item value) {
	Tree root = malloc(sizeof(TreeNode));
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
void init(Tree *root, Item value) {
	// TODO 1
	*root = malloc(sizeof(TreeNode));
	(*root)->value = value;
	(*root)->left = NULL;
	(*root)->right = NULL;
}

/*
*	Funcție care inserează o valoare într-un arbore binar, respectând
* proprietățile unui arbore binar de căutare
*/
Tree insert(Tree root, Item value) {
	// TODO 2
	if (root == NULL)
		return createTree(value);
	if (root->value == value) {
		// printf("Valoarea există!\n");
		return root;
	}
	if (value < root->value) {
		if (root->left == NULL) {
			root->left = createTree(value);
			return root;
		} else {
			root->left = insert(root->left, value);
			return root;
		}
	} else {
		if (root->right == NULL) {
			root->right = createTree(value);
			return root;
		} else {
			root->right = insert(root->right, value);
			return root;
		}
	}
}

/*
*	Funcție care afișează nodurile folosind parcurgere în postordine
*/
void printPostorder(Tree root) {
	// TODO 3
	if (!root)
		return;
	printPostorder(root->left);
	printPostorder(root->right);
	printf("%d ", root->value);
}

/*
*	Funcție care afișează nodurile folosind parcurgere în preordine
*/
void printPreorder(Tree root) {
	// TODO 4
	if (!root)
		return;
	printf("%d ", root->value);
	printPreorder(root->left);
	printPreorder(root->right);
}

/*
*	Funcție care afișează nodurile folosind parcurgere în inordine
*/
void printInorder(Tree root) {
	// TODO 5
	if (!root)
		return;
	printInorder(root->left);
	printf("%d ", root->value);
	printInorder(root->right);
}

/*
*	Funcție care dealocă întreaga memorie alocată pentru un arbore binar
*		- root va pointa către NULL după ce se va apela funcția
*/
void freeTree(Tree *root) {
	// TODO 6
	if (*root == NULL)
		return;
	freeTree(&((*root)->left));
	freeTree(&((*root)->right));
	free(*root);
	*root = NULL;
}


/*
*	Funcție care determină numărul de noduri dintr-un arbore binar
*/
int size(Tree root) {
	// TODO 7
	if (!root)
		return 0;
	return 1 + size(root->left) + size(root->right);
}

/*
*	Funcție care returnează adâncimea maximă a arborelui
*/
int maxDepth(Tree root) {
	// TODO 8
	if (!root)
		return -1;
	int lDepth, rDepth;
	lDepth = maxDepth(root->left);
	rDepth = maxDepth(root->right);
	if (lDepth > rDepth)
		return lDepth + 1;
	else
		return rDepth + 1;
}

/*
*	Funcție care construiește oglinditul unui arbore binar
*/
void mirror(Tree root) {
	// TODO 9
	if (!root)
		return;
	mirror(root->left);
	mirror(root->right);
	Tree aux = root->left;
	root->left = root->right;
	root->right = aux;
}

/*
*	Funcție care verifică dacă doi arbori binari sunt identici
*/
int sameTree(Tree root1, Tree root2) {
	// TODO 10
	if (root1 == NULL && root2 == NULL)
		return 1;
	if (root1 == NULL || root2 == NULL)
		return 0;
	if (root1->value != root2->value)
		return 0;
	int lResult, rResult;
	lResult = sameTree(root1->left, root2->left);
	rResult = sameTree(root1->right, root2->right);
	return lResult && rResult;
}
