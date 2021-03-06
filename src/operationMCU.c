#include "../include/operationMCU.h"

/**
 * Transforme une MCU avec pixels RGB en MCUTransforme (avec Y, Cb, Cr)
 * @param mcuPixels
 * @return
 */
MCUTransform rgbTOycbcr(MCUPixels mcuPixels, int h1, int l1) {
    MCUTransform mcuTransform;
    mcuTransform.tailleY = h1 * l1;
    mcuTransform.tailleCb = h1 * l1;
    mcuTransform.tailleCr = h1 * l1;
    mcuTransform.Y = malloc(mcuTransform.tailleY * sizeof(void *));
    test_malloc(mcuTransform.Y);
    mcuTransform.Cb = malloc(mcuTransform.tailleCb * sizeof(void *));
    test_malloc(mcuTransform.Cb);
    mcuTransform.Cr = malloc(mcuTransform.tailleCr * sizeof(void *));
    test_malloc(mcuTransform.Cr);
    for (int i = 0; i < mcuTransform.tailleY; ++i) {
        mcuTransform.Y[i] = malloc(64 * sizeof(int16_t));
        test_malloc(mcuTransform.Y[i]);
        mcuTransform.Cb[i] = malloc(64 * sizeof(int16_t));
        test_malloc(mcuTransform.Cb[i]);
        mcuTransform.Cr[i] = malloc(64 * sizeof(int16_t));
        test_malloc(mcuTransform.Cr[i]);
        for (int j = 0; j < 64; ++j) {
            mcuTransform.Y[i][j] = round(0.299 * mcuPixels.blocsRGB[i][j].rouge
                                         + 0.587 * mcuPixels.blocsRGB[i][j].vert
                                         + 0.114 * mcuPixels.blocsRGB[i][j].bleu);

            mcuTransform.Cb[i][j] = round(-0.1687 * mcuPixels.blocsRGB[i][j].rouge
                                          - 0.3313 * mcuPixels.blocsRGB[i][j].vert
                                          + 0.5 * mcuPixels.blocsRGB[i][j].bleu + 128);

            mcuTransform.Cr[i][j] = round(0.5 * mcuPixels.blocsRGB[i][j].rouge
                                          - 0.4187 * mcuPixels.blocsRGB[i][j].vert
                                          - 0.0813 * mcuPixels.blocsRGB[i][j].bleu + 128);

        }
    }
    return mcuTransform;
}

/**
 * Transforme une MCU avec pixels Noirs et blanc en MCUTransform avec Y, et CB et Cr qui sont NULL
 * @param mcuPixels
 * @return
 */
MCUTransform nbTOy(MCUPixels mcuPixels) {
    MCUTransform mcuTransform;
    mcuTransform.tailleY = 1;
    mcuTransform.Y = malloc(mcuTransform.tailleY * sizeof(void *));
    test_malloc(mcuTransform.Y);
    for (int i = 0; i < mcuTransform.tailleY; ++i) {
        mcuTransform.Y[i] = malloc(64 * sizeof(int16_t));
        test_malloc(mcuTransform.Y[0]);
        for (int j = 0; j < 64; ++j) {
            mcuTransform.Y[i][j] = (uint16_t) mcuPixels.blocsNB[i][j];
        }
    }
    mcuTransform.Cb = NULL;
    mcuTransform.Cr = NULL;
    mcuTransform.tailleCr = 0;
    mcuTransform.tailleCb = 0;
    return mcuTransform;
}

/**
 * Transforme une structure MCUsMatrice (tableau avec tous les MCUPixels) en MCUsTransformMat (tableau avec les MCUS Y, Cb, Cr)
 * @param mcusMat
 * @return
 */
MCUsTransformMat *rgbTOycbcrAllMcus(MCUsMatrice *mcusMat, int h1, int l1) {
    int taille = mcusMat->nbcol * mcusMat->nblignes;
    MCUsTransformMat *mcusTransform = malloc(sizeof(MCUsTransformMat));
    test_malloc(mcusTransform);
    mcusTransform->mcus = malloc(taille * sizeof(MCUTransform));
    test_malloc(mcusTransform->mcus);
    mcusTransform->nblignes = mcusMat->nblignes;
    mcusTransform->nbcol = mcusMat->nbcol;
    if (mcusMat->type == NB) {
        for (int i = 0; i < taille; ++i) {
            mcusTransform->mcus[i] = nbTOy(mcusMat->mcus[i]);
        }
        libererMCUsMatrice(mcusMat);
        return mcusTransform;
    }
    for (int i = 0; i < taille; ++i) {
        mcusTransform->mcus[i] = rgbTOycbcr(mcusMat->mcus[i], h1, l1);
    }
    libererMCUsMatrice(mcusMat);
    return mcusTransform;
}

/**
 * Applique le DCT, le zigzag et la quantification pour tous les MCUS
 * @param mcusTransformMat
 */
