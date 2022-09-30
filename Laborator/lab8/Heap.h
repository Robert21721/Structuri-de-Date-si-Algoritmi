#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdlib.h>
#include <math.h>

/* We assume there is a defined primitive type Item. */
typedef struct {
	int prior; // Item priority
	Item data; // Item d
}ItemType;

typedef struct heap{
	long maxHeapSize; // capacity
	long size; // number of elements
	ItemType *elem; // array of elements
} PriQueue, *APriQueue;


PriQueue* makeQueue(int maxHeapSize){
	// Allocate prio structure
	PriQueue *h = (PriQueue*) malloc(sizeof(PriQueue));

	// Allocate array of elements
	h->elem = (ItemType*) malloc(maxHeapSize * sizeof(ItemType));

	// Set heap capacity
	h->maxHeapSize = maxHeapSize;

	// Set the number of elements in the heap
	h->size = 0;

	return h;
}

int getLeftChild(int i){
	return ((i << 1) + 1);
}

int getRightChild(int i) {
	return ((i + 1) << 1);
}

int getParent(int i) {
	return ((i - 1) >> 1);
}

ItemType getMax(APriQueue h) {
	return h->elem[0];
}

void siftUp(APriQueue h, int idx) {
	int parent = getParent(idx);

	/*
	 * While we still have elements in the tree
	 * and the parent[prio] < current[prio].
	 */
	while (parent >= 0 && h->elem[parent].prior < h->elem[idx].prior) {
		/* swap current with parent */
		ItemType aux = h->elem[parent];
		h->elem[parent] = h->elem[idx];
		h->elem[idx] = aux;

		// make the current the parent
		idx = parent;

		// Get its parent
		parent = getParent(idx);
	}
}


void insert(PriQueue *h, ItemType x) {
	if (h->size == h->maxHeapSize) {
		h->maxHeapSize <<= 1;
		h->elem = (ItemType *) realloc(h->elem, h->maxHeapSize * sizeof(ItemType));
	}

	h->elem[h->size] = x;
	siftUp(h, h->size);
	h->size++;
}

void siftDown(APriQueue h, int idx){

	int leftChild, rightChild;
	int largest = idx;

	do{
		/*
		 * At every iteration we must set idx
		 * to the largest of the two children.
		 */
		idx = largest;

		// Get left and right
		leftChild = getLeftChild(idx);
		rightChild = getRightChild(idx);

		// If we have not reached the end and we find a higher prio to the left
		if(leftChild < h->size && h->elem[leftChild].prior > h->elem[largest].prior)
			largest = leftChild; // set largest

		// If we have not reached the end and we find a higher prio to the right
		if(rightChild < h->size && h->elem[rightChild].prior > h->elem[largest].prior)
			largest = rightChild; // set largest

		// If we discovered a higher prio element swap
		if(largest != idx){
			ItemType aux = h->elem[largest];
			h->elem[largest] = h->elem[idx];
			h->elem[idx] = aux;
		}
	}while(largest != idx);
}

void siftDownRecursive(APriQueue h, int idx) {
	int leftChild = getLeftChild(idx);
	int rightChild = getRightChild(idx);
	int largest = idx;

	if (leftChild < h->size && h->elem[leftChild].prior > h->elem[largest].prior) {
		largest = leftChild;
	}

	if (rightChild < h->size && h->elem[rightChild].prior > h->elem[largest].prior) {
		largest = rightChild;
	}

	if (largest != idx) {
		ItemType aux = h->elem[largest];
		h->elem[largest] = h->elem[idx];
		h->elem[idx] = aux;

		/* Carry on. */
		siftDownRecursive(h, largest);
	}
}

void removeMax(APriQueue h) {
	h->elem[0] = h->elem[h->size - 1];
	h->size--;
	siftDown(h, 0);
}

void freeQueue(APriQueue h){
	free(h->elem);
	free(h);
}

#endif

