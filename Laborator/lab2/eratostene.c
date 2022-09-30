#include <stdio.h>

typedef int T;
#include "SortedList.h"

TSortedList getNaturals(int A, int B) {
    TSortedList nats = NULL;
    int i;
    for (i = A; i <= B; i++) {
        nats = insert(nats, i);
    }
    return nats;
}

TSortedList getPrimes(int N) {
    TSortedList aux1, aux2, aux3;
    TSortedList nats = getNaturals(2, N);

    aux1 = nats;
    while(!isEmpty(aux1)) {
        aux2 = aux1;
        while (!isEmpty(aux2->next)) {
            if (aux2->next->value % aux1->value == 0) {
                aux3 = aux2->next;
                aux2->next = aux3->next;
                free(aux3);
            } else {
                aux2 = aux2->next;
            }
        }
        aux1 = aux1->next;
    }
    return nats;
}

void printInts(TSortedList list) {
    while (!isEmpty(list)) {
        printf("%d ", list->value);
        list = list->next;
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    printInts(getPrimes(100));
    return 0;
}
