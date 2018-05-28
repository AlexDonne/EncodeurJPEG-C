#include "../include/operationMCU.h"

/**
 * Transforme une MCU avec pixels RGB en MCUTransforme (avec Y, Cb, Cr)
 * @param mcuPixels
 * @return
 */
MCUTransform rgbTOycbcr(MCUPixels mcuPixels, int h1, int l1) {
    MCUTransform mcuTransform;
    mcuTransform.tailleY = h1*l1;
    mcuTransform.tailleCb = h1*l1;
    mcuTransform.tailleCr = h1*l1;
    printf("%d\n", mcuPixels.tailleBlocs);
    printf("%d %d %d\n", mcuTransform.tailleY, mcuTransform.tailleCr, mcuTransform.tailleCb);
    mcuTransform.Y = malloc(mcuTransform.tailleY * sizeof(void*));
    test_malloc(mcuTransform.Y);
    mcuTransform.Cb = malloc(mcuTransform.tailleCb * sizeof(void*));
    test_malloc(mcuTransform.Cb);
    mcuTransform.Cr = malloc(mcuTransform.tailleCr* sizeof(void*));
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
MCUTransform nbTOy(MCUPixels mcuPixels, int h1, int l1) {
    MCUTransform mcuTransform;
    mcuTransform.tailleY = h1*l1;
    mcuTransform.Y = malloc(mcuTransform.tailleY* sizeof(int16_t));
    test_malloc(mcuTransform.Y);
    for (int i = 0; i < mcuTransform.tailleY; ++i) {
        mcuTransform.Y[i] = malloc(64 * sizeof(int16_t));
        test_malloc(mcuTransform.Y[0]);
        for (int j = 0; j < 64; ++j) {
            mcuTransform.Y[i][j] = (uint16_t) mcuPixels.blocsNB[i][j];
        }
    }
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
    if (mcusMat->mcus[0].blocsRGB == NULL) {
        for (int i = 0; i < taille; ++i) {
            mcusTransform->mcus[i] = nbTOy(mcusMat->mcus[i], h1, l1);
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
 * Libère l'espace mémoire occupée par une structure MCUsMatrice
 * @param mcusMat
 */
void libererMCUsMatrice(MCUsMatrice *mcusMat) {
    for (int i = 0; i < mcusMat->nblignes * mcusMat->nbcol; ++i) {
        free(mcusMat->mcus[i].blocsRGB);
        free(mcusMat->mcus[i].blocsNB);
    }
    free(mcusMat->mcus);
    free(mcusMat);
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
            intermediaire->Cb = malloc(intermediaire->tailleCb * sizeof(int16_t));
            test_malloc(intermediaire->Cb);

            intermediaire->tailleCr = mcusTransformMat->mcus[i].tailleCr;
            intermediaire->Cr = malloc(intermediaire->tailleCr * sizeof(int16_t));
            test_malloc(intermediaire->Cr);

            intermediaire->tailleY = mcusTransformMat->mcus[i].tailleY;
            intermediaire->Y = malloc(intermediaire->tailleY * sizeof(int16_t));
            test_malloc(intermediaire->Y);
            MCUToQuantifRGB(&(mcusTransformMat->mcus[i]), intermediaire, &(mcusTransformMat->mcus[i]));
            libererIntermediaire(intermediaire);
        }
    } else {
        for (int i = 0; i < mcusTransformMat->nbcol * mcusTransformMat->nblignes; ++i) {
            MCUTransform *intermediaire = malloc(sizeof(MCUTransform));
            test_malloc(intermediaire);
            intermediaire->Cb = NULL;
            intermediaire->Cr = NULL;
            intermediaire->tailleY = mcusTransformMat->mcus[i].tailleY;
            intermediaire->Y = malloc(intermediaire->tailleY * sizeof(int16_t));
            test_malloc(intermediaire->Y);
            MCUToQuantifNB(&(mcusTransformMat->mcus[i]), intermediaire, &(mcusTransformMat->mcus[i]));
            libererIntermediaire(intermediaire);
        }
    }
}

/**
 * Applique le DCT, le zigzag et la quantification pour un MCU d'une image RGB
 * @param mcu
 * @param dct_mcu
 */
void MCUToQuantifRGB(MCUTransform *mcu, MCUTransform *dct_mcu, MCUTransform *final) {
    for (int i = 0; i < mcu->tailleY; ++i) {
        dct_mcu->Y[i] = calloc(64, sizeof(int16_t));
        test_malloc(dct_mcu->Y[i]);
        discrete_cosinus_transform(mcu->Y[i], dct_mcu->Y[i]);
        free(final->Y[i]);
        final->Y[i] = calloc(64, sizeof(int16_t));
        test_malloc(final->Y[i]);
        zigzag(dct_mcu->Y[i], final->Y[i]);
        quantificationY(final->Y[i]);
    }
    for (int j = 0; j < mcu->tailleCb; ++j) {
        dct_mcu->Cb[j] = calloc(64, sizeof(int16_t));
        test_malloc(dct_mcu->Cb[j]);
        discrete_cosinus_transform(mcu->Cb[j], dct_mcu->Cb[j]);
        free(final->Cb[j]);
        final->Cb[j] = calloc(64, sizeof(int16_t));
        test_malloc(final->Cb[j]);
        zigzag(dct_mcu->Cb[j], final->Cb[j]);
        quantificationCbCr(final->Cb[j]);
    }

    for (int k = 0; k < mcu->tailleCr; ++k) {
        dct_mcu->Cr[k] = calloc(64, sizeof(int16_t));
        test_malloc(dct_mcu->Cr[k]);
        discrete_cosinus_transform(mcu->Cr[k], dct_mcu->Cr[k]);
        free(final->Cr[k]);
        final->Cr[k] = calloc(64, sizeof(int16_t));
        test_malloc(final->Cr[k]);
        zigzag(dct_mcu->Cr[k], final->Cr[k]);
        quantificationCbCr(final->Cr[k]);
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
    free(intermediaire);
}

/**
 * Applique le DCT, le zigzag et la quantification pour un MCU d'une image noir et blanc
 * @param mcu
 * @param dct_mcu
 */
void MCUToQuantifNB(MCUTransform *mcu, MCUTransform *dct_mcu, MCUTransform *final) {
    for (int i = 0; i < mcu->tailleY; ++i) {
        dct_mcu->Y[i] = calloc(64, sizeof(int16_t));
        test_malloc(dct_mcu->Y[i]);
        discrete_cosinus_transform(mcu->Y[i], dct_mcu->Y[i]);
        free(final->Y[i]);
        final->Y[i] = calloc(64, sizeof(int16_t));
        test_malloc(final->Y[i]);
        zigzag(dct_mcu->Y[i], final->Y[i]);
        quantificationY(final->Y[i]);
    }
}