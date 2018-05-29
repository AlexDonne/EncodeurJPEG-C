#ifndef ETU_LIBERERSTRUCTURES_H
#define ETU_LIBERERSTRUCTURES_H

#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

/**
 * Libère les tableaux de pixels de l'image
 * @param image
 * @param hauteur
 */
void libererPixels(ImagePPM *image, int hauteur);

void libererPixelsRGB(PixelRGB **pixels, int hauteur);

void libererPixelsNB(PixelNB **pixels, int hauteur);

/**
 * Libère l'espace mémoire occupée par une structure MCUsMatrice
 * @param mcusMat
 */
void libererMCUsMatrice(MCUsMatrice *mcusMat);

void libererMCUsPixels(MCUsMatrice *mcusMat);

void libererImage(ImagePPM *image);

void libererMCUsTransform(MCUsTransformMat *mcUsTransform);

void libererMCUTransform(MCUTransform *mcuTransform);

void libererCbs(MCUTransform *mcuTransform);

void libererCrs(MCUTransform *mcuTransform);

#endif //ETU_LIBERERSTRUCTURES_H
