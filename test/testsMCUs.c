#include "../src/operationMCU.c"
#include "../src/algo_matrices.c"

const uint8_t matrice_test[64] = {
        0xa6, 0xa0, 0x9a, 0x98, 0x9a, 0x9a, 0x96, 0x91,
        0xa0, 0xa3, 0x9d, 0x8e, 0x88, 0x8f, 0x95, 0x94,
        0xa5, 0x97, 0x96, 0xa1, 0x9f, 0x90, 0x90, 0x9e,
        0xa6, 0x9a, 0x91, 0x91, 0x92, 0x90, 0x90, 0x93,
        0xc9, 0xd9, 0xc8, 0x98, 0x85, 0x98, 0xa2, 0x95,
        0xf0, 0xf5, 0xf9, 0xea, 0xbf, 0x98, 0x90, 0x9d,
        0xe9, 0xe1, 0xf3, 0xfd, 0xf2, 0xaf, 0x8a, 0x90,
        0xe6, 0xf2, 0xf1, 0xed, 0xf8, 0xfb, 0xd0, 0x95
};


void afficher_mcu(MCUTransform mcu) {
    printf("MCU\n");
    for (int i = 0; i < mcu.tailleY; ++i) {
        printf("J%i\n", i);
        for (int j = 0; j < 64; ++j) {
            if (j % 8 == 0) {
                printf("\n");
            }
            printf("%x ", mcu.Y[i][j]);
        }
        printf("\n");
    }
    printf("Cb\n");
    for (int j = 0; j < 64; ++j) {
        if (j % 8 == 0) {
            printf("\n");
        }
        printf("%x ", mcu.Cb[j]);
    }
    printf("\n");
    printf("Cr\n");
    for (int j = 0; j < 64; ++j) {
        if (j % 8 == 0) {
            printf("\n");
        }
        printf("%x ", mcu.Cr[j]);
    }
    printf("\n");
    printf("\n");

}

int main() {
    MCUTransform mcu;
    mcu.tailleY = 2;
    mcu.Y = malloc(mcu.tailleY * sizeof(int16_t));
    for (int i = 0; i < mcu.tailleY; ++i) {
        mcu.Y[i] = malloc(64 * sizeof(int16_t));
        for (int j = 0; j < 64; ++j) {
            mcu.Y[i][j] = matrice_test[j];
        }
    }
    for (int j = 0; j < 64; ++j) {
        mcu.Cb[j] = matrice_test[j];
    }
    for (int j = 0; j < 64; ++j) {
        mcu.Cr[j] = matrice_test[j];
    }
    printf("MCU de base\n");
    afficher_mcu(mcu);
    MCUTransform dct_mcu;
    dct_mcu.tailleY = 2;
    dct_mcu.Y = malloc(dct_mcu.tailleY * sizeof(int16_t));
    for (int i = 0; i < dct_mcu.tailleY; ++i) {
        dct_mcu.Y[i] = malloc(64 * sizeof(int16_t));
    }
    MCUdct(&mcu, &dct_mcu);
    printf("MCU apres DCT \n");
    afficher_mcu(dct_mcu);

    MCUTransform zig_mcu;
    zig_mcu.tailleY = 2;
    zig_mcu.Y = malloc(zig_mcu.tailleY * sizeof(int16_t));
    for (int i = 0; i < zig_mcu.tailleY; ++i) {
        zig_mcu.Y[i] = malloc(64 * sizeof(int16_t));
    }
    MCUzigzag(&dct_mcu, &zig_mcu);
    printf("MCU apres zigzag\n");
    afficher_mcu(zig_mcu);

    MCUquantification(&zig_mcu);
    printf("MCU apres quantification\n");
    afficher_mcu(zig_mcu);


    return EXIT_SUCCESS;
}

