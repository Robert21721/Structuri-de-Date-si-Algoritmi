#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>

#include "Cipher.h"
#define N 1000


/* Construieste un multi-dictionar pe baza unui fisier text
 * Cheia (elem) unui nod va fi reprezentata de un cuvant din text
 * iar valoarea (info) va fi indexul de inceput al acelui cuvant
 * ignorand caracterele separatoare (",.? \n\r") - i.e. numarul
 * de caractere A-Z precedente
 *
 * E.g: THIS IS AN EXAMPLE
 *
 * 			IS (4)
 *		    / \
 * 	  (6) AN  THIS (0)
 *         \
 *	 	 EXAMPLE (8)
 * */
void buildTreeFromFile(char* fileName, TTree* tree) {

	// Verificarea argumentelor
	if(fileName == NULL || tree == NULL)
		return;

	char prop[N], *p, sep[] = ",.? \n\r";
	int value = 0;
	FILE *file = fopen(fileName, "rt");

	// citesc randurile din fisier
	while (fgets(prop, sizeof(prop), file)) {
		p = strtok(prop, sep);

		// pentru fiecare cuvant creez elementul si informatia
		// si le adaug in arbore (insert)
		while(p != NULL) {

			void* elem = tree->createElement(p);
			void* val_ptr = tree->createInfo(&value);
			insert(tree, elem, val_ptr);

			// eliberez memorie (am stat 3 ore sa-mi dau seama ca terbuie <3)
			tree->destroyElement(elem);
			tree->destroyInfo(val_ptr);

			value = value + strlen(p);
			p = strtok(NULL, sep);
		}
	}
	fclose(file);
}


/* Functie pentru afisarea unei chei de criptare
 * O cheie este reprezentata print-o serie de offset-uri
 *
 * E.g: key = [1, 2, 3, 4]
 * input text  = A    A    A    A
 *			   +1|  +2|  +3|  +4|
 * 				 V    V    V    V
 * output text = B    C    D    E
 *
 */
void printKey(char *fileName, Range *key) {

	FILE *f = fopen(fileName, "w");

	if (key == NULL) {
		fprintf(f, "No key provided!\n");
		fclose(f);
		return;
	}

	fprintf(f, "Decryption key of length %d is:\n", key->size);

	// Afiseaza fiecare offset din cheie % 26 (lungimea
	// alfabetului A-Z)
	for (int i = 0; i < key->size; i++) {
		fprintf(f, "%d ", key->index[i] % 26);

		// Rand nou pentru lizibilitate
		if ((i + 1) % 10 == 0)
			fprintf(f, "\n");
	}
	fclose(f);
}


// functie care creeaza un range
Range* init_range (int n) {

	Range *range = (Range*) malloc (sizeof(Range));
	range->index = (int*) malloc (N * sizeof(int));
	range->size = 0;
	range->capacity = n;

	return range;
}

/* Returneaza cheia obtinuta de parcurgerea in ordine
 * crescatoare a nodurilor arborelui (parcurgand si listele
 * de duplicate)
 */


Range* inorderKeyQuery(TTree* tree) {
	if (tree == NULL || tree->root == NULL)
		return NULL;

	Range *range = init_range(N);
	TreeNode *idx = tree->root;

	// parcurg lista pana la cel mai mic element
		while (idx->prev != NULL) {
			idx = idx->prev;
			idx = idx->end;
		}

	// parcurg lista si retin in range->index valoarea info din idx
	while (idx != NULL) {
		range->index[range->size++] = *((int*)(idx->info));
		idx = idx->next;
	}

	return range;
}

// functie care intoarce nivelul pe care se afla un nod
int node_level(TreeNode *node) {

	TreeNode *idx = node;
	int i = 0;

	while (idx != NULL) {
		i++;
		idx = idx->parent;
	}
	return i;
}



/* Functie pentru extragerea cheii formate din valorile
 * nodurilor de la nivelul ce contine cel mai frecvent cuvant
 * (in cazul in care exista mai multe cuvinte cu numar maxim
 * de aparitii atunci se va considera primul nod dintre acestea conform
 * parcurgerii in inordine a arborelui)
 */

