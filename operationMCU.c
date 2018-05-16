#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "include/structures.h"



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
