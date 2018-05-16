#include <stdint.h>
#ifndef ETU_STRUCTURES_H
#define ETU_STRUCTURES_H


typedef struct pixelRGB{
        int rouge;
        int bleu;
        int vert;
}PixelRGB;

typedef struct pixelNB{
    int valeur;
}PixelNB;

typedef struct mcupixels{
    PixelRGB blocsRGB[64];
}MCUPixels;

typedef struct mcuTransform{
    int8_t **Y;
    int tailleY;
    int8_t Cb[64];
    int8_t Cr[64];

}MCUTransform;

#endif //ETU_STRUCTURES_H
