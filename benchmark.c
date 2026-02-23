#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "match.h"

int main(void) {
    const char *pattern = "h.*o";
    const char *text = "Hello world!";
    const int N = 1000000; // 1 mil matches, can change it to whatever

    clock_t start = clock();
    for (int i = 0; i < N; i++) {
        match(pattern, text);
    }
    clock_t end = clock();

    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Ran %d matches in %f seconds\n", N, elapsed);
    return 0;
}
