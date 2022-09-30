#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TreeMap.h"

#define MAX(a, b) (((a) >= (b))?(a):(b))


/* Creeaza un arbore cu o serie de metode asociate
 *
 * return: arborele creat
 */
TTree* createTree(void* (*createElement)(void*), void (*destroyElement)(void*),
		void* (*createInfo)(void*), void (*destroyInfo)(void*), int compare(void*, void*)) {

	TTree  *tree = (TTree*) malloc(sizeof(TTree));
	tree->root = NULL;

	tree->createElement = createElement;
	tree->destroyElement = destroyElement;
	tree->createInfo = createInfo;
	tree->destroyInfo = destroyInfo;
	tree->compare = compare;
	tree->size = 0;
	
	return tree;
}


/* Verifica daca un arbore este gol (vid)
 * 		1 - daca arborele este gol
 * 		0 - in caz contrar
 */
int isEmpty(TTree* tree) {
	return tree == NULL || tree->root == NULL;
}


/* Cauta un anumit element in interiorul unui arbore
 *
 * tree: structura cu metodele asociate arborelui
 *   !folosit pentru a putea compara elementul curent cu cel cautat
 *
 * x: radacina arborelui curent (in care se face cautarea)
 * elem: elementul ce trebuie cautat
 */
TreeNode* search(TTree* tree, TreeNode* x, void* elem) {

	if (x == NULL || tree == NULL || tree->root == NULL)
		return NULL;

	if (tree->compare(elem, x->elem) == 0)
		return x;
	else if (tree->compare(elem, x->elem) < 0)
		return search(tree, x->left, elem);
	else
		return search(tree, x->right, elem);
}


/* Gaseste nodul cu elementul minim dintr-un arbore
 * avand radacina in x
 */
TreeNode* minimum(TreeNode* x) {

	if (x == NULL)
		return NULL;

	while (x->left != NULL) {
		x = x->left;
	}
	return x;
}

/* Gaseste nodul cu elementul maxim dintr-un arbore
 * avand radacina in x
 */
TreeNode* maximum(TreeNode* x) {

	if (x == NULL)
		return NULL;

	while (x->right != NULL) {
		x = x->right;
	}
	return x;
}


/* Functie pentru gasirea succesorului unui nod
 * (succesorul in inordine)
 */
TreeNode* successor(TreeNode* x) {

	if (x == NULL)
		return NULL;

	if (x->right == NULL) {
		TreeNode *p = x->parent;

		while (maximum(p) == maximum(x)) {
			p = p->parent;
		}

		return p;
	}
	x = x->right;

	return minimum(x);
}


/* Functie pentru gasirea predecesorului unui nod
 * (predecesorul in inordine)
 */
TreeNode* predecessor(TreeNode* x) {

	if (x == NULL)
		return NULL;

	if (x->left == NULL) {
		TreeNode *p = x->parent;

		while (minimum(p) == minimum(x)) {
			p = p->parent;
		}

		return p;
	}
	x = x->left;

	return maximum(x);
}


/* Actualizeaza inaltimea unui nod din arbore
 */
void updateHeight(TreeNode* x) {

	int leftHeight = 0;
	int rightHeight = 0;

	if (x != NULL) {
		if (x->left != NULL)  leftHeight  = x->left->height;
		if (x->right != NULL) rightHeight = x->right->height;
		x->height = MAX(leftHeight, rightHeight) + 1;
	}
}


/* Functie ce primeste adresa unui arbore si
 * a unui nod x si realizeaza o rotatie la stanga
 * a subarborelui care are varful in x
 *
 *   	 (x)	 Rotatie    (y)
 *	 / \     stanga     / \
 *  	a  (y)    ---->   (x)  c
 *	  / \		 / \
 *   	 b   c          a   b
 */
void avlRotateLeft(TTree* tree, TreeNode* x) {
	if (tree == NULL || x == NULL || tree->root == NULL)
		return;

	if (x->right == NULL)
		return;

	TreeNode *y = x->right;

	x->right = y->left;
	if (y->left != NULL)
		y->left->parent = x;

	if (tree->root == x) {
		tree->root = y;
		y->parent = NULL;
	} else {
		y->parent = x->parent;

		if (x->parent->left == x)
			x->parent->left = y;
		else
			x->parent->right = y;
	}

	x->parent = y;
	y->left = x;

	updateHeight(x);
	updateHeight(y);
}