void MCUsTransformToQuantif(MCUsTransformMat *mcusTransformMat) {
    if (mcusTransformMat->mcus[0].Cb != NULL) {
        for (int i = 0; i < mcusTransformMat->nbcol * mcusTransformMat->nblignes; ++i) {
            MCUTransform *intermediaire = malloc(sizeof(MCUTransform));
            test_malloc(intermediaire);

            intermediaire->tailleCb = mcusTransformMat->mcus[i].tailleCb;
            intermediaire->Cb = malloc(intermediaire->tailleCb * sizeof(void *));
            test_malloc(intermediaire->Cb);

            intermediaire->tailleCr = mcusTransformMat->mcus[i].tailleCr;
            intermediaire->Cr = malloc(intermediaire->tailleCr * sizeof(void *));
            test_malloc(intermediaire->Cr);

            intermediaire->tailleY = mcusTransformMat->mcus[i].tailleY;
            intermediaire->Y = malloc(intermediaire->tailleY * sizeof(void *));
            test_malloc(intermediaire->Y);
            MCUToQuantifRGB(&(mcusTransformMat->mcus[i]), intermediaire);
            libererIntermediaire(&(mcusTransformMat->mcus[i]));
            mcusTransformMat->mcus[i] = *intermediaire;
            free(intermediaire);
        }
    } else {
        for (int i = 0; i < mcusTransformMat->nbcol * mcusTransformMat->nblignes; ++i) {
            MCUTransform *intermediaire = malloc(sizeof(MCUTransform));
            test_malloc(intermediaire);
            intermediaire->Cb = NULL;
            intermediaire->Cr = NULL;
            intermediaire->tailleY = mcusTransformMat->mcus[i].tailleY;
            intermediaire->Y = malloc(intermediaire->tailleY * sizeof(void *));
            test_malloc(intermediaire->Y);
            MCUToQuantifNB(&(mcusTransformMat->mcus[i]), intermediaire);
            libererIntermediaire(&(mcusTransformMat->mcus[i]));
            mcusTransformMat->mcus[i] = *intermediaire;
            free(intermediaire);
        }
    }
}

/**
 * Applique le DCT, le zigzag et la quantification pour un MCU d'une image RGB
 * @param mcu
 * @param dct_mcu
 */
void MCUToQuantifRGB(MCUTransform *mcu, MCUTransform *dct_mcu) {
    for (int i = 0; i < mcu->tailleY; ++i) {
        dct_mcu->Y[i] = calloc(64, sizeof(int16_t));
        test_malloc(dct_mcu->Y[i]);
        fast_discrete_cosinus_transform(mcu->Y[i]);
        zigzag(mcu->Y[i], dct_mcu->Y[i]);
        quantificationY(dct_mcu->Y[i]);
    }
    for (int j = 0; j < mcu->tailleCb; ++j) {
        dct_mcu->Cb[j] = calloc(64, sizeof(int16_t));
        test_malloc(dct_mcu->Cb[j]);
        fast_discrete_cosinus_transform(mcu->Cb[j]);
        zigzag(mcu->Cb[j], dct_mcu->Cb[j]);
        quantificationCbCr(dct_mcu->Cb[j]);
    }

    for (int k = 0; k < mcu->tailleCr; ++k) {
        dct_mcu->Cr[k] = calloc(64, sizeof(int16_t));
        test_malloc(dct_mcu->Cr[k]);
        fast_discrete_cosinus_transform(mcu->Cr[k]);
        zigzag(mcu->Cr[k], dct_mcu->Cr[k]);
        quantificationCbCr(dct_mcu->Cr[k]);
    }

}

/**
 * Libère la MCUTransform intermédiaire utilisée
 * @param intermediaire
 */
void libererIntermediaire(MCUTransform *intermediaire) {
    if (intermediaire->Cb != NULL) {
        for (int i = 0; i < intermediaire->tailleCb; ++i) {
            free(intermediaire->Cb[i]);
        }
        free(intermediaire->Cb);
        for (int i = 0; i < intermediaire->tailleCr; ++i) {
            free(intermediaire->Cr[i]);
        }
        free(intermediaire->Cr);
    }
    for (int i = 0; i < intermediaire->tailleY; ++i) {
        free(intermediaire->Y[i]);
    }
    free(intermediaire->Y);
}

/**
 * Applique le DCT, le zigzag et la quantification pour un MCU d'une image noir et blanc
 * @param mcu
 * @param dct_mcu
 */
void MCUToQuantifNB(MCUTransform *mcu, MCUTransform *dct_mcu) {
    for (int i = 0; i < mcu->tailleY; ++i) {
        dct_mcu->Y[i] = calloc(64, sizeof(int16_t));
        test_malloc(dct_mcu->Y[i]);
        fast_discrete_cosinus_transform(mcu->Y[i]);
        zigzag(mcu->Y[i], dct_mcu->Y[i]);
        quantificationY(dct_mcu->Y[i]);
    }
}