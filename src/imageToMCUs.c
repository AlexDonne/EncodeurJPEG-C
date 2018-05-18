#include "../include/imageToMCUs.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/**
 * Transforme la matrice de pixels de la structure image en un tableau de MCUs
 * @param image
 * @return
 */
MCUsMatrice *imageToMCUs(ImagePPM *image) {
    int nouvHauteur = image->hauteur, nouvLargeur = image->largeur;
    if (image->largeur % 8 != 0 || image->hauteur % 8 != 0) {
        adaptationMCU(image, &nouvHauteur, &nouvLargeur);
    }
    MCUsMatrice *matMCUs = malloc(sizeof(MCUsMatrice));
    matMCUs->nbcol = nouvLargeur / 8;
    matMCUs->nblignes = nouvHauteur/ 8;
    int taille = matMCUs->nbcol * matMCUs->nblignes;
    matMCUs->mcus = malloc(taille * sizeof(MCUPixels));
    int debcolbase = 0;
    int deblignebase = 0;
    int finligne = 8;
    int fincol = 8;
    for (int indTab = 0; indTab < taille; indTab++) {
        MCUPixels mcu;
        if (image->type == RGB) {
            mcu.blocsRGB = malloc(64 * sizeof(PixelRGB));
            mcu.blocsNB = NULL;
            for (int i = 0, debligne = deblignebase; debligne < finligne; debligne++, i++) {
                for (int j = 0, debcol = debcolbase; debcol < fincol; debcol++, j++) {
                    mcu.blocsRGB[i * 8 + j] = image->pixelsRGB[debligne][debcol];
                }
            }
        } else {
            mcu.blocsNB = malloc(64 * sizeof(PixelNB));
            mcu.blocsRGB = NULL;
            for (int i = 0, debligne = deblignebase; debligne < finligne; debligne++, i++) {
                for (int j = 0, debcol = debcolbase; debcol < fincol; debcol++, j++) {
                    mcu.blocsNB[i * 8 + j] = image->pixelsNB[debligne][debcol];
                }
            }
        }
        if (fincol == nouvLargeur) {
            finligne += 8;
            deblignebase += 8;
            fincol = 8;
            debcolbase = 0;
        } else {
            fincol += 8;
            debcolbase += 8;
        }
        matMCUs->mcus[indTab] = mcu;
    }
    afficherMCUs(matMCUs);

    return matMCUs;
}

/**
 * Duplique les dernières lignes ou/et dernières colonnes pour avoir une hauteur et une largeur multiple de 8
 * @param image
 */
void adaptationMCU(ImagePPM *image, int *nouvHauteur, int *nouvLargeur) {
    while (*nouvLargeur % 8 != 0) {
        *nouvLargeur = *nouvLargeur + 1;
    }
    while (*nouvHauteur % 8 != 0) {
        *nouvHauteur = *nouvHauteur + 1;
    }
    if (image->type == RGB) {
        PixelRGB **nouvPixels = malloc(*nouvHauteur * sizeof(PixelRGB *));
        for (int i = 0; i < *nouvHauteur; ++i) {
            nouvPixels[i] = malloc(*nouvLargeur * sizeof(PixelRGB));
            for (int j = 0; j < *nouvLargeur; ++j) {
                if (i < image->hauteur && j < image->largeur) {
                    nouvPixels[i][j] = image->pixelsRGB[i][j];
                } else if (i < image->hauteur && j >= image->largeur) {
                    nouvPixels[i][j] = image->pixelsRGB[i][image->largeur - 1];
                } else if (i >= image->hauteur && j < image->largeur) {
                    nouvPixels[i][j] = image->pixelsRGB[image->hauteur - 1][j];
                } else {
                    nouvPixels[i][j] = image->pixelsRGB[image->hauteur - 1][image->largeur - 1];
                }
            }
        }
        libererPixelsRGB(image->pixelsRGB, image->hauteur);
        image->pixelsRGB = nouvPixels;
    } else {
        PixelNB **nouvPixels = malloc(*nouvHauteur * sizeof(PixelNB *));
        for (int i = 0; i < *nouvHauteur; ++i) {
            nouvPixels[i] = malloc(*nouvLargeur * sizeof(PixelNB));
            for (int j = 0; j < *nouvLargeur; ++j) {
                if (i < image->hauteur && j < image->largeur) {
                    nouvPixels[i][j] = image->pixelsNB[i][j];
                } else if (i < image->hauteur && j >= image->largeur) {
                    nouvPixels[i][j] = image->pixelsNB[i][image->largeur - 1];
                } else if (i >= image->hauteur && j < image->largeur) {
                    nouvPixels[i][j] = image->pixelsNB[image->hauteur - 1][j];
                } else {
                    nouvPixels[i][j] = image->pixelsNB[image->hauteur - 1][image->largeur - 1];
                }
            }
        }
        libererPixelsNB(image->pixelsNB, image->hauteur);
        image->pixelsNB = nouvPixels;
    }
}

void afficheImageNB(ImagePPM* image, int hauteur, int largeur){
    for (int i=0; i<hauteur; i++){
        for(int j=0; j<largeur; j++){
            printf("%02hhx ", image->pixelsNB[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

void libererPixelsRGB (PixelRGB** pixels, int hauteur){
    for (int i = 0; i < hauteur; i++) {
        free(pixels[i]);
    }
    free(pixels);
}

void libererPixelsNB (PixelNB** pixels, int hauteur){
    for (int i = 0; i < hauteur; i++) {
        free(pixels[i]);
    }
    free(pixels);
}

/**
 * Afiche tous les mcus d'une MCUsMAtrice
 * @param tabMcus
 */
void afficherMCUs(MCUsMatrice *tabMcus) {
    if (tabMcus->mcus[0].blocsRGB != NULL) {
        for (int i = 0; i < tabMcus->nbcol * tabMcus->nblignes; i++) {
            printf("MCU #%d\n", i);
            afficherMCURGB(tabMcus->mcus[i]);
            printf("\n\n");

        }
    } else {
        for (int i = 0; i < tabMcus->nbcol * tabMcus->nblignes; i++) {
            printf("MCU #%d\n", i);
            afficherMCUNB(tabMcus->mcus[i]);
            printf("\n\n");

        }
    }
}

/**
 * Affiche une MCU comprenant des pixels RGB
 * @param mcu
 */
void afficherMCURGB(MCUPixels mcu) {
    for (int i = 0; i < 64; i++) {
        if (i % 8 == 0) {
            printf("\n");
        }
        printf("%02hhx%02hhx%02hhx  ", mcu.blocsRGB[i].rouge, mcu.blocsRGB[i].vert, mcu.blocsRGB[i].bleu);
    }
}

/**
 * Affiche une MCU comprenant des pixels nb
 * @param mcu
 */
void afficherMCUNB(MCUPixels mcu) {
    for (int i = 0; i < 64; i++) {
        if (i % 8 == 0) {
            printf("\n");
        }
        printf("%02hhx ", mcu.blocsNB[i]);
    }
}