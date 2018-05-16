#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "include/structures.h"

/*
void liberer_matrice8x8(int8_t **matrice) {
  // Destruction de la matrice d'origine
  for (int i = 0; i < 8; i++) {
    free(matrice[i]);
  }
  free(matrice);
}
*/

void afficher_mcu(MCUTransform mcu){
    printf("MCU\n");
    for (int i = 0; i < mcu.tailleY; ++i) {
        printf("J%i\n", i);
        for (int j = 0; j < 64 ; ++j) {
            printf("%i ", mcu.Y[i][j]);
        }
        printf("\n");
    }
    printf("Cb\n");
    for (int j = 0; j < 64 ; ++j) {
        printf("%i ", mcu.Cb[j]);
    }
    printf("\n");
    printf("Cr\n");
    for (int j = 0; j < 64 ; ++j) {
        printf("%i ", mcu.Cr[j]);
    }
    printf("\n");
    printf("\n");

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


void zigzag(int8_t *matrice, int8_t *zig_matrice) {
  /* Modifie le tableau zig_matrice en appliquant le zig-zag*/
  int indice = 0;
  for (int i = 0; i < 64; i++) {
          zig_matrice[i]= matrice[(ordre_zigzag[indice] >> 4)*8+(ordre_zigzag[indice] & 7)];
          indice++;
      }

  }

void MCUzigzag(MCUTransform *mcu, MCUTransform *zig_mcu){
  //Renvoie les blocks du MCU en zigzag
    for (int i = 0; i < mcu->tailleY ; ++i) {
        zigzag(mcu->Y[i], zig_mcu->Y[i]);
    }

    zigzag(mcu->Cb, zig_mcu->Cb);
    zigzag(mcu->Cr, zig_mcu->Cr);
}

int main() {
    MCUTransform mcu;
    mcu.tailleY = 2;
    mcu.Y = malloc(mcu.tailleY * sizeof(int8_t));
    for (int i = 0; i < mcu.tailleY; ++i) {
        mcu.Y[i] = malloc(64 * sizeof(int8_t));
        for (int j = 0; j < 64; ++j) {
            mcu.Y[i][j] = i + j;
        }
    }
    for (int j = 0; j < 64; ++j) {
        mcu.Cb[j]= j+3;
    }
    for (int j = 0; j < 64; ++j) {
        mcu.Cr[j]= j+4;
    }
    afficher_mcu(mcu);
    MCUTransform zig_mcu;
    zig_mcu.tailleY = 2;
    zig_mcu.Y = malloc(zig_mcu.tailleY * sizeof(int8_t));
    for (int i = 0; i < zig_mcu.tailleY; ++i) {
        zig_mcu.Y[i] = malloc(64 * sizeof(int8_t));
    }
    MCUzigzag(&mcu, &zig_mcu);
    afficher_mcu(zig_mcu);
  return EXIT_SUCCESS;
}
