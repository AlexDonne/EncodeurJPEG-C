#include "../include/test_malloc.h"
#include <stdio.h>
#include <stdlib.h>

void test_malloc(void *ptr) {
    if (ptr == NULL) {
        printf("Erreur Malloc");
        exit(EXIT_FAILURE);
    }
}
