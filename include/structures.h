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


typedef struct mcupixel{
    PixelRGB *blocsRGB;
    PixelNB  *blocsNB;
}MCUPixels;

typedef struct mcusMatrice{
    MCUPixels *mcus;
    int nbcol;
    int nblignes;
}MCUsMatrice;

typedef struct mcuTransform{
    int16_t **Y;
    int tailleY;
    int16_t *Cb;
    int16_t *Cr;

}MCUTransform;

typedef struct mcusTransformMat{
    int nbcol;
    int nblignes;
    MCUTransform* mcus;
}MCUsTransformMat;

#endif //ETU_STRUCTURES_H