/* Functie ce primeste adresa unui arbore si
 * a unui nod x si realizeaza o rotatie la dreapta
 * a subarborelui care are varful in y
 *
 *     (y)	 Rotatie    (x)
 *	   / \   dreapta    / \
 *   (x)  c   ---->    a  (y)
 *	 / \			      / \
 *  a   b                b   c
 */
void avlRotateRight(TTree* tree, TreeNode* y) {
	if (tree == NULL || y == NULL || tree->root == NULL)
		return;

	if (y->left == NULL)
		return;

	TreeNode *x = y->left;

	y->left = x->right;
	if (x->right != NULL)
		x->right->parent = y;

	if (tree->root == y) {
		tree->root = x;
		x->parent = NULL;
	} else {
		x->parent = y->parent;

		if (y->parent->right == y)
			y->parent->right = x;
		else
			y->parent->left = x;
	}

	y->parent = x;
	x->right = y;

	updateHeight(y);
	updateHeight(x);
}


/* Calculeaza factorul de echilibrare pentru un nod x
 * (AVL balance factor)
*/
int avlGetBalance(TreeNode *x) {
int x_l_h = 0, x_r_h = 0;

	// daca x->left == NULL atunci i_l_h ramane 0
	if (x->left != NULL)
		x_l_h = x->left->height;
		

	// daca x->right == NULL atunci i_lrh ramane 0
	if (x->right != NULL)
		x_r_h = x->right->height;

	return x_l_h - x_r_h;
}


/* Functie pentru reechilibrarea unui arbore AVL
 * in urma unei inserari prin rotatii simple sau duble
 * si recalcularea inaltimii fiecarui nod intalnit parcurgand
 * arborele de jos in sus, spre radacina
 *
 */
void avlFixUp(TTree* tree, TreeNode* y) {

	if (y == NULL)
		return;

	while (y != NULL) {
		updateHeight(y);

		if (avlGetBalance(y) == 2) {

			if (avlGetBalance(y->left) == 1) {
				avlRotateRight(tree, y);
			} else if (avlGetBalance(y->left) == -1) {
				avlRotateLeft(tree, y->left);
				avlRotateRight(tree, y);
			} else {
				avlRotateRight(tree, y);
			}
		}
				
		if (avlGetBalance(y) == -2) {

			if (avlGetBalance(y->right) == -1) {
				avlRotateLeft(tree, y);
			} else if (avlGetBalance(y->right) == 1){
				avlRotateRight(tree, y->right);
				avlRotateLeft(tree, y);
			}	else {
				avlRotateLeft(tree, y);
			}
		}
		y = y->parent;
	}	
}


/* Functie pentru crearea unui nod
 *
 * value: valoarea/cheia din cadrul arborelui
 * info: informatia/valoarea din dictionar
 */
TreeNode* createTreeNode(TTree *tree, void* value, void* info) {

	if (tree == NULL)
		return NULL;

	// Alocarea memoriei
	TreeNode* node = (TreeNode*) malloc(sizeof(TreeNode));

	// Setarea elementului si a informatiei
	node->elem = tree->createElement(value);
	node->info = tree->createInfo(info);

	// Initializarea legaturilor din cadrul arborelui
	node->parent = node->right = node->left = NULL;

	// Initializarea legaturilor din cadrul listei dublu inlantuite
	node->next = node->prev = NULL;
	node->end = node;


	// Inaltimea unui nod nou este 1
	// Inaltimea lui NULL este 0
	node->height = 1;

	return node;
}


/* Inserarea unul nou nod in cadrul multi-dictionarului
 * ! In urma adaugarii arborele trebuie sa fie echilibrat
 *
 */
