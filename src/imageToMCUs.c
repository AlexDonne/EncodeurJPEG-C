#include "../include/imageToMCUs.h"

/**
 * Transforme la matrice de pixels de la structure image en un tableau de MCUs
 * @param image
 * @return
 */
MCUsMatrice *imageToMCUs(ImagePPM *image, int h1, int l1) {
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
            mcu.blocsRGB = malloc(1 * sizeof(PixelRGB *));
            mcu.blocsRGB[0] = malloc(64 * sizeof(PixelRGB));
            mcu.tailleBlocs = 1;
            test_malloc(mcu.blocsRGB[0]);
            mcu.blocsNB = NULL;
            for (int i = 0, debligne = deblignebase; debligne < finligne; debligne++, i++) {
                for (int j = 0, debcol = debcolbase; debcol < fincol; debcol++, j++) {
                    mcu.blocsRGB[0][i * 8 + j] = image->pixelsRGB[debligne][debcol];
                }
            }
        } else {
            mcu.blocsNB = malloc(1 * sizeof(PixelNB *));
            mcu.blocsNB[0] = malloc(64 * sizeof(PixelNB));
            mcu.tailleBlocs = 1;
            test_malloc(mcu.blocsNB);
            mcu.blocsRGB = NULL;
            for (int i = 0, debligne = deblignebase; debligne < finligne; debligne++, i++) {
                for (int j = 0, debcol = debcolbase; debcol < fincol; debcol++, j++) {
                    mcu.blocsNB[0][i * 8 + j] = image->pixelsNB[debligne][debcol];
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
    if (image->type == RGB) {
        if (l1 == 2 && matMCUs->nblignes % 2 != 0) {
            adapterPourEchantillonageHorizontal(matMCUs);
        }
        if (h1 == 2 && matMCUs->nbcol % 2 != 0) {
            adapterPourEchantillonageVertical(matMCUs);
        }
    }

    matMCUs->type = image->type;
    libererPixels(image, nouvHauteur);
    return matMCUs;
}

/**
 * Si l'échantillonage est par deux (par exemple 1x2) mais que le nombre de lignes de la matrice des MCUs n'est pas pair, on rajoute une ligne de MCUs en bas.
 * @param mcusMatrice
 */
void adapterPourEchantillonageHorizontal(MCUsMatrice *mcusMatrice) {
    MCUPixels *nouv = realloc(mcusMatrice->mcus,
                              (mcusMatrice->nblignes + 1) * mcusMatrice->nbcol * sizeof(MCUPixels));
    test_malloc(nouv);
    for (int i = 0, ind = mcusMatrice->nblignes * mcusMatrice->nbcol; i < mcusMatrice->nbcol; ++i, ++ind) {
        MCUPixels mcu;
        mcu.blocsRGB = malloc(1 * sizeof(PixelRGB *));
        test_malloc(mcu.blocsRGB);
        mcu.blocsRGB[0] = malloc(64 * sizeof(PixelRGB));
        test_malloc(mcu.blocsRGB[0]);
        mcu.blocsNB = NULL;
        mcu.tailleBlocs = 1;
        int indice = (mcusMatrice->nblignes - 1) * (mcusMatrice->nbcol) + i;
        for (int j = 0, k = 56; j < 64; ++j, k++) {
            mcu.blocsRGB[0][j] = nouv[indice].blocsRGB[0][k];

            if (k == 63) {
                k = 55;
            }
        }

        nouv[ind] = mcu;
    }

    mcusMatrice->mcus = nouv;
    mcusMatrice->nblignes++;
}

/**
 * Si l'échantillonage est par deux (exemple 2x1) mais que le nombre de colonnes de la matrice des MCUs n'est pas pair, on rajoute une colonne de MCUs à droite.
 * @param mcusMatrice
 */
void adapterPourEchantillonageVertical(MCUsMatrice *mcusMatrice) {
    MCUPixels *nouv = realloc(mcusMatrice->mcus,
                              (mcusMatrice->nblignes) * (mcusMatrice->nbcol + 1) * sizeof(MCUPixels));
    test_malloc(nouv);
    for (int i = 0; i < mcusMatrice->nblignes; ++i) {
        MCUPixels mcu;
        mcu.blocsRGB = malloc(1 * sizeof(PixelRGB *));
        test_malloc(mcu.blocsRGB);
        mcu.blocsRGB[0] = malloc(64 * sizeof(PixelRGB));
        test_malloc(mcu.blocsRGB[0]);
        mcu.blocsNB = NULL;
        mcu.tailleBlocs = 1;
        int indice = i * mcusMatrice->nbcol + mcusMatrice->nbcol - 1 + i;
        for (int m = (mcusMatrice->nbcol + 1) * mcusMatrice->nblignes - 1; m > indice; --m) {
            nouv[m] = nouv[m - 1];
        }
        for (int j = 0, k = 7; j < 64; j += 8, k += 8) {
            for (int l = 0; l < 8; ++l) {
                mcu.blocsRGB[0][j + l] = nouv[indice].blocsRGB[0][k];
            }
        }

        nouv[indice + 1] = mcu;
    }

    mcusMatrice->mcus = nouv;
    mcusMatrice->nbcol++;

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