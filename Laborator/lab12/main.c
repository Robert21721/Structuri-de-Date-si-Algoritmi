/*
*	Created by Nan Mihai on 16.05.2022
*	Copyright (c) 2022 Nan Mihai. All rights reserved.
*	Laborator - Structuri de date și Algoritmi (Grafuri)
*	Facultatea de Automatica si Calculatoare
*	Anul Universitar 2021-2022, Seria CD
*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "graph.h"
#include "stack.h"

int check(int *v1, int *v2, int size) {
	int i;
	for (i = 0; i < size; i++) {
		if (v1[i] != v2[i])
			return 0;
	}
	return 1;
}

Stack dfs(Graph graph, int start, Stack stack) {
	List list = graph->adjLists[start];
	graph->visited[start] = 1;
	while (list) {
		if (!graph->visited[list->data.v]) {
			stack = dfs(graph, list->data.v, stack);
		}
		list = list->next;
	}
	stack = push(stack, start);
	return stack;
}

/*
* Funcție care determină sortarea topologică pentru un graf orientat aciclic
* Obs: Rezultatul este reținut în vectorul result care a fost deja alocat!
*/
void topologicalSort(Graph graph, int *result) {
	Stack stack = NULL;
	int i;
	for (i = 0; i < graph->V; i++) {
		if (!graph->visited[i]) {
			stack = dfs(graph, i, stack);
		}
	}
	i = 0;
	while (!isEmptyStack(stack)) {
		result[i] = top(stack);
		stack = pop(stack);
		i++;
	}
}

/*
* Funcție care determină costurile drumurilor minime de la orice nod la orice nod
* Obs: Rezultatul este reținut în matricea distances care a fost deja alocată!
*/
void FloydWarshall(Graph graph, int **distances) {
	int i, j, k;
	for (i = 0; i < graph->V; i++) {
		for (j = 0; j < graph->V; j++) {
			distances[i][j] = getCost(graph, i, j);
		}
	}
	for (k = 0; k < graph->V; k++) {
		for (i = 0; i < graph->V; i++) {
			for (j = 0; j < graph->V; j++) {
				if (distances[i][k] == INFINITY || distances[k][j] == INFINITY)
					continue;
				if (distances[i][j] > distances[i][k] + distances[k][j]) {
					distances[i][j] = distances[i][k] + distances[k][j];
				}
			}
		}
	}
}

/*
* Funcție care determină costurile drumurilor minime care pornesc din start
* Obs: Rezultatul este reținut în vectorul distances care a fost deja alocată!
*/
void BellmanFord(Graph graph, int start, int *distances) {
	int i, j, u, v;
	for (i = 0; i < graph->V; i++) {
		distances[i] = getCost(graph, start, i);
	}
	distances[start] = 0;
	for (i = 0; i < graph->V - 2; i++) {
		for (j = 0; j < graph->V; j++) {
			List list = graph->adjLists[j];
			while (list) {
				u = j;
				v = list->data.v;
				if (distances[v] > distances[u] + list->data.cost) {
					distances[v] = distances[u] + list->data.cost;
				}
				list = list->next;
			}
		}
	}
}