void insert(TTree* tree, void* elem, void* info) {

	if (tree == NULL)
		return;

	// creez noul nod
	TreeNode *node = createTreeNode(tree, elem, info);

	// daca radacina este NULL, nodul devine radacina
	if (tree->root == NULL) {
		tree->root = node;
		tree->size++;
		return;
	}

	TreeNode *x = tree->root;
	TreeNode *parent = NULL;

	// parcurg arborele pana gasesc locul de inserare
	while (x != NULL) {

		if (tree->compare(elem, x->elem) < 0) {
			parent = x;
			x = x->left;
		} else if (tree->compare(elem, x->elem) > 0) {
			parent = x;
			x = x->right;
		} else {
			// daca valoarea este duplicat inserez in lista
			List_insert_existed_node(x, node);
			return;
		}
	}

	// adaug nodul in arbore
	if (tree->compare(elem, parent->elem) < 0) {
		
		parent->left = node;
		node->parent = parent;
	}
	else if (tree->compare(elem, parent->elem) > 0) {
		parent->right = node;
		node->parent = parent;
	}

	// in functie de valoarea nodului fac diferit inserarea in lista
	if (tree->compare(node->elem, tree->root->elem) > 0)
		List_insert_node_greater(tree, node);
	if (tree->compare(node->elem, tree->root->elem) < 0)
		List_insert_node_lower(tree, node);

	// repar arborele si incrementez dimensiunea
	avlFixUp(tree, parent);
	tree->size++;
}

// functie care insereaza un nod in lista daca este duplicat
void List_insert_existed_node(TreeNode *first_node, TreeNode *node) {

	node->next = first_node->end->next;
	if (first_node->end->next != NULL)
		first_node->end->next->prev = node;
	node->prev = first_node->end;
	first_node->end->next = node;

	first_node->end = node;
	node->end = first_node;
}


// functie care insereaza un nod in lista daca valoarea este mai mare decat radacina
void List_insert_node_greater(TTree *tree, TreeNode *node) {

	TreeNode *idx = tree->root;

	while (tree->compare(node->elem, idx->elem) > 0) {
		idx = idx->end;

		if (idx->next == NULL) {
			idx->next = node;
			node->prev = idx;
			return;
		}
		idx = idx->next;
	}

	idx = idx->prev;

	idx->next->prev = node;
	node->next = idx->next;
	idx->next = node;
	node->prev = idx;
}

// functie care insereaza un nod in lista daca valoarea este mai mica decat radacina
void List_insert_node_lower(TTree *tree, TreeNode *node) {
	TreeNode *idx = tree->root;

	while (tree->compare(node->elem, idx->elem) < 0) {

		if (idx->prev == NULL) {
			idx->prev = node;
			node->next = idx;
			return;
		}

		idx = idx->prev;
		idx = idx->end;
	}

	idx = idx->end;
	idx->next->prev = node;
	node->next = idx->next;
	idx->next = node;
	node->prev = idx;
}


/* Eliminarea unui nod dintr-un arbore
 *
 * ! tree trebuie folosit pentru eliberarea
 *   campurilor `elem` si `info`
 * */
void destroyTreeNode(TTree *tree, TreeNode* node){

	// Verificarea argumentelor functiei
	if(tree == NULL || node == NULL) return;

	// Folosirea metodelor arborelui
	// pentru de-alocarea campurilor nodului
	tree->destroyElement(node->elem);
	tree->destroyInfo(node->info);

	// Eliberarea memoriei nodului
	free(node);
}


/* Eliminarea unui nod din arbore
 *
 * elem: cheia nodului ce trebuie sters
 * 	! In cazul in care exista chei duplicate
 *	  se va sterge ultimul nod din lista de duplicate
 */
