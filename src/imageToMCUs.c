#include "../include/imageToMCUs.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

MCUsMatrice* imageToMCUs(ImagePPM *image) {

    if (image->largeur % 8 != 0 || image->hauteur % 8 != 0) {
        //Fonction qui règle problème
        exit(1);
    }
    MCUsMatrice *matMCUs = malloc(sizeof(MCUsMatrice));
    matMCUs->nbcol = image->largeur/8;
    matMCUs->nblignes = image->hauteur/8;
    int taille = matMCUs->nbcol * matMCUs->nblignes;
    matMCUs->mcus = malloc(taille * sizeof(MCUPixels));
    int debcolbase = 0;
    int deblignebase = 0;
    int finligne = 8;
    int fincol = 8;
    printf("largeur %d", image->largeur);
    for (int indTab = 0; indTab < taille; indTab++){
        MCUPixels mcu;
        if (image->type == RGB) {
            mcu.blocsRGB = malloc(64 * sizeof(PixelRGB));
            mcu.blocsNB = NULL;
            for (int i = 0, debligne = deblignebase; debligne < finligne; debligne++, i++) {
                for (int j = 0, debcol = debcolbase; debcol < fincol; debcol++, j++) {
                    mcu.blocsRGB[i * 8 + j] = image->pixelsRGB[debligne][debcol];
                }
            }
        }
        else {
            mcu.blocsNB = malloc(64 * sizeof(PixelNB));
            mcu.blocsRGB = NULL;
            for (int i = 0, debligne = deblignebase; debligne < finligne; debligne++, i++) {
                for (int j = 0, debcol = debcolbase; debcol < fincol; debcol++, j++) {
                    mcu.blocsNB[i * 8 + j] = image->pixelsNB[debligne][debcol];
                }
            }
        }
        if (fincol == image->largeur){
            finligne += 8;
            deblignebase += 8;
            fincol = 8;
            debcolbase = 0;
        }
        else{
            fincol += 8;
            debcolbase += 8;
        }
        matMCUs->mcus[indTab] = mcu;
    }

    afficherMCUs(matMCUs);
    libererImage(image);

    return matMCUs;
}

void libererImage(ImagePPM *image){
    if (image->type == RGB){
        for (int i=0; i<image->hauteur; i++){
            free(image->pixelsRGB[i]);
        }
        free(image->pixelsRGB);
    }
    else{
        for (int i=0; i<image->hauteur; i++){
            free(image->pixelsNB[i]);
        }
        free(image->pixelsNB);
    }
}

void afficherMCUs(MCUsMatrice *tabMcus) {
    if (tabMcus->mcus[0].blocsRGB != NULL) {
        for (int i = 0; i < tabMcus->nbcol*tabMcus->nblignes; i++) {
            printf("MCU #%d\n", i);
            afficherMCURGB(tabMcus->mcus[i]);
            printf("\n\n");

        }
    }
    else {
        for (int i = 0; i < tabMcus->nbcol*tabMcus->nblignes; i++) {
            printf("MCU #%d\n", i);
            afficherMCUNB(tabMcus->mcus[i]);
            printf("\n\n");

        }
    }
}

void afficherMCURGB(MCUPixels mcu) {
    for (int i = 0; i < 64; i++) {
            printf("%x%x%x ", mcu.blocsRGB[i].rouge, mcu.blocsRGB[i].vert,mcu.blocsRGB[i].bleu);
        }
}


void afficherMCUNB(MCUPixels mcu) {
    for (int i = 0; i < 64; i++) {
        printf("%x ", mcu.blocsNB[i]);
    }
}