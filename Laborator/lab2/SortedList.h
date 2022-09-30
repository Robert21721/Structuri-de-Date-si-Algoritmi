#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <stdlib.h>
#include <assert.h>

/*
	IMPORTANT!

	As we stick to pure C, we cannot use templates. We will just asume
	some type T was previously defined.
*/

// -----------------------------------------------------------------------------

typedef struct node {
	T value;
	struct node* next;
} TNode, *TSortedList;

// TODO : Cerința 1 & Cerința 2

TSortedList create(T value) {
	TSortedList head = (TSortedList) malloc(sizeof(TNode));
	head->value = value;
	head->next = NULL;
	return head;
}

int isEmpty(TSortedList head) {
	return head == NULL;
}

int contains(TSortedList head, T value) {
	TSortedList temp = head;
	while (temp != NULL) {
		if (temp->value == value)
			return 1;
		temp = temp->next;
	}
	return 0;
}

TSortedList insert(TSortedList head, T value) {
	if (isEmpty(head)) {
		head = create(value);
		return head;
	}
	TSortedList prev, next, node;
	if (head->value >= value) {
		node = create(value);
		node->next = head;
		return node;	
	}
	prev = head;
	next = head->next;
	while (next != NULL) {
		if (next->value > value) {
			node = create(value);
			node->next = next;
			prev->next = node;
			return head;
		}
		prev = next;
		next = next->next;
	}
	node = create(value);
	prev->next = node;
	return head;
}

TSortedList deleteOnce(TSortedList head, T value) {
	if (isEmpty(head)) {
		return head;
	}
	TSortedList iter = head->next, tmp, prev = head;
	if (head->value == value) {
		tmp = head;
		head = head->next;
		free(tmp);
		return head;
	}
	while (iter != NULL) {
		if (iter->value == value) {
			tmp = iter;
			prev->next = iter->next;
			free(tmp);
			return head;
		}
		prev = iter;
		iter = iter->next;
	}
	return head;
}

long length(TSortedList head) {
	if (isEmpty(head))
		return 0l;
	return 1l + length(head->next);
}

T getNth(TSortedList head, int idx) {
	TSortedList iter = head;
	while (iter != NULL && idx > 1) {
		iter = iter->next;
		idx--;
	}
	return iter->value;
}

TSortedList freeTSortedList(TSortedList head) {
	TSortedList tmp, iter;
	iter = head;
	while (iter != NULL) {
		tmp = iter;
		iter = iter->next;
		free(tmp);
	}
	return NULL;
}
#endif