void delete(TTree* tree, void* elem) {

	if (tree == NULL || tree->root == NULL)
		return;

	TreeNode *x = tree->root;

	// cauta elementeul in arbore
	while (x != NULL) {
		if (tree->compare(elem, x->elem) < 0) {
			x = x->left;
		} else if (tree->compare(elem, x->elem) > 0) {
			x = x->right;
		} else {
			// daca a gasit elementul decrementeaza dimensiunea si opreste cautarea
			tree->size--;
			break;
		}
	}

	// daca exista duplicat refac legaturile si sterg ultimul nod duplicat din lista
	if (x != x->end) {
	
		TreeNode *x_prev = x;
		TreeNode* del_node = x->end;
		x = del_node->prev;

		x_prev->end = x;
		x->end = x_prev;

		if (x->next->next != NULL)
			x->next->next->prev = x;
		
		x->next = x->next->next;
		destroyTreeNode(tree, del_node);
		return;
	}

	// daca unul dintre copii este NULL
	if (x->left == NULL || x->right == NULL) {

		// refac legaturile in lista
		if (x->next != NULL)
			x->next->prev = x->prev;
		if (x->prev != NULL)
			x->prev->next = x->next;

		// daca ambii copii sunt NULL
		if (x->left == NULL && x->right == NULL) {
			// daca nodul este radacina arborelui
			if (x == tree->root) {
				destroyTreeNode(tree, x);
				tree->root = NULL;
				return;
			}

			// daca nodul este frunza
			TreeNode *parent_x = x->parent;
			if (x->parent->left == x)
				x->parent->left = NULL;
			else
				x->parent->right = NULL;

			destroyTreeNode(tree, x);
			avlFixUp(tree, parent_x);
			return;
		}

	// daca copilul stang este NULL
		if (x->left == NULL) {

		// daca nodul este radacina arborelui	
			if (x == tree->root) {
				tree->root = x->right;
				x->right->parent = NULL;
				destroyTreeNode(tree, x);
				// avlFixUp(tree, tree->root);
				return;
			}

		// daca nu este radacina
			if (x->parent->left == x)
				x->parent->left = x->right;
			else
				x->parent->right = x->right;

			TreeNode *parent_x = x->parent;
			x->right->parent = x->parent;

			destroyTreeNode(tree, x);
			avlFixUp(tree, parent_x);
			return;
		}

	// aceeasi metoda pentru copilul drept
		if (x->right == NULL) {

			if (x == tree->root) {
				tree->root = x->left;
				x->left->parent = NULL;
				destroyTreeNode(tree, x);
				return;
			}

			if (x->parent->left == x)
				x->parent->left = x->left;
			else
				x->parent->right = x->left;

			TreeNode *parent_x = x->parent;
			x->left->parent = x->parent;

			destroyTreeNode(tree, x);
			avlFixUp(tree, parent_x);
			return;
		}
	}

	// daca ambii copii sunt diferiti de NULL aflu succesorul
	TreeNode *suc_x = successor(x);

	// actualizez legaturile in arbore pentru succesor
	if (suc_x->parent->left == suc_x)
		suc_x->parent->left = suc_x->right;
		else
		suc_x->parent->right = suc_x->right;

	if (suc_x->right != NULL)
		suc_x->right->parent = suc_x->parent;

	TreeNode *parent_x = x->parent;
	
	// aduc succesorul in locul nodului x
	if (x == tree->root) {
		tree->root = suc_x;

		} else {

	if (x->parent->left == x)
		x->parent->left = suc_x;
		else
		x->parent->right = suc_x;
	}

	suc_x->parent = x->parent;

	suc_x->left = x->left;
	if (suc_x->left != NULL)
		suc_x->left->parent = suc_x;

	suc_x->right = x->right;
	if (suc_x->right != NULL)
		suc_x->right->parent = suc_x;

	// actualizez legaturile in lista pentru succesor
	if (suc_x->prev != NULL)
		suc_x->prev->next = suc_x->end->next;
	if (suc_x->end->next != NULL)
		suc_x->end->next->prev = suc_x->prev;


	suc_x->end->next = x->next;
	suc_x->prev = x->prev;
	if (x->prev != NULL)
		x->prev->next = suc_x;
	if (x->next != NULL)
		x->next->prev = suc_x->end;

	destroyTreeNode(tree, x);
	avlFixUp(tree, parent_x);
	return;
}


/* Eliberarea memoriei unui arbore
 */

void destroyTree(TTree* tree){

	/* Se poate folosi lista dublu intalntuita
	 * pentru eliberarea memoriei
	 */
	if (tree == NULL || tree->root == NULL)
		return;

	// sterg radacina cat timp inca am noduri in arbelui
	while (tree->root != NULL) {
		delete(tree, tree->root->elem);
	}
	free(tree);
}
