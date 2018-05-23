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

/**
 * Structure pour stocker toutes les informations de l'image
 */
typedef struct imagePPM{
    TYPE_IMAGE type;
    char* nom;
    char* chemin;
    int hauteur;
    int largeur;
    PixelRGB **pixelsRGB;
    PixelNB **pixelsNB;
}ImagePPM;

/**
 * Structure pour un MCU, si en couleur alors blocsNB est NULL, et réciproquement
 */
typedef struct mcupixel{
    PixelRGB *blocsRGB;
    PixelNB  *blocsNB;
}MCUPixels;

/**
 * Structure comportant un tableau de MCU, et le nombre de colonnes et de lignes (nécessaire pour faire l'échantillonage ensuite)
 */
typedef struct mcusMatrice{
    MCUPixels *mcus;
    int nbcol;
    int nblignes;
}MCUsMatrice;

/**
 * Structure pour représenter un MCU comprenant le Y, Cb, CR
 * Y un tableau pour échantillonage
 */
typedef struct mcuTransform{
    int16_t **Y;
    int tailleY;
    int16_t **Cb;
    int16_t **Cr;
    int tailleCb;
    int tailleCr;

}MCUTransform;

/**
 * Structure pour le tableau des MCUsTransform, avec nombre lignes et nombre colonnes (nécessaire pour échantillonage)
 */
typedef struct mcusTransformMat{
    int nbcol;
    int nblignes;
    MCUTransform* mcus;
}MCUsTransformMat;

#endif //ETU_STRUCTURES_H
