#include "../include/imageToMCUs.h"

/**
 * Transforme la matrice de pixels de la structure image en un tableau de MCUs
 * @param image
 * @return
 */
MCUsMatrice *imageToMCUs(ImagePPM *image, int l1) {
    int nouvHauteur = image->hauteur, nouvLargeur = image->largeur;
    if (image->largeur % 8 != 0 || image->hauteur % 8 != 0) {
        adaptationMCU(image, &nouvHauteur, &nouvLargeur);
    }
    MCUsMatrice *matMCUs = malloc(sizeof(MCUsMatrice));
    test_malloc(matMCUs);
    matMCUs->nbcol = nouvLargeur / 8;
    matMCUs->nblignes = nouvHauteur / 8;
    int taille = matMCUs->nbcol * matMCUs->nblignes;
    matMCUs->mcus = malloc(taille * sizeof(MCUPixels));
    test_malloc(matMCUs->mcus);
    int debcolbase = 0;
    int deblignebase = 0;
    int finligne = 8;
    int fincol = 8;
    for (int indTab = 0; indTab < taille; indTab++) {
        MCUPixels mcu;
        if (image->type == RGB) {
            mcu.blocsRGB = malloc(64 * sizeof(PixelRGB));
            test_malloc(mcu.blocsRGB);
            mcu.blocsNB = NULL;
            for (int i = 0, debligne = deblignebase; debligne < finligne; debligne++, i++) {
                for (int j = 0, debcol = debcolbase; debcol < fincol; debcol++, j++) {
                    mcu.blocsRGB[i * 8 + j] = image->pixelsRGB[debligne][debcol];
                }
            }
        } else {
            mcu.blocsNB = malloc(64 * sizeof(PixelNB));
            test_malloc(mcu.blocsNB);
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

    if (l1 == 2 && matMCUs->nblignes % 2 != 0) {
        adapterPourEchantillonageHorizontal(matMCUs, image->type);
    }
    libererPixels(image, nouvHauteur);
    return matMCUs;
}

//TODO: free l'ancien
void adapterPourEchantillonageHorizontal(MCUsMatrice *mcusMatrice, TYPE_IMAGE type) {
    MCUPixels *nouv = realloc(mcusMatrice->mcus,
                              (mcusMatrice->nblignes + 1) * mcusMatrice->nbcol * sizeof(MCUPixels));
    test_malloc(nouv);
    if (type == RGB) {
        for (int i = 0, ind = mcusMatrice->nblignes * mcusMatrice->nbcol; i < mcusMatrice->nbcol; ++i, ++ind) {
            MCUPixels mcu;
            mcu.blocsRGB = malloc(64 * sizeof(PixelRGB));
            test_malloc(mcu.blocsRGB);
            if (i % 2 == 0) {
                int indice = (mcusMatrice->nblignes - 1) * (mcusMatrice->nbcol) + i + 1;
                for (int j = 0, k = 56; j < 64; ++j, k++) {
                    mcu.blocsRGB[j] = mcusMatrice->mcus[indice].blocsRGB[k];
                    if (k == 63) {
                        k = 55;
                    }
                }
            } else {
                int indice = (mcusMatrice->nblignes - 2) * mcusMatrice->nbcol + i - 1;
                for (int j = 0, k = 56; j < 64; ++j, ++k) {
                    mcu.blocsRGB[j] = mcusMatrice->mcus[indice].blocsRGB[k];
                    if (k == 63) {
                        k = 55;
                    }
                }
            }
            nouv[ind] = mcu;
        }
    }
    else {
        for (int i = 0, ind = mcusMatrice->nblignes * mcusMatrice->nbcol; i < mcusMatrice->nbcol; ++i, ++ind) {
            MCUPixels mcu;
            mcu.blocsNB = malloc(64 * sizeof(PixelNB));
            test_malloc(mcu.blocsNB);
            if (i % 2 == 0) {
                int indice = (mcusMatrice->nblignes - 1) * (mcusMatrice->nbcol) + i + 1;
                for (int j = 0, k = 56; j < 64; ++j, k++) {
                    mcu.blocsNB[j] = mcusMatrice->mcus[indice].blocsNB[k];
                    if (k == 63) {
                        k = 55;
                    }
                }
            } else {
                int indice = (mcusMatrice->nblignes - 2) * mcusMatrice->nbcol + i - 1;
                for (int j = 0, k = 56; j < 64; ++j, ++k) {
                    mcu.blocsNB[j] = mcusMatrice->mcus[indice].blocsNB[k];
                    if (k == 63) {
                        k = 55;
                    }
                }
            }
            nouv[ind] = mcu;
        }
    }
    mcusMatrice->mcus = nouv;
    mcusMatrice->nblignes++;
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
        test_malloc(nouvPixels);
        for (int i = 0; i < *nouvHauteur; ++i) {
            nouvPixels[i] = malloc(*nouvLargeur * sizeof(PixelRGB));
            test_malloc(nouvPixels[i]);
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
        test_malloc(nouvPixels);
        for (int i = 0; i < *nouvHauteur; ++i) {
            nouvPixels[i] = malloc(*nouvLargeur * sizeof(PixelNB));
            test_malloc(nouvPixels[i]);
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

/**
 * Libère les tableaux de pixels de l'image
 * @param image
 * @param hauteur
 */
void libererPixels(ImagePPM *image, int hauteur) {
    if (image->type == RGB) {
        libererPixelsRGB(image->pixelsRGB, hauteur);
    } else {
        libererPixelsNB(image->pixelsNB, hauteur);
    }
}

void libererPixelsRGB(PixelRGB **pixels, int hauteur) {
    for (int i = 0; i < hauteur; i++) {
        free(pixels[i]);
    }
    free(pixels);
}

void libererPixelsNB(PixelNB **pixels, int hauteur) {
    for (int i = 0; i < hauteur; i++) {
        free(pixels[i]);
    }
    free(pixels);
}