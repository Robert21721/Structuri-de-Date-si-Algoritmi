build:
	gcc -std=c9x test.c -lm -Wall -D_GNU_SOURCE -o test

test: build
	./test

clean:
	rm -f test

.PHONY: test
