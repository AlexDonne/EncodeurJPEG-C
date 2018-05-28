#include "../include/affichageStructures.h"


/**
* Affiche en hexa l'image (que couleur)
* @param image
*/
void afficheImageCouleur(ImagePPM *image) {
    for (int i = 0; i < image->hauteur; i++) {
        for (int j = 0; j < image->largeur; j++) {
            printf("%x%x%x ", image->pixelsRGB[i][j].rouge, image->pixelsRGB[i][j].vert, image->pixelsRGB[i][j].bleu);
        }
        printf("\n");
    }
}


/**
 * Affiche en hexa une image en niveau de gris
 * @param image
 * @param hauteur
 * @param largeur
 */
void afficheImageNB(ImagePPM *image, int hauteur, int largeur) {
    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            printf("%02hhx ", image->pixelsNB[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

/**
 * Afiche tous les mcus d'une MCUsMAtrice
 * @param tabMcus
 */
void afficherMCUsMatrice(MCUsMatrice *tabMcus) {
    if (tabMcus->mcus[0].blocsRGB != NULL) {
        for (int i = 0; i < tabMcus->nbcol * tabMcus->nblignes; i++) {
            printf("MCU #%d\n", i);
            afficherMCUMatrice_RGB(tabMcus->mcus[i]);
            printf("\n\n");

        }
    } else {
        for (int i = 0; i < tabMcus->nbcol * tabMcus->nblignes; i++) {
            printf("MCU #%d\n", i);
            afficherMCUMatrice_NB(tabMcus->mcus[i]);
            printf("\n\n");

        }
    }
}

/**
 * Affiche une MCU comprenant des pixels RGB
 * @param mcu
 */
void afficherMCUMatrice_RGB(MCUPixels mcu) {
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
void afficherMCUMatrice_NB(MCUPixels mcu) {
    for (int i = 0; i < 64; i++) {
        if (i % 8 == 0) {
            printf("\n");
        }
        printf("%02hhx ", mcu.blocsNB[i]);
    }
}

/**
 * Affiche tous les MCUs Transform
 * @param mcusTransformMat
 */
void afficherAllMCUsTransform(MCUsTransformMat *mcusTransformMat) {
    for (int i = 0; i < mcusTransformMat->nblignes * mcusTransformMat->nbcol; ++i) {
        printf("MCU #%d\n", i);
        afficher_mcu_Transform(mcusTransformMat->mcus[i]);
    }
}

/**
 * Affiche un MCU Transform
 * @param mcu
 */
void afficher_mcu_Transform(MCUTransform mcu) {
    for (int i = 0; i < mcu.tailleY; ++i) {
        printf("Y%i\n", i);
        for (int j = 0; j < 64; ++j) {
            if (j % 8 == 0) {
                printf("\n");
            }
            printf("%04hx ", mcu.Y[i][j]);
        }
        printf("\n\n");
    }
    if (mcu.Cb != NULL) {
        printf("[Cb]\n");
        for (int j = 0; j < 64; ++j) {
            if (j % 8 == 0) {
                printf("\n");
            }
            printf("%04hx  ", mcu.Cb[j]);
        }
        printf("\n\n");
        printf("[Cr]\n");
        for (int j = 0; j < 64; ++j) {
            if (j % 8 == 0) {
                printf("\n");
            }
            printf("%04hx  ", mcu.Cr[j]);
        }
        printf("\n");
    }
    printf("\n");
}


