#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint8_t **creer_matrice8x8() {
  uint8_t **matrice = malloc(sizeof(*matrice) * 8); // 8 lignes
  for (int i = 0; i < 8; i++) {
    matrice[i] = malloc(sizeof(**matrice) * 8);
  }
  return matrice;
}

void liberer_matrice8x8(uint8_t **matrice) {
  /* Destruction de la matrice d'origine */
  for (int i = 0; i < 8; i++) {
    free(matrice[i]);
  }
  free(matrice);
}

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


uint8_t **zigzag(uint8_t **matrice) {
  /* Renvoie la matrice en zigzag et détruit la matrice d'origine */
  uint8_t **zig_matrice = creer_matrice8x8();
  int indice = 0;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      zig_matrice[i][j] = matrice[ordre_zigzag[indice] >> 4][ordre_zigzag[indice] & 7];
      indice++;
    }
  }
  liberer_matrice8x8(matrice);
  return zig_matrice;
}



int main() {
  uint8_t **matrice = creer_matrice8x8();
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      matrice[i][j] = i + j;
    }
  }
  uint8_t **zig_matrice = zigzag(matrice);
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      printf("%i\n", zig_matrice[i][j]);
    }
  }
  liberer_matrice8x8(zig_matrice);
  return EXIT_SUCCESS;
}
