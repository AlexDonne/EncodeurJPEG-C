#ifndef ETU_IMAGETOMCUS_H
#define ETU_IMAGETOMCUS_H

#include "structures.h"
#include "test_malloc.h"
MCUsMatrice* imageToMCUs(ImagePPM *image);

void adaptationMCU(ImagePPM* image, int *nouvHauteur, int *nouvLargeur);

void libererPixels(ImagePPM* image, int hauteur);

void libererPixelsRGB (PixelRGB** pixels, int hauteur);

void afficheImageNB(ImagePPM* image, int hauteur, int largeur);

void libererPixelsNB (PixelNB** pixels, int hauteur);

void afficherMCUs(MCUsMatrice *tabMcus);

void afficherMCURGB(MCUPixels mcu);

void afficherMCUNB(MCUPixels mcu);

#endif //ETU_IMAGETOMCUS_H
