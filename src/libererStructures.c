#include "../include/libererStructures.h"


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

/**
 * Libère l'espace mémoire occupée par une structure MCUsMatrice
 * @param mcusMat
 */
void libererMCUsMatrice(MCUsMatrice *mcusMat) {
    libererMCUsPixels(mcusMat);
    free(mcusMat->mcus);
    free(mcusMat);
}

void libererMCUsPixels(MCUsMatrice *mcusMat) {
    if (mcusMat->type == RGB) {
        for (int i = 0; i < mcusMat->nblignes * mcusMat->nbcol; ++i) {
            for (int j = 0; j < mcusMat->mcus[i].tailleBlocs; ++j) {
                free(mcusMat->mcus[i].blocsRGB[j]);
            }
            free(mcusMat->mcus[i].blocsRGB);
        }
    } else {
        for (int i = 0; i < mcusMat->nblignes * mcusMat->nbcol; ++i) {
            for (int j = 0; j < mcusMat->mcus[i].tailleBlocs; ++j) {
                free(mcusMat->mcus[i].blocsNB[j]);
            }
            free(mcusMat->mcus[i].blocsNB);
        }
    }
}

/**
 * Libère l'espace mémoire d'une ImagePPM
 * @param image
 */
void libererImage(ImagePPM *image) {
    free(image->nom);
    free(image);
}

void libererMCUsTransform(MCUsTransformMat *mcUsTransform) {
    for (int i = 0; i < mcUsTransform->nblignes * mcUsTransform->nbcol; ++i) {
        libererMCUTransform(&(mcUsTransform->mcus[i]));
    }
    free(mcUsTransform->mcus);
    free(mcUsTransform);
}

void libererMCUTransform(MCUTransform *mcuTransform) {
    for (int i = 0; i < mcuTransform->tailleY; ++i) {
        free(mcuTransform->Y[i]);
    }
    free(mcuTransform->Y);
    if (mcuTransform->Cb != NULL) {
        libererCbs(mcuTransform);
        libererCrs(mcuTransform);
    }
}

void libererCbs(MCUTransform *mcuTransform) {
    for (int i = 0; i < mcuTransform->tailleCb; ++i) {
        free(mcuTransform->Cb[i]);
    }
    free(mcuTransform->Cb);
}

void libererCrs(MCUTransform *mcuTransform) {
    for (int i = 0; i < mcuTransform->tailleCr; ++i) {
        free(mcuTransform->Cr[i]);
    }
    free(mcuTransform->Cr);
}