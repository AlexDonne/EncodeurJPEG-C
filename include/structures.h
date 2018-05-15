#include <stdint.h>
#ifndef ETU_STRUCTURES_H
#define ETU_STRUCTURES_H

typedef struct pixelRGB{
    uint8_t rouge;
    uint8_t bleu;
    uint8_t vert;
}PixelRGB;

typedef struct pixelNB{
    uint8_t valeur;
}PixelNB;

typedef struct mcupixels{
    PixelRGB blocsRGB[8][8];
    PixelNB blocsNB[8][8];
}MCUPixels;

typedef struct mcuTransform{
    uint8_t Y[8][8];
    int8_t Cb[8][8];
    int8_t Cr[8][8];

}MCUTransform;

#endif //ETU_STRUCTURES_H
