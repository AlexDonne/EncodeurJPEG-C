#include "../include/operationMCU.h"

/**
 * Transforme une MCU avec pixels RGB en MCUTransforme (avec Y, Cb, Cr)
 * @param mcuPixels
 * @return
 */
MCUTransform rgbTOycbcr(MCUPixels mcuPixels) {
    MCUTransform mcuTransform;
    mcuTransform.tailleY = 1;
    mcuTransform.tailleCb = 1;
    mcuTransform.tailleCr = 1;
    mcuTransform.Y = malloc(sizeof(int16_t));
    test_malloc(mcuTransform.Y);
    mcuTransform.Y[0] = malloc(64 * sizeof(int16_t));
    test_malloc(mcuTransform.Y[0]);


    mcuTransform.Cb = malloc(sizeof(int16_t));
    test_malloc(mcuTransform.Cb);
    mcuTransform.Cb[0] = malloc(64 * sizeof(int16_t));
    test_malloc(mcuTransform.Cb[0]);

    mcuTransform.Cr = malloc(sizeof(int16_t));
    test_malloc(mcuTransform.Cr);
    mcuTransform.Cr[0] = malloc(64 * sizeof(int16_t));
    test_malloc(mcuTransform.Cr[0]);

    for (int j = 0; j < 64; ++j) {
        mcuTransform.Y[0][j] = round(0.299 * mcuPixels.blocsRGB[j].rouge
                                     + 0.587 * mcuPixels.blocsRGB[j].vert
                                     + 0.114 * mcuPixels.blocsRGB[j].bleu);

        mcuTransform.Cb[0][j] = round(-0.1687 * mcuPixels.blocsRGB[j].rouge
                                   - 0.3313 * mcuPixels.blocsRGB[j].vert
                                   + 0.5 * mcuPixels.blocsRGB[j].bleu + 128);

        mcuTransform.Cr[0][j] = round(0.5 * mcuPixels.blocsRGB[j].rouge
                                   - 0.4187 * mcuPixels.blocsRGB[j].vert
                                   - 0.0813 * mcuPixels.blocsRGB[j].bleu + 128);

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
    mcuTransform.Y = malloc(sizeof(int16_t));
    test_malloc(mcuTransform.Y);
    mcuTransform.Y[0] = malloc(64 * sizeof(int16_t));
    test_malloc(mcuTransform.Y[0]);
    for (int j = 0; j < 64; ++j) {
        mcuTransform.Y[0][j] = (uint16_t) mcuPixels.blocsNB[j];
    }
    return mcuTransform;
}

/**
 * Transforme une structure MCUsMatrice (tableau avec tous les MCUPixels) en MCUsTransformMat (tableau avec les MCUS Y, Cb, Cr)
 * @param mcusMat
 * @return
 */
MCUsTransformMat *rgbTOycbcrAllMcus(MCUsMatrice *mcusMat) {
    int taille = mcusMat->nbcol * mcusMat->nblignes;
    MCUsTransformMat *mcusTransform = malloc(sizeof(MCUsTransformMat));
    test_malloc(mcusTransform);
    mcusTransform->mcus = malloc(taille * sizeof(MCUTransform));
    test_malloc(mcusTransform->mcus);
    mcusTransform->nblignes = mcusMat->nblignes;
    mcusTransform->nbcol = mcusMat->nbcol;
    if (mcusMat->mcus[0].blocsRGB == NULL) {
        for (int i = 0; i < taille; ++i) {
            mcusTransform->mcus[i] = nbTOy(mcusMat->mcus[i]);
        }
        libererMCUsMatrice(mcusMat);
        return mcusTransform;
    }
    for (int i = 0; i < taille; ++i) {
        mcusTransform->mcus[i] = rgbTOycbcr(mcusMat->mcus[i]);
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