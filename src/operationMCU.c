#include "../include/operationMCU.h"
#include "../include/algo_matrices.h"

MCUTransform rgbTOycbcr(MCUPixelsRGB mcuPixels) {
    MCUTransform mcuTransform;
    mcuTransform.tailleY = 1;
    mcuTransform.Y = malloc(sizeof(uint16_t));
    for (int j = 0; j < 64; ++j) {
        mcuTransform.Y[0][j] = 0.299*mcuPixels.blocsRGB[j].rouge
                               + 0.587*mcuPixels.blocsRGB[j].vert
                               + 0.114*mcuPixels.blocsRGB[j].bleu;

        mcuTransform.Cb[j] = -0.1687*mcuPixels.blocsRGB[j].rouge
                             -0.3313*mcuPixels.blocsRGB[j].vert
                             + 0.5*mcuPixels.blocsRGB[j].bleu + 128;

        mcuTransform.Cr[j] = 0.5*mcuPixels.blocsRGB[j].rouge
                             - 0.4187*mcuPixels.blocsRGB[j].vert
                             - 0.0813*mcuPixels.blocsRGB[j].bleu + 128;

    }
    return mcuTransform;
}


void MCUzigzag(MCUTransform *mcu, MCUTransform *zig_mcu){
    //Renvoie les blocks du MCU en zigzag
    for (int i = 0; i < mcu->tailleY ; ++i) {
        zigzag(mcu->Y[i], zig_mcu->Y[i]);
    }

    zigzag(mcu->Cb, zig_mcu->Cb);
    zigzag(mcu->Cr, zig_mcu->Cr);
}

void MCUdct(MCUTransform *mcu, MCUTransform *dct_mcu){
    for (int i = 0; i < mcu->tailleY; ++i) {
        discrete_cosinus_transform(mcu->Y[i], dct_mcu->Y[i]);
    }
    discrete_cosinus_transform(mcu->Cb, dct_mcu->Cb);
    discrete_cosinus_transform(mcu->Cr, dct_mcu->Cr);
}

void MCUquantification(MCUTransform *mcu){
    for (int i = 0; i < mcu->tailleY; ++i) {
        quantificationY(mcu->Y[i]);
    }
    quantificationCbCr(mcu->Cb);
    quantificationCbCr(mcu->Cr);

}