#include "../include/operationMCU.h"
#include <math.h>
MCUTransform rgbTOycbcr(MCUPixelsRGB mcuPixels) {
    MCUTransform mcuTransform;
    mcuTransform.tailleY = 1;
    mcuTransform.Y = malloc(1*sizeof(uint16_t));
    mcuTransform.Y[0] = malloc(64*sizeof(uint16_t));
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


MCUTransform* rgbTOycbcrAllMcus(MCUPixelsRGB* mcus, int taille){
    MCUTransform* mcusTransform = malloc(taille* sizeof(MCUTransform));
    for (int i = 0; i < taille; ++i) {
        mcusTransform[i] = rgbTOycbcr(mcus[i]);
    }
    return mcusTransform;
}

void MCUzigzag(MCUTransform *mcu, MCUTransform *zig_mcu){
  //Renvoie les blocks du MCU en zigzag
    for (int i = 0; i < mcu->tailleY ; ++i) {
        zigzag(mcu->Y[i], zig_mcu->Y[i]);
    }

    zigzag(mcu->Cb, zig_mcu->Cb);
    zigzag(mcu->Cr, zig_mcu->Cr);
}
