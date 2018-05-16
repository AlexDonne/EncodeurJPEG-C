#include <stdint.h>
#ifndef ETU_STRUCTURES_H
#define ETU_STRUCTURES_H

typedef enum TYPE_IMAGE{
    NB,
    RGB
}TYPE_IMAGE;

typedef struct pixelRGB{
    uint8_t rouge;
    uint8_t bleu;
    uint8_t vert;
}PixelRGB;

typedef uint8_t PixelNB;


typedef struct imagePPM{
    TYPE_IMAGE type;
    char* nom;
    char* chemin;
    int hauteur;
    int largeur;
    PixelRGB **pixelsRGB;
    PixelNB **pixelsNB;
}ImagePPM;


typedef struct mcupixelsRGB{
    PixelRGB blocsRGB[64];
}MCUPixelsRGB;

typedef struct mcupixelsNB{
    PixelNB blocsNB[64];
}MCUPixelsNB;

typedef struct mcuTransform{
    uint8_t Y[8][8];
    int8_t Cb[8][8];
    int8_t Cr[8][8];

}MCUTransform;

#endif //ETU_STRUCTURES_H
