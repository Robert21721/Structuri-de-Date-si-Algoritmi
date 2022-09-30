#ifndef _DOUBLE_LINKED_LIST_H_
#define _DOUBLE_LINKED_LIST_H_
#include <stdlib.h>

/**
 *  Linked list representation -- R2 Curs2 (Slide 8)
 */
typedef struct ListNode{
	Item elem; // Stored node value
	struct ListNode* next; // link to next node
	struct ListNode* prev; // link to prev node
} ListNode;

/**
 *  Double linked list representation -- desen Curs 3 (Slide 19)
 */
typedef struct List{
	ListNode* first; // link to the first node
	ListNode* last; // link to the last node
}List;

ListNode *createNode(Item elem) {
	ListNode *node = (ListNode*) malloc(sizeof(ListNode));
	node->elem = elem;
	node->next = NULL;
	node->prev = NULL;
	return node;
}

/**
 * Create a new (empty)  list  -- Silde-urile 13/14 Curs 2
 *  [input]: None
 *  [output]: List*
 */
List* createList(void){
	List* list = (List*)malloc(sizeof(List));
	list->first = list->last = NULL;
	return list;
}

/**
 * Determines if the list is empty
 *  [input]: List*
 *  [output]: 1 - empty/ 0 - not empty
 */
int isEmpty(List *list){
	return (list == NULL) || (list->first == NULL);
}

/**
 * Determines if a list contains a specified element
 *  [input]: List*, Item
 *  [output]: 1 - contains/ 0 - not contains
 */
int contains(List *list, Item elem){
	// Corner case: list NULL
	if(list == NULL) return 0;

	// List iterator
	ListNode* iterator = list->first;

	// Iterate over the list
	while(iterator != NULL){
		if(iterator->elem == elem)
			return 1;
		iterator = iterator->next;
	}

	// Element was not found
	return 0;
}

/**
 * Insert a new element in the list at the specified position.
 * Note: Position numbering starts with the position at index zero
 *  [input]: List*, Item, int
 *  [output]: void
 */
void insertAt(List* list, Item elem, int pos){

	// Guard against young player errors
	if(list == NULL) return;

	ListNode* iterator = list->first;

	// Find the insertion point if possible
	while(pos && iterator != NULL){
		pos--;
		iterator = iterator->next;
	}

	// If a position was found:
	// 1 allocate a new node
	// 2. init node links with NULL to always maintain consistency and write less code
	// 3. store the element
	// 4. insert the node
	if(pos == 0){
		ListNode* node = (ListNode*) malloc(sizeof(ListNode));
		node->elem = elem;
		node->prev = node->next = NULL;


		// Corner case 1: list is empty
		if(isEmpty(list)){
			list->first = list->last = node;
			return;
		}

		// Corner case 2: adding to the end of the list
		if(iterator == NULL){
			node->prev = list->last;
			list->last->next = node;
			list->last = node;
			return;
		}

		// Corner case 3: adding to the beginning of the list
		if(iterator->prev == NULL){
			node->next = iterator;
			iterator->prev = node;
			list->first = node;
			return;
		}



		// Normal case.
		node->next = iterator;
		node->prev = iterator->prev;
		iterator->prev->next = node;
		iterator->prev = node;
	}
}


/**
 * Recursive implementation for insertAt - for reference
 * Note: first - helper implementation & second - warper iplementation
 */
void insertAtRecvHelper(List* list, ListNode* iterator, Item elem, int pos){
	if(pos == 0){
		ListNode* node = (ListNode*) malloc(sizeof(ListNode));
		node->elem = elem;
		node->prev = node->next = NULL;


		// Corner case 1: list is empty
		if(isEmpty(list)){
			list->first = list->last = node;
			return;
		}

		// Corner case 2: adding to the end of the list
		if(iterator == NULL){
			node->prev = list->last;
			list->last->next = node;
			list->last = node;
			return;
		}

		// Corner case 3: adding to the beginning of the list
		if(iterator->prev == NULL){
			node->next = iterator;
			iterator->prev = node;
			list->first = node;
			return;
		}

		// Normal case.
		node->next = iterator;
		node->prev = iterator->prev;
		iterator->prev->next = node;
		iterator->prev = node;
	}else if(iterator != NULL){
		insertAtRecvHelper(list, iterator->next, elem, --pos);
	}
}

void insertAtRecursive(List* list, Item elem, int pos){
	// Guard against young player errors
	if(list == NULL) return;

	// Recursive call start
	insertAtRecvHelper(list, list->first, elem, pos);
}

/**
 * Delete the first element instance form a list.
 *  [input]: List*, Item
 *  [output]: void
 */
void deleteOnce(List *list, Item elem){
	// Guard against young player errors
	if(list == NULL) return;


	// In majority of cases we can iterate with a for loop (but not always).
	for(ListNode *iterator = list->first; iterator != NULL; iterator = iterator->next){
		if(iterator->elem == elem){

			//Corner case 1: the list becomes empty
			if((iterator->prev ==  NULL) && (iterator->next == NULL)){
				list->first = list->last = NULL;

			// Corner case 2: deleting the last element
			}else if(iterator->next == NULL){
				iterator->prev->next = NULL;
				list->last = iterator->prev;

			// Corner case 3: deleting first element
			}else if(iterator->prev == NULL){
				iterator->next->prev = NULL;
				list->first = iterator->next;

			// Normal case: splicing out the node
			}else{
				iterator->next->prev = iterator->prev;
				iterator->prev->next = iterator->next;
			}

			// De-allocate the little bastard
			free(iterator);

			// No more work to do
			return;
		}
	}
}


/**
 * Recursive delete function variant.
 * Note: first - helper implementation & second - warper iplementation
 */
void deleteOnceRecvHelper(List *list, ListNode *iterator, Item elem){

	if(iterator != NULL && iterator->elem != elem){
		deleteOnceRecvHelper(list, iterator->next, elem);
	}else if(iterator != NULL){
		//Corner case 1: the list becomes empty
		if((iterator->prev ==  NULL) && (iterator->next == NULL)){
			list->first = list->last = NULL;

		// Corner case 2: deleting the last element
		}else if(iterator->next == NULL){
			iterator->prev->next = NULL;
			list->last = iterator->prev;

		// Corner case 3: deleting first element
		}else if(iterator->prev == NULL){
			iterator->next->prev = NULL;
			list->first = iterator->next;

		// Normal case: splicing out the node
		}else{
			iterator->next->prev = iterator->prev;
			iterator->prev->next = iterator->next;
		}

		// De-allocate the little bastard
		free(iterator);
	}
}

void deleteOnceRecursive(List *list, Item elem){
	// Guard against young player errors
	if(list == NULL) return;

	deleteOnceRecvHelper(list, list->first, elem);
}


/**
 * Compute list length
 *  [input]: List*
 *  [output]: int
 */
int length(List *list){
	// Guard against young player errors
	if(list == NULL) return 0;

	int len = 0;

	// List iterator
	ListNode* iterator = list->first;

	// Iterate over the list
	while(iterator != NULL){
		len++;
		iterator = iterator->next;
	}

	return len;
}



/**
 * Destroy a list.
 *  [input]: List*
 *  [output]: void
 */
List *destroyList(List* list){
	// Guard against young player errors
	if(list == NULL) return;

	// Note that this is one of the instances where we can't use a for loop
	ListNode* iterator = list->first;
	ListNode* aux;
	while(iterator != NULL){
		aux = iterator;
		iterator = iterator->next;

		free(aux);
	}
	free(list);

	// Yet another guard against young player errors
	return NULL;

}
#endif //_DOUBLE_LINKED_LIST_H_