int main() {
	freopen("test0.in", "r", stdin);
	int V, type, i, x, y, M, cost, j, ok;
	double punctaj = 0;
	int res1[] = {0, 1, 3, 5, 0, 3};
	int top1[] = {0, 3, 2, 1, 4, 5};
	scanf("%d %d", &V, &type);
	Graph graph = initGraph(V, type);
	scanf("%d", &M);
	for (i = 0; i < M; i++) {
		scanf("%d %d %d", &x, &y, &cost);
		graph = insertEdge(graph, x, y, cost);
	}
	printGraph(graph);
	drawGraph(graph, "graph0.dot");
	int *result;
	result = calloc(graph->V, sizeof(int));
	topologicalSort(graph, result);
	printf("\nRezultat sortarea topologica: ");
	for (i = 0; i < graph->V; i++) {
		printf("%d ", result[i]);
	}
	printf("\n");
	if (check(result, top1, graph->V)) {
		printf("Corect\n");
		punctaj += 1.5;
	} else {
		printf("Incorect\n");
	}
	printf("\nRezultat Bellman Ford: ");
	BellmanFord(graph, 0, result);
	for (i = 0; i < graph->V; i++) {
		printf("%d ", result[i]);
	}
	printf("\n");
	if (check(result, res1, graph->V)) {
		printf("Corect\n");
		punctaj += 1.5;
	} else {
		printf("Incorect\n");
	}
	
	printf("\nRezultat Floyd Warshall\n");
	int **distances;
	distances = malloc(graph->V * sizeof(int*));
	for (i = 0; i < graph->V; i++) {
		distances[i] = calloc(graph->V, sizeof(int));
	}
	FloydWarshall(graph, distances);
	FILE *fin = fopen("test0.ref", "r");
	ok = 1;
	for (i = 0; i < graph->V; i++) {
		for (j = 0; j < graph->V; j++) {
			printf("%6d ", distances[i][j]);
			fscanf(fin, "%d", &cost);
			if (cost != distances[i][j]) {
				ok = 0;
			}
		}
		free(distances[i]);
		printf("\n");
	}
	if (!ok) {
		printf("Incorect\n");
	} else {
		printf("Corect\n");
		punctaj += 1.5;
	}
	graph = freeGraph(graph);
	free(distances);
	free(result);
	fclose(fin);
	printf("=============================================\n");

	freopen("test1.in", "r", stdin);
	int top2[][7] = {{0, 3, 5, 2, 1, 4, 6}, {0, 3, 2, 1, 4, 5, 6},
					{0, 3, 2, 1, 5, 4, 6}, {0, 3, 2, 5, 1, 4, 6}};
	int res2[] = {0, 1, 3, 5, 0, 4, 3};
	scanf("%d %d", &V, &type);
	graph = initGraph(V, type);
	scanf("%d", &M);
	for (i = 0; i < M; i++) {
		scanf("%d %d %d", &x, &y, &cost);
		graph = insertEdge(graph, x, y, cost);
	}
	printGraph(graph);
	drawGraph(graph, "graph1.dot");
	result = calloc(graph->V, sizeof(int));
	topologicalSort(graph, result);
	printf("\nRezultat sortarea topologica: ");
	for (i = 0; i < graph->V; i++) {
		printf("%d ", result[i]);
	}
	printf("\n");
	if (check(result, top2[0], graph->V) || check(result, top2[1], graph->V) ||
		check(result, top2[2], graph->V) || check(result, top2[3], graph->V)) {
		printf("Corect\n");
		punctaj += 1.5;
	} else {
		printf("Incorect\n");
	}
	printf("\nRezultat Bellman Ford: ");
	BellmanFord(graph, 0, result);
	for (i = 0; i < graph->V; i++) {
		printf("%d ", result[i]);
	}
	printf("\n");
	if (check(result, res2, graph->V)) {
		printf("Corect\n");
		punctaj += 1.5;
	} else {
		printf("Incorect\n");
	}
	
	printf("\nRezultat Floyd Warshall\n");
	distances = malloc(graph->V * sizeof(int*));
	for (i = 0; i < graph->V; i++) {
		distances[i] = calloc(graph->V, sizeof(int));
	}
	FloydWarshall(graph, distances);
	fin = fopen("test1.ref", "r");
	ok = 1;
	for (i = 0; i < graph->V; i++) {
		for (j = 0; j < graph->V; j++) {
			printf("%6d ", distances[i][j]);
			fscanf(fin, "%d", &cost);
			if (cost != distances[i][j]) {
				ok = 0;
			}
		}
		free(distances[i]);
		printf("\n");
	}
	if (!ok) {
		printf("Incorect\n");
	} else {
		printf("Corect\n");
		punctaj += 1.5;
	}
	graph = freeGraph(graph);
	free(distances);
	free(result);
	fclose(fin);


	printf("=============================================\n");
	printf("Punctaj total: %.2lf\n", punctaj);
	printf("Observație: Se acordă 1p de către asistent dacă nu există erori / pierderi de memorie\n");
	return 0;
}