Range* levelKeyQuery(TTree* tree) {

	if (tree == NULL || tree->root == NULL)
		return NULL;

	TreeNode *start_idx = tree->root;
	// parcurg lista pana la cel mai mic element
	while (start_idx->prev != NULL) {
		start_idx = start_idx->prev;
		start_idx = start_idx->end;
	}

	TreeNode *node_to_find = NULL;
	TreeNode *start_pos = start_idx;
	TreeNode *idx = start_idx;
	int nr_max_ap = 0;
	int i = 0;

	// parcurg lista
	while (idx != NULL) {
		// aflu nodul cu numar maxim de aparitii
		if (tree->compare(start_pos->elem, idx->elem) == 0) {
			i++;
		} else {
			i = 0;
			start_pos = idx;
		}

	// actualizez numarul maxi de aparitii
		if (i > nr_max_ap) {
			node_to_find = start_pos;
			nr_max_ap = i;
		}
		idx = idx->next;
	}

	int node_lev = node_level(node_to_find);
	Range *range = init_range(N);

	idx = start_idx;
	// parcurg lista de la inceput
	while (idx != NULL) {
		// daca nodul este pe nivelul care trebuie
		if (node_level(idx) == node_lev) {
			TreeNode *list_idx = idx;

			// daca nodul are duplicate
			while (idx != NULL && tree->compare(idx->elem, list_idx->elem) == 0) {
				range->index[range->size++] = *((int*)(idx->info));
				idx = idx->next;
			}
			// ajung pe pozitia care trebuie
			continue;
		}
		// in caz contrar parcurg lista
		idx = idx->end;
		idx = idx->next;
	}
	return range;
}


/* Extragerea cheii din nodurile aflate intr-un anumit
 * domeniu de valori specificat
 */
Range* rangeKeyQuery(TTree* tree, char* q, char* p) {

	if (tree == NULL || tree->root == NULL || q == NULL || p == NULL)
		return NULL;

	TreeNode *idx = tree->root;

	// parcurg lista pana la cel mai mic element
	while (idx->prev != NULL) {
		idx = idx->prev;
		idx = idx->end;
	}
	
	// creez elementele
	Range *range = init_range(N);
	void* elem_p = tree->createElement(p);
	void* elem_q = tree->createElement(q);

	// inaitez in lista pana la primul element care apartine intervalului
	while (idx != NULL && tree->compare(idx->elem, elem_q) < 0) {
		idx = idx->end;
		idx = idx->next;
	}
	
	// retin valorarea nodurilor din interiorul intervalului
	while (idx != NULL && tree->compare(idx->elem, elem_p) <= 0) {
		range->index[range->size++] = *((int*)(idx->info));
		idx = idx->next;
	}

	// distrug elemetele
	tree->destroyElement(elem_p);
	tree->destroyElement(elem_q);

	return range;
}


void encrypt(char *inputFile, char *outputFile, Range *key) {

	FILE * f_in  = fopen(inputFile,  "r");
	FILE * f_out = fopen(outputFile, "w");

	if (f_in == NULL)
		return;

	char *buff = (char*) malloc(BUFLEN+1);
	char c;

	int idx = 0;

	while (fgets(buff, BUFLEN, f_in) != NULL) {

		for (int i = 0; i < strlen(buff); i++) {
			if (buff[i] != ' ' && buff[i] != '\n' && buff[i] != '\r') {
				c = ((toupper(buff[i]) - 'A') + key->index[idx] % 26) % 26 + 'A';
				idx += 1;
			} else
			 	c = buff[i];

			fprintf(f_out, "%c", c);

			if (idx == key->size)
				idx = 0;
		}
	}

	free(buff);
	fclose(f_in);
	fclose(f_out);
}


void decrypt(char *inputFile, char *outputFile, Range *key) {
 	
	FILE * f_in  = fopen(inputFile,  "r");
	FILE * f_out = fopen(outputFile, "w");

	if (f_in == NULL)
		return;

	char *buff = (char*) malloc(BUFLEN+1);
	char c;

	int idx = 0;

	while (fgets(buff, BUFLEN, f_in) != NULL) {

		for (int i = 0; i < strlen(buff); i++) {
			if (buff[i] != ' ' && buff[i] != '\n' && buff[i] != '\r') {
				c = ((toupper(buff[i]) - 'A') - (key->index[idx] % 26) + 26) % 26 + 'A';
				idx += 1;
			} else
			 	c = buff[i];

			fprintf(f_out, "%c", c);

			if (idx == key->size)
				idx = 0;
		}
	}

	free(buff);
	fclose(f_in);
	fclose(f_out);
}