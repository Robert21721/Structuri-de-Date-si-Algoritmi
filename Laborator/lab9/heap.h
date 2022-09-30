#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdlib.h>
#include <math.h>


typedef struct {
    T elem;
    int prior;
} HeapNode, *PHeapNode;

typedef struct {
    long int capacity;
    long int size;
    PHeapNode *nodes;
} Heap, *PHeap;


PHeap makeHeap(int capacity) {
    PHeap heap = (PHeap)calloc(1, sizeof(Heap));
    heap->capacity = capacity;
    heap->size = 0;
    heap->nodes = (PHeapNode*)calloc(capacity, sizeof(HeapNode*));
    return heap;
}

int getLeftChild(int i) {
    return 2 * i + 1;
}

int getRightChild(int i) {
    return 2 * i + 2;
}

int getParent(int i) {
    return floor((i - 1) / 2);
}

void siftUp(PHeap h, int idx) {
    int parent = getParent(idx);

    while (parent >= 0 && h->nodes[parent]->prior > h->nodes[idx]->prior) {
        PHeapNode aux = h->nodes[parent];
        h->nodes[parent] = h->nodes[idx];
        h->nodes[idx] = aux;

        idx = parent;
        parent = getParent(idx);
    }
}

void insert(PHeap h, T elem, int prior) {
    PHeapNode x = (PHeapNode)calloc(1, sizeof(HeapNode));
    x->elem = elem;
    x->prior = prior;
    if (h->size == h->capacity) {
        h->capacity *= 2;
        h->nodes = (PHeapNode*)realloc(h->nodes, h->capacity * sizeof(HeapNode*));
    }

    h->nodes[h->size] = x;
    h->size++;
    siftUp(h, h->size - 1);
}

PHeapNode getMin(PHeap h) {
    return h->nodes[0];
}

void siftDown(PHeap h, int idx) {
    int leftChild = getLeftChild(idx);
    int rightChild = getRightChild(idx);
    int smallest = idx;

    if (leftChild < h->size && h->nodes[leftChild]->prior < h->nodes[smallest]->prior) {
        smallest = leftChild;
    }

    if (rightChild < h->size && h->nodes[rightChild]->prior < h->nodes[smallest]->prior) {
        smallest = rightChild;
    }

    if (smallest != idx) {
        PHeapNode aux = h->nodes[smallest];
        h->nodes[smallest] = h->nodes[idx];
        h->nodes[idx] = aux;

        /* Carry on. */
        siftDown(h, smallest);
    }
}

PHeapNode removeMin(PHeap h) {
    PHeapNode max = getMin(h);
    PHeapNode last = h->nodes[h->size - 1];

    /* Remove the last elem from the vector. */
    h->size--;

    /* Set latest elem as the first and call heapify downwards. */
    h->nodes[0] = last;
    siftDown(h, 0);

    return max;
}

void freeHeap(PHeap h) {
    /* Apply only when size = 0*/
    free(h->nodes);
    free(h);
}

#endif
