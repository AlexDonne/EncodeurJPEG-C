#include "../include/operationMCU.h"
#include <math.h>
MCUTransform rgbTOycbcr(MCUPixels mcuPixels) {
    MCUTransform mcuTransform;
    mcuTransform.tailleY = 1;
    mcuTransform.Y = malloc(1*sizeof(uint16_t));
    mcuTransform.Y[0] = malloc(64*sizeof(uint16_t));
    mcuTransform.Cb = malloc(64*sizeof(uint16_t));
    mcuTransform.Cr = malloc(64*sizeof(uint16_t));
    for (int j = 0; j < 64; ++j) {
        mcuTransform.Y[0][j] = round(0.299*mcuPixels.blocsRGB[j].rouge
                      + 0.587*mcuPixels.blocsRGB[j].vert
                      + 0.114*mcuPixels.blocsRGB[j].bleu);

        mcuTransform.Cb[j] = round(-0.1687*mcuPixels.blocsRGB[j].rouge
                    -0.3313*mcuPixels.blocsRGB[j].vert
                    + 0.5*mcuPixels.blocsRGB[j].bleu + 128);

        mcuTransform.Cr[j] = round(0.5*mcuPixels.blocsRGB[j].rouge
                    - 0.4187*mcuPixels.blocsRGB[j].vert
                    - 0.0813*mcuPixels.blocsRGB[j].bleu + 128);

    }
    return mcuTransform;
}


MCUTransform nbTOy(MCUPixels mcuPixels){
    MCUTransform mcuTransform;
    mcuTransform.tailleY = 1;
    mcuTransform.Y = malloc(1*sizeof(uint16_t));
    mcuTransform.Y[0] = malloc(64*sizeof(uint16_t));
    for (int j = 0; j < 64; ++j) {
        mcuTransform.Y[0][j] = (uint16_t) mcuPixels.blocsNB[j];
    }
    return mcuTransform;
}

MCUsTransformMat* rgbTOycbcrAllMcus(MCUsMatrice *mcusMat){
    int taille = mcusMat->nbcol*mcusMat->nblignes;
    MCUsTransformMat *mcusTransform = malloc(sizeof(MCUsTransformMat));
    mcusTransform->mcus = malloc(taille* sizeof(MCUTransform));
    mcusTransform->nblignes = mcusMat->nblignes;
    mcusTransform->nbcol = mcusMat->nblignes;
    if (mcusMat->mcus[0].blocsRGB == NULL){
        for (int i=0; i<taille; ++i) {
            mcusTransform->mcus[i] = nbTOy(mcusMat->mcus[i]);
        }
        return mcusTransform;
    }
    for (int i = 0; i < taille; ++i) {
        mcusTransform->mcus[i] = rgbTOycbcr(mcusMat->mcus[i]);
    }
    libererMCUsMatrice(mcusMat);
    return mcusTransform;
}


void libererMCUsMatrice(MCUsMatrice* mcusMat){
    for (int i = 0; i < mcusMat->nblignes * mcusMat->nbcol; ++i) {
        free(mcusMat->mcus[i].blocsRGB);
        free(mcusMat->mcus[i].blocsNB);
    }
    free(mcusMat->mcus);
    free(mcusMat);
}

void MCUzigzag(MCUTransform *mcu, MCUTransform *zig_mcu){
    for (int i = 0; i < mcu->tailleY ; ++i) {
        zigzag(mcu->Y[i], zig_mcu->Y[i]);
    }

    zigzag(mcu->Cb, zig_mcu->Cb);
    zigzag(mcu->Cr, zig_mcu->Cr);
}
