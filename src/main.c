#include <stdio.h>
#include <stdlib.h>
#include "../include/readPPM.h"
#include "../include/imageToMCUs.h"
#include "../include/operationMCU.h"

void afficher_mcu(MCUTransform mcu) {
    printf("MCU\n");
    for (int i = 0; i < mcu.tailleY; ++i) {
        printf("Y%i\n", i);
        for (int j = 0; j < 64; ++j) {
            printf("%x ", mcu.Y[i][j]);
        }
        printf("\n");
    }
    if (mcu.Cb != NULL) {
        printf("Cb\n");
        for (int j = 0; j < 64; ++j) {
            printf("%x ", mcu.Cb[j]);
        }
        printf("\n");
        printf("Cr\n");
        for (int j = 0; j < 64; ++j) {
            printf("%x ", mcu.Cr[j]);
        }
        printf("\n");
    }
    printf("\n");

}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Argument requis : nom du fichier");
        exit(1);
    }
    ImagePPM *image = creerImagePPM(argv[1]);
    MCUsMatrice *mat = imageToMCUs(image);
    MCUsTransformMat *mcusTransform = rgbTOycbcrAllMcus(mat);
    for (int i = 0; i < mat->nblignes * mat->nbcol; ++i) {
        afficher_mcu(mcusTransform->mcus[i]);

    }
}