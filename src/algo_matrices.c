#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../include/qtables.h"
#include "../include/algo_matrices.h"

const uint8_t ordre_zigzag[64] = {
        0x00,
        0x01, 0x10,
        0x20, 0x11, 0x02,
        0x03, 0x12, 0x21, 0x30,
        0x40, 0x31, 0x22, 0x13, 0x04,
        0x05, 0x14, 0x23, 0x32, 0x41, 0x50,
        0x60, 0x51, 0x42, 0x33, 0x24, 0x15, 0x06,
        0x07, 0x16, 0x25, 0x34, 0x43, 0x52, 0x61, 0x70,
        0x71, 0x62, 0x53, 0x44, 0x35, 0x26, 0x17,
        0x27, 0x36, 0x45, 0x54, 0x63, 0x72,
        0x73, 0x64, 0x55, 0x46, 0x37,
        0x47, 0x56, 0x65, 0x74,
        0x75, 0x66, 0x57,
        0x67, 0x76,
        0x77
};

#ifndef M_PI
#define M_PI 3.14159265
#endif

void zigzag(int16_t *matrice, int16_t *zig_matrice) {
    for (int i = 0; i < 64; i++) {
        zig_matrice[i] = matrice[(ordre_zigzag[i] >> 4) * 8 + (ordre_zigzag[i] & 7)];
    }
}


void discrete_cosinus_transform(int16_t *matrice, int16_t *dct_matrice) {
    /* on réalise l'opération -128 */
    for (int i = 0; i < 64; i++) {
        matrice[i] -= 128;
    }
    float somme;
    float n = 8;
    float c_i;
    float c_j;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            somme = 0;
            for (int x = 0; x < 8; x++) {
                for (int y = 0; y < 8; y++) {
                    somme += ( matrice[x * 8 + y]) * cos((2 * x + 1) * i * M_PI / (2 * n)) *
                             cos((2 * y + 1) * j * M_PI / (2 * n));
                }
            }
            c_i = (i == 0) ? 1 / sqrt(2) : 1;
            c_j = (j == 0) ? 1 / sqrt(2) : 1;
            dct_matrice[i * 8 + j] = round(2 / n * c_i * c_j * somme);
        }
    }
}

void quantificationY(int16_t *matrice) {
    for (int i = 0; i < 64; i++) {
        matrice[i] = matrice[i] / compressed_Y_table[i];
    }
}

void quantificationCbCr(int16_t *matrice) {
    for (int i = 0; i < 64; i++) {
        matrice[i] = matrice[i] / compressed_CbCr_table[i];
    }
}