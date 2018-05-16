//
// Created by monsinjs on 5/15/18.
//
#include <stdint.h>
#include "../include/structures.h"


MCUTransform* rgbTOycbcr(MCUPixels mcuPixels) {
    MCUTransform mcuTransform;
    mcu.tailleY = 1;
    mcu.Y = malloc(sizeof(uint8_t));
    for (int j = 0; j < 64; ++j) {
            mcu.Y[0][j] = 0.299*mcuPixels.blocsRGB[j].rouge
                          + 0.587*mcuPixels.blocsRGB[j].vert
                          + 0.114*mcuPixels.blocsRGB[j].bleu;
            mcu.Cb[j] = -0.1687*mcuPixels.blocsRGB[j].rouge
                        -0.3313*mcuPixels.blocsRGB[j].vert
                        + 0.5*mcuPixels.blocsRGB[j].bleu + 128;
            mcu.Cr[j] = 0.5*mcuPixels.blocsRGB[j].rouge
                        - 0.4187*mcuPixels.blocsRGB[j].vert
                        - 0.0813*mcuPixels.blocsRGB[j].bleu + 128;

    }
    return mcuTransform;
}







