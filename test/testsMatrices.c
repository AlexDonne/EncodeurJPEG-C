#include "../include/algo_matrices.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "../src/algo_matrices.c"


uint8_t matrice_test[64] = {
        0xa6, 0xa0, 0x9a, 0x98, 0x9a, 0x9a, 0x96, 0x91,
        0xa0, 0xa3, 0x9d, 0x8e, 0x88, 0x8f, 0x95, 0x94,
        0xa5, 0x97, 0x96, 0xa1, 0x9f, 0x90, 0x90, 0x9e,
        0xa6, 0x9a, 0x91, 0x91, 0x92, 0x90, 0x90, 0x93,
        0xc9, 0xd9, 0xc8, 0x98, 0x85, 0x98, 0xa2, 0x95,
        0xf0, 0xf5, 0xf9, 0xea, 0xbf, 0x98, 0x90, 0x9d,
        0xe9, 0xe1, 0xf3, 0xfd, 0xf2, 0xaf, 0x8a, 0x90,
        0xe6, 0xf2, 0xf1, 0xed, 0xf8, 0xfb, 0xd0, 0x95
};

void afficher_matrice8x8_8(uint8_t *matrice) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%x ", matrice[i * 8 + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void afficher_matrice8x8_16(uint16_t *matrice) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%4.4x ", matrice[i * 8 + j]);
        }
        printf("\n");
    }
    printf("\n");
}


int main() {
    afficher_matrice8x8_8(matrice_test);
    int16_t dct_matrice[64];
    discrete_cosinus_transform(matrice_test, dct_matrice);
    afficher_matrice8x8_16(dct_matrice);
    int16_t zig_matrice[64];
    zigzag(dct_matrice, zig_matrice);
    afficher_matrice8x8_16(zig_matrice);
    quantificationY(zig_matrice);
    afficher_matrice8x8_16(zig_matrice);
    return EXIT_SUCCESS;
}
