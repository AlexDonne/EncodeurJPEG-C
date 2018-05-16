#ifndef ETU_IMAGETOMCUS_H
#define ETU_IMAGETOMCUS_H

MCUPixelsRGB *imageCouleurToMCUs(ImagePPM *image);

void afficherMCUs(MCUPixelsRGB *tabMcus, int taille);

void afficherMCU(MCUPixelsRGB mcu);

#endif //ETU_IMAGETOMCUS_H
