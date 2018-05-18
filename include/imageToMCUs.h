#ifndef ETU_IMAGETOMCUS_H
#define ETU_IMAGETOMCUS_H

#include "structures.h"

MCUsMatrice* imageToMCUs(ImagePPM *image);

void adaptationMCU(ImagePPM* image);

void libererPixelsRGB (PixelRGB** pixels, int hauteur);

void libererPixelsNB (PixelNB** pixels, int hauteur);

void libererImage(ImagePPM *image);

void afficherMCUs(MCUsMatrice *tabMcus);

void afficherMCURGB(MCUPixels mcu);

void afficherMCUNB(MCUPixels mcu);

#endif //ETU_IMAGETOMCUS_H
