#include <stdio.h>

typedef long T;
#include "OrderedSet.h"

/*
  Function that returns an ordered set with long elements between
  @start and @end that are multiples of @div
*/

OrderedSet* divisors(T start, T end, T div) {
  OrderedSet* s = createOrderedSet(end - start + 1);

  while (start <= end) {
    if ((start % div) == 0)
      add(s, start);
    start++;
  }

  return s;
}

// -----------------------------------------------------------------------------

/*
  Function that prints the size and the elements of an ordered set.
*/

void printOrderedSet(OrderedSet* set) {
  long idx;
  printf("There are %lu elements:", set->size);                // print the size
  for(idx = 0; idx < set->size; idx++)
    printf(" %ld", set->elements[idx]);                    // print each element
  printf("\n");
}


int main(int argc, char* argv[]) {
  OrderedSet *s1, *s2, *sU, *sI;

  s1 = divisors(4, 25, 3);
  s2 = divisors(5, 30, 4);
  sU = unionOrderedSets(s1, s2);
  sI = intersectOrderedSets(s1, s2);

  printOrderedSet(s1);
  printOrderedSet(s2);
  printOrderedSet(sU);
  printOrderedSet(sI);

  return 0;
}
