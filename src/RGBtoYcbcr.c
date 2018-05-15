//
// Created by monsinjs on 5/15/18.
//
#include <stdint.h>
#include "../include/structures.h"


MCUTransform* rgbTOycbcr(MCUPixels mcu) {
    MCUTransform *mcuTransform = malloc(sizeof(MCUTransform));
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            mcuTransform->Y[i][j] = 0.299*mcu.blocsRGB[i][j].rouge +
                                    0.587*mcu.blocsRGB[i][j].vert +
                                    0.114*mcu.blocsRGB[i][j].bleu;
            mcuTransform->Cb[i][j] = -0.1687*mcu.blocsRGB[i][j].rouge -
                                     0.3313*mcu.blocsRGB[i][j].vert -
                                     0.5*mcu.blocsRGB[i][j].bleu + 128;
            mcuTransform->Cr[i][j] = 0.5*mcu.blocsRGB[i][j].rouge -
                                     0.4187*mcu.blocsRGB[i][j].vert -
                                     0.0813*mcu.blocsRGB[i][j].bleu + 128;

        }
    }
    return mcuTransform;
}







