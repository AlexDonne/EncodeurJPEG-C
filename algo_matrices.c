#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "qtables.h"


void afficher_matrice8x8_8(uint8_t *matrice) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      printf("%x ",matrice[i*8 + j]);
    }
    printf("\n");
  }
  printf("\n");
}

void afficher_matrice8x8_16(uint16_t *matrice) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      printf("%4.4x ",matrice[i*8 + j]);
    }
    printf("\n");
  }
  printf("\n");
}

void zigzag(int16_t *matrice, int16_t *zig_matrice) {
  for (int i = 0; i < 64; i++) {
      zig_matrice[i] = matrice[(ordre_zigzag[i] >> 4)*8 + (ordre_zigzag[i] & 7)];
    }
}


void discrete_cosinus_transform(uint8_t *matrice, int16_t *dct_matrice) {
  /* on réalise l'opération -128 */
  for (int i = 0; i < 64; i++) {
    matrice[i] = (matrice[i] >= 128) ? matrice[i] - 128 : matrice[i] + 128;
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
          somme += ((int16_t) matrice[x*8+y]) * cos((2*x+1)*i*M_PI/(2*n)) * cos((2*y+1)*j*M_PI/(2*n));
        }
      }
      c_i = (i == 0) ? 1/sqrt(2): 1;
      c_j = (j == 0) ? 1/sqrt(2): 1;
      dct_matrice[i*8+j] = (int16_t) round(2/n * c_i * c_j * somme);
    }
  }
}

void quantificationY(int16_t *matrice) {
  for (int i = 0; i < 64; i ++) {
    matrice[i] = matrice[i] / compressed_Y_table[i];
  }
}


/*
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
*/