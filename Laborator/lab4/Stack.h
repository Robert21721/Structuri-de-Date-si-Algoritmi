#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include <stdlib.h>



typedef struct StackNode{
	Item elem;
	struct StackNode *next;
}StackNode;

typedef StackNode TCelSt; // Redenumirea unui nod la notatiile din curs 
typedef StackNode *ACelSt; // Redenumirea unui pointer la nod la notatiile din curs

typedef struct Stack{
	StackNode* head;  // Varful stivei
	long size; // Numarul de elemente din stiva
}Stack;

typedef Stack TStiva; // Redenumirea stivei la notatiile din curs
typedef Stack *ASt;//  // Redenumirea unui pointer la stiva la notatiile din curs

Stack* createStack(void){
	Stack* s = (Stack*) malloc(sizeof(Stack));
	s->head = NULL;
	s->size = 0;
}

int isStackEmpty(Stack* stack){
	return (stack == NULL || stack->head == NULL);
}

void push(Stack *stack, Item elem){

	if(stack == NULL) return;

	StackNode *newNode = (StackNode*) malloc(sizeof(StackNode));
	if(newNode == NULL) return;
	newNode->elem = elem;

	newNode->next = stack->head;
	stack->head = newNode;
	stack->size++;
}

Item top(Stack *stack){	
	return stack->head->elem;
} 

void pop(Stack *stack){
	StackNode* aux = stack->head;
	stack->head = stack->head->next;
	free(aux);
	stack->size--;
}

void destroyStack(Stack *stack){
	while(!isStackEmpty(stack)){
		pop(stack);
	}
	free(stack);
}

#endif 
